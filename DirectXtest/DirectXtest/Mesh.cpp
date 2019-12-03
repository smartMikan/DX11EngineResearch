#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, 
	ID3D11DeviceContext* deviceContext, 
	std::vector<VertexType>& vertices, 
	std::vector<DWORD>& indices, 
	std::vector<Texture>& textures, 
	const DirectX::XMMATRIX& transformMatrix
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

	
}

Mesh::Mesh(const Mesh& other)
{
	this->m_deviceContext = other.m_deviceContext;
	this->m_indexBuffer = other.m_indexBuffer;
	this->m_vertexBuffer = other.m_vertexBuffer;
	this->m_textures = other.m_textures;
	this->transformMatrix = other.transformMatrix;
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

const DirectX::XMMATRIX& Mesh::GetTransformMatrix()
{
	return this->transformMatrix;
}

