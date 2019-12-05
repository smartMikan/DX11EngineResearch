#pragma once
#ifndef  MESH_GEOMETRY_CLASS
#define MESH_GEOMETRY_CLASS

#include<Windows.h>
#include<iostream>
#include<vector>
#include<d3d11.h>
#include<D3Dcommon.h>  //???
//#include<D3DX11.h>
#include"Macro.h"

#include<directxmath.h>
using namespace DirectX;
using std::vector;

class MeshGeometryClass
{

public:
	//分组
	struct Subset
	{

		UINT Id;
		UINT VertexStart;
		UINT VertexCount;
		UINT FaceStart;
		UINT FaceCount;

		Subset() :Id(-1), VertexStart(0), VertexCount(0), FaceStart(0), FaceCount(0)
		{

		}
	};
private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	DXGI_FORMAT mIndexBufferFormat; //32位 对应于unsigned long
	UINT mVertexStride;  //顶点跨度
	vector<Subset> mSubsetTable;
	

private:
	MeshGeometryClass(const MeshGeometryClass& rhs);
	//MeshGeometryClass& operator=(const MeshGeometryClass& rhs);


public:

	MeshGeometryClass();
	~MeshGeometryClass();

public:
	template<typename VertexTypeA>
	bool SetVertices(ID3D11Device* device, const VertexTypeA*vertices, UINT count);

	bool SetIndices(ID3D11Device* device, const unsigned long* indices, UINT count);

	void SetSubsetTable(vector<Subset>& SubsetTable);

	void Draw(ID3D11DeviceContext* deviceContext, UINT subsetId);
};


template<typename VertexTypeA>
bool  MeshGeometryClass::SetVertices(ID3D11Device* device, const VertexTypeA*vertices, UINT count)
{
	//释放顶点缓存
	ReleaseCOM(mVertexBuffer);
	
	mVertexStride = sizeof(VertexTypeA);

	//填充(顶点)缓存形容结构体和子资源数据结构体,并创建顶点缓存
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeA) * count;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	HR(device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer));

	return true;
}
#endif // ! MESH_GEOMETRY_CLASS
