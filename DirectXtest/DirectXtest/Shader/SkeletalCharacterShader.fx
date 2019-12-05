Texture2D DiffuseMap:register(t0);  //漫反射纹理资源
Texture2D NormalMap:register(t1);  //法线纹理资源
SamplerState SampleType:register(s0);   //采样方式

//VertexShader
cbuffer CBMatrix:register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
	matrix WorldInvTranspose;
};

cbuffer CBLight:register(b1)
{
	float4 AmbientLight;
	float4 DiffuseLight;
	float3 LightDirection;
	float pad;
}

cbuffer CBBoneTranform:register(b2)
{
	matrix BoneTranforms[80];
}

cbuffer Material : register(b3)
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
}


cbuffer Camera : register(b4)
{
	float3 CameraPos;
	float pad1;
}


struct VertexIn
{
	float3 PosL       : POSITION;
	float3 NormalL    : NORMAL;
	float2 Tex        : TEXCOORD;
	float4 TangentL   : TANGENT;
	float3 Weights    : WEIGHTS;
	uint4 BoneIndices : BONEINDICES;
};


struct VertexOut
{
	float4 Pos:SV_POSITION;
	float2 Tex:TEXCOORD0;
	float3 W_Normal:NORMAL;  //世界空间的法线
	float3 W_Pos:POSITION;
	float4 W_Tangent:TANGENT; //世界空间的切线
};



float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float4 tangentW)
{
	//将法线贴图顶点像素颜色大小转为单位向量值
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	//求出T矩阵
	float3 N = unitNormalW;
	float3 T = normalize(tangentW.xyz - dot(tangentW.xyz, N)*N);
	float3 B = tangentW.w*cross(N, T);

	//TBN世界变换矩阵(从切线空间到局部空间再到世界空间)
	float3x3 TBN = float3x3(T, B, N);

	//位于切线空间的法向量乘以TBN矩阵直接从切线空间变到世界空间
	float3 bumpedNormalW = normalize(mul(normalT, TBN));

	return bumpedNormalW;
}


VertexOut VS(VertexIn ina)
{

	VertexOut outa;

	//初始化顶点的骨头权重
	float weights[4] = { 0.0f,0.0f,0.0f,0.0f };
	weights[0] = ina.Weights.x;
	weights[1] = ina.Weights.y;
	weights[2] = ina.Weights.z;
	weights[3] = 1- ina.Weights.x- ina.Weights.y- ina.Weights.z;

	float3 PosL = float3(0.0f, 0.0f, 0.0f);
	float3 NormalL= float3(0.0f, 0.0f, 0.0f);
	float3 TangentL = float3(0.0f, 0.0f, 0.0f);

	//将顶点按骨骼权重比进行相应的骨骼变换
	for (int i = 0; i < 4; ++i)
	{
		PosL += weights[i] * mul(float4(ina.PosL, 1.0f), BoneTranforms[ina.BoneIndices[i]]).xyz;
		NormalL+= weights[i] * mul(ina.NormalL, (float3x3)BoneTranforms[ina.BoneIndices[i]]);
		TangentL+= weights[i] * mul(ina.TangentL.xyz, (float3x3)BoneTranforms[ina.BoneIndices[i]]);
	}

	//变换到齐次裁剪空间
	//outa.Pos = mul(ina.PosL, World); float3*float4X4  这个并不报错，忽略了分值w为1.0f
	outa.Pos = mul(float4(PosL, 1.0f), World);

	outa.W_Pos = outa.Pos.xyz;
	outa.Pos = mul(outa.Pos, View);
	outa.Pos = mul(outa.Pos, Proj);

	//法线变换到世界空间
	outa.W_Normal = mul(NormalL, (float3x3)WorldInvTranspose);  //此事世界逆转置矩阵的第四行本来就没啥用
	outa.W_Normal = normalize(outa.W_Normal);

	outa.Tex= ina.Tex;

	//切线变换到世界空间
	outa.W_Tangent = float4(normalize(mul(TangentL, (float3x3)World)), ina.TangentL.w);

	return outa;
}




float4 PS(VertexOut outa) : SV_Target
{
	float DiffuseFactor;
	float SpecularFactor;
	float4 DiffuseMapColor;
	float4 DiffuseColor;
	float4 AmbientColor;
	float4 SpecualrColor;
	float4 color;
	float3 NormalMapColor;

   
	DiffuseMapColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//第一,获取采样颜色
	DiffuseMapColor = DiffuseMap.Sample(SampleType, outa.Tex);

	//对法线贴图进采样
	NormalMapColor=NormalMap.Sample(SampleType, outa.Tex).rgb;

	//利用世界空间的法线和切线求出处于法线贴图上的在世界空间的法向量
	float3 bumpNormalW = NormalSampleToWorldSpace(NormalMapColor, outa.W_Normal, outa.W_Tangent);


	//规格化灯光方向
	float3 NormalLightDir = normalize(LightDirection);

	//第二,求出DiffuseFactor
	float3 InvLightDir = -NormalLightDir;
	DiffuseFactor = saturate(dot(InvLightDir, bumpNormalW));
	 
	//第三,求出SpecularFactor
	float3 ReflectLightDir = reflect(LightDirection, bumpNormalW);

	//规格化反射光反向
	ReflectLightDir = normalize(ReflectLightDir);

	//求出像素到相机的方向
	float3 PixelToCameraDir = CameraPos - outa.W_Pos;

	//规格化PixelToCameraDir
	PixelToCameraDir = normalize(PixelToCameraDir);

	SpecularFactor = saturate(pow(dot(PixelToCameraDir, ReflectLightDir), Specular.w));

	//第四,求出color
	color = saturate(AmbientLight*Ambient + DiffuseFactor*DiffuseLight*Diffuse + SpecularFactor*DiffuseLight*Specular);

	color.a = 1.0f; 
		
	//第五,用灯光颜色调节纹理颜色(人物的DiffuseMap为透明贴图，无颜色部分是透明的)

	color = color*DiffuseMapColor;


	return color;
}