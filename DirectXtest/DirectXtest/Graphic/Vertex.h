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

//struct VertexTypePositionNormalTangentColorTexture
//{
//	VertexTypePositionNormalTangentColorTexture() = default;
//
//	VertexTypePositionNormalTangentColorTexture(const VertexTypePositionNormalTangentColorTexture&) = default;
//	VertexTypePositionNormalTangentColorTexture& operator=(const VertexTypePositionNormalTangentColorTexture&) = default;
//
//	VertexTypePositionNormalTangentColorTexture(VertexTypePositionNormalTangentColorTexture&&) = default;
//	VertexTypePositionNormalTangentColorTexture& operator=(VertexTypePositionNormalTangentColorTexture&&) = default;
//
//	XMFLOAT3 position;
//	XMFLOAT3 normal;
//	XMFLOAT4 tangent;
//	uint32_t color;
//	XMFLOAT2 textureCoordinate;
//
//	VertexTypePositionNormalTangentColorTexture(XMFLOAT3 const& iposition, XMFLOAT3 const& inormal, XMFLOAT4 const& itangent, uint32_t irgba, XMFLOAT2 const& itextureCoordinate)
//		: position(iposition),
//		normal(inormal),
//		tangent(itangent),
//		color(irgba),
//		textureCoordinate(itextureCoordinate)
//	{
//	}
//
//	VertexTypePositionNormalTangentColorTexture(FXMVECTOR iposition, FXMVECTOR inormal, FXMVECTOR itangent, uint32_t irgba, CXMVECTOR itextureCoordinate)
//		: color(irgba)
//	{
//		XMStoreFloat3(&this->position, iposition);
//		XMStoreFloat3(&this->normal, inormal);
//		XMStoreFloat4(&this->tangent, itangent);
//		XMStoreFloat2(&this->textureCoordinate, itextureCoordinate);
//	}
//
//	VertexTypePositionNormalTangentColorTexture(XMFLOAT3 const& iposition, XMFLOAT3 const& inormal, XMFLOAT4 const& itangent, XMFLOAT4 const& icolor, XMFLOAT2 const& itextureCoordinate)
//		: position(iposition),
//		normal(inormal),
//		tangent(itangent),
//		color{},
//		textureCoordinate(itextureCoordinate)
//	{
//		SetColor(icolor);
//	}
//
//	VertexTypePositionNormalTangentColorTexture(FXMVECTOR iposition, FXMVECTOR inormal, FXMVECTOR itangent, CXMVECTOR icolor, CXMVECTOR itextureCoordinate) :
//		color{}
//	{
//		XMStoreFloat3(&this->position, iposition);
//		XMStoreFloat3(&this->normal, inormal);
//		XMStoreFloat4(&this->tangent, itangent);
//		XMStoreFloat2(&this->textureCoordinate, itextureCoordinate);
//
//		SetColor(icolor);
//	}
//
//	void __cdecl SetColor(XMFLOAT4 const& icolor) { SetColor(XMLoadFloat4(&icolor)); }
//	void XM_CALLCONV SetColor(FXMVECTOR icolor);
//
//	static const int InputElementCount = 5;
//	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
//};
//
//struct VertexTypePositionNormalTangentColorTextureSkinning : public VertexTypePositionNormalTangentColorTexture
//{
//	VertexTypePositionNormalTangentColorTextureSkinning() = default;
//
//	VertexTypePositionNormalTangentColorTextureSkinning(const VertexTypePositionNormalTangentColorTextureSkinning&) = default;
//	VertexTypePositionNormalTangentColorTextureSkinning& operator=(const VertexTypePositionNormalTangentColorTextureSkinning&) = default;
//
//	VertexTypePositionNormalTangentColorTextureSkinning(VertexTypePositionNormalTangentColorTextureSkinning&&) = default;
//	VertexTypePositionNormalTangentColorTextureSkinning& operator=(VertexTypePositionNormalTangentColorTextureSkinning&&) = default;
//
//	uint32_t indices;
//	uint32_t weights;
//
//	VertexTypePositionNormalTangentColorTextureSkinning(
//		XMFLOAT3 const& iposition,
//		XMFLOAT3 const& inormal,
//		XMFLOAT4 const& itangent,
//		uint32_t irgba,
//		XMFLOAT2 const& itextureCoordinate,
//		XMUINT4 const& iindices,
//		XMFLOAT4 const& iweights)
//		: VertexTypePositionNormalTangentColorTexture(iposition, inormal, itangent, irgba, itextureCoordinate),
//		indices{},
//		weights{}
//	{
//		SetBlendIndices(iindices);
//		SetBlendWeights(iweights);
//	}
//
//	VertexTypePositionNormalTangentColorTextureSkinning(
//		FXMVECTOR iposition,
//		FXMVECTOR inormal,
//		FXMVECTOR itangent,
//		uint32_t irgba,
//		CXMVECTOR itextureCoordinate,
//		XMUINT4 const& iindices,
//		CXMVECTOR iweights)
//		: VertexTypePositionNormalTangentColorTexture(iposition, inormal, itangent, irgba, itextureCoordinate),
//		indices{},
//		weights{}
//	{
//		SetBlendIndices(iindices);
//		SetBlendWeights(iweights);
//	}
//
//	VertexTypePositionNormalTangentColorTextureSkinning(
//		XMFLOAT3 const& iposition,
//		XMFLOAT3 const& inormal,
//		XMFLOAT4 const& itangent,
//		XMFLOAT4 const& icolor,
//		XMFLOAT2 const& itextureCoordinate,
//		XMUINT4 const& iindices,
//		XMFLOAT4 const& iweights)
//		: VertexTypePositionNormalTangentColorTexture(iposition, inormal, itangent, icolor, itextureCoordinate),
//		indices{},
//		weights{}
//	{
//		SetBlendIndices(iindices);
//		SetBlendWeights(iweights);
//	}
//
//	VertexTypePositionNormalTangentColorTextureSkinning(
//		FXMVECTOR iposition,
//		FXMVECTOR inormal,
//		FXMVECTOR itangent,
//		CXMVECTOR icolor,
//		CXMVECTOR itextureCoordinate,
//		XMUINT4 const& iindices,
//		CXMVECTOR iweights)
//		: VertexTypePositionNormalTangentColorTexture(iposition, inormal, itangent, icolor, itextureCoordinate),
//		indices{},
//		weights{}
//	{
//		SetBlendIndices(iindices);
//		SetBlendWeights(iweights);
//	}
//
//	void __cdecl SetBlendIndices(XMUINT4 const& iindices);
//
//	void __cdecl SetBlendWeights(XMFLOAT4 const& iweights) { SetBlendWeights(XMLoadFloat4(&iweights)); }
//	void XM_CALLCONV SetBlendWeights(FXMVECTOR iweights);
//
//	static const int InputElementCount = 7;
//	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
//};




