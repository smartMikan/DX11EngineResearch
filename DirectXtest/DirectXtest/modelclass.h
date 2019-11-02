#pragma once
<<<<<<< HEAD
=======
#pragma once
>>>>>>> origin/master
////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
<<<<<<< HEAD

using namespace DirectX;

#include "textureclass.h"
=======
using namespace DirectX;

>>>>>>> origin/master

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	//Here is the definition of our vertex type that will be used with the vertex buffer in this ModelClass.Also take note that this typedef must match the layout in the ColorShaderClass that will be looked at later in the tutorial.

<<<<<<< HEAD
	//struct VertexType
	//{
	//	XMFLOAT3 position;
	//	XMFLOAT4 color;
	//};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
=======
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
>>>>>>> origin/master
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	//The functions here handle initializing and shutdown of the model's vertex and index buffers. The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.

<<<<<<< HEAD
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*);
=======
	bool Initialize(ID3D11Device*);
>>>>>>> origin/master
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
<<<<<<< HEAD
	ID3D11ShaderResourceView* GetTexture();
=======
>>>>>>> origin/master

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	//The private variables in the ModelClass are the vertex and index buffer as well as two integers to keep track of the size of each buffer.Note that all DirectX 11 buffers generally use the generic ID3D11Buffer type and are more clearly identified by a buffer description when they are first created.

<<<<<<< HEAD
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
=======
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
>>>>>>> origin/master
};

#endif
