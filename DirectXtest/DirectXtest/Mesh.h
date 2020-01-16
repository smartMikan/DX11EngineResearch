#pragma once
#include <vector>
#include<map>
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

//#define NUM_BONES_PER_VEREX 4
	struct VertexType : public MyVertex::VertexTypePositionNormalTangentColorTexture {};
//
//	struct BoneInfo
//	{
//		XMMATRIX BoneOffset;
//		XMMATRIX FinalTransformation;
//
//		BoneInfo()
//		{
//			BoneOffset = XMMatrixIdentity();
//			FinalTransformation = XMMatrixIdentity();
//		}
//	};
//
//	struct VertexBoneData
//	{
//		UINT IDs[NUM_BONES_PER_VEREX];
//		float Weights[NUM_BONES_PER_VEREX];
//
//		VertexBoneData()
//		{
//			Reset();
//		};
//
//		void Reset()
//		{
//			ZeroMemory(&IDs, sizeof(IDs));
//			ZeroMemory(&Weights, sizeof(Weights));
//		}
//
//		void AddBoneData(UINT BoneID, float Weight);
//	};
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexType>& vertices, std::vector<DWORD>& indices,std::vector<Texture>& textures,const DirectX::XMMATRIX& transformMatrix);
	Mesh(const Mesh& other);
	void Draw();
	int GetIndexSize();
	int GetVertexSize();
	//UINT GetNumBones() const {
	//	return m_NumBones;
	//}
	const DirectX::XMMATRIX& GetTransformMatrix();
	//void LoadBones(UINT MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);

	//std::vector<VertexBoneData> m_Bones;
	//std::vector<BoneInfo> m_BoneInfo;
	//std::map<std::string, UINT> m_BoneMapping; // maps a bone name to its index

private:

	VertexBuffer<VertexType> m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	ID3D11DeviceContext* m_deviceContext;
	std::vector<Texture> m_textures;
	DirectX::XMMATRIX transformMatrix;
	UINT m_NumBones = 0;
};

