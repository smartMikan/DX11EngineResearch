#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "../Buffers/ConstantBuffer.h"
using namespace DirectX;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyCubeShaderClass
////////////////////////////////////////////////////////////////////////////////
class SkyCubeShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	SkyCubeShaderClass();
	SkyCubeShaderClass(const SkyCubeShaderClass&);
	~SkyCubeShaderClass();

	bool Initialize(ID3D11Device*, HWND, ID3D11DeviceContext* deviceContext);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView* texture);

private:
	bool InitializeShader(ID3D11Device*, ID3D11DeviceContext* deviceContext, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	//ID3D11Buffer* m_matrixBuffer; 
	//ID3D11Buffer* m_colorBuffer;
	ConstantBuffer<MatrixBufferType>* m_matrixBuffer;
};

