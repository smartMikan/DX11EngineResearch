#include "PSCommon.hlsli"



struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float3 inWorldPos : WORLD_POSITION;
    float3 inNormal : NORMAL;
    float3 inTangent : TANGENT;
    float3 inBitangent : BITANGENT;
    float2 inTexCoord : TEXCOORD;
};




float Shadow(float3 worldPos, float3 normal, float3 light_dir)
{
    float4 proj_coords = mul(float4(worldPos, 1.0f), ShadowMatrix);
    proj_coords.xyz /= proj_coords.w;
	
    float2 shadow_uv = proj_coords.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    //float closest_depth = shadowTexture.Sample(shadowSamplerState, shadow_uv).r;
    //float bias = max(0.005 * (1.0 - dot(normal, light_dir)), 0.0005f);
    float bias = 0.005f;
    float current_depth = proj_coords.z - bias;
	
    float shadow = 0.0f;
    int range = 2;
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


float4 main(PS_INPUT input) : SV_TARGET
{
    Material material = Mat;
    float4 inpos = input.inPosition;
    float3 normal = input.inNormal;
    // do normal mapping
    if (material.HasNormalTexture)
    {
        float3 normal_sample = normalTexture.Sample(objSamplerState, input.inTexCoord).xyz;
        normal = normal_sample * 2.0 - 1.0;
        //normal.y = -normal.y;
        float3x3 tbn = float3x3(normalize(input.inTangent), normalize(input.inBitangent), normalize(input.inNormal));
        normal = mul(normal, tbn);
    }
    normal = normalize(normal);
    float3 worldPos = input.inWorldPos.xyz;
    
    // diffuse
    if (material.HasDiffuseTexture)
    {
        material.DiffuseColor = diffuseTexture.Sample(objSamplerState, input.inTexCoord);
    }
    //sampleColor = float3(1,1,1);
    
    if (material.HasSpecularTexture)
    {
        material.SpecularColor = specularTexture.Sample(objSamplerState, input.inTexCoord);
    }
    	// ambient
    float3 ambient = material.AmbientColor.rgb;
    if (material.HasAmbientTexture)
    {
        if (any(ambient))
        {
            ambient *= material.AmbientColor.rgb;
        }
        else
        {
            ambient = material.AmbientColor.rgb;
        }
    }

    ambient *= material.GlobalAmbient.rgb;
    ambient *= material.DiffuseColor.rgb;

	// emissive
    float3 emissive = material.EmissiveColor.rgb;
    if (material.HasEmissiveTexture)
    {
        float3 sample = emissiveTexture.Sample(objSamplerState, input.inTexCoord).rgb;
		
        if (any(emissive))
        {
            emissive *= emissiveTexture.Sample(objSamplerState, input.inTexCoord).rgb;
        }
        else
        {
            emissive = emissiveTexture.Sample(objSamplerState, input.inTexCoord).rgb;
        }
    }
    
    float3 finalColor;
    
    // light
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 appliedLight = ambientLight;
    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
    float3 diffuseLightIntensity = max(dot(vectorToLight, normal), 0);
    float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
    float attenuationFactor = 1 / (dynamicLightAttenuation_a + dynamicLightAttenuation_b * distanceToLight + dynamicLightAttenuation_c * pow(distanceToLight, 2));
    
    //float3 refv = reflect(vectorToLight, normal);
    //refv = normalize(refv);
    float3 eyev = normalize(cameraPos - worldPos);
    //float rv = dot(-refv, eyev);
    //rv = saturate(rv);
    //float3 specular = material.SpecularColor.rgb * pow(rv, material.SpecularPower);
    
    float light = dot(vectorToLight, normal) / 2.0 + 0.5;
    light *= Shadow(worldPos, normal, vectorToLight);
    float facing = dot(eyev, normal);
    float2 toneTexcoord = float2(light, facing);
    float3 toneColor = toneTexture.Sample(objSamplerState, toneTexcoord).rgb;
    
    //diffuseLightIntensity = (diffuseLightIntensity + specular) * attenuationFactor * Shadow(worldPos, normal, vectorToLight);
    diffuseLightIntensity = toneColor * attenuationFactor;// * Shadow(worldPos, normal, vectorToLight);
    float3 diffuseLight = diffuseLightIntensity * dynamicStrength * dynamicLight;
    appliedLight += diffuseLight;
    
    
    
    finalColor = material.DiffuseColor.xyz * appliedLight;
    
    return float4(finalColor, 1.0f) * material.Opacity;
}

