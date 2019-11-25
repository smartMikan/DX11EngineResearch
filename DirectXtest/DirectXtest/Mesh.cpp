#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexType>& vertices, std::vector<DWORD>& indices)
{
	this->m_deviceContext = deviceContext;
	this->m_vertexBuffer.Initialize(device, vertices.data(), vertices.size());
	this->m_indexBuffer.Initialize(device, indices.data(), indices.size());
}

Mesh::Mesh(const Mesh& other)
{
	this->m_deviceContext = other.m_deviceContext;
	this->m_indexBuffer = other.m_indexBuffer;
	this->m_vertexBuffer = other.m_vertexBuffer;
}

void Mesh::Draw()
{
	unsigned int offset;

	// Set vertex buffer stride and offset.
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddress(), m_vertexBuffer.StridePtr(), &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//m_deviceContext->DrawIndexed(this->m_indexBuffer.BufferSize(), 0, 0);

}

int Mesh::GetIndexSize()
{
	return m_indexBuffer.BufferSize();
}
