cbuffer PerFrameBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//We have a new fog constant buffer that will be set outside the shader so we can know the start point and end point of the fog effect.

cbuffer FogBuffer
{
	float fogStart;
	float fogEnd;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	float4 cameraPosition;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	//We first calculate the Z coordinate of the vertex in view space. 
	//We then use that with the fog end and start position in the fog factor equation to produce a fog factor that we send into the pixel shader.
	
	// Calculate the camera position.
	cameraPosition = mul(input.position, worldMatrix);
	cameraPosition = mul(cameraPosition, viewMatrix);

	// Calculate linear fog.    
	output.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));

	return output;
}