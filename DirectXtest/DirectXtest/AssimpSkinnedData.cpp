#include "AssimpSkinnedData.h"
namespace AssimpModel {
	KeyFrame::KeyFrame() :TimePos(0.0f), Value(0.0f, 0.0f, 0.0f, 1.0f)
	{

	}


	KeyFrame::~KeyFrame()
	{

	}


	float Channel::GetTransStartTime() const
	{
		return TranslationKeyFrames.front().TimePos;
	}

	float Channel::GetTransEndTime() const
	{
		return TranslationKeyFrames.back().TimePos;;
	}

	float Channel::GetScaleStartTime() const
	{
		return ScaleKeyFrames.front().TimePos;
	}

	float Channel::GetScaleEndTime() const
	{
		return ScaleKeyFrames.back().TimePos;
	}

	float Channel::GetRotStartTime() const
	{
		return RotationQuatKeyFrames.front().TimePos;
	}

	float Channel::GetRotEndTime() const
	{
		return RotationQuatKeyFrames.back().TimePos;
	}

	//find min end time
	float Channel::GetStartTime()const
	{

		float t = 100000000.0f;
		float tx[3];
		tx[0] = GetTransStartTime();
		tx[1] = GetScaleStartTime();
		tx[2] = GetRotStartTime();

		for (int i = 0; i < 3; i++)
		{
			if (t >= tx[i]) {
				t = tx[i];
			}
		}

		return t;
	}

	//find max end time
	float Channel::GetEndTime()const
	{

		float t = 0.0f;
		float tx[3];
		tx[0] = GetTransEndTime();
		tx[1] = GetScaleEndTime();
		tx[2] = GetRotEndTime();

		for (int i = 0; i < 3; i++)
		{
			if (t <= tx[i]) {
				t = tx[i];
			}
		}

		return t;
	}


	void Channel::Interpolate(float t, XMMATRIX& out)const
	{
		XMVECTOR S, P, Q;
		bool Pok = false;
		bool Qok = false;
		bool Sok = false;

		//Position
		if (t < TranslationKeyFrames.front().TimePos) {
			P = XMLoadFloat4(&TranslationKeyFrames.front().Value);
			Pok = true;
		}
		else if (t >= TranslationKeyFrames.back().TimePos) {
			P = XMLoadFloat4(&TranslationKeyFrames.back().Value);
			Pok = true;
		}
		else
		{
			for (UINT i = 0; i < TranslationKeyFrames.size() - 1; ++i)
			{
				if (t >= TranslationKeyFrames[i].TimePos && t < TranslationKeyFrames[i + 1].TimePos)
				{
					float lerpPercent = (t - TranslationKeyFrames[i].TimePos) / (TranslationKeyFrames[i + 1].TimePos - TranslationKeyFrames[i].TimePos);

					XMVECTOR P0 = XMLoadFloat4(&TranslationKeyFrames[i].Value);
					XMVECTOR P1 = XMLoadFloat4(&TranslationKeyFrames[i + 1].Value);

					//对相邻两个帧数的动画的相应变化量进行插值

					//Translation进行插值
					P = XMVectorLerp(P0, P1, lerpPercent);
					Pok = true;
				}
			}
		}

		//Scale
		if (t < ScaleKeyFrames.front().TimePos) {
			S = XMLoadFloat4(&ScaleKeyFrames.front().Value);
			Sok = true;
		}
		else if (t >= ScaleKeyFrames.back().TimePos) {
			S = XMLoadFloat4(&ScaleKeyFrames.back().Value);
			Sok = true;
		}
		else
		{
			for (UINT i = 0; i < ScaleKeyFrames.size() - 1; ++i)
			{
				if (t >= ScaleKeyFrames[i].TimePos && t < ScaleKeyFrames[i + 1].TimePos)
				{
					float lerpPercent = (t - ScaleKeyFrames[i].TimePos) / (ScaleKeyFrames[i + 1].TimePos - ScaleKeyFrames[i].TimePos);

					XMVECTOR S0 = XMLoadFloat4(&ScaleKeyFrames[i].Value);
					XMVECTOR S1 = XMLoadFloat4(&ScaleKeyFrames[i + 1].Value);

					//对相邻两个帧数的动画的相应变化量进行插值

					//Scale进行插值
					S = XMVectorLerp(S0, S1, lerpPercent);
					Sok = true;
				}
			}
		}

		//Quat
		if (t < RotationQuatKeyFrames.front().TimePos) {
			Q = XMLoadFloat4(&RotationQuatKeyFrames.front().Value);
			Qok = true;
		}
		else if (t >= RotationQuatKeyFrames.back().TimePos) {
			Q = XMLoadFloat4(&RotationQuatKeyFrames.back().Value);
			Qok = true;
		}
		else
		{
			for (UINT i = 0; i < RotationQuatKeyFrames.size() - 1; ++i)
			{
				if (t >= RotationQuatKeyFrames[i].TimePos && t < RotationQuatKeyFrames[i + 1].TimePos)
				{
					float lerpPercent = (t - RotationQuatKeyFrames[i].TimePos) / (RotationQuatKeyFrames[i + 1].TimePos - RotationQuatKeyFrames[i].TimePos);

					XMVECTOR Q0 = XMLoadFloat4(&RotationQuatKeyFrames[i].Value);
					XMVECTOR Q1 = XMLoadFloat4(&RotationQuatKeyFrames[i + 1].Value);

					//对相邻两个帧数的动画的相应变化量进行插值

					//RotationQuat进行插值
					Q = XMVectorLerp(Q0, Q1, lerpPercent);
					Qok = true;
				}
			}
		}
		if (Pok&&Qok&&Sok) {
			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			//XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
			out = XMMatrixAffineTransformation(S, zero, Q, P);
		}
	}

