Texture2D shaderTexture;
SamplerState SampleType;

Texture2D reflectionTexture;


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float2 reflectTexCoord;
    float4 reflectionColor;
    float4 color;


    // Sample the texture pixel at this location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    //The input reflection position homogenous coordinates need to be converted to proper texture coordinates.
    //To do so first divide by the W coordinate. 
    //This leaves us with tu and tv coordinates in the -1 to +1 range, to fix it to map to a 0 to +1 range just divide by 2 and add 0.5.
    // Calculate the projected reflection texture coordinates.
    reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
    reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

    //Now when we sample from the reflection texture we use the projected reflection coordinates that have been converted to get the right reflection pixel for this projected reflection position.
    // Sample the texture pixel from the reflection texture using the projected texture coordinates.
    reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);

    //Finally we blend the texture from the floor with the reflection texture to create the effect of the reflected cube on the floor. Here we use a linear interpolation between the two textures with a factor of 0.15.
    //You can change this to a different blend equation or change the factor amount for a different or stronger effect.

    // Do a linear interpolation between the two textures for a blend effect.
    color = lerp(textureColor, reflectionColor, 0.15f);

    return color;
}
