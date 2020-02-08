TextureCube TexCube : register(t7);
SamplerState Sampler : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 domePosition : TEXCOORD0;
};


float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 pos;
    pos.xyz = input.domePosition.xyz;

	//Sample the pixel color from the texture using the sampler at this texture coordinate location
    textureColor = TexCube.Sample(Sampler, pos);
	
    return textureColor;
}