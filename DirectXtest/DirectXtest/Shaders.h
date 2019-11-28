#pragma once
#include "Utility/ErrorLoger.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl/client.h>
#pragma comment(lib,"D3DCompiler.lib")

class VertexShader
{
public:
	bool Initialize(ID3D11Device* device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutdesc,UINT elementCount);
	ID3D11VertexShader* GetShder();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout() {
		return this->m_inputlayout.Get();
	}
	

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputlayout = nullptr;
};

class PixelShader
{
public:
	bool Initialize(ID3D11Device* device, std::wstring shaderPath, D3D11_SAMPLER_DESC* samplerDesc);
	ID3D11PixelShader* GetShder();
	ID3D10Blob* GetBuffer();
	ID3D11SamplerState* GetSamplerState() {
		return this->m_samplerState.Get();
	}
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState = nullptr;
};
