cbuffer wvpBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

#define MAX_BONES 256

cbuffer SkeletalAnimationData : register(b1)
{
    float4x4 BoneTransforms[MAX_BONES];
};

cbuffer ReflectionBuffer : register(b2)
{
    matrix reflectionMatrix;
};