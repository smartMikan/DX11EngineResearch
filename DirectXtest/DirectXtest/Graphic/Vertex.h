#pragma once
#if defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d11_x.h>
#else
#include <d3d11_1.h>
#endif

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;


struct VertexTypePositionTexture
{
	VertexTypePositionTexture() {}
	VertexTypePositionTexture(float x, float y, float z, float u, float v)
		: position(x, y, z), texture(u, v) {}

	XMFLOAT3 position;
	XMFLOAT2 texture;
};

struct VertexTypePositionTextureNormal
{
	VertexTypePositionTextureNormal() {}
	VertexTypePositionTextureNormal(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: position(x, y, z), texture(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

struct Vertex2D
{
	Vertex2D() {}
	Vertex2D(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
};



