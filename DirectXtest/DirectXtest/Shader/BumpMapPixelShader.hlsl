Texture2D shaderTextures[2];
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
};



struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

//The pixel shader works as we described above with a couple additional lines of code. 
//First we sample the pixel from the color texture and the normal map. 
//We then multiply the normal map value by two and then subtract one to move it into the -1.0 to +1.0 float range.
//We have to do this because the sampled value that is presented to us in the 0.0 to +1.0 texture range which only covers half the range we need for bump map normal calculations. 
//After that we then calculate the bump normal which uses the equation we described earlier. 
//This bump normal is normalized and then used to determine the light intensity at this pixel by doing a dot product with the light direction. 
//Once we have the light intensity at this pixel the bump mapping is now done. 
//We use the light intensity with the light color and texture color to get the final pixel color.

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float lightIntensity;
    float4 color;


    // Sample the texture pixel at this location.
    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	
    // Sample the pixel in the bump map.
    bumpMap = shaderTextures[1].Sample(SampleType, input.tex);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the bump map.
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	
    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel based on the bump map normal value.
    lightIntensity = saturate(dot(bumpNormal, lightDir));

    // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    color = saturate(diffuseColor * lightIntensity);

    // Combine the final bump light color with the texture color.
    color = color * textureColor;
	
    return color;
}