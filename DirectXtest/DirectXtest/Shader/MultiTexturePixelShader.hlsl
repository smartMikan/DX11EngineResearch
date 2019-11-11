//We have added a two element texture array resource here for the two different textures that will be blended together. 
//Texture arrays are more efficient that using single texture resources in terms of performance on the graphics card.
//Switching textures was very costly in earlier versions of DirectX forcing most engines to be written around texture and material switches. 
//Texture arrays help reduce that performance cost.
Texture2D shaderTextures[2];
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

//The pixel shader is where all the work of this tutorial is done. 
//We take a sample of the pixel from both textures at this current texture coordinate. 
//After that we combine them using multiplication since they are non-linear due to gamma correction. 
//We also multiply by a gamma value, we have used 2.0 in this example as it is close to most monitor's gamma value. 
//Once we have the blended pixel we saturate it and then return it as our final result. 
//Notice also the indexing method used to access the two textures in the texture array.

float4 main(PixelInputType input) : SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 blendColor;


    // Get the pixel color from the first texture.
    color1 = shaderTextures[0].Sample(SampleType, input.tex);

    // Get the pixel color from the second texture.
    color2 = shaderTextures[1].Sample(SampleType, input.tex);

    // Blend the two pixels together and multiply by the gamma value.
    blendColor = color1 * color2 * 2.0;
    
    // Saturate the final color.
    blendColor = saturate(blendColor);

    return blendColor;
}