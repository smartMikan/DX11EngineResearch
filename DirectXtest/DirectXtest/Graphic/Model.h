#pragma once

#include <unordered_map>

#include "Vertex.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/ConstantBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "AssimpSkinnedData.h"
#include "../Engine/positionclass.h"

using namespace DirectX;
using namespace AssimpModel;

enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

class Model
{

public:

	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_MatrixBuffer>& wvpMatrix, ConstantBuffer<CB_PS_Material>& cb_ps_material, IVertexShader* pVertexShader);
	
	//void Update(float timepos);

	void Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix);
	bool InitAnimation(ConstantBuffer<ConstantBuffer_Bones>* cbufBone, Animator* animator_out, AnimationComponent* animComp);
	bool AddAnimation(const std::string& filePath,Animator* animator_out, AnimationComponent* animComp, bool disablerootTrans,bool disablerootRot, bool disablerootScale);


private:
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix);
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* mat, unsigned int index, aiTextureType textureType);
	void LoadMaterialTextures(Material& material, aiMaterial* aimaterial, aiTextureType textureType, const aiScene* scene);
	int GetTextureIndex(aiString* pStr);

	//void ProcessMaterials(const aiScene* scene, vector<AssimpMaterial>& mats);
	//ID3D11ShaderResourceView* CreateTexture(ID3D11Device* device, std::wstring FileName);

	int AddBone(aiNode* node, aiBone* bone, int parent_index);
	void AddAiBone(aiBone* pBone);
	int FindBoneByName(const std::string& name)const;
	int FindNodeByName(const std::string& name)const;
	void AddBoneWeight(std::vector<XMUINT4>* boneindices, std::vector<XMFLOAT4>* weights, unsigned int vertex_id, unsigned int bone_id, float weight);
	int AddAvatorNode(aiNode* node, int parent_index);
	void BuildAvator(aiNode* node, int parent_index);
	aiBone* GetAiBoneByName(const std::string& name);
	void ProcessAnimation(Animator* animator_out, ConstantBuffer<ConstantBuffer_Bones>* cbufBone, AnimationComponent* animComp);
	

private:

	std::vector<Mesh> m_meshes;
	const aiScene* m_pScene;
	Assimp::Importer m_Importer;

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	ConstantBuffer<CB_VS_MatrixBuffer>* m_wvpMatrixBuffer = nullptr; 
	ConstantBuffer<CB_PS_Material>* m_cb_ps_material = nullptr;
	IVertexShader* pVertexShader = nullptr;
	std::string directory = "";

	std::vector<BoneData> m_Bones;
	std::vector<BoneNode> m_Avator;
	std::unordered_map<std::string, aiBone*> m_mapBoneNameToAiBone;
	std::unordered_map<std::string, int> m_mapBoneNameToIndex;
	std::unordered_map<std::string, int> m_mapNodeNameToIndex;
	std::string RootBoneName;

	std::vector<AssimpModel::AnimationClip> m_Animations;
};




//https://sourceforge.net/p/assimp/discussion/817654/thread/5462cbf5/
//http://www.richardssoftware.net/2013/10/skinned-models-in-directx-11-with.html
//https://sourceforge.net/p/assimp/discussion/817654/thread/a7bf155b/
//https://gamedev.stackovernet.com/ja/q/6807