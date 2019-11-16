#pragma once

#include <d3d11.h>
#include <iostream>
#include <memory>

template<class T>
class VertexBuffer
{
private:
	VertexBuffer(const VertexBuffer<T>& rhs);

private:
	ID3D11Buffer* m_buffer;
	std::unique_ptr<UINT> m_stride;
	UINT m_bufferSize = 0;
public:
	VertexBuffer() {}
	~VertexBuffer() {}
	
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

	const UINT Stride() const 
	{
		return *this->m_stride.get();
	}
	const UINT* StridePtr() const 
	{
		return this->m_stride.get();
	}

	HRESULT Initialize(ID3D11Device* device, T * data, UINT m_vertexCount) 
	{
		this->m_bufferSize = m_vertexCount;
		this->m_stride = std::make_unique<UINT>(sizeof(T));

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_buffer);
		return hr;
	}

	void Release() 
	{
		if (m_stride) 
		{
			m_stride.release();
		}
		m_stride = nullptr;
		if (m_buffer) 
		{
			m_buffer->Release();
		}
		m_buffer = nullptr;
		return;
	}

};


