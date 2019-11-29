cbuffer MatrixBuffer :register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix worldInvTranspose;
};

cbuffer CBBoneTranform : register(b1)
{
    matrix BoneTranforms[80];
}


struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    uint color : COLOR;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float3 weights : BLENDWEIGHT;
};

struct PixelInputType
{   
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    uint color : COLOR;
    float3 W_Normal : NORMAL; //����ռ�ķ���
    float3 W_Pos : POSITION;
    float4 W_Tangent : TANGENT; //����ռ������
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;

	//��ʼ������Ĺ�ͷȨ��
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.weights.x;
    weights[1] = input.weights.y;
    weights[2] = input.weights.z;
    weights[3] = 1 - input.weights.x - input.weights.y - input.weights.z;

    float3 PosL = float3(0.0f, 0.0f, 0.0f);
    float3 NormalL = float3(0.0f, 0.0f, 0.0f);
    float3 TangentL = float3(0.0f, 0.0f, 0.0f);

    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
    
	//�����㰴����Ȩ�رȽ�����Ӧ�Ĺ����任
    for (int i = 0; i < 4; ++i)
    {
        PosL += weights[i] * mul(input.position, BoneTranforms[input.boneIndices[i]]).xyz;
        NormalL += weights[i] * mul(input.normal, (float3x3) BoneTranforms[input.boneIndices[i]]);
        TangentL += weights[i] * mul(input.tangent.xyz, (float3x3) BoneTranforms[input.boneIndices[i]]);
    }

	//�任����βü��ռ�
	//output.Pos = mul(ina.PosL, World); float3*float4X4  ����������������˷�ֵwΪ1.0f
    output.Pos = mul(float4(PosL, 1.0f), worldMatrix);

    output.W_Pos = output.Pos.xyz;
    output.Pos = mul(output.Pos, viewMatrix);
    output.Pos = mul(output.Pos, projectionMatrix);

	//���߱任������ռ�
    output.W_Normal = mul(NormalL, (float3x3) worldInvTranspose); //����������ת�þ���ĵ����б�����ûɶ��
    output.W_Normal = normalize(output.W_Normal);

    output.Tex = input.tex;

	//���߱任������ռ�
    output.W_Tangent = float4(normalize(mul(TangentL, (float3x3) worldMatrix)), input.tangent.w);

    output.color = input.color;
    
    return output;

}
