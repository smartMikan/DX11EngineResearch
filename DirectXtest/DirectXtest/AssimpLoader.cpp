#include "AssimpLoader.h"

void AssimpLoader::ReadMaterials(const aiScene * aiscene, UINT numMaterials, vector<aiMaterial>& mats)
{
}

void AssimpLoader::ReadSubsetTable(aiScene* aiscene, UINT numSubsets, vector<MeshGeometryClass::Subset>& subsets)
{
}

void AssimpLoader::ReadVertices(aiScene* aicene, UINT numVertices, vector<PosNormalTexTanVertex>& vertices)
{
}

void AssimpLoader::ReadSkinnedVertices(aiScene* aicene, UINT numVertices, vector<PosNormalTexTanSkinnedVertex>& vertices)
{
}

void AssimpLoader::ReadTriangles(aiScene* aicene, UINT numTriangles, vector<unsigned long>& indices)
{
}

void AssimpLoader::ReadBoneOffets(aiScene* aicene, UINT numBones, vector<XMFLOAT4X4>& boneOffsets)
{
}

void AssimpLoader::ReadBoneHierarchy(aiScene* aicene, UINT numBones, vector<int>& boneIndexToParentIndex)
{
}

void AssimpLoader::ReadAnimationClips(aiScene* aicene, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations)
{
}

void AssimpLoader::ReadBoneKeyFrame(aiScene* aicene, UINT numBones, BoneAnimation & boneAnimation)
{
}

bool AssimpLoader::LoadAssimp(const string & FileName, vector<PosNormalTexTanVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats)
{
	return false;
}

bool AssimpLoader::LoadAssimp(const string & FileName, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats, SkinnedDataClass & skinInfo)
{
	this->directory = StringHelper::GetDirectoryFromPath(FileName);

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(FileName,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);


	if (pScene == nullptr)
		return false;

	UINT numMaterials = 0;
	UINT numVertices = 0;
	UINT numTriangles = 0;
	UINT numBones = 0;
	UINT numAnimationsClips = 0;

	vector<XMFLOAT4X4> boneOffets;
	vector<int> boneToParentIndex;
	map<string, AnimationClip> animations;

	ReadMaterials(pScene, numMaterials, mats);
	ReadSubsetTable(pScene, numMaterials, subsets);
	ReadSkinnedVertices(pScene, numVertices, vertices);
	ReadTriangles(pScene, numTriangles, indices);
	ReadBoneOffets(pScene, numBones, boneOffets);
	ReadBoneHierarchy(pScene, numBones, boneToParentIndex);
	ReadAnimationClips(pScene, numBones, numAnimationsClips, animations);

	skinInfo.Set(boneToParentIndex, boneOffets, animations);
	return true;
}
