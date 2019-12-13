#pragma once

#include"Utility/StringHelper.h"

#include"MeshGeometryClass.h"
#include"SkinnedDataClass.h"
#include"CommonVertexFormat.h"
#include"SkinnedDataClass.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include<iostream>
#include<fstream>
#include<istream>
#include <sstream>
using std::string;
using std::wstring;
using std::ifstream;
using std::istringstream;


class AssimpLoader
{

private:

	//load materials
	void ReadMaterials(const aiScene * aiscene, UINT numMaterials, vector<aiMaterial>& mats);

	//load subset table
	void ReadSubsetTable(const aiScene* aiscene, UINT numSubsets, vector<MeshGeometryClass::Subset>& subsets);

	//read vertieces
	void ReadVertices(const aiScene* aicene, UINT numVertices, vector<PosNormalTexTanVertex>& vertices);

	//read skinned vertieces
	void ReadSkinnedVertices(const aiScene* aicene, UINT numVertices, vector<PosNormalTexTanSkinnedVertex>& vertices);

	//read indices
	void ReadTriangles(const aiScene* aicene, UINT numTriangles, vector<unsigned long>& indices);

	//read bone offset
	void ReadBoneOffets(const aiScene* aicene, UINT numBones, vector<XMFLOAT4X4>& boneOffsets);

	//read bone hierachy
	void ReadBoneHierarchy(const aiScene* aicene, UINT numBones, vector<int>& boneIndexToParentIndex);

	//read animation clips
	void ReadAnimationClips(const aiScene* aicene, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations);

	//read bone keyframes
	void ReadBoneKeyFrame(const aiScene* aicene, UINT numBones, BoneAnimation& boneAnimation);



	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats, SkinnedDataClass & skinInfo);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix, UINT meshID, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats, SkinnedDataClass & skinInfo);
	std::vector<Texture>LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, const aiScene* scene);
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* mat, unsigned int index, aiTextureType textureType);
	int GetTextureIndex(aiString* pStr);

private:
	std::string directory = "";

public:

	bool LoadAssimp(const string& FileName, vector<PosNormalTexTanVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats);

	bool LoadAssimp(const string& FileName, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats, SkinnedDataClass& skinInfo);
};

