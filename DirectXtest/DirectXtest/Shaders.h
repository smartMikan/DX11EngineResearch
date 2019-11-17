#pragma once
#include"Utility/ErrorLoger.h"
#include<d3dcompiler.h>
#include<d3d11.h>
#pragma comment(lib,"D3DCompiler.lib")

class VertexShader
{
public:
	VertexShader();
	~VertexShader();

	bool Initialize(ID3D11Device* device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutdesc,UINT elementCount);
	void Shutdown();
	ID3D11VertexShader* GetShder();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout() {
		return this->m_inputlayout;
	}
	

private:
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D10Blob* m_shaderBuffer = nullptr;
	ID3D11InputLayout* m_inputlayout = nullptr;
};


