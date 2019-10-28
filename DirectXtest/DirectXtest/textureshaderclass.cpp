#include "textureshaderclass.h"



TextureShaderClass::TextureShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;

	m_sampleState = 0;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass &)
{
}


TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device *device, HWND hwnd)
{
	bool result;

	//Initializethe vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"TextureVertexShader.hlsl", L"TexturePixelShader.hlsl");
	if (!result) {
		return false;
	}
	return true;
}

void TextureShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;

	//Set the shader parameters that it will use for rendering.
	//result = SetShaderParameters();

	return false;
}


bool TextureShaderClass::InitializeShader(ID3D11Device *, HWND, const WCHAR *, const WCHAR *)
{
	return false;
}

void TextureShaderClass::ShutdownShader()
{
}

void TextureShaderClass::OutputShaderErrorMessage(ID3D11DeviceContext *, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView *)
{
}

void TextureShaderClass::SetShaderParameters(ID3D11DeviceContext *, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView *)
{
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext *, int)
{
}
