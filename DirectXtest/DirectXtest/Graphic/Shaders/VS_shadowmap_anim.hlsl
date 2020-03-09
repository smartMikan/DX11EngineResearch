
#define MAX_BONES 256
cbuffer perObjectBuffer : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projMatrix;
};

cbuffer AvatorData : register(b1)
{
    float4x4 BoneTransforms[MAX_BONES];
};


struct VS_INPUT
{
    float3 inPos : POSITION;
    uint4 boneids : BONEID;
    float4 boneweights : BONEWEIGHT;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4x4 world = worldMatrix;
    
    float4x4 bone_transform = mul(BoneTransforms[input.boneids.x], input.boneweights.x);
    bone_transform += mul(BoneTransforms[input.boneids.y], input.boneweights.y);
    bone_transform += mul(BoneTransforms[input.boneids.z], input.boneweights.z);
    bone_transform += mul(BoneTransforms[input.boneids.w], input.boneweights.w);
    
    world = mul(bone_transform, world);

    float3 worldPos = mul(float4(input.inPos, 1.0f), world).xyz;
    output.outPosition = mul(float4(worldPos, 1.0f), viewMatrix);
    output.outPosition = mul(output.outPosition, projMatrix);
    
    output.depthPosition = output.outPosition;
    
    return output;
}