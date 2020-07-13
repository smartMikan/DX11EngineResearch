#pragma once


#include<directxmath.h>
#include <string>
#include<vector>
#include "Buffers/ConstantBuffer.h"


namespace AssimpModel 
{

	struct BoneData 
	{
		std::string name;
		int index;
		DirectX::XMMATRIX inverse_transform;
	};

	struct BoneNode
	{
		DirectX::XMMATRIX local_transform;
		int parent_index;
	};


	template<typename T>
	struct  KeyFrame
	{
		T value;
		
		float timePos;

		bool operator==(const KeyFrame& rhs) 
		{
			return value == rhs.value;
		}
		bool operator!=(const KeyFrame& rhs)
		{
			return value != rhs.value;
		}
		bool operator<(const KeyFrame& rhs)
		{
			return timePos < rhs.timePos;
		}
		bool operator<=(const KeyFrame& rhs)
		{
			return timePos <= rhs.timePos;
		}
		bool operator>(const KeyFrame& rhs)
		{
			return timePos > rhs.timePos;
		}
		bool operator>=(const KeyFrame& rhs)
		{
			return timePos >= rhs.timePos;
		}

	};

	using PositionKeyFrame = KeyFrame<DirectX::XMFLOAT3>; // Value is position in bone space
	using RotationKeyFrame = KeyFrame<DirectX::XMFLOAT4>; // Value is rotation in bone space as a quaternion
	using ScaleKeyFrame = KeyFrame<DirectX::XMFLOAT3>;   // Value is scale in bone space


	class BoneChannel
	{
	public:
		//Get boneTrans of given time pos
		DirectX::XMMATRIX GetChannelKeyFrameSample(float timestamp) const;
	public:
		std::vector<PositionKeyFrame> position_keyframes;
		std::vector<RotationKeyFrame> rotation_keyframes;
		std::vector<ScaleKeyFrame> scale_keyframes;
		int node_index;
		std::string bone_name;

	private:
		void ResetCache() const;

	private:
		mutable size_t last_pos_index = 0;
		mutable size_t last_rot_index = 0;
		mutable size_t last_scale_index = 0;
		mutable float last_timepos = 0.0f;

	};

	struct AnimationClip 
	{
		std::string name;
		std::vector<BoneChannel> bonechannels;
		BoneChannel rootBoneChannel;
		float duration;

		//Get all bone trans at specific timepos
		std::vector<DirectX::XMMATRIX> GetSample(float timepos, const std::vector<BoneNode>& bonenodes) const;
	};

	
	class Animator 
	{
	public:
		Animator() = default;
		Animator(std::vector<BoneNode> avator, std::vector<BoneData> bones,std::vector<AnimationClip> animations,ConstantBuffer<ConstantBuffer_Bones>* boneConstantBuffer):
			m_AllNodeAvator(std::move(avator)),
			m_Bones(std::move(bones)),
			m_Animations(std::move(animations)),
			m_BoneConstantBuffer(boneConstantBuffer)
		{}

		void SetBufferPointer(ConstantBuffer<ConstantBuffer_Bones>* boneConstantBuffer);
		void SetBones(std::vector<BoneNode> avator, std::vector<BoneData> bones) { m_AllNodeAvator = std::move(avator); m_Bones = std::move(bones); }


		void Bind(ID3D11DeviceContext* deviceContext);


		size_t GetNumAnimations() const { return m_Animations.size(); }
		const AnimationClip& GetAnimation(size_t index)const { return m_Animations[index]; }
		const AnimationClip* GetAnimationByName(const std::string& animationName)const;

		size_t GetCurrentAnimationIndex() const { return m_CurrentAnimationIndex; }
		void SetCurrentAnimationIndex(size_t index) { m_CurrentAnimationIndex = index; }
		const AnimationClip& GetCurrentAnimation()const { return m_Animations[m_CurrentAnimationIndex]; }

		void AddAnim(AnimationClip& anim);

		float GetTimePos() const { return m_TimePos; }
		void SetTimpPos(float timepos) { m_TimePos = timepos; }

	
		void GetPoseOffsetTransforms(DirectX::XMMATRIX* out, const AnimationClip& animation, float timePos) const;

		int GetBoneSize() const { return m_AllNodeAvator.size(); }

	private:
		std::vector<BoneData> m_Bones;
		std::vector<BoneNode> m_AllNodeAvator;
		std::vector<AnimationClip> m_Animations;

		ConstantBuffer<ConstantBuffer_Bones>* m_BoneConstantBuffer;
		size_t m_CurrentAnimationIndex = 0;
		float m_TimePos = 0.0f;

	};

	struct AnimationNameAndBoneChannel
	{
		AnimationNameAndBoneChannel(const std::string& bonename, size_t channelIndex) :
			m_bonename(bonename),
			index(channelIndex)
		{}

		std::string m_bonename;
		size_t index;

	};

	class AnimationComponent 
	{
	public:
		AnimationComponent(Animator* animator)
			:
			m_Animator(animator)
		{}

		void AddBoneChannel(const std::string& nodeName, size_t channelIndex) 
		{
			m_BoneChannels.emplace_back(nodeName, channelIndex);
		}


		const BoneChannel* GetChannelByBoneName(const std::string& bonename)
		{
			for (const AnimationNameAndBoneChannel& channel : m_BoneChannels)
			{
				if (channel.m_bonename == bonename)
				{
					const AnimationClip& currentAnim = m_Animator->GetCurrentAnimation();
					return &currentAnim.bonechannels[channel.index];
				}
			}
			return nullptr;
		}


		const BoneChannel* GetCurrentBoneChannel() 
		{
			const AnimationClip& currentAnim = m_Animator->GetCurrentAnimation();
			return GetChannelByBoneName(currentAnim.bonechannels[0].bone_name);
		}


		DirectX::XMMATRIX GetSample()
		{
			const BoneChannel* channel = GetCurrentBoneChannel();
			float timepos = m_Animator->GetTimePos();
			return channel->GetChannelKeyFrameSample(timepos);
		}



		Animator* GetAnimator()
		{
			return m_Animator;
		}

	private:
		

		std::vector<AnimationNameAndBoneChannel> m_BoneChannels;
		Animator* m_Animator;
	};

}


