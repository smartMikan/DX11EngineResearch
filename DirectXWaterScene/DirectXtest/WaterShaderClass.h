#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include <fstream>
#include "Loader/DDSTextureLoader.h"
#include "Loader/WICTextureLoader.h"


using namespace std;
using namespace DirectX;

class WaterShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	//We have two new structures for the new reflection and water constant buffers that are inside the vertex and pixel shader.
	
	struct ReflectionBufferType
	{
		XMMATRIX reflection;
	};

	struct WaterBufferType
	{
		float waterTranslation;
		float reflectRefractScale;
		XMFLOAT2 padding;
	};

public:
	WaterShaderClass();
	WaterShaderClass(const WaterShaderClass& other);
	~WaterShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX reflectionMatrix, ID3D11ShaderResourceView* reflectionTexture,
		ID3D11ShaderResourceView* refractionTexture, ID3D11ShaderResourceView* normalTexture, float waterTranslation, float reflectRefractScale);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX reflectionMatrix, ID3D11ShaderResourceView* reflectionTexture,
		ID3D11ShaderResourceView* refractionTexture, ID3D11ShaderResourceView* normalTexture, float waterTranslation, float reflectRefractScale);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;

	//We have two new buffers for the new reflectionand water constant buffers that are inside the vertexand pixel shader.
	ID3D11Buffer* m_reflectionBuffer;
	ID3D11Buffer* m_waterBuffer;
};

