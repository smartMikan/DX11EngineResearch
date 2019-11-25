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
using namespace DirectX;

class Mesh
{

public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexType>& vertices, std::vector<DWORD>& indices);
	Mesh(const Mesh& other);
	void Draw();
	int GetIndexSize();

private:
	VertexBuffer<VertexType> m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	ID3D11DeviceContext* m_deviceContext;
};

