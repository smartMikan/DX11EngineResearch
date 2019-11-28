#include "Shaders.h"


bool VertexShader::Initialize(ID3D11Device * device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutdesc, UINT elementCount)
{
	HRESULT result = D3DReadFileToBlob(shaderPath.c_str(), this->m_shaderBuffer.GetAddressOf());
	if (FAILED(result)) 
	{
		std::wstring errorMsg = L"Filed to load shader: ";
		errorMsg += shaderPath;
		ErrorLoger::Log(result, errorMsg);
		return false;
	}

	result = device->CreateVertexShader(this->m_shaderBuffer->GetBufferPointer(), this->m_shaderBuffer->GetBufferSize(), nullptr, this->m_vertexShader.GetAddressOf());
	if (FAILED(result)) 
	{
		std::wstring errorMsg = L"Filed to create vertex shader: ";
		errorMsg += shaderPath;
		ErrorLoger::Log(result, errorMsg);
		return false;
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(layoutdesc, elementCount, m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(),
		m_inputlayout.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	return true;
}


ID3D11VertexShader * VertexShader::GetShder()
{
	return this->m_vertexShader.Get();
}

ID3D10Blob * VertexShader::GetBuffer()
{
	return this->m_shaderBuffer.Get();
}


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
bool PixelShader::Initialize(ID3D11Device* device, std::wstring shaderPath, D3D11_SAMPLER_DESC* samplerDesc)
{

	HRESULT result = D3DReadFileToBlob(shaderPath.c_str(), this->m_shaderBuffer.GetAddressOf());
	if (FAILED(result)) 
	{
		std::wstring errorMsg = L"Filed to load shader: ";
		errorMsg += shaderPath;
		ErrorLoger::Log(result, errorMsg);
		return false;
	}

	result = device->CreatePixelShader(this->m_shaderBuffer->GetBufferPointer(), this->m_shaderBuffer->GetBufferSize(), NULL, this->m_pixelShader.GetAddressOf());
	if (FAILED(result)) 
	{
		std::wstring errorMsg = L"Filed to create pixel shader: ";
		errorMsg += shaderPath;
		ErrorLoger::Log(result, errorMsg);
		return false;
	}


	// Create the texture sampler state.
	result = device->CreateSamplerState(samplerDesc, m_samplerState.GetAddressOf());
	if (FAILED(result)) 
	{
		return false;
	}
	return true;
}

ID3D11PixelShader* PixelShader::GetShder()
{
	return this->m_pixelShader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->m_shaderBuffer.Get();
}
