SamplerState SampleType;
//The water shader will need three textures. 
//A reflection texture for the scene reflection. 
//A refraction texture for the refraction of the scene.
//And finally a normal map texture for simulating water ripples.
Texture2D reflectionTexture;
Texture2D refractionTexture;
Texture2D normalTexture;

//The water translation variable in the buffer will be used for simulating water motion by translating the texture sampling coordinates each frame. 
//The reflectRefractScale variable in the buffer is used for controlling the size of the water ripples in relation to the normal map. 
//Some normal maps will be slightly different in how drastic the normals rise and fall. 
//Having a variable to control this becomes very useful.

cbuffer WaterBuffer
{
    float waterTranslation;
    float reflectRefractScale;
    float2 padding;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
    float4 refractionPosition : TEXCOORD2;
};


float4 main(PixelInputType input) : SV_TARGET
{
    float2 reflectTexCoord;
    float2 refractTexCoord;
    float4 normalMap;
    float3 normal;
    float4 reflectionColor;
    float4 refractionColor;
    float4 color;

    // Just like the translate shader tutorial we use a translation variable updated each frame to move the water normal map texture along the Y axis to simulate motion.

    // Move the position the water normal is sampled from to simulate moving water.	
    input.tex.y += waterTranslation;

    // Convert both the reflection and refraction coordinates into texture coordinates in the -1 to +1 range.
    // Calculate the projected reflection texture coordinates.
    reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
    reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;
	
    // Calculate the projected refraction texture coordinates.
    refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
    refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;

    // Sample the normal for this pixel from the normal map and expand the range to be in the -1 to +1 range.
    // Sample the normal from the normal map texture.
    normalMap = normalTexture.Sample(SampleType, input.tex);

    // Expand the range of the normal from (0,1) to (-1,+1).
    normal = (normalMap.xyz * 2.0f) - 1.0f;

    // Now distort the reflection and refraction coordinates by the normal map value. 
    // This creates the rippling effect by using the normal transitioning from -1 to +1 to distort our view just as water waves distort light. 
    // The normal map value is multiplied by the reflectRefractScale to make the ripples less pronounced and more natural looking.

    // Re-position the texture coordinate sampling position by the normal map value to simulate the rippling wave effect.
    reflectTexCoord = reflectTexCoord + (normal.xy * reflectRefractScale);
    refractTexCoord = refractTexCoord + (normal.xy * reflectRefractScale);

    // Next sample the reflection and refraction pixel based on the updated texture sampling coordinates.
    // Sample the texture pixels from the textures using the updated texture coordinates.
    reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);
    refractionColor = refractionTexture.Sample(SampleType, refractTexCoord);

    //Finally combine the reflection and refraction pixel using a linear interpolation.
    // Combine the reflection and refraction results for the final color.
    color = lerp(reflectionColor, refractionColor, 0.6f);
	
    return color;

}