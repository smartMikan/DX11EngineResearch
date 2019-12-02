#pragma once
#include <vector>
#include"System/Vertex.h"
#include"System/ConstantBuffer.h"
#include"System/VertexBuffer.h"
#include"System/IndexBuffer.h"
#include <directxmath.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Texture.h"
#include "SkinnedDataClass.h"

using namespace DirectX;
using namespace MyVertex;

class Mesh
{
public: 
	struct VertexType : public MyVertex::VertexTypePositionNormalTangentColorTextureSkinning {};

public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexType>& vertices, std::vector<DWORD>& indices,std::vector<Texture>& textures,const DirectX::XMMATRIX& transformMatrix, vector<int>& bongHierarchy, vector<XMFLOAT4X4>& boneOffsets, map<string, AnimationClip>& animations);
	Mesh(const Mesh& other);
	void Draw();
	int GetIndexSize();
	const DirectX::XMMATRIX& GetTransformMatrix();
	
	SkinnedDataClass m_skindata;

private:
	VertexBuffer<VertexType> m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	ID3D11DeviceContext* m_deviceContext;
	std::vector<Texture> m_textures;
	DirectX::XMMATRIX transformMatrix;
	
};

