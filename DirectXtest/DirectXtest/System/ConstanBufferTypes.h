#pragma once
#include <directxmath.h>

struct CB_VS_vertexshader
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct CB_PS_pixelshader
{
	DirectX::XMMATRIX matrix;
};