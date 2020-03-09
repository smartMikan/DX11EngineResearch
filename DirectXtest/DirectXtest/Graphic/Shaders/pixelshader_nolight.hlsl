#include "PSCommon.hlsli"


struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    Material material = Mat;
    
    if (material.HasDiffuseTexture)
    {
        material.DiffuseColor = diffuseTexture.Sample(objSamplerState, input.inTexCoord);
    }
    float3 sampleColor = material.DiffuseColor;

    return float4(sampleColor, 1.0f) * material.Opacity;
 
}