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


	class AnimationChannel
	{
	public:
		//Get boneTrans of given time pos
		DirectX::XMMATRIX GetSample(float timestamp) const;
	public:
		std::vector<PositionKeyFrame> position_keyframes;
		std::vector<RotationKeyFrame> rotation_keyframes;
		std::vector<ScaleKeyFrame> scale_keyframes;
		int node_index;

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
		std::vector<AnimationChannel> channels;
		float duration;

		//Get all bone trans at specific timepos
		std::vector<DirectX::XMMATRIX> GetSample(float timepos, const std::vector<BoneNode>& avatar) const;
	};

	
	class Animator 
	{
	public:
		Animator() = default;
		Animator(std::vector<BoneNode> avator, std::vector<BoneData> bones,std::vector<AnimationClip> animations,ConstantBuffer<ConstantBuffer_Bones>* boneConstantBuffer):
			m_Avator(std::move(avator)),
			m_Bones(std::move(bones)),
			m_Animations(std::move(animations)),
			m_BoneConstantBuffer(boneConstantBuffer)
		{}

		void Bind(ID3D11DeviceContext* deviceContext);

		size_t GetNumAnimations() const { return m_Animations.size(); }
		const AnimationClip& GetAnimation(size_t index)const { return m_Animations[index]; }
		const AnimationClip* GetAnimationByName(const std::string& animationName)const;

		size_t GetCurrentAnimationIndex() const { return m_CurrentAnimationIndex; }
		void SetCurrentAnimationIndex(size_t index) { m_CurrentAnimationIndex = index; }
		const AnimationClip& GetCurrentAnimation()const { return m_Animations[m_CurrentAnimationIndex]; }

		float GetTimePos() const { return m_TimePos; }
		void SetTimpPos(float timepos) { m_TimePos = timepos; }

	private:
		void GetPoseOffsetTransforms(DirectX::XMMATRIX* out, const AnimationClip& animation, float timepos) const;

	private:
		std::vector<BoneData> m_Bones;
		std::vector<BoneNode> m_Avator;
		std::vector<AnimationClip> m_Animations;

		ConstantBuffer<ConstantBuffer_Bones>* m_BoneConstantBuffer;
		size_t m_CurrentAnimationIndex = 0;
		float m_TimePos = 0.0f;

	};

	struct AnimationNameAndChannel
	{
		AnimationNameAndChannel(const std::string& name, size_t channelIndex) :
			name(name),
			index(channelIndex)
		{}

		std::string name;
		size_t index;

	};

	class AnimationComponent 
	{
	public:
		AnimationComponent(Animator* animator)
			:
			m_Animator(animator)
		{}

		void AddChannel(const std::string& animName, size_t channelIndex) 
		{
			m_Channels.emplace_back(animName, channelIndex);
		}


		const AnimationChannel* GetChannel(const std::string& name)
		{
			for (const AnimationNameAndChannel& channel : m_Channels)
			{
				if (channel.name == name) 
				{
					const AnimationClip& currentAnim = m_Animator->GetCurrentAnimation();
					return &currentAnim.channels[channel.index];
				}
			}
			return nullptr;
		}


		const AnimationChannel* GetCurrentChannel() 
		{
			const AnimationClip& currentAnim = m_Animator->GetCurrentAnimation();
			return GetChannel(currentAnim.name);
		}


		DirectX::XMMATRIX GetSample()
		{
			const AnimationChannel* channel = GetCurrentChannel();
			float timepos = m_Animator->GetTimePos();
			return channel->GetSample(timepos);
		}



		Animator* GetAnimator()
		{
			return m_Animator;
		}

	private:
		

		std::vector<AnimationNameAndChannel> m_Channels;
		Animator* m_Animator;
	};

}


