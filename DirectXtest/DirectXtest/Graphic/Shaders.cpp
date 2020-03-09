#include "Shaders.h"
#include <assert.h>
#include "../Engine/MemoryStream.h"



D3DVertexShader::D3DVertexShader(ID3D11Device* pDevice, const std::string& filename)
	:
	m_szName(filename)
{
	LoadFromFile(pDevice, filename, true);
}

D3DVertexShader::~D3DVertexShader()
{
}

ID3D11VertexShader* D3DVertexShader::GetShader(ID3D11Device* pDevice)
{
	if (IsDirty())
	{
		LoadFromFile(pDevice, m_szName, false);

		SetDirty(false);
	}

	return m_pShader.Get();
}

void D3DVertexShader::CreateInputLayoutDescFromVertexShaderSignature(ID3D11Device* pDevice, const void* pCodeBytes, const size_t size)
{
	// Reflect shader info 
	ComPtr<ID3D11ShaderReflection> pVertexShaderReflection;

	COM_ERROR_IF_FAILED(
		D3DReflect(pCodeBytes, size, IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection),
		"Failed to get d3dreflect"
	);
	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = i;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		auto attribute = AttributeInfo::SemanticToAttribute(paramDesc.SemanticName);
		if (attribute == AttributeInfo::VertexAttribute::Invalid)
		{
			COM_ERROR_IF_FAILED(-1, "Unknown semantic type");
			return;
		}
		m_bUsesAttribute[(int)attribute] = true;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	pDevice->CreateInputLayout(inputLayoutDesc.data(), (UINT)inputLayoutDesc.size(), pCodeBytes, size, &m_pInputLayout);
}

void D3DVertexShader::LoadFromFile(ID3D11Device* pDevice, const std::string& filename, bool crash_on_error)
{
	for (int i = 0; i < (int)AttributeInfo::VertexAttribute::TotalAttributes; i++)
	{
		m_bUsesAttribute[i] = false;
	}

	// compiled shader object
	if (StringHelper::GetFileExtension(filename) == "cso")
	{
		auto bytes = MemoryStream::FromFile(filename);

		COM_ERROR_IF_FAILED(pDevice->CreateVertexShader(bytes.Base(), bytes.Size(), NULL, &m_pShader), "Failed to Create Shader");
		CreateInputLayoutDescFromVertexShaderSignature(pDevice, bytes.Base(), bytes.Size());
	}
	// not compiled, lets compile ourselves
	else
	{
		ComPtr<ID3DBlob> errorMessage;
		ComPtr<ID3DBlob> vertexShaderBuffer;

		UINT flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		COM_ERROR_IF_FAILED(D3DCompileFromFile(StringHelper::StringToWide(filename).c_str(), NULL, NULL, "main", "vs_5_0", flags, 0, &vertexShaderBuffer, &errorMessage),
			"Failed to compile shader");


		COM_ERROR_IF_FAILED(pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pShader), "Failed to create shader");
		CreateInputLayoutDescFromVertexShaderSignature(pDevice, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize());
	}
}

void D3DVertexShader::OnFileChanged(const std::string & filename)
{
	SetDirty(true);
}






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
bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderPath)
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

	return true;
	//// Create the texture sampler state.
	//result = device->CreateSamplerState(samplerDesc, m_samplerState.GetAddressOf());
	//if (FAILED(result)) 
	//{
	//	return false;
	//}
	//return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->m_pixelShader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->m_shaderBuffer.Get();
}
