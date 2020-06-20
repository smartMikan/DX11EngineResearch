#pragma once


#include <directxmath.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include"Vertex.h"
#include"Buffers/ConstantBuffer.h"
#include"Buffers/VertexBuffer.h"
#include"Buffers/IndexBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "../../Engine/ResourceManager.h"
#include "../Engine/Component.h"


struct MeshParameters
{
	std::vector<DirectX::XMFLOAT3> position;
	std::vector<DirectX::XMFLOAT4> color;
	std::vector<DirectX::XMFLOAT3> normal;
	std::vector<DirectX::XMFLOAT2> texcoord;
	std::vector<DirectX::XMFLOAT3> tangent;
	std::vector<DirectX::XMFLOAT3> bitangent;
	std::vector<DirectX::XMUINT4> bone_indice;
	std::vector<DirectX::XMFLOAT4> bone_weights;
};


class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_PS_Material>& cb_ps_material,MeshParameters& params, std::vector<DWORD>& indices,Material& material,const DirectX::XMMATRIX& transformMatrix);
	Mesh(const Mesh& other);

	void Bind(IVertexShader* pVertexShader) const;
	void BindMaterial() const;

	void SetData(MeshParameters& params);

	void Draw(IVertexShader* pVertexShader);

	int GetIndexSize();
	const DirectX::XMMATRIX& GetTransformMatrix();
	bool HasTangentsAndBitangents() const { return m_Tangent.VertexCount() > 0; }

	
private:

	VertexBuffer<DirectX::XMFLOAT3> m_Position;
	VertexBuffer<DirectX::XMFLOAT4> m_Color;
	VertexBuffer<DirectX::XMFLOAT3> m_Normal;
	VertexBuffer<DirectX::XMFLOAT2> m_TexCoord;
	VertexBuffer<DirectX::XMFLOAT3> m_Tangent;
	VertexBuffer<DirectX::XMFLOAT3> m_Bitangent;
	VertexBuffer<DirectX::XMUINT4> m_BoneIndice;
	VertexBuffer<DirectX::XMFLOAT4> m_BoneWeights;
	IndexBuffer m_indexBuffer;

	ConstantBuffer<CB_PS_Material>* cb_ps_material;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	Material material;

	DirectX::XMMATRIX transformMatrix;
};