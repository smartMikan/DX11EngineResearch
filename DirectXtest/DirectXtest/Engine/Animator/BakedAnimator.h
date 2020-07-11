#pragma once

//
//#include<directxmath.h>
//#include <string>
//#include<vector>
//#include "../../Graphic/Buffers/ConstantBuffer.h"
#include"../ModelBaker/ModelBaker.h"
using namespace DirectX;

class BakedAnimator
{
public:
	BakedAnimator() = default;
	BakedAnimator(ConstantBuffer<ConstantBuffer_BakedBones>* boneConstantBuffer) :m_BoneConstantBuffer(boneConstantBuffer) {}
	~BakedAnimator();

	std::vector<BakedAnimationClip> bakedclips;


	void Bind(ID3D11DeviceContext* deviceContext, int animnum, double timepos);


	bool LoadAnimFromFile(std::string filename);

	float GetAnimDuration(int animindex);

private:
	int SetTransBuffer(XMMATRIX* prevout, int animnum, double timepos);

	ConstantBuffer<ConstantBuffer_BakedBones>* m_BoneConstantBuffer;
};
