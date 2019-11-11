Texture2D shaderTextures[3];
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDirection : TEXCOORD1;
};


float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float4 specularIntensity;
    float3 reflection;
    float4 specular;

    //The first part of the shader is the regular bump map shader code.
    //Samplethe texturepixel at this location.
    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	
    // Sample the pixel in the bump map.
    bumpMap = shaderTextures[1].Sample(SampleType, input.tex);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the bump map.
    bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;
	
    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);

    // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel based on the bump map normal value.
    lightIntensity = saturate(dot(bumpNormal, lightDir));

    
    // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    color = saturate(diffuseColor * lightIntensity);

   

    //If the light intensity is greater than zero we will do specular light calculations.
    if (lightIntensity > 0.0f)
    {
        //Here is where we sample the specular map for the intensity of specular light at this pixel.
        // Sample the pixel from the specular map texture.
        specularIntensity = shaderTextures[2].Sample(SampleType, input.tex);

        //In the reflection calculation we use the bump map normal instead of the regular input normal.
        // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);

        // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = specularColor * pow(saturate(dot(reflection, input.viewDirection)), specularPower);

        //Now that we have the amount of specular light at this pixel we then multiply it by the specular intensity from the specular map to get a final value.
         // Use the specular map to determine the intensity of specular light at this pixel.
        specular = specular * specularIntensity;
		
       
    }

    // Combine the final bump light color with the texture color.
    color = color * textureColor;
    
    // Add the specular component last to the output color.
    color = saturate(color + specular);
    return color;
}