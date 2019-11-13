Texture2D shaderTexture;
SamplerState SampleType;

//We add a new constant buffer to hold the blendAmount variable. 
//The blendAmount indicates was percentage to blend the texture. 
//The acceptable value range is a float value from 0 to 1. 
//In this tutorial it is set to 0.5 which means 50% transparency.

cbuffer TransparentBuffer
{
    float blendAmount;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 color;
	// Sample the texture pixel at this location.
    color = shaderTexture.Sample(SampleType, input.tex);

    //Here we use the blendAmount variable to set the transparency of the texture.
    // We set the alpha value of the pixel to the blend amount and when rendered it will use the alpha value with the blend state we setup to create the transparency effect.

     // Set the alpha value of this pixel to the blending amount to create the alpha blending effect.
    color.a = blendAmount;

    return color;
}