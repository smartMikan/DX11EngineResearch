#include "Mesh.h"

void Mesh::VertexBoneData::AddBoneData(UINT BoneID, float Weight)
{

	for (UINT i = 0; i < ARRAYSIZE(IDs); i++) {
		if (Weights[i] == 0.0) {
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}
	// should never get here - more bones than we have space for
	assert(0);
}

Mesh::Mesh(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	std::vector<VertexType>& vertices,
	std::vector<DWORD>& indices,
	std::vector<Texture>& textures,
	const DirectX::XMMATRIX& transformMatrix,
	UINT meshID,
	const aiMesh* aimesh
	)
{
	this->m_deviceContext = deviceContext;
	this->m_textures = textures;
	this->transformMatrix = transformMatrix;
	

	HRESULT result = this->m_vertexBuffer.Initialize(device, vertices.data(), vertices.size());
	if (FAILED(result)) {
		ErrorLoger::Log(result, "Failed to initialize vertex buffer for mesh.");
	}
	result = this->m_indexBuffer.Initialize(device, indices.data(), indices.size());
	if (FAILED(result)) {
		ErrorLoger::Log(result, "Failed to initialize index buffer for mesh.");
	}

	LoadBones(meshID,aimesh,m_Bones);
}

Mesh::Mesh(const Mesh& other)
{
	this->m_deviceContext = other.m_deviceContext;
	this->m_indexBuffer = other.m_indexBuffer;
	this->m_vertexBuffer = other.m_vertexBuffer;
	this->m_textures = other.m_textures;
	this->transformMatrix = other.transformMatrix;
	this->m_BoneInfo = other.m_BoneInfo;
	this->m_BoneMapping = other.m_BoneMapping;
	this->m_Bones = other.m_Bones;
	this->m_NumBones = other.m_NumBones;
}

void Mesh::Draw()
{
	unsigned int offset;

	// Set vertex buffer stride and offset.
	offset = 0;

	for (int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE) 
		{
			this->m_deviceContext->PSSetShaderResources(0, 1, m_textures[i].GetTextureResourceViewAddress());
			break;
		}
		if (m_textures[i].GetType() == aiTextureType::aiTextureType_NORMALS) 
		{
			this->m_deviceContext->PSSetShaderResources(1, 1, m_textures[i].GetTextureResourceViewAddress());
			break;
		}
	}

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddress(), m_vertexBuffer.StridePtr(), &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);


	m_deviceContext->DrawIndexed(this->m_indexBuffer.IndexCount(), 0, 0);

}

int Mesh::GetIndexSize()
{
	return m_indexBuffer.IndexCount();
}

int Mesh::GetVertexSize()
{
	return m_vertexBuffer.VertexCount();
}

const DirectX::XMMATRIX& Mesh::GetTransformMatrix()
{
	return this->transformMatrix;
}

void Mesh::LoadBones(UINT MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
	m_NumBones = 0;
	//Bones.reserve(pMesh->mNumBones);
	Bones.resize(pMesh->mNumVertices);
	for (UINT i = 0; i < pMesh->mNumBones; i++) {
		UINT BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			// Allocate an index for a new bone
			BoneIndex = m_NumBones;
			m_NumBones += 1;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
			//m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
			m_BoneInfo[BoneIndex].BoneOffset = XMMatrixTranspose(XMMATRIX(&pMesh->mBones[i]->mOffsetMatrix.a1));
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (UINT j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			//UINT VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			UINT VertexID = pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

