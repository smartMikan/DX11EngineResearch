////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

//We have a new constant buffer that contains the pixelColor value. 
//We use this to control the color of the pixel that will be used to draw the font text.

cbuffer PixelBuffer
{
    float4 pixelColor;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

//The FontPixelShader first samples the font texture to get the pixel. 
//If it samples a pixel that is black then it is just part of the background triangle and not a text pixel. 
//In this case we set the alpha of this pixel to zero so when the blending is calculated it will determine that this pixel should be see-through. 
//If the color of the input pixel is not black then it is a text pixel. 
//In this case we multiply it by the pixelColor to get the pixel colored the way we want and then draw that to the screen.

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
    float4 color;
	
	
    // Sample the texture pixel at this location.
    color = shaderTexture.Sample(SampleType, input.tex);
	
    // If the color is black on the texture then treat this pixel as transparent.
    if (color.r == 0.0f)
    {
        color.a = 0.0f;
    }
	
    // If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
    else
    {
        color.a = 1.0f;
        color = color * pixelColor;
    }

    return color;
}
