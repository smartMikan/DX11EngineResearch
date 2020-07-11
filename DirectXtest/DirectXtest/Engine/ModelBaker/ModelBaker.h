#pragma once


#include"../../Graphic/AssimpSkinnedData.h"
const int BAKE_FPS = 30;
const std::string MyAnimFileExtension = ".anim";

#ifdef _DEBUG // Debug Mode
#ifdef _WIN64 // x64
const std::string bakedmodelfolder = "..\\x64\\Debug\\BakedModels\\";
#else	// x86
const std::string bakedmodelfolder = "..\\Debug\\BakedModels\\";

#endif
#else	// Release Mode
#ifdef _WIN64
const std::string bakedmodelfolder = "..\\x64\\Release\\BakedModels\\";
#else	// x86
const std::string bakedmodelfolder = "..\\Release\\BakedModels\\";
#endif
#endif

using namespace DirectX;

struct BakedAnimationClip
{
	uint32_t keyframe_count;
	float	animduration;
	uint32_t bonesize;
	std::vector<std::vector<XMMATRIX>>baked_bone_trans_datas;

};




class ModelBaker
{
public:
	ModelBaker();
	~ModelBaker();
};

class AnimationBaker
{
public:
	AnimationBaker();
	~AnimationBaker();

	bool ReadBakedFile(BakedAnimationClip& outputAddress, std::string filename);
	bool BakeAnim(AssimpModel::Animator* animator, const AssimpModel::AnimationClip& animation, std::string outputfilename);

};