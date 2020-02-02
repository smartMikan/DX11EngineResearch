#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "Graphic/Buffers/ConstantBuffer.h"
using namespace DirectX;
using namespace std;

class TerrainShaderClass
{

private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};


public:
	TerrainShaderClass();
	TerrainShaderClass(const TerrainShaderClass& other);
	~TerrainShaderClass();


	bool Initialize(ID3D11Device* device, HWND hwnd, ID3D11DeviceContext* deviceContext);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView * normalmap, XMFLOAT3 lightdir, XMFLOAT4 diffuseColor);

private:
	bool InitializeShader(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const WCHAR* vsfilename, const WCHAR* psfilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* message, HWND hwnd,const WCHAR* filename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView * normalmap, XMFLOAT3 lightdir, XMFLOAT4 diffuseColor);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	//ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	//ID3D11Buffer* m_lightBuffer;
	ConstantBuffer<MatrixBufferType>* m_matrixBuffer;
	ConstantBuffer<LightBufferType>* m_lightBuffer;
};

