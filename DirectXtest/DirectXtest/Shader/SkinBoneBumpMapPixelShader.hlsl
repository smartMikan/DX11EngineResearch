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
    float3 W_Normal : NORMAL; //����ռ�ķ���
    float3 W_Pos : POSITION;
    float4 W_Tangent : TANGENT; //����ռ������
};


float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float4 tangentW)
{
	//��������ͼ����������ɫ��СתΪ��λ����ֵ
    float3 normalT = 2.0f * normalMapSample - 1.0f;

	//���T����
    float3 N = unitNormalW;
    float3 T = normalize(tangentW.xyz - dot(tangentW.xyz, N) * N);
    float3 B = tangentW.w * cross(N, T);

	//TBN����任����(�����߿ռ䵽�ֲ��ռ��ٵ�����ռ�)
    float3x3 TBN = float3x3(T, B, N);

	//λ�����߿ռ�ķ���������TBN����ֱ�Ӵ����߿ռ�䵽����ռ�
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

	//��һ,��ȡ������ɫ
    DiffuseMapColor = DiffuseMap.Sample(SampleType, input.Tex);

	//�Է�����ͼ������
    NormalMapColor = NormalMap.Sample(SampleType, input.Tex).rgb;

	//��������ռ�ķ��ߺ�����������ڷ�����ͼ�ϵ�������ռ�ķ�����
    float3 bumpNormalW = NormalSampleToWorldSpace(NormalMapColor, input.W_Normal, input.W_Tangent);


	//��񻯵ƹⷽ��
    float3 NormalLightDir = normalize(LightDirection);

	//�ڶ�,���DiffuseFactor
    float3 InvLightDir = -NormalLightDir;
    DiffuseFactor = saturate(dot(InvLightDir, bumpNormalW));
	 
	//����,���SpecularFactor
    float3 ReflectLightDir = reflect(LightDirection, bumpNormalW);

	//��񻯷���ⷴ��
    ReflectLightDir = normalize(ReflectLightDir);

	//������ص�����ķ���
    float3 PixelToCameraDir = CameraPos - input.W_Pos;

	//���PixelToCameraDir
    PixelToCameraDir = normalize(PixelToCameraDir);

    SpecularFactor = saturate(pow(dot(PixelToCameraDir, ReflectLightDir), Specular.w));

	//����,���color
    color = saturate(AmbientLight * Ambient + DiffuseFactor * DiffuseLight * Diffuse + SpecularFactor * DiffuseLight * Specular);

    color.a = 1.0f;
		
	//����,�õƹ���ɫ����������ɫ
    color = color * DiffuseMapColor;

    return color;
}