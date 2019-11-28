#pragma once
#include <directxmath.h>

struct CB_VS_MatrixBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct CB_VS_Light_CameraBuffer
{
	DirectX::XMFLOAT3 cameraPosition;
	float padding;
};


struct CB_PS_pixelshader
{
	DirectX::XMMATRIX matrix;
};

struct CB_PS_LightBuffer
{
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	//float padding;
	float specularPower;
	XMFLOAT4 specularColor;
};