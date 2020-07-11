#include "AssimpSkinnedData.h"
#include <algorithm>

namespace AssimpModel {
	template<typename T>
	static void GetPrevAndNextKeyFrame(const std::vector<KeyFrame<T>>& keyframes, float timePos, size_t start_at, size_t* pPrevKeyFrame, size_t* pNextKeyFrame)
	{
		*pPrevKeyFrame = start_at;
		*pNextKeyFrame = *pPrevKeyFrame;
		for (size_t i = start_at; i < keyframes.size(); i++)
		{
			*pNextKeyFrame = i;
			if (keyframes[*pNextKeyFrame].timePos > timePos)
			{
				break;
			}
			*pPrevKeyFrame = *pNextKeyFrame;
		}
	}

	static DirectX::XMFLOAT3 InterpolatePosKeyFrame(const PositionKeyFrame& prev_key, const PositionKeyFrame& next_key, float timePos)
	{
		const float range = next_key.timePos - prev_key.timePos;
		const float dt = (timePos - prev_key.timePos) / range;
		DirectX::XMFLOAT3 result = { next_key.value.x * dt ,next_key.value.y * dt ,next_key.value.z * dt };
		result.x += prev_key.value.x * (1 - dt);
		result.y += prev_key.value.y * (1 - dt);
		result.z += prev_key.value.z * (1 - dt);
		return result;
	}

