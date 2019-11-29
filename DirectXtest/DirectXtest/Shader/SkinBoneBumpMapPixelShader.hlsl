Texture2D DiffuseMap : register(t0); 
Texture2D NormalMap : register(t1); 
SamplerState SampleType : register(s0);

cbuffer CBLight : register(b0)
{
    float4 AmbientLight;
    float4 DiffuseLight;
    float3 LightDirection;
    float pad;
}

cbuffer Material : register(b1)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; 
}

cbuffer Camera : register(b2)
{
    float3 CameraPos;
    float padding;
}

struct PixelInputType
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    uint color : COLOR;
    float3 W_Normal : NORMAL; //世界空间的法线
    float3 W_Pos : POSITION;
    float4 W_Tangent : TANGENT; //世界空间的切线
};


float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float4 tangentW)
{
	//将法线贴图顶点像素颜色大小转为单位向量值
    float3 normalT = 2.0f * normalMapSample - 1.0f;

	//求出T矩阵
    float3 N = unitNormalW;
    float3 T = normalize(tangentW.xyz - dot(tangentW.xyz, N) * N);
    float3 B = tangentW.w * cross(N, T);

	//TBN世界变换矩阵(从切线空间到局部空间再到世界空间)
    float3x3 TBN = float3x3(T, B, N);

	//位于切线空间的法向量乘以TBN矩阵直接从切线空间变到世界空间
    float3 bumpedNormalW = normalize(mul(normalT, TBN));

    return bumpedNormalW;
}



float4 main(PixelInputType input) : SV_TARGET
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
    DiffuseMapColor = DiffuseMap.Sample(SampleType, input.Tex);

	//对法线贴图进采样
    NormalMapColor = NormalMap.Sample(SampleType, input.Tex).rgb;

	//利用世界空间的法线和切线求出处于法线贴图上的在世界空间的法向量
    float3 bumpNormalW = NormalSampleToWorldSpace(NormalMapColor, input.W_Normal, input.W_Tangent);


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
    float3 PixelToCameraDir = CameraPos - input.W_Pos;

	//规格化PixelToCameraDir
    PixelToCameraDir = normalize(PixelToCameraDir);

    SpecularFactor = saturate(pow(dot(PixelToCameraDir, ReflectLightDir), Specular.w));

	//第四,求出color
    color = saturate(AmbientLight * Ambient + DiffuseFactor * DiffuseLight * Diffuse + SpecularFactor * DiffuseLight * Specular);

    color.a = 1.0f;
		
	//第五,用灯光颜色调节纹理颜色
    color = color * DiffuseMapColor;

    return color;
}