#pragma once

//The multitexture shader code is based on the TextureShaderClass with some slight modifications.

#ifndef _MULTITEXTURESHADERCLASS_H_
#define _MULTITEXTURESHADERCLASS_H_

//////////////
// INCLUDES //
//////////////

#include <d3d11.h>
#include <directxmath.h>
#include<d3dcompiler.h>
#include <fstream>

using namespace std;
using namespace DirectX;

class MultiTextureShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	MultiTextureShaderClass();
	MultiTextureShaderClass(const MultiTextureShaderClass& other);
	~MultiTextureShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, 
				XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, 
				ID3D11ShaderResourceView** textureArray);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* fileName);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif