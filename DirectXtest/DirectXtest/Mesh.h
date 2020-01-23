#pragma once
#include <vector>
#include<map>
//#include"System/Vertex.h"
#include"System/ConstantBuffer.h"
#include"System/VertexBuffer.h"
#include"System/IndexBuffer.h"
#include <directxmath.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Texture.h"
#include"CommonVertexFormat.h"

using namespace DirectX;
//using namespace MyVertex;

class Mesh
{
	
public: 

	//struct VertexType : public MyVertex::VertexTypePositionNormalTangentColorTexture {};

	
	
public:
	//Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexType>& vertices, std::vector<DWORD>& indices,std::vector<Texture>& textures,const DirectX::XMMATRIX& transformMatrix);
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<PosNormalTexTanSkinnedVertex>& vertices, std::vector<DWORD>& indices,std::vector<Texture>& textures,const DirectX::XMMATRIX& transformMatrix);
	Mesh(const Mesh& other);
	void Draw();
	int GetIndexSize();
	int GetVertexSize();
	//UINT GetNumBones() const {
	//	return m_NumBones;
	//}
	const DirectX::XMMATRIX& GetTransformMatrix();
	
	std::vector<Texture> m_textures;
	
	

private:

	//VertexBuffer<VertexType> m_vertexBuffer;
	VertexBuffer<PosNormalTexTanSkinnedVertex> m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	ID3D11DeviceContext* m_deviceContext;
	
	DirectX::XMMATRIX transformMatrix;
};

