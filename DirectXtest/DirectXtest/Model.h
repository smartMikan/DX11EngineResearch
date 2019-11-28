#pragma once

#include "Mesh.h"
#include "ShaderManagerClass.h"
using namespace DirectX;
using namespace MyVertex;

class Model
{
public:
	Model();
	~Model();
	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	
	void Draw(ShaderManagerClass * shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor);
	bool LoadModel(const std::string& filePath);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix);
	std::vector<Texture>LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, const aiScene* scene);
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* mat, unsigned int index, aiTextureType textureType);
	int GetTextureIndex(aiString* pStr);

private:

	std::vector<Mesh> m_meshes;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	std::string directory = "";
};

