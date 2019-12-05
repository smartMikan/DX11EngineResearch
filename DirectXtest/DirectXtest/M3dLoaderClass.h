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

//M3d����ģ�͵Ĳ�������
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

	//��ȡ����
	void ReadMaterials(ifstream& FileIn, UINT numMaterials, vector<M3dMaterial>& mats);

	//��ȡ�Ӽ���
	void ReadSubsetTable(ifstream& FileIn, UINT numSubsets, vector<MeshGeometryClass::Subset>& subsets);

	//��ȡ��������Լ���
	void ReadVertices(ifstream& FileIn, UINT numVertices, vector<PosNormalTexTanVertex>& vertices);

	//��ȡ��Ƥ��������
	void ReadSkinnedVertices(ifstream& FileIn, UINT numVertices, vector<PosNormalTexTanSkinnedVertex>& vertices);

	//��ȡ����������
	void ReadTriangles(ifstream& FileIn, UINT numTriangles, vector<unsigned long>& indices);

	//��ȡ����λ��
	void ReadBoneOffets(ifstream& FileIn, UINT numBones, vector<XMFLOAT4X4>& boneOffsets);

	//��ȡ�����㼶(��������ʽ���ֵĹ�������ϵ)
	void ReadBoneHierarchy(ifstream& FileIn, UINT numBones, vector<int>& boneIndexToParentIndex);

	//��ȡ����Ƭ��
	void ReadAnimationClips(ifstream& FileIn, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations);

	//��ȡ��ͷ�ؼ�֡
	void ReadBoneKeyFrame(ifstream& FileIn, UINT numBones, BoneAnimation& boneAnimation);


public:
	bool LoadM3d(const string& FileName, vector<PosNormalTexTanVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<M3dMaterial>& mats);

	bool LoadM3d(const string& FileName, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<M3dMaterial>& mats, SkinnedDataClass& skinInfo);
};

