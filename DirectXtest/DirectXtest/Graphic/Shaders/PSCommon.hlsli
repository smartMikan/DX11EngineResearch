struct Material
{
    float4 GlobalAmbient;
    float4 AmbientColor;
    float4 EmissiveColor;
    float4 DiffuseColor;
    float4 SpecularColor;
    float4 ReflectColor;
    
    float Opacity;
    float SpecularPower;
    float IndexOfRefraction;
    bool HasAmbientTexture;
    
    bool HasEmissiveTexture;
    bool HasDiffuseTexture;
    bool HasSpecularTexture;
    bool HasSpecularPowerTexture;
    
    bool HasNormalTexture;
    bool HasBumpTexture;
    bool HasOpacityTexture;
    float BumpIntensity;
    
    float SpecularScale;
    float AlphaThreshold;
    float2 Padding;
    
};

cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;

    float3 dynamicLight;
    float dynamicStrength;
    float3 dynamicLightPosition;

    float dynamicLightAttenuation_a;
    float dynamicLightAttenuation_b;
    float dynamicLightAttenuation_c;
}

cbuffer cameraBuffer : register(b1)
{
    float3 cameraPos;
    float pad;
}

cbuffer Material : register(b2)
{
    Material Mat;
}

cbuffer Matrices : register(b3)
{
    float4x4 ShadowMatrix;
}

cbuffer FadeBuffer : register(b4)
{
    float fadeAmount;
    float3 padding;
};





Texture2D diffuseTexture : TEXTURE : register(t0);
Texture2D normalTexture : TEXTURE : register(t1);
Texture2D specularTexture : TEXTURE : register(t2);
Texture2D emissiveTexture : TEXTURE : register(t3);
Texture2D shadowTexture : TEXTURE : register(t4);
Texture2D toneTexture : TEXTURE : register(t5);
Texture2D reflectionTexture : TEXTURE : register(t6);
TextureCube TexCube : register(t7);

SamplerState objSamplerState : SAMPLER : register(s0);
SamplerState shadowSamplerState : SAMPLER : register(s1);


