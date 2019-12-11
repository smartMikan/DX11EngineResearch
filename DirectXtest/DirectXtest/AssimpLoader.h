#pragma once

#include"Utility/StringHelper.h"

#include"MeshGeometryClass.h"
#include"SkinnedDataClass.h"
#include"CommonVertexFormat.h"
#include"SkinnedDataClass.h"

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

	//读取材质
	void ReadMaterials(const aiScene * aiscene, UINT numMaterials, vector<aiMaterial>& mats);

	//读取子集表
	void ReadSubsetTable(aiScene* aiscene, UINT numSubsets, vector<MeshGeometryClass::Subset>& subsets);

	//读取顶点的属性集合
	void ReadVertices(aiScene* aicene, UINT numVertices, vector<PosNormalTexTanVertex>& vertices);

	//读取蒙皮顶点属性
	void ReadSkinnedVertices(aiScene* aicene, UINT numVertices, vector<PosNormalTexTanSkinnedVertex>& vertices);

	//读取三角形索引
	void ReadTriangles(aiScene* aicene, UINT numTriangles, vector<unsigned long>& indices);

	//读取骨骼位移
	void ReadBoneOffets(aiScene* aicene, UINT numBones, vector<XMFLOAT4X4>& boneOffsets);

	//读取骨骼层级(以数组形式呈现的骨骼树关系)
	void ReadBoneHierarchy(aiScene* aicene, UINT numBones, vector<int>& boneIndexToParentIndex);

	//读取动画片段
	void ReadAnimationClips(aiScene* aicene, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations);

	//读取骨头关键帧
	void ReadBoneKeyFrame(aiScene* aicene, UINT numBones, BoneAnimation& boneAnimation);


private:
	std::string directory = "";

public:

	bool LoadAssimp(const string& FileName, vector<PosNormalTexTanVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats);

	bool LoadAssimp(const string& FileName, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats, SkinnedDataClass& skinInfo);
};

