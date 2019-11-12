Texture2D shaderTexture;
SamplerState SampleType;


//We add a new buffer with a float variable called textureTranslation. 
//This is set in the graphicsclass.cpp during the Render call to update the position of the texture translation. 
//The value set here will be between 0 and 1.

cbuffer TranslationBuffer
{
    float textureTranslation;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	// Translate the position where we sample the pixel from.
    input.tex.x += textureTranslation;

    return shaderTexture.Sample(SampleType, input.tex);
}