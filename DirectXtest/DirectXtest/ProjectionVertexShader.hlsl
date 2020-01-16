cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix worldInvTranspose;
};



float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}