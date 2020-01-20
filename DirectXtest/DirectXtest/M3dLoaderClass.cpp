#include "M3dLoaderClass.h"
using namespace M3d;

//仅仅是读取相应的网格数据，并未有骨骼动画
bool M3dLoaderClass::LoadM3d(const string& FileName, vector<PosNormalTexTanVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<M3dMaterial>& mats)
{
	//打开M3d.文件格式
	ifstream fin(FileName);

	UINT numMaterials = 0;
	UINT numVertices = 0;
	UINT numTriangles = 0;
	UINT numBones = 0;
	UINT numAnimationsClips = 0;

	string ignore;
	if (fin)
	{

		fin >> ignore;//跳过一行
		fin >> ignore >> numMaterials;
		fin >> ignore >> numVertices;
		fin >> ignore >> numTriangles;
		fin >> ignore >> numBones;
		fin >> ignore >> numAnimationsClips;

		ReadMaterials(fin, numMaterials, mats);
		ReadSubsetTable(fin, numMaterials, subsets);
		ReadVertices(fin, numVertices, vertices);
		ReadTriangles(fin, numTriangles, indices);
		return true;
	}

	return false;
}



//读取相应的网格数据和动画数据
bool M3dLoaderClass::LoadM3d(const string& FileName, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<M3dMaterial>& mats, SkinnedDataClass& skinInfo)
{

	//打开M3d.文件格式
	ifstream fin(FileName);

	UINT numMaterials = 0;
	UINT numVertices = 0;
	UINT numTriangles = 0;
	UINT numBones = 0;
	UINT numAnimationsClips = 0;

	string ignore;
	if (fin)
	{

		fin >> ignore;//跳过一行
		fin >> ignore >> numMaterials;
		fin >> ignore >> numVertices;
		fin >> ignore >> numTriangles;
		fin >> ignore >> numBones;
		fin >> ignore >> numAnimationsClips;

		vector<XMFLOAT4X4> boneOffets;
		vector<int> boneToParentIndex;
		map<string, AnimationClip> animations;

		ReadMaterials(fin, numMaterials, mats);
		ReadSubsetTable(fin, numMaterials, subsets);
		ReadSkinnedVertices(fin, numVertices, vertices);
		ReadTriangles(fin, numTriangles, indices);
		ReadBoneOffets(fin, numBones, boneOffets);
		ReadBoneHierarchy(fin, numBones, boneToParentIndex);
		ReadAnimationClips(fin, numBones, numAnimationsClips, animations);

		skinInfo.Set(boneToParentIndex, boneOffets, animations);
		return true;
	}

	return false;
}



//读取材质
void M3dLoaderClass::ReadMaterials(ifstream& FileIn, UINT numMaterials, vector<M3dMaterial>& mats)
{
	string ignore;
	mats.resize(numMaterials);

	string DiffuseMapName;
	string NormalMapName;

	FileIn >> ignore; //跳过文件头一行
	for (UINT i = 0; i < numMaterials; ++i)
	{
		FileIn >> ignore >> mats[i].Mat.Ambient.x >> mats[i].Mat.Ambient.y >> mats[i].Mat.Ambient.z;
		FileIn >> ignore >> mats[i].Mat.Diffuse.x >> mats[i].Mat.Diffuse.y >> mats[i].Mat.Diffuse.z;
		FileIn >> ignore >> mats[i].Mat.Specular.x >> mats[i].Mat.Specular.y >> mats[i].Mat.Specular.z;
		FileIn >> ignore >> mats[i].Mat.Specular.w;
		FileIn >> ignore >> mats[i].Mat.Reflect.x >> mats[i].Mat.Reflect.y >> mats[i].Mat.Reflect.z;
		FileIn >> ignore >> mats[i].AlphaClip;
		FileIn >> ignore >> mats[i].EffectTypeName;
		FileIn >> ignore >> DiffuseMapName;
		FileIn >> ignore >> NormalMapName;


		//将DiffuseMapName和NormalMapName从string变为wstring格式
		mats[i].DiffuseMapName.resize(DiffuseMapName.size(), ' ');
		mats[i].NormalMapName.resize(NormalMapName.size(), ' ');
		std::copy(DiffuseMapName.begin(), DiffuseMapName.end(), mats[i].DiffuseMapName.begin());
		std::copy(NormalMapName.begin(), NormalMapName.end(), mats[i].NormalMapName.begin());
	}
}

//读取子集表
void M3dLoaderClass::ReadSubsetTable(ifstream& FileIn, UINT numSubsets, vector<MeshGeometryClass::Subset>& subsets)
{
	string ignore;
	subsets.resize(numSubsets);

	FileIn >> ignore; //跳过头一行
	for (UINT i = 0; i < numSubsets; ++i)
	{
		FileIn >> ignore >> subsets[i].Id;
		FileIn >> ignore >> subsets[i].VertexStart;
		FileIn >> ignore >> subsets[i].VertexCount;
		FileIn >> ignore >> subsets[i].FaceStart;
		FileIn >> ignore >> subsets[i].FaceCount;
	}
}

//读取顶点的属性集合
void M3dLoaderClass::ReadVertices(ifstream& FileIn, UINT numVertices, vector<PosNormalTexTanVertex>& vertices)
{
	string ignore;
	vertices.resize(numVertices);
	FileIn >> ignore; //跳过头一行

	for (UINT i = 0; i < numVertices; ++i)
	{
		FileIn >> ignore >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		FileIn >> ignore >> vertices[i].TangentU.x >> vertices[i].TangentU.y >> vertices[i].TangentU.z >> vertices[i].TangentU.w;
		FileIn >> ignore >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
		FileIn >> ignore >> vertices[i].Tex.x >> vertices[i].Tex.y;

	}

}

// 读取蒙皮顶点属性
void M3dLoaderClass::ReadSkinnedVertices(ifstream& FileIn, UINT numVertices, vector<PosNormalTexTanSkinnedVertex>& vertices)
{
	string ignore;
	vertices.resize(numVertices);
	FileIn >> ignore; //跳过头一行
	int boneIndices[4];
	for (UINT i = 0; i < numVertices; ++i)
	{
		FileIn >> ignore >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		FileIn >> ignore >> vertices[i].TangentU.x >> vertices[i].TangentU.y >> vertices[i].TangentU.z >> vertices[i].TangentU.w;
		FileIn >> ignore >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
		FileIn >> ignore >> vertices[i].Tex.x >> vertices[i].Tex.y;
		FileIn >> ignore >> vertices[i].Weights.x >> vertices[i].Weights.y >> vertices[i].Weights.z >> ignore;
		FileIn >> ignore >> boneIndices[0] >> boneIndices[1] >> boneIndices[2] >> boneIndices[3];

		vertices[i].BoneIndices[0] = (BYTE)boneIndices[0];
		vertices[i].BoneIndices[1] = (BYTE)boneIndices[1];
		vertices[i].BoneIndices[2] = (BYTE)boneIndices[2];
		vertices[i].BoneIndices[3] = (BYTE)boneIndices[3];
	}
}


//读取三角形索引
void M3dLoaderClass::ReadTriangles(ifstream& FileIn, UINT numTriangles, vector<unsigned long>& indices)
{
	string ignore;

	//numTriangles*3个索引数目
	indices.resize(numTriangles * 3);
	FileIn >> ignore; //跳过头一行
	for (UINT i = 0; i < numTriangles; ++i)
	{
		FileIn >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
}



//读取骨骼位移
void M3dLoaderClass::ReadBoneOffets(ifstream& FileIn, UINT numBones, vector<XMFLOAT4X4>& boneOffsets)
{
	string ignore;
	boneOffsets.resize(numBones);
	FileIn >> ignore; //跳过头一行

	for (UINT i = 0; i < numBones; ++i)
	{
		FileIn >> ignore >> boneOffsets[i](0, 0) >> boneOffsets[i](0, 1) >> boneOffsets[i](0, 2) >> boneOffsets[i](0, 3) >>
			boneOffsets[i](1, 0) >> boneOffsets[i](1, 1) >> boneOffsets[i](1, 2) >> boneOffsets[i](1, 3) >>
			boneOffsets[i](2, 0) >> boneOffsets[i](2, 1) >> boneOffsets[i](2, 2) >> boneOffsets[i](2, 3) >>
			boneOffsets[i](3, 0) >> boneOffsets[i](3, 1) >> boneOffsets[i](3, 2) >> boneOffsets[i](3, 3);
	}
}


//读取骨骼层级(以数组形式呈现的骨骼树关系)
void M3dLoaderClass::ReadBoneHierarchy(ifstream& FileIn, UINT numBones, vector<int>& boneIndexToParentIndex)
{
	string ignore;
	boneIndexToParentIndex.resize(numBones);
	FileIn >> ignore; //跳过头一行
	for (UINT i = 0; i < numBones; ++i)
	{
		FileIn >> ignore >> boneIndexToParentIndex[i];
	}
}

//读取动画片段
void M3dLoaderClass::ReadAnimationClips(ifstream& FileIn, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations)
{
	string ignore;
	FileIn >> ignore; //跳过头一行
	for (UINT clipIndex = 0; clipIndex < numAnimationClips; ++clipIndex)
	{
		string clipName;

		//获取动画片段名
		FileIn >> ignore >> clipName;
		FileIn >> ignore; //跳过"{"
		AnimationClip clip;
		clip.BoneAnimations.resize(numBones);

		for (UINT boneIndex = 0; boneIndex < numBones; ++boneIndex)
		{
			ReadBoneKeyFrame(FileIn, numBones, clip.BoneAnimations[boneIndex]);
		}

		FileIn >> ignore; //跳过"}"
		animations[clipName] = clip;
	}
}

//读取骨头关键帧
void M3dLoaderClass::ReadBoneKeyFrame(ifstream& FileIn, UINT numBones, BoneAnimation& boneAnimation)
{
	string ignore;
	UINT numKeyframes = 0;
	FileIn >> ignore >> ignore >> numKeyframes;
	FileIn >> ignore; //跳过“{”
	boneAnimation.keyFrames.resize(numKeyframes);

	for (UINT KeyFrameIndex = 0; KeyFrameIndex < numKeyframes; ++KeyFrameIndex)
	{
		XMFLOAT3 Translation;
		XMFLOAT3 Scale;
		XMFLOAT4 Quat;
		FileIn >> ignore >> boneAnimation.keyFrames[KeyFrameIndex].TimePos;
		FileIn >> ignore >> Translation.x >> Translation.y >> Translation.z;
		FileIn >> ignore >> Scale.x >> Scale.y >> Scale.z;
		FileIn >> ignore >> Quat.x >> Quat.y >> Quat.z >> Quat.w;
		boneAnimation.keyFrames[KeyFrameIndex].Translation = Translation;
		boneAnimation.keyFrames[KeyFrameIndex].Scale = Scale;
		boneAnimation.keyFrames[KeyFrameIndex].RotationQuat = Quat;
	}
	FileIn >> ignore; //跳过“}”

}