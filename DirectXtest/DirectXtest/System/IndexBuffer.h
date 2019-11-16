#pragma once

#include <d3d11.h>
#include <iostream>
#include <memory>


class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);

private:
	ID3D11Buffer* m_buffer;
	UINT m_bufferSize = 0;
public:
	IndexBuffer() {}
	~IndexBuffer() {}

	ID3D11Buffer* Get()const
	{
		return m_buffer;
	}

	ID3D11Buffer* const* GetAddress()const
	{
		return &m_buffer;
	}

	UINT BufferSize() const
	{
		return this->m_bufferSize;
	}


	HRESULT Initialize(ID3D11Device* device, DWORD * data, UINT m_indicesCount)
	{
		this->m_bufferSize = m_indicesCount;
		
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * m_indicesCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData;
		ZeroMemory(&indexBufferData, sizeof(indexBufferData));
		indexBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_buffer);
		return hr;
	}

	void Release()
	{
		if (m_buffer) 
		{
			m_buffer->Release();
		}
		m_buffer = nullptr;
		return;
	}

};