	float AnimationClip::GetClipStartTime()const
	{
		//找到在一个动画片段所有骨头的开始时间的最小值
		float t = 10000000.0f;

		for (UINT i = 0; i < mChannel.size(); ++i)
		{
			t = std::min(t, mChannel[i].GetStartTime());
		}

		return t;
	}

	float AnimationClip::GetClipEndTime()const
	{
		//找到在一个动画片段所有骨头的结束时间的最大值
		float t = 0.0f;

		for (UINT i = 0; i < mChannel.size(); ++i)
		{
			t = std::max(t, mChannel[i].GetEndTime());
		}

		return t;
	}

	//得到所有骨头在时间t的变换矩阵数组,boneTransform[i]代表第i根骨头在时间t的变换矩阵
	void AnimationClip::Interpolate(float t, vector<XMFLOAT4X4>& boneTransform)const
	{
		for (UINT i = 0; i < mChannel.size(); i++)
		{
			mChannel[i].Interpolate(t, mChannel[i].mJoint->mAnimatedTransf);
		}

	}


	float AssimpSkinnedData::GetClipStartTime(const string& AnimationClipName)const
	{
		auto AniClip = mAnimations.find(AnimationClipName);
		return AniClip->second.GetClipStartTime();
	}


	float AssimpSkinnedData::GetClipEndTime(const string& AnimationClipName)const
	{
		auto AniClip = mAnimations.find(AnimationClipName);
		return AniClip->second.GetClipEndTime();
	}

	UINT  AssimpSkinnedData::BoneCount()const
	{
		return mBoneHierarchy.size();
	}

	void  AssimpSkinnedData::Set(vector<int>& boneHierarchy, vector<XMFLOAT4X4>& boneOffsets, map<string, AnimationClip>& animations)
	{
		mBoneHierarchy = boneHierarchy;
		mBoneOffsets = boneOffsets;
		mAnimations = animations;
	}

	void AssimpSkinnedData::SetAnimations(map<string, AnimationClip>& animations)
	{
		mAnimations = animations;
	}

	void AssimpSkinnedData::SetBoneHierarchy(vector<int>& boneHierarchy)
	{
		mBoneHierarchy = boneHierarchy;
		
	}

	void AssimpSkinnedData::SetBoneOffsets(vector<XMFLOAT4X4>& boneOffsets){
		mBoneOffsets = boneOffsets;
	}

	//获取某一时间点的某一个动画片段的所有骨头的变换矩阵
	void  AssimpSkinnedData::GetFinalTransforms(const string& AnimationClipName, float TimePos, vector<XMFLOAT4X4>& finalTransforms) const
	{
		
		auto AniClip = mAnimations.find(AnimationClipName);

		finalTransforms.clear();

		UINT numBones = AniClip->second.mChannel.size();

		vector<XMFLOAT4X4> toParentTransform;

		AniClip->second.Interpolate(TimePos, toParentTransform);

		//UINT numBones = mBoneOffsets.size();

		////插值得到某个动画片段在某个时间点所有骨头到母骨头的变换矩阵
		//vector<XMFLOAT4X4> toParentTransform(numBones);

		//auto AniClip = mAnimations.find(AnimationClipName);

		////求出某个时间点所有骨头到相应的母骨头的变换矩阵，其实M3D各骨头存储的Scale,Translation,Quat为变换到母骨头的变换属性
		//AniClip->second.Interpolate(TimePos, toParentTransform);

		////遍历所有层级，变换所有骨头到根节点空间
		//vector<XMFLOAT4X4> toRootTransforms(numBones);

		////根骨头的下标为0,根骨头没有母骨头,因此它的根空间变换也就是根节点的局部空间变换
		//toRootTransforms[0] = toParentTransform[0];

		////现在找到每根骨头到根空间的变换(自顶向下,复杂度为O(n),如果自底向上，则复杂度为O(n*n))
		//for (UINT i = 1; i < numBones; ++i)
		//{
		//	XMMATRIX toParent = XMLoadFloat4x4(&toParentTransform[i]);

		//	int parentIndex = mBoneHierarchy[i];

		//	XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		//	XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

		//	XMStoreFloat4x4(&toRootTransforms[i], toRoot);
		//}

		////得到动画片段所有骨头在某个时间点的最后变换(先平移，在进行根空间变化)
		//for (UINT i = 0; i < numBones; i++)
		//{
		//	XMMATRIX offset = XMLoadFloat4x4(&mBoneOffsets[i]);

		//	XMMATRIX ToRoot = XMLoadFloat4x4(&toRootTransforms[i]);
		//	XMStoreFloat4x4(&finalTransforms[i], XMMatrixMultiply(offset, ToRoot));
		//}

	}

	void AssimpSkinnedData::CombineTransforms(Joint * pJoint, const XMMATRIX & P, vector<XMFLOAT4X4>& finalTransforms)
	{
		XMMATRIX final = pJoint->mAnimatedTransf * P;
		XMFLOAT4X4 trans;
		XMStoreFloat4x4(&trans, pJoint->mOffsetTransf * final);
		finalTransforms.push_back(trans);

		for (UINT i = 0; i < pJoint->mChildren.size(); i++)
		{
			CombineTransforms(pJoint->mChildren[i],final, finalTransforms);
		}
	}


	AssimpSkinnedData::AssimpSkinnedData()
	{
	}


	AssimpSkinnedData::~AssimpSkinnedData()
	{
	}


}


