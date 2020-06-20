#pragma once


#include "../../Engine/d3dclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "fontshaderclass.h"
#include "terrainshaderclass.h"
#include "skydomeshaderclass.h"
#include "skycubeshaderclass.h"
#include "particleshaderclass.h"
#include "shadowshaderclass.h"
#include "depthshaderclass.h"


#include "../Buffers/ConstantBuffer.h"
#include "../Buffers/ConstanBufferTypes.h"

class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND, ID3D11DeviceContext*);
	bool InitializeMyShader(ID3D11Device*, std::wstring vertexShaderCsoPath, std::wstring pixelShaderCsoPath);
	void Shutdown();

	bool RenderColorShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);
	bool RenderTextureShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	
	//bool RenderLightShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	bool RenderLightShader(ID3D11DeviceContext*deviceContext, int indexCount, ID3D11ShaderResourceView ** textureArray, XMMATRIX world, XMMATRIX view, XMMATRIX projection, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor);

	bool RenderFontShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	bool RenderSkyDomeShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4, XMFLOAT4);
	bool RenderSkyCubeShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	bool RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView * normalMap, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor);
	bool RenderParticleShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

	bool RenderShadowShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix,
		ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture, XMFLOAT3 lightDirection,
		XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor);

	bool RenderDepthShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);
	bool RenderDepthShader(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);

	void DrawSetWithMyShader(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, XMFLOAT4 materialAmbientColor, XMFLOAT4 materialDiffuseColor, XMFLOAT4 SpecularColor);

private:
	ColorShaderClass * m_ColorShader;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	FontShaderClass* m_FontShader;
	SkyDomeShaderClass* m_SkyDomeShader;
	SkyCubeShaderClass* m_SkyCubeShader;
	TerrainShaderClass* m_TerrainShader;
	ParticleShaderClass* m_ParticleShader;
	ShadowShaderClass* m_ShadowShader;
	DepthShaderClass* m_DepthShader;



	ConstantBuffer<CB_VS_MatrixBuffer> m_MatrixBuffer;
	//ConstantBuffer<CB_VS_Light_CameraBuffer> m_Light_CameraBuffer;
	ConstantBuffer<CB_PS_LightBuffer> m_LightBuffer;

	//ConstantBuffer<CB_VS_SkinBoneBumpMapMatrixBuffer> m_BoneMatrixBuffer;
	//ConstantBuffer<CB_VS_SkinBoneBumpMapMatrixTransformBuffer> m_BoneTransformBuffer;
	//ConstantBuffer<CB_PS_SkinBoneBumpMapMaterialBuffer> m_BoneMaterialBuffer;
	//ConstantBuffer<CB_PS_SkinBoneBumpMapCameraBuffer> m_BoneCameraBuffer;
	//ConstantBuffer<CB_PS_SkinBoneBumpMapLightBuffer> m_BoneLightBuffer;

};
