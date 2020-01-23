#pragma once

#include "Mesh.h"
#include "ShaderManagerClass.h"
#include <list>
#include <stdint.h>
#include "positionclass.h"
#include "AssimpSkinnedData.h"

#include"MeshGeometryClass.h"
#include"CommonVertexFormat.h"

using namespace DirectX;
//using namespace MyVertex;
using namespace AssimpModel;

struct AssimpMaterial
{
	Material Mat;
	bool AlphaClip;
	string EffectTypeName;
	wstring DiffuseMapName;
	wstring NormalMapName;
};


struct AssimpTempVertexType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

struct AssimpVectorType
{
	float x, y, z;
};

struct BoneInfo
{
	XMMATRIX BoneOffset;
	XMMATRIX FinalTransformation;

	BoneInfo()
	{
		BoneOffset = XMMatrixIdentity();
		FinalTransformation = XMMatrixIdentity();
	}
};


#define NUM_BONES_PER_VEREX 4

struct VertexBoneData
{
	UINT IDs[NUM_BONES_PER_VEREX];
	float Weights[NUM_BONES_PER_VEREX];

	VertexBoneData()
	{
		Reset();
	};

	void Reset()
	{
		ZeroMemory(&IDs, sizeof(IDs));
		ZeroMemory(&Weights, sizeof(Weights));
	}

	void AddBoneData(UINT BoneID, float Weight);
};


class Model
{

public:
	Model();
	~Model();
	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	
	void Update(float timepos);

	void Draw(ShaderManagerClass * shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor, float timepos, ID3D11ShaderResourceView* DiffuseMap,
		ID3D11ShaderResourceView* NormalMap);
	bool LoadModel(const std::string& filePath);
	//void BoneTransform(float TimeInSeconds, vector<XMMATRIX>& Transforms,Mesh* mesh);
	ID3D11ShaderResourceView* CreateTexture(ID3D11Device* device, std::wstring FileName);



public:
	int numOfAnim;
	string nameOfAnim;
	int numOfBones;

	//骨骼顶点数据
	//vector<PosNormalTexTanSkinnedVertex> Vertices;

	AssimpSkinnedData skinnedData;
	MeshGeometryClass ModelMesh;

private:
	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix, UINT meshID);
	std::vector<Texture>LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, const aiScene* scene);
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* mat, unsigned int index, aiTextureType textureType);
	int GetTextureIndex(aiString* pStr);

	void ProcessMaterials(const aiScene* scene, vector<AssimpMaterial>& mats);
	

	
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const XMMATRIX& ParentTransform);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	UINT FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	UINT FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	UINT FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
	void BoneTransform(float TimeInSeconds, vector<XMMATRIX>& Transforms);



	void ProcessAnimation(AssimpSkinnedData* skindata, const aiScene* modelScene);
	const aiNodeAnim* FindNodeAnim(const aiNode * node, const aiAnimation * anim, AssimpModel::AnimationClip & clip);
	void CreateBoneKeyFrame(AssimpModel::Channel & boneAnimation);
	bool ReadBoneKeyFrame(const aiNodeAnim* mSingleBone, AssimpModel::Channel& boneAnimation, double Animduration);
	void ProcessNodeMap(map<string, int>& boneNameToNumberIndexMap, map<int, string> &boneNumberToNameIndexMap, vector<XMFLOAT4X4> &boneOffsets,const aiNode * node,int &currentNodeNumber);
	void ProcessBoneToParentIndex(vector<int>& boneToParentIndex, map<string, int>& boneNameToNumberIndexMap, const aiNode * node);
	const aiNode* FindNodeRecursivelyByName(const aiNode* node, aiString nodeName);
	Joint* FormHierarchy(aiBone* pBone);
	void LoadBones(aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);

private:

	std::vector<Mesh> m_meshes;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	Joint* pJoint;


	std::string directory = "";
	//const aiScene* modelScene;
	//std::vector<AnimationClip> m_animation;

	//SkinnedDataClass m_skindata;


	const aiScene* m_pScene;
	Assimp::Importer m_Importer;

	XMMATRIX m_GlobalInverseTransform;


	map<std::string, int> boneNameToNumberIndexMap;
	map<int, std::string> boneNumberToNameIndexMap;
	std::vector<XMFLOAT4X4> FinalTransforms;
	Material mat;
	map<string, AnimationClip> animations;

	std::vector<VertexBoneData> m_Bones;
	std::vector<BoneInfo> m_BoneInfo;
	std::map<std::string, UINT> m_BoneMapping; // maps a bone name to its index
	UINT m_NumBones = 0;
};


struct AssimpSkinnedModelInstance
{
	PositionClass* position;
	Model* Model;
	float TimePos;
	std::string ClipName;
	XMFLOAT4X4 World;
	std::vector<XMFLOAT4X4> FinalTransforms;

	void Update(float dt);
	void DeUpdate(float dt);
};


//https://sourceforge.net/p/assimp/discussion/817654/thread/5462cbf5/
//http://www.richardssoftware.net/2013/10/skinned-models-in-directx-11-with.html
//https://sourceforge.net/p/assimp/discussion/817654/thread/a7bf155b/
//https://gamedev.stackovernet.com/ja/q/6807