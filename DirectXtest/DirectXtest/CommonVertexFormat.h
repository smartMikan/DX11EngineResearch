#pragma once
#ifndef _COMMON_VERTEX_FORMAT_H
#define _COMMON_VERTEX_FORMAT_H

#include<Windows.h>

#include<directxmath.h>
using namespace DirectX;

namespace M3d {
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 Tex;
		XMFLOAT3 normal;
		XMFLOAT4 color;
	};

	struct VertexPos
	{
		float x, y, z;
	};

	struct PosNormalTexTanVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
	};

	struct PosNormalTexTanSkinnedVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
		float Weights[3];
		UINT BoneIndices[4];
	};

	struct Material
	{
		Material() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular; // w = SpecPower
		XMFLOAT4 Reflect;
	};

}


#endif 