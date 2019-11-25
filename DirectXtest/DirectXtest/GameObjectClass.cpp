#include "GameObjectClass.h"

GameObjectClass::GameObjectClass()
{
	m_Position = 0;
	m_Model = 0;
}

GameObjectClass::GameObjectClass(const GameObjectClass& other)
{
}

GameObjectClass::~GameObjectClass()
{
}

bool GameObjectClass::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* textureFilename1, const WCHAR* textureFilename2, const WCHAR* textureFilename3)
{
	bool result;
	m_Model = new ModelClass();
	if (!m_Model) {
		return false;
	}
	result = m_Model->Initialize(filePath, device, deviceContext, textureFilename1, textureFilename2, textureFilename3);
	if (result == false) {
		return false;
	}
	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation.
	m_Position->SetPosition(128.0f, 10.0f, 128.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	return true;
}

void GameObjectClass::Shutdown()
{
	// Release the model object.
	if (m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}
}

void GameObjectClass::RenderMesh(int meshNumber)
{
	m_Model->RenderMesh(meshNumber);
}

XMMATRIX GameObjectClass::GetWorldMatrix()
{
	return worldPosition;
}

bool GameObjectClass::SetWorldMatrix(XMMATRIX world)
{
	worldPosition = world;
	return true;
}




