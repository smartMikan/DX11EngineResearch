#pragma once

#include<d3d11.h>
#include<d3dcompiler.h>
#include<directxmath.h>
#include<fstream>

using namespace DirectX;
using namespace std;



class FogShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		float padding1, padding2;
	};

public:
	FogShaderClass();
	FogShaderClass(const FogShaderClass&);
	~FogShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, float fogStart, float fogEnd);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND , const WCHAR* );
	
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, float fogStart, float fogEnd);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader * m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_fogBuffer;

	ID3D11SamplerState * m_sampleState;

};

