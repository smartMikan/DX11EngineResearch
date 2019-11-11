
Texture2D shaderTextures[2];
SamplerState SampleType;


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

//The light map pixel shader is very simple.
//It multiplies the color texture pixel and the light map texture value to get the desired output.
//This is not much different from just a regular multitexture blend other than there is no need to correct for gamma.

float4 main(PixelInputType input) : SV_TARGET
{
    float4 color;
    float4 lightColor;
    float4 finalColor;


    // Get the pixel color from the color texture.
    color = shaderTextures[0].Sample(SampleType, input.tex);

    // Get the pixel color from the light map.
    lightColor = shaderTextures[1].Sample(SampleType, input.tex);

    // Blend the two pixels together.
    finalColor = color * lightColor;

    return finalColor;
}