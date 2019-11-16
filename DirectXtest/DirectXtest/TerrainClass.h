#pragma once


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include "System/VertexBuffer.h"
#include "System/IndexBuffer.h"
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	//ID3D11Buffer /** m_vertexBuffer,*/ *m_indexBuffer;
	IndexBuffer* m_indexBuffer;
	VertexBuffer<VertexType> *m_vertexBuffer;
	int m_vertexCount, m_indexCount;
};

