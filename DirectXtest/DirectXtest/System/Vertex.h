#pragma once
#if defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d11_x.h>
#else
#include <d3d11_1.h>
#endif

#include <DirectXMath.h>
using namespace DirectX;

namespace MyVertex
{
	struct VertexTypePositionTexture
	{
		VertexTypePositionTexture() {}
		VertexTypePositionTexture(float x, float y, float z, float u, float v) : position(x, y, z), texture(u, v) {}

		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct VertexTypePositionTextureNormal
	{
		VertexTypePositionTextureNormal() {}
		VertexTypePositionTextureNormal(float x, float y, float z, float u, float v, float nx, float ny, float nz) : position(x, y, z), texture(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
}
	

