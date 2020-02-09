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







float4 main(PS_INPUT input) : SV_TARGET
{
    Material material = Mat;
    float4 inpos = input.inPosition;
    float3 normal = input.inNormal;
    // do normal mapping
    if(material.HasNormalTexture)
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
    float3 vectorToLight;
    float attenuationFactor;
    
    if (LightType == 0)
    {
        vectorToLight = normalize(dynamicLightDirection);
        attenuationFactor = 1 / (dynamicLightAttenuation_a);

    }
    else if (LightType == 1)
    {
        vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
        float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
        attenuationFactor = 1 / (dynamicLightAttenuation_a + dynamicLightAttenuation_b * distanceToLight + dynamicLightAttenuation_c * pow(distanceToLight, 2));
    
    }
    else
    {
        vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
        attenuationFactor = 1 / (dynamicLightAttenuation_a);
    }
    
    
    float3 refv = reflect(vectorToLight, normal);
    refv = normalize(refv);
    float3 eyev = normalize(cameraPos - worldPos);
    float rv = dot(-refv, eyev);
    rv = saturate(rv);
    float3 specular = material.SpecularColor.rgb * pow(rv, material.SpecularPower);
    
    float3 diffuseLightIntensity = max(dot(vectorToLight, normal), 0);
    diffuseLightIntensity = (diffuseLightIntensity + specular) * attenuationFactor * Shadow(worldPos, normal, vectorToLight);
    float3 diffuseLight = diffuseLightIntensity * dynamicStrength * dynamicLight;
    appliedLight += diffuseLight;
    
    
    
    finalColor = material.DiffuseColor.xyz * appliedLight;
    
    return float4(finalColor, 1.0f) * material.Opacity;
}

