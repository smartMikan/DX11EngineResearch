#include "GameObjectClass.h"

GameObjectClass::GameObjectClass()
{
	m_Position = 0;
}

GameObjectClass::GameObjectClass(const GameObjectClass& other)
{
}

GameObjectClass::~GameObjectClass()
{
}

bool GameObjectClass::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;
	
	result = m_Model.Initialize(filePath, device, deviceContext);
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
	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}
}

void GameObjectClass::Draw(ShaderManagerClass* shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor, float time)
{
	m_Model.Draw(shaderManager, worldMatrix, viewMatrix, projectionMatrix, cameraPosition, ambientColor, diffuseColor, lightDirection, specularPower, specularColor,time);
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




