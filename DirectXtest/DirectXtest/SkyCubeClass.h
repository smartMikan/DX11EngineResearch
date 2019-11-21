#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include "System/VertexBuffer.h"
#include "System/IndexBuffer.h"
#include "Loader/WICTextureLoader.h"
using namespace DirectX;
using namespace std;


class SkyCubeClass
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		XMFLOAT3 position;

	};


public:
	SkyCubeClass();	


	bool Initialize(ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		const WCHAR* cubemapFilename,
		bool generateMips = false);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTextureCube();
	int GetIndexCount();


private:

	bool LoadSkyDomeModel(const WCHAR*);
	void ReleaseSkyDomeModel();

	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	HRESULT CreateWICTexture2DCubeFromFile(
		ID3D11Device* d3dDevice,
		ID3D11DeviceContext* d3dDeviceContext,
		const WCHAR* cubeMapFileName,
		ID3D11Texture2D** textureArray,
		ID3D11ShaderResourceView** textureCubeView,
		bool generateMips = false);


private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;

	ID3D11ShaderResourceView* m_TextureCubeSRV;

	ModelType* m_model;
	int m_vertexCount, m_indexCount;
};

