#include"SkinnedDataClass.h"
namespace M3d {

	KeyFrame::KeyFrame() :TimePos(0.0f), Translation(0.0f, 0.0f, 0.0f), Scale(1.0f, 1.0f, 1.0f), RotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
	{

	}


	KeyFrame::~KeyFrame()
	{

	}


	float BoneAnimation::GetStartTime()const
	{
		//第一帧的时间点为开始时间
		return keyFrames.front().TimePos;
	}

	float BoneAnimation::GetEndTime()const
	{
		//最后一帧的时间点为结束时间
		return keyFrames.back().TimePos;
	}


	//得到某个骨头在时间t的变换矩阵
	void BoneAnimation::Interpolate(float t, XMFLOAT4X4& M)const
	{
		if (t < keyFrames.front().TimePos)
		{
			XMVECTOR S = XMLoadFloat3(&keyFrames.front().Scale);  //缩放矩阵
			XMVECTOR P = XMLoadFloat3(&keyFrames.front().Translation);  //平移矩阵
			XMVECTOR Q = XMLoadFloat4(&keyFrames.front().RotationQuat); //旋转四元素

			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			// XMMatrixAffineTransformation的第二个参数为旋转中心	
			XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
		}

		else if (t >= keyFrames.back().TimePos)
		{
			XMVECTOR S = XMLoadFloat3(&keyFrames.back().Scale);  //缩放矩阵
			XMVECTOR P = XMLoadFloat3(&keyFrames.back().Translation);  //平移矩阵
			XMVECTOR Q = XMLoadFloat4(&keyFrames.back().RotationQuat); //旋转四元素

			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			// XMMatrixAffineTransformation的第二个参数为旋转中心	
			XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
		}
		else
		{
			for (UINT i = 0; i < keyFrames.size() - 1; ++i)
			{
				if (t >= keyFrames[i].TimePos&& t < keyFrames[i + 1].TimePos)
				{
					float lerpPercent = (t - keyFrames[i].TimePos) / (keyFrames[i + 1].TimePos - keyFrames[i].TimePos);

					XMVECTOR S0 = XMLoadFloat3(&keyFrames[i].Scale);
					XMVECTOR S1 = XMLoadFloat3(&keyFrames[i + 1].Scale);

					XMVECTOR P0 = XMLoadFloat3(&keyFrames[i].Translation);
					XMVECTOR P1 = XMLoadFloat3(&keyFrames[i + 1].Translation);

					XMVECTOR Q0 = XMLoadFloat4(&keyFrames[i].RotationQuat);
					XMVECTOR Q1 = XMLoadFloat4(&keyFrames[i + 1].RotationQuat);

					//对相邻两个帧数的动画的相应变化量进行插值

					//缩放量进行插值
					XMVECTOR S = XMVectorLerp(S0, S1, lerpPercent);

					//移动量进行插值
					XMVECTOR P = XMVectorLerp(P0, P1, lerpPercent);

					//对旋转量进行插值
					XMVECTOR Q = XMVectorLerp(Q0, Q1, lerpPercent);

					XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

					XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
				}
			}

		}
	}


	float AnimationClip::GetClipStartTime()const
	{
		//找到在一个动画片段所有骨头的开始时间的最小值
		float t = 10000000.0f;

		for (UINT i = 0; i < BoneAnimations.size(); ++i)
		{
			t = std::min(t, BoneAnimations[i].GetStartTime());
		}

		return t;
	}


	float AnimationClip::GetClipEndTime()const
	{
		//找到在一个动画片段所有骨头的结束时间的最大值
		float t = 0.0f;

		for (UINT i = 0; i < BoneAnimations.size(); ++i)
		{
			t = std::max(t, BoneAnimations[i].GetEndTime());
		}

		return t;
	}

	//得到所有骨头在时间t的变换矩阵数组,boneTransform[i]代表第i根骨头在时间t的变换矩阵
	void AnimationClip::Interpolate(float t, vector<XMFLOAT4X4>& boneTransform)const
	{
		for (UINT i = 0; i < BoneAnimations.size(); ++i)
		{
			BoneAnimations[i].Interpolate(t, boneTransform[i]);
		}
	}


	float SkinnedDataClass::GetClipStartTime(const string& AnimationClipName)const
	{
		auto AniClip = mAnimations.find(AnimationClipName);
		return AniClip->second.GetClipStartTime();
	}


	float SkinnedDataClass::GetClipEndTime(const string& AnimationClipName)const
	{
		auto AniClip = mAnimations.find(AnimationClipName);
		return AniClip->second.GetClipEndTime();
	}

	UINT  SkinnedDataClass::BoneCount()const
	{
		return mBoneHierarchy.size();
	}


	void  SkinnedDataClass::Set(vector<int>& boneHierarchy, vector<XMFLOAT4X4>& boneOffsets, map<string, AnimationClip>& animations)
	{
		mBoneHierarchy = boneHierarchy;
		mBoneOffsets = boneOffsets;
		mAnimations = animations;
	}


	//获取某一时间点的某一个动画片段的所有骨头的变换矩阵
	void  SkinnedDataClass::GetFinalTransforms(const string& AnimationClipName, float TimePos, vector<XMFLOAT4X4>& finalTransforms) const
	{
		UINT numBones = mBoneOffsets.size();

		//插值得到某个动画片段在某个时间点所有骨头到母骨头的变换矩阵

		vector<XMFLOAT4X4> toParentTransform(numBones);

		auto AniClip = mAnimations.find(AnimationClipName);

		//求出某个时间点所有骨头到相应的母骨头的变换矩阵，其实M3D各骨头存储的Scale,Translation,Quat为变换到母骨头的变换属性
		AniClip->second.Interpolate(TimePos, toParentTransform);

		//遍历所有层级，变换所有骨头到根节点空间

		vector<XMFLOAT4X4> toRootTransforms(numBones);

		//根骨头的下标为0,根骨头没有母骨头,因此它的根空间变换也就是根节点的局部空间变换
		toRootTransforms[0] = toParentTransform[0];

		//现在找到每根骨头到根空间的变换(自顶向下,复杂度为O(n),如果自底向上，则复杂度为O(n*n))
		for (UINT i = 1; i < numBones; ++i)
		{
			XMMATRIX toParent = XMLoadFloat4x4(&toParentTransform[i]);

			int parentIndex = mBoneHierarchy[i];

			XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

			XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

			XMStoreFloat4x4(&toRootTransforms[i], toRoot);
		}

		//得到动画片段所有骨头在某个时间点的最后变换(先平移，在进行根空间变化)
		for (UINT i = 0; i < numBones; ++i)
		{
			XMMATRIX offset = XMLoadFloat4x4(&mBoneOffsets[i]);

			XMMATRIX ToRoot = XMLoadFloat4x4(&toRootTransforms[i]);
			XMStoreFloat4x4(&finalTransforms[i], XMMatrixMultiply(offset, ToRoot));
		}

	}
}