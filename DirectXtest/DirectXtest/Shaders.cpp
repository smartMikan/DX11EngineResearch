#include "Shaders.h"

VertexShader::VertexShader()
{
}

VertexShader::~VertexShader()
{
}

bool VertexShader::Initialize(ID3D11Device * device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutdesc, UINT elementCount)
{
	HRESULT result = D3DReadFileToBlob(shaderPath.c_str(), &this->m_shaderBuffer);
	if (FAILED(result)) {
		std::wstring errorMsg = L"Filed to load shader: ";
		errorMsg += shaderPath;
		ErrorLoger::Log(result, errorMsg);
		return false;
	}

	result = device->CreateVertexShader(&this->m_shaderBuffer, sizeof(&this->m_shaderBuffer), NULL, &this->m_vertexShader);
	if (FAILED(result)) {
		std::wstring errorMsg = L"Filed to create vertex shader: ";
		errorMsg += shaderPath;
		ErrorLoger::Log(result, errorMsg);
		return false;
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(layoutdesc, elementCount, m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(),
		&m_inputlayout);
	if (FAILED(result))
	{
		return false;
	}

	if (m_shaderBuffer)
	{
		m_shaderBuffer->Release();
		delete  m_shaderBuffer;
		m_shaderBuffer = 0;
	}



	return true;
}

void VertexShader::Shutdown()
{
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		delete m_vertexShader;
		m_vertexShader = 0;
	}
	if (m_shaderBuffer)
	{
		m_shaderBuffer->Release();
		delete  m_shaderBuffer;
		m_shaderBuffer = 0;
	}
}

ID3D11VertexShader * VertexShader::GetShder()
{
	return this->m_vertexShader;
}

ID3D10Blob * VertexShader::GetBuffer()
{
	return this->m_shaderBuffer;
}