#pragma once
#include "positionclass.h"
#include "Model.h"


class GameObjectClass
{
public:
	GameObjectClass();
	GameObjectClass(const GameObjectClass& other);
	~GameObjectClass();

	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Shutdown();

	void Draw(ShaderManagerClass* shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor, float timepos, ID3D11ShaderResourceView* DiffuseMap,
		ID3D11ShaderResourceView* NormalMap);

	XMMATRIX GetWorldMatrix();
	bool SetWorldMatrix(XMMATRIX world);

	PositionClass* m_Position;
	Model m_Model;

private:
	XMMATRIX worldPosition = XMMatrixIdentity();
	
};

