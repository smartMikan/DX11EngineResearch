#pragma once

#include<Windows.h>
#include<directxmath.h>
#include<iostream>
#include<map>
#include<vector>
#include <algorithm>
using std::map;
using std::vector;
using std::string;
using namespace DirectX;

struct  KeyFrame
{
	KeyFrame();
	~KeyFrame();

	float TimePos;
	XMFLOAT3 Translation;
	XMFLOAT3 Scale;
	XMFLOAT4 RotationQuat;
};

struct BoneAnimation
{
	float GetStartTime()const;
	float GetEndTime()const;

	void Interpolate(float t, XMFLOAT4X4& M)const;

	vector<KeyFrame> keyFrames;
};


struct AnimationClip
{

	float GetClipStartTime()const;
	float GetClipEndTime()const;

	void Interpolate(float t, vector<XMFLOAT4X4>& boneTransform)const;

	vector<BoneAnimation> BoneAnimations;

};

class SkinnedDataClass
{

private:

	
	vector<int> mBoneHierarchy;

	vector<XMFLOAT4X4> mBoneOffsets;

	map<string, AnimationClip> mAnimations;


public:
	UINT BoneCount()const;

	float GetClipStartTime(const string& AnimationClipName)const;
	float GetClipEndTime(const string& AnimationClipName)const;

	void Set(vector<int>& bongHierarchy, vector<XMFLOAT4X4>& boneOffsets, map<string, AnimationClip>& animations);


	//获取某一时间点的某一个动画片段的所有骨头的变换
	void GetFinalTransforms(const string& AnimationClipName, float TimePos, vector<XMFLOAT4X4>& finalTransforms)const;


};