	static DirectX::XMFLOAT4 InterpolateRotKeyFrame(const RotationKeyFrame& prev_key, const RotationKeyFrame& next_key, float timePos)
	{
		float dt;
		if (next_key.timePos == prev_key.timePos)
			dt = 0.0f;
		else
		{
			const float range = next_key.timePos - prev_key.timePos;
			dt = (timePos - prev_key.timePos) / range;
		}
		DirectX::XMFLOAT4 ret;
		DirectX::XMStoreFloat4(&ret, DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&prev_key.value), DirectX::XMLoadFloat4(&next_key.value), dt));
		return ret;
	}

	static DirectX::XMFLOAT3 InterpolateScaleKeyFrame(const ScaleKeyFrame& prev_key, const ScaleKeyFrame& next_key, float timePos)
	{
		const float range = next_key.timePos - prev_key.timePos;
		const float dt = (timePos - prev_key.timePos) / range;
		DirectX::XMFLOAT3 result = { next_key.value.x * dt ,next_key.value.y * dt ,next_key.value.z * dt };
		result.x += prev_key.value.x * (1 - dt);
		result.y += prev_key.value.y * (1 - dt);
		result.z += prev_key.value.z * (1 - dt);
		return result;
	}

	static DirectX::XMFLOAT3 GetPosKeyFrameAt(const std::vector<PositionKeyFrame>& keyframes, size_t* last_pos_index, float timePos)
	{
		if (keyframes.size() == 1)
			return keyframes[0].value;

		// clamp timestamp
		if (timePos <= keyframes.front().timePos)
			return keyframes.front().value;
		if (timePos >= keyframes.back().timePos)
			return keyframes.back().value;

		size_t prev_key_frame;
		size_t next_key_frame;
		GetPrevAndNextKeyFrame(keyframes, timePos, *last_pos_index, &prev_key_frame, &next_key_frame);
		*last_pos_index = prev_key_frame;
		return InterpolatePosKeyFrame(keyframes[prev_key_frame], keyframes[next_key_frame], timePos);
	}

	static DirectX::XMFLOAT4 GetRotKeyFrameAt(const std::vector<RotationKeyFrame>& keyframes, size_t* last_rot_index, float timestamp)
	{
		if (keyframes.size() == 1)
			return keyframes[0].value;
		size_t prev_key_frame;
		size_t next_key_frame;
		GetPrevAndNextKeyFrame(keyframes, timestamp, *last_rot_index, &prev_key_frame, &next_key_frame);
		*last_rot_index = prev_key_frame;
		return InterpolateRotKeyFrame(keyframes[prev_key_frame], keyframes[next_key_frame], timestamp);
	}

	static DirectX::XMFLOAT3 GetScaleKeyFrameAt(const std::vector<ScaleKeyFrame>& keyframes, size_t* last_pos_index, float timePos)
	{
		if (keyframes.size() == 1)
			return keyframes[0].value;

		// clamp timestamp
		if (timePos <= keyframes.front().timePos)
			return keyframes.front().value;
		if (timePos >= keyframes.back().timePos)
			return keyframes.back().value;

		size_t prev_key_frame;
		size_t next_key_frame;
		GetPrevAndNextKeyFrame(keyframes, timePos, *last_pos_index, &prev_key_frame, &next_key_frame);
		*last_pos_index = prev_key_frame;
		return InterpolateScaleKeyFrame(keyframes[prev_key_frame], keyframes[next_key_frame], timePos);
	}

	DirectX::XMMATRIX AnimationChannel::GetChannelKeyFrameSample(float timePos) const
	{
		if (timePos < last_timepos)
		{
			ResetCache();
		}
		last_timepos = timePos;

		// No channels case
		if (position_keyframes.empty() || rotation_keyframes.empty() || rotation_keyframes.empty())
		{
			assert(position_keyframes.empty() && rotation_keyframes.empty() && rotation_keyframes.empty(), "no keyframe type provided");
			return DirectX::XMMatrixIdentity();
		}

		DirectX::XMFLOAT3 interp_pos = GetPosKeyFrameAt(position_keyframes, &last_pos_index, timePos);
		DirectX::XMFLOAT4 interp_rot = GetRotKeyFrameAt(rotation_keyframes, &last_rot_index, timePos);
		DirectX::XMFLOAT3 interp_scale = GetScaleKeyFrameAt(scale_keyframes, &last_scale_index, timePos);

		return DirectX::XMMatrixScaling(interp_scale.x, interp_scale.y, interp_scale.z) * DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&interp_rot)) *
			DirectX::XMMatrixTranslation(interp_pos.x, interp_pos.y, interp_pos.z);
	}

	void AnimationChannel::ResetCache() const
	{
		last_pos_index = 0;
		last_rot_index = 0;
		last_scale_index = 0;
	}

	std::vector<DirectX::XMMATRIX> AnimationClip::GetSample(float timePos, const std::vector<BoneNode>& bonenodes) const
	{

		//double time;
		//LARGE_INTEGER t1, t2, tc;
		//QueryPerformanceFrequency(&tc);


		std::vector<DirectX::XMMATRIX> node_localtransforms;
		node_localtransforms.reserve(bonenodes.size());


		std::transform(bonenodes.begin(), bonenodes.end(), std::back_inserter(node_localtransforms), [](const BoneNode& node)
		{
			return DirectX::XMMatrixIdentity();

		});

		//QueryPerformanceCounter(&t1);

		for (const AnimationChannel& channel : channels)
		{
			node_localtransforms[channel.node_index] = channel.GetChannelKeyFrameSample(timePos);
		}
		
		/*for (int i = 0,ie = channels.size();i<ie;i++)
		{
			node_localtransforms[channels[i].node_index] = channels[i].GetChannelKeyFrameSample(timePos);
		}*/
		//QueryPerformanceCounter(&t2);
		//time = (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;

		//
		//all BoneNode WillGet OffsetTransformMatrix At keyFrame and Non-BoneNode will get XMMatrixIdentity();
		//ErrorLoger::Log(std::to_string(time) + "ms");


		return node_localtransforms;
	}

	void Animator::Bind(ID3D11DeviceContext* deviceContext)
	{
		const AnimationClip& animation = GetAnimation(GetCurrentAnimationIndex());

		GetPoseOffsetTransforms(m_BoneConstantBuffer->data.bone_Transforms, animation, GetTimePos());

		m_BoneConstantBuffer->ApplyChanges();
		deviceContext->VSSetConstantBuffers(1, 1, m_BoneConstantBuffer->GetAddress());
	}

	

	const AnimationClip* Animator::GetAnimationByName(const std::string& name) const
	{
		for (const AnimationClip animation : m_Animations)
		{
			if (animation.name == name)
				return &animation;
		}
		return nullptr;
	}

	void Animator::AddAnim(AnimationClip& anim)
	{
		m_Animations.push_back(anim);
	}

	void Animator::GetPoseOffsetTransforms(DirectX::XMMATRIX* out, const AnimationClip& animation, float timePos) const
	{
		/*double time;
		LARGE_INTEGER t1, t2, tc;
		QueryPerformanceFrequency(&tc);*/

		assert(m_Bones.size() <= MAX_BONES, "bone num out of limit");


		//Contarins All Node include Non-Bone Nodes
		std::vector<DirectX::XMMATRIX> node_localtransforms = animation.GetSample(timePos, m_AllNodeAvator);


		//QueryPerformanceCounter(&t1);
		for (int i = 1, ie = node_localtransforms.size(); i < ie; i++)
		{
			//For This is an Top-Down Three(Such as node[i].parentIndex will actually less than i)
			//so All Nodes Front Should Get Correct Transform to root Before Nodes Back 
			//then these back nodes can just multiply front nodes transfrom to get correct final transform  
			node_localtransforms[i] = node_localtransforms[i] * node_localtransforms[m_AllNodeAvator[i].parent_index];

		}


		for (int i = 0; i < m_Bones.size(); i++)
		{
			//Only Return Bone-Node Data, m_Bones[i].inverse_transform has store the Bones initial offset to rootNode so that we don't need to get localtrans for that bones again like meshes
			out[i] = XMMatrixTranspose(m_Bones[i].inverse_transform * node_localtransforms[m_Bones[i].index]);
		}

		//QueryPerformanceCounter(&t2);


		//time = (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
		//ErrorLoger::Log(std::to_string(time) + "ms");
		//ErrorLoger::Log(std::to_string(m_Bones.size()));
	}

}




