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

namespace AssimpModel 
{
	struct  KeyFrame
	{
		KeyFrame();
		~KeyFrame();

		float TimePos;
		XMFLOAT4 Value;
	};

	struct Joint
	{
		XMMATRIX mOffsetTransf;
		XMMATRIX mAnimatedTransf;

		vector<Joint*>mChildren;
	};

	struct Channel
	{
		float GetTransStartTime()const;
		float GetTransEndTime()const;
		float GetScaleStartTime()const;
		float GetScaleEndTime()const;
		float GetRotStartTime()const;
		float GetRotEndTime()const;
		float GetStartTime()const;
		float GetEndTime()const;
		void Interpolate(float t, XMMATRIX& out)const;
		vector<KeyFrame> TranslationKeyFrames;
		vector<KeyFrame> ScaleKeyFrames;
		vector<KeyFrame> RotationQuatKeyFrames;
		Joint* mJoint;
	};

	struct AnimationClip
	{
		float GetClipStartTime()const;
		float GetClipEndTime()const;
		void Interpolate(float t, vector<XMFLOAT4X4>& boneTransform)const;
		vector<Channel> mChannel;
	};


	


	class AssimpSkinnedData
	{
	public:
		AssimpSkinnedData();
		~AssimpSkinnedData();

		UINT BoneCount()const;

		float GetClipStartTime(const string& AnimationClipName)const;
		float GetClipEndTime(const string& AnimationClipName)const;

		void Set(vector<int>& bongHierarchy, vector<XMFLOAT4X4>& boneOffsets, map<string, AnimationClip>& animations);
		void SetAnimations(map<string, AnimationClip>& animations);
		void SetBoneHierarchy(vector<int>& boneHierarchy);
		void SetBoneOffsets(vector<XMFLOAT4X4>& boneOffsets);
		//获取某一时间点的某一个动画片段的所有骨头的变换
		void GetFinalTransforms(const string& AnimationClipName, float TimePos, vector<XMFLOAT4X4>& finalTransforms)const;
		void CombineTransforms(Joint* pJoint, const XMMATRIX& P, vector<XMFLOAT4X4>& finalTransforms);

		vector<XMFLOAT4X4> mBoneOffsets;

	private:

		vector<int> mBoneHierarchy;

		map<string, AnimationClip> mAnimations;
	
	};

}


