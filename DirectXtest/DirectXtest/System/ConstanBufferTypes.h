#pragma once
#include <directxmath.h>

struct CB_VS_MatrixBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct CB_VS_SkinBoneBumpMapMatrixBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX worldInvTranspose;
};

struct CB_VS_SkinBoneBumpMapMatrixTransformBuffer
{
	DirectX::XMMATRIX* boneTranforms;
};


struct CB_VS_Light_CameraBuffer
{
	DirectX::XMFLOAT3 cameraPosition; //12
	float padding; //4
};


struct CB_PS_pixelshader
{
	DirectX::XMMATRIX matrix;
};

struct CB_PS_LightBuffer
{
	XMFLOAT4 ambientColor; //16
	XMFLOAT4 diffuseColor; //16
	XMFLOAT3 lightDirection; //12
	//float padding;
	float specularPower; //4
	XMFLOAT4 specularColor; //16
};

struct CB_PS_SkinBoneBumpMapMaterialBuffer
{
	XMFLOAT4 ambientColor; //16
	XMFLOAT4 diffuseColor; //16
	XMFLOAT4 SpecularColor; //16
};

struct CB_PS_SkinBoneBumpMapCameraBuffer
{
	DirectX::XMFLOAT3 cameraPosition; //12
	float padding; //4
};

struct CB_PS_SkinBoneBumpMapLightBuffer
{
	XMFLOAT4 ambientColor; //16
	XMFLOAT4 diffuseColor; //16
	XMFLOAT3 lightDirection; //12
	float padding;
};