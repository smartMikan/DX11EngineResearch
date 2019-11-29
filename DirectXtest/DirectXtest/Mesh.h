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

using namespace DirectX;
using namespace MyVertex;

class Mesh
{
public: 
	struct VertexType : public MyVertex::VertexTypePositionNormalTangentColorTextureSkinning {};

public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexType>& vertices, std::vector<DWORD>& indices,std::vector<Texture>& textures,const DirectX::XMMATRIX& transformMatrix);
	Mesh(const Mesh& other);
	void Draw();
	int GetIndexSize();
	const DirectX::XMMATRIX& GetTransformMatrix();

private:
	VertexBuffer<VertexType> m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	ID3D11DeviceContext* m_deviceContext;
	std::vector<Texture> m_textures;
	DirectX::XMMATRIX transformMatrix;
};

