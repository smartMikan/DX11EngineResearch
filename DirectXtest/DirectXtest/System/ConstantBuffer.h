#pragma once

#include <d3d11.h>
#include <iostream>
#include <memory>
#include "..//Utility/ErrorLoger.h"

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

private:
	ID3D11Buffer* m_buffer;
	ID3D11DeviceContext* deviceContext = nullptr;


public:
	ConstantBuffer() {}
	~ConstantBuffer() {}

	T data;

	ID3D11Buffer* Get()const
	{
		return m_buffer;
	}

	ID3D11Buffer* const* GetAddress()const
	{
		return &m_buffer;
	}



	HRESULT Initialize(ID3D11Device* device)
	{
		if (m_buffer != nullptr) {
			Release();
		}
		//this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));

		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		constantBufferDesc.StructureByteStride = 0;


		HRESULT hr = device->CreateBuffer(&constantBufferDesc, 0, &m_buffer);
		return hr;
	}

	HRESULT ApplyChanges(ID3D11DeviceContext* deviceContext)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = deviceContext->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr)) {
			ErrorLoger::Log(hr, "Failed to map constant buffer");
			return hr;
		}
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		deviceContext->Unmap(m_buffer, 0);
		return hr;
	}


	void Release()
	{
		if (m_buffer) 
		{
			m_buffer->Release();
		}
		m_buffer = nullptr;
		if (deviceContext) 
		{
			deviceContext->Release();
		}
		deviceContext = nullptr;
		return;
	}

};


