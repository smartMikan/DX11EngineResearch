#include "BakedAnimator.h"



BakedAnimator::~BakedAnimator()
{
}



int BakedAnimator::Bind(ID3D11DeviceContext* deviceContext, int animnum, double timepos)
{
	int curnnetindex = SetTransBuffer(m_BoneConstantBuffer->data.bone_Transforms_Prev, animnum, timepos);
	m_BoneConstantBuffer->ApplyChanges();
	deviceContext->VSSetConstantBuffers(1, 1, m_BoneConstantBuffer->GetAddress());
	return curnnetindex;
}

bool BakedAnimator::LoadAnimFromFile(std::string filename)
{
	AnimationBaker baker;

	BakedAnimationClip clip;
	if (!baker.ReadBakedFile(clip, filename)) {
		return false;
	}

	bakedclips.emplace_back(clip);

	return true;
}

float BakedAnimator::GetAnimDuration(int animindex)
{
	return bakedclips[animindex].animduration;
}


int BakedAnimator::SetTransBuffer(XMMATRIX* prevout, int animnum, double timepos)
{
	double timestep = timepos * BAKE_FPS;

	int targetindex = (int)timestep;


	for (size_t i = 0; i < bakedclips[animnum].bonesize; i++)
	{
		prevout[i] = bakedclips[animnum].baked_bone_trans_datas[targetindex][i];
	}

	return targetindex;
}


