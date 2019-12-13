#pragma once

#include "Mesh.h"
#include "ShaderManagerClass.h"
#include "SkinnedDataClass.h"
#include <list>
#include <stdint.h>

using namespace DirectX;
using namespace MyVertex;

class Model
{

public:
	Model();
	~Model();
	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	
	void Draw(ShaderManagerClass * shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor, float time);
	bool LoadModel(const std::string& filePath);
	void BoneTransform(float TimeInSeconds, vector<XMMATRIX>& Transforms,Mesh* mesh);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix, UINT meshID);
	std::vector<Texture>LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, const aiScene* scene);
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* mat, unsigned int index, aiTextureType textureType);
	int GetTextureIndex(aiString* pStr);


	

	
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const XMMATRIX& ParentTransform, Mesh* mesh);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	UINT FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	UINT FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	UINT FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
	
	void ProcessAnimation(vector<AnimationClip>& animations, aiScene* modelScene);
	const aiNode* FindNodeRecursivelyByName(const aiNode* node, aiString nodeName);

private:

	std::vector<Mesh> m_meshes;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	std::string directory = "";
	//const aiScene* modelScene;
	std::vector<AnimationClip> m_animation;

	SkinnedDataClass m_skindata;

	vector<int> bongHierarchy;
	vector<XMFLOAT4X4> boneOffsets;
	map<string, AnimationClip> animations;

	




	const aiScene* m_pScene;
	Assimp::Importer m_Importer;

	XMMATRIX m_GlobalInverseTransform;
};


//https://sourceforge.net/p/assimp/discussion/817654/thread/5462cbf5/
//http://www.richardssoftware.net/2013/10/skinned-models-in-directx-11-with.html
//https://sourceforge.net/p/assimp/discussion/817654/thread/a7bf155b/
//https://gamedev.stackovernet.com/ja/q/6807