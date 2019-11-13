Texture2D shaderTexture;
SamplerState SampleType;


//We add a new buffer called FadeBuffer to control the color level of the texture by using the fadeAmount variable. 
//This value will be incremented each frame to make the image brighter and brighter creating the fade in effect. 
//The value range is between 0.0 (0%) and 1.0 (100%). 
//The padding variable is just to ensure the buffer is a multiple of 16.
cbuffer FadeBuffer
{
    float fadeAmount;
    float3 padding;
};


struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex: TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	 float4 color;


    // Sample the texture pixel at this location.
    color = shaderTexture.Sample(SampleType, input.tex);

	//Here is where we use the fadeAmount to control the output color of all pixels from the texture to be set to a certain brightness.

	// Reduce the color brightness to the current fade percentage.
    color = color * fadeAmount;

    return color;
}