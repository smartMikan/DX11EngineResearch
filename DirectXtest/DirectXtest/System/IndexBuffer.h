#pragma once

#include <d3d11.h>
#include <iostream>
#include<wrl/client.h>
#include <memory>


class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_bufferSize = 0;
public:
	IndexBuffer() {}

	ID3D11Buffer* Get()const
	{
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddress()const
	{
		return m_buffer.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return this->m_bufferSize;
	}


	HRESULT Initialize(ID3D11Device* device, DWORD * data, UINT m_indicesCount)
	{
		if (m_buffer.Get() != nullptr) {
			m_buffer.Reset();
		}
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

		HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, m_buffer.GetAddressOf());
		return hr;
	}

};


