#pragma once

#include "Mesh.h"
#include "ShaderManagerClass.h"
#include "SkinnedDataClass.h"
#include <list>
using namespace DirectX;
using namespace MyVertex;

class Model
{
public: 
	struct Bone {
		string Name;
		// Bind space transform
		XMMATRIX Offset;
		// local matrix transform
		XMMATRIX LocalTransform;
		// To-root transform
		XMMATRIX GlobalTransform;
		// copy of the original local transform
		XMMATRIX OriginalLocalTransform;
		// parent bone reference
		Bone Parent;
		// child bone references
		std::list<Bone> Children;
		Bone() {
			Children = new list<Bone>();
		}
	};


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


	void ProcessAnimation(vector<AnimationClip>& animations,aiAnimation** aianimation);

private:

	std::vector<Mesh> m_meshes;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	std::string directory = "";
	const aiScene* modelScene;
	std::vector<AnimationClip> m_animation;

	SkinnedDataClass m_skindata;

	vector<int>& bongHierarchy;
	vector<XMFLOAT4X4>& boneOffsets;
	map<string, AnimationClip>& animations;

};


//https://sourceforge.net/p/assimp/discussion/817654/thread/5462cbf5/
//http://www.richardssoftware.net/2013/10/skinned-models-in-directx-11-with.html
//https://sourceforge.net/p/assimp/discussion/817654/thread/a7bf155b/
//https://gamedev.stackovernet.com/ja/q/6807