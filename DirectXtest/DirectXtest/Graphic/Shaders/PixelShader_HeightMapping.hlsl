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


float HeightMapShadow(float3 worldPos, float3 normal, float3 light_dir)
{
    float4 lightPorjCoord = mul(float4(worldPos, 1.0f), ShadowMatrix);
    lightPorjCoord.xyz /= lightPorjCoord.w;
    
    float2 shadowmapUV = lightPorjCoord.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    float closestDepth = shadowTexture.Sample(objSamplerState, shadowmapUV).r;
    float fragDepth = lightPorjCoord.z;
    
    if (fragDepth > 1.0f)
    {
        return 1.0f;
    }
    
    float bias = 0.005f;
    float shadow = fragDepth - bias > closestDepth ? 0.0f : 1.0f;
    
    return shadow;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	// height mapping
	float3 worldPos = input.inWorldPos.xyz;
	float3 eyev = normalize(cameraPos - worldPos);
	float3x3 mat = { input.inTangent, -input.inBitangent, input.inNormal};
	float3 invEyev = mul(eyev, mat).xyz;
    float heightMap = (specularTexture.Sample(objSamplerState, input.inTexCoord).r - 0.5) * 0.25;
	input.inTexCoord += invEyev.xy * heightMap;
	float3 sampleColor = diffuseTexture.Sample(objSamplerState, input.inTexCoord);
    //sampleColor = float3(1,1,1);
    
	float3x3 tbn = float3x3(normalize(-input.inTangent), normalize(input.inBitangent), normalize(input.inNormal));
	float3 normal = input.inNormal;
    // do normal mapping
	float3 normal_sample = normalTexture.Sample(objSamplerState, input.inTexCoord).xyz;
	normal = normal_sample * 2.0 - 1.0;
    //normal.y = -normal.y;
    
	normal = normalize(mul(normal, tbn));
    
    
    //sampleColor = normal;
    
	float3 ambientLight = ambientLightColor * ambientLightStrength;
	float3 appliedLight = ambientLight;
	float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
	float3 diffuseLightIntensity = max(dot(vectorToLight, normal), 0);
	float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
	float attenuationFactor = 1 / (dynamicLightAttenuation_a + dynamicLightAttenuation_b * distanceToLight + dynamicLightAttenuation_c * pow(distanceToLight, 2));
    
	float3 refv = reflect(vectorToLight, normal);
	refv = normalize(refv);
	float specular = -dot(eyev, refv);
	specular = saturate(specular);
	specular = pow(specular, 64);
    
    diffuseLightIntensity = (diffuseLightIntensity + specular) * attenuationFactor * HeightMapShadow(worldPos, normal, vectorToLight);
	float3 diffuseLight = diffuseLightIntensity * dynamicStrength * dynamicLight;
	appliedLight += diffuseLight;
    
    
    
	float3 finalColor = sampleColor * appliedLight;

	return float4(finalColor, 1.0f);
}

