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
    
    float3 dynamicLightDirection;
    float dynamicLightAttenuation_b;
    float dynamicLightAttenuation_c;
    int LightType;
    
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


#define MAX_PCFRANGE 2

float Shadow(float3 worldPos, float3 normal, float3 light_dir)
{
	//pxiel position in shadow viewprojection matrix
    float4 proj_coords = mul(float4(worldPos, 1.0f), ShadowMatrix);
    
	//back to unnormalized value
	proj_coords.xyz /= proj_coords.w;
	
	
	


	//x from (-1,1) to (0,1) y from(-1,1) to (1,0)
    float2 shadow_uv = proj_coords.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    //float closest_depth = shadowTexture.Sample(shadowSamplerState, shadow_uv).r;
    //float bias = max(0.005 * (1.0 - dot(normal, light_dir)), 0.0005f);
    float bias = 0.00025f;
	
	//currentdepth of this pixel in shadow map
    float current_depth = proj_coords.z - bias;
	
	if (current_depth < 0.0f)
	{
		return 0.0f;
	}

    float shadow = 0.0f;
    
	//Percentage-Closer Filtering Blur
    int range = MAX_PCFRANGE;
    float2 texelSize = 1.0 / 1024.0;
    
    for (int y = -range; y <= range; y++)
    {
        for (int x = -range; x <= range; x++)
        {

            float pcfDepth = shadowTexture.Sample(shadowSamplerState, shadow_uv + float2(x, y) * texelSize).r;
            shadow += current_depth > pcfDepth ? 0.0f : 1.0f;

        }
    }
    shadow /= (range * 2 + 1) * (range * 2 + 1);
    //shadow = current_depth > closest_depth ? 0.0f : 1.0f;
	
    return shadow;
}
