#include "../System/Vertex.h"
using namespace MyVertex;

const D3D11_INPUT_ELEMENT_DESC VertexTypePositionNormalTangentColorTexture::InputElements[] =
{
	{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexTypePositionNormalTangentColorTexture) == 52, "Vertex struct/layout mismatch");

void XM_CALLCONV VertexTypePositionNormalTangentColorTexture::SetColor(FXMVECTOR icolor)
{
	XMUBYTEN4 rgba;
	XMStoreUByteN4(&rgba, icolor);
	this->color = rgba.v;
}


const D3D11_INPUT_ELEMENT_DESC VertexTypePositionNormalTangentColorTextureSkinning::InputElements[] =
{
	{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(VertexTypePositionNormalTangentColorTextureSkinning::InputElementCount == VertexTypePositionNormalTangentColorTexture::InputElementCount + 2, "layout mismatch");

static_assert(sizeof(VertexTypePositionNormalTangentColorTextureSkinning) == 60, "Vertex struct/layout mismatch");

void VertexTypePositionNormalTangentColorTextureSkinning::SetBlendIndices(XMUINT4 const& iindices)
{
	this->indices = ((iindices.w & 0xff) << 24) | ((iindices.z & 0xff) << 16) | ((iindices.y & 0xff) << 8) | (iindices.x & 0xff);
}

void XM_CALLCONV VertexTypePositionNormalTangentColorTextureSkinning::SetBlendWeights(FXMVECTOR iweights)
{
	XMUBYTEN4 packed;
	XMStoreUByteN4(&packed, iweights);
	this->weights = packed.v;
}

