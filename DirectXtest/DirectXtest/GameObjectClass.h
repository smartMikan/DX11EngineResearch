#pragma once
#include "positionclass.h"
#include "modelclass.h"


class GameObjectClass
{
public:
	GameObjectClass();
	GameObjectClass(const GameObjectClass& other);
	~GameObjectClass();

	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* textureFilename1, const WCHAR* textureFilename2, const WCHAR* textureFilename3);
	void Shutdown();
	void RenderMesh(int meshNumber);

	XMMATRIX GetWorldMatrix();
	bool SetWorldMatrix(XMMATRIX world);

	PositionClass* m_Position;
	ModelClass* m_Model;

private:
	XMMATRIX worldPosition = XMMatrixIdentity();
};

