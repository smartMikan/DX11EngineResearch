#pragma once



#include"MeshGeometryClass.h"
#include"SkinnedDataClass.h"
#include"CommonVertexFormat.h"
#include"SkinnedDataClass.h"
#include<iostream>
#include<fstream>
#include<istream>
#include <sstream>
using std::string;
using std::wstring;
using std::ifstream;
using std::istringstream;

//M3d网格模型的材质属性
struct M3dMaterial
{
	Material Mat;
	bool AlphaClip;
	string EffectTypeName;
	wstring DiffuseMapName;
	wstring NormalMapName;
};

class M3dLoaderClass
{
private:

	//读取材质
	void ReadMaterials(ifstream& FileIn, UINT numMaterials, vector<M3dMaterial>& mats);

	//读取子集表
	void ReadSubsetTable(ifstream& FileIn, UINT numSubsets, vector<MeshGeometryClass::Subset>& subsets);

	//读取顶点的属性集合
	void ReadVertices(ifstream& FileIn, UINT numVertices, vector<PosNormalTexTanVertex>& vertices);

	//读取蒙皮顶点属性
	void ReadSkinnedVertices(ifstream& FileIn, UINT numVertices, vector<PosNormalTexTanSkinnedVertex>& vertices);

	//读取三角形索引
	void ReadTriangles(ifstream& FileIn, UINT numTriangles, vector<unsigned long>& indices);

	//读取骨骼位移
	void ReadBoneOffets(ifstream& FileIn, UINT numBones, vector<XMFLOAT4X4>& boneOffsets);

	//读取骨骼层级(以数组形式呈现的骨骼树关系)
	void ReadBoneHierarchy(ifstream& FileIn, UINT numBones, vector<int>& boneIndexToParentIndex);

	//读取动画片段
	void ReadAnimationClips(ifstream& FileIn, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations);

	//读取骨头关键帧
	void ReadBoneKeyFrame(ifstream& FileIn, UINT numBones, BoneAnimation& boneAnimation);


public:
	bool LoadM3d(const string& FileName, vector<PosNormalTexTanVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<M3dMaterial>& mats);

	bool LoadM3d(const string& FileName, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<M3dMaterial>& mats, SkinnedDataClass& skinInfo);
};

