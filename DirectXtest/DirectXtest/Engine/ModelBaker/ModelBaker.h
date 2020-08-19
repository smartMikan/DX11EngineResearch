#pragma once

#include"../../Graphic/AssimpSkinnedData.h"
const int BAKE_FPS = 30;
const std::wstring MyAnimFileExtension = L".anim";


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

	bool ReadBakedFile(BakedAnimationClip& outputAddress, std::wstring filename);
	bool BakeAnim(AssimpModel::Animator* animator, const AssimpModel::AnimationClip& animation, std::wstring outputfilename);

};