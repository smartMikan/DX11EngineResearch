#pragma once

#include <d3d11.h>
#include <iostream>
#include<wrl/client.h>
#include <memory>

template<class T>
class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	std::shared_ptr<UINT> m_stride;
	UINT m_bufferSize = 0;
public:
	VertexBuffer() {}
	VertexBuffer(const VertexBuffer<T>& rhs) {
		this->m_buffer = rhs.m_buffer;
		this->m_bufferSize = rhs.m_bufferSize;
		this->m_stride = rhs.m_stride;
	}
	
	VertexBuffer<T>& operator = (const VertexBuffer<T>& a){
		this->m_buffer = a.m_buffer;
		this->m_bufferSize = a.m_bufferSize;
		this->m_stride = a.m_stride;
		return *this;
	}
	
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
		if (m_buffer.Get() != nullptr) {
			m_buffer.Reset();
		}
		this->m_bufferSize = m_vertexCount;

		if (this->m_stride.get() == nullptr) {
			this->m_stride = std::make_shared<UINT>(sizeof(T));
		}
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->m_buffer.GetAddressOf());
		return hr;
	}

};


