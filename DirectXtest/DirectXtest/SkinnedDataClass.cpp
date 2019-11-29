#include"SkinnedDataClass.h"

KeyFrame::KeyFrame() :TimePos(0.0f), Translation(0.0f, 0.0f, 0.0f), Scale(1.0f, 1.0f, 1.0f), RotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
{

}


KeyFrame::~KeyFrame()
{

}


float BoneAnimation::GetStartTime()const
{
	//��һ֡��ʱ���Ϊ��ʼʱ��
	return keyFrames.front().TimePos;
}

float BoneAnimation::GetEndTime()const
{
	//���һ֡��ʱ���Ϊ����ʱ��
	return keyFrames.back().TimePos;
}

//�õ�ĳ����ͷ��ʱ��t�ı任����
void BoneAnimation::Interpolate(float t, XMFLOAT4X4& M)const
{
	if (t < keyFrames.front().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&keyFrames.front().Scale);  //���ž���
		XMVECTOR P = XMLoadFloat3(&keyFrames.front().Translation);  //ƽ�ƾ���
		XMVECTOR Q = XMLoadFloat4(&keyFrames.front().RotationQuat); //��ת��Ԫ��

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		// XMMatrixAffineTransformation�ĵڶ�������Ϊ��ת����	
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}

	else if (t >= keyFrames.back().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&keyFrames.back().Scale);  //���ž���
		XMVECTOR P = XMLoadFloat3(&keyFrames.back().Translation);  //ƽ�ƾ���
		XMVECTOR Q = XMLoadFloat4(&keyFrames.back().RotationQuat); //��ת��Ԫ��

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		// XMMatrixAffineTransformation�ĵڶ�������Ϊ��ת����	
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else
	{
		for (UINT i = 0; i < keyFrames.size() - 1; ++i)
		{
			if (t > keyFrames[i].TimePos&& t < keyFrames[i + 1].TimePos)
			{
				float lerpPercent = (t - keyFrames[i].TimePos) / (keyFrames[i + 1].TimePos - keyFrames[i].TimePos);

				XMVECTOR S0 = XMLoadFloat3(&keyFrames[i].Scale);
				XMVECTOR S1 = XMLoadFloat3(&keyFrames[i + 1].Scale);

				XMVECTOR P0 = XMLoadFloat3(&keyFrames[i].Translation);
				XMVECTOR P1 = XMLoadFloat3(&keyFrames[i + 1].Translation);

				XMVECTOR Q0 = XMLoadFloat4(&keyFrames[i].RotationQuat);
				XMVECTOR Q1 = XMLoadFloat4(&keyFrames[i + 1].RotationQuat);

				//����������֡���Ķ�������Ӧ�仯�����в�ֵ

				//���������в�ֵ
				XMVECTOR S = XMVectorLerp(S0, S1, lerpPercent);

				//�ƶ������в�ֵ
				XMVECTOR P = XMVectorLerp(P0, P1, lerpPercent);

				//����ת�����в�ֵ
				XMVECTOR Q = XMVectorLerp(Q0, Q1, lerpPercent);

				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

				XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
			}
		}

	}
}


float AnimationClip::GetClipStartTime()const
{
	//�ҵ���һ������Ƭ�����й�ͷ�Ŀ�ʼʱ�����Сֵ
	float t = 10000000.0f;

	for (UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = std::min(t, BoneAnimations[i].GetStartTime());
	}

	return t;
}


float AnimationClip::GetClipEndTime()const
{
	//�ҵ���һ������Ƭ�����й�ͷ�Ľ���ʱ������ֵ
	float t = 0.0f;

	for (UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = std::max(t, BoneAnimations[i].GetEndTime());
	}

	return t;
}

//�õ����й�ͷ��ʱ��t�ı任��������,boneTransform[i]�����i����ͷ��ʱ��t�ı任����
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


//��ȡĳһʱ����ĳһ������Ƭ�ε����й�ͷ�ı任����
void  SkinnedDataClass::GetFinalTransforms(const string& AnimationClipName, float TimePos, vector<XMFLOAT4X4>& finalTransforms) const
{
	UINT numBones = mBoneOffsets.size();

	//��ֵ�õ�ĳ������Ƭ����ĳ��ʱ������й�ͷ��ĸ��ͷ�ı任����
	vector<XMFLOAT4X4> toParentTransform(numBones);

	auto AniClip = mAnimations.find(AnimationClipName);

	//���ĳ��ʱ������й�ͷ����Ӧ��ĸ��ͷ�ı任������ʵM3D����ͷ�洢��Scale,Translation,QuatΪ�任��ĸ��ͷ�ı任����
	AniClip->second.Interpolate(TimePos, toParentTransform);

	//�������в㼶���任���й�ͷ�����ڵ�ռ�
	vector<XMFLOAT4X4> toRootTransforms(numBones);

	//����ͷ���±�Ϊ0,����ͷû��ĸ��ͷ,������ĸ��ռ�任Ҳ���Ǹ��ڵ�ľֲ��ռ�任
	toRootTransforms[0] = toParentTransform[0];

	//�����ҵ�ÿ����ͷ�����ռ�ı任(�Զ�����,���Ӷ�ΪO(n),����Ե����ϣ����Ӷ�ΪO(n*n))
	for (UINT i = 1; i < numBones; ++i)
	{
		XMMATRIX toParent = XMLoadFloat4x4(&toParentTransform[i]);

		int parentIndex = mBoneHierarchy[i];

		XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

		XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	}

	//�õ�����Ƭ�����й�ͷ��ĳ��ʱ�������任(��ƽ�ƣ��ڽ��и��ռ�仯)
	for (UINT i = 0; i < numBones; ++i)
	{
		XMMATRIX offset = XMLoadFloat4x4(&mBoneOffsets[i]);

		XMMATRIX ToRoot = XMLoadFloat4x4(&toRootTransforms[i]);
		XMStoreFloat4x4(&finalTransforms[i], XMMatrixMultiply(offset, ToRoot));
	}

}
