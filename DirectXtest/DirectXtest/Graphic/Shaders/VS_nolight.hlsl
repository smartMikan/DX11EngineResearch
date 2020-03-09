
cbuffer perObjectBuffer : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projMatrix;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
};
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.inPos, 1.0);
    
    pos = mul(pos, worldMatrix);
    output.outPosition = mul(pos, viewMatrix);
    output.outPosition = mul(output.outPosition, projMatrix);
    output.outTexCoord = input.inTexCoord;
    return output;
}