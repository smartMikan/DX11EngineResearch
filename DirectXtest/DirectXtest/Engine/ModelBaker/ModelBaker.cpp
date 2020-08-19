#include "ModelBaker.h"

#include<iostream>
#include<fstream>
#include<bitset>

ModelBaker::ModelBaker()
{
}

ModelBaker::~ModelBaker()
{
}

AnimationBaker::AnimationBaker()
{
}

AnimationBaker::~AnimationBaker()
{
}

bool AnimationBaker::ReadBakedFile(BakedAnimationClip& outputAddress, std::wstring filename)
{
	std::wstring bakedmodelfolder = L"BakedModels/";
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG // Debug Mode
#ifdef _WIN64 // x64
		bakedmodelfolder = L"../x64/Debug/BakedModels/";
#else	// x86
		bakedmodelfolder = L"../Debug/BakedModels/";

#endif
#else	// Release Mode
#ifdef _WIN64
		bakedmodelfolder = L"../x64/Release/BakedModels/";
#else	// x86
		bakedmodelfolder = L"../Release/BakedModels/";
#endif
#endif
	}
	BakedAnimationClip AnimClip;


	std::ifstream inputfile(bakedmodelfolder + filename, std::ios::in | std::ios::binary);

	if (!inputfile.is_open())
	{
		ErrorLoger::Log(L"File Doesn't exist" + bakedmodelfolder + filename);
		return false;
	}

	inputfile.read((char*)&AnimClip.keyframe_count, sizeof(AnimClip.keyframe_count));
	inputfile.read((char*)&AnimClip.animduration, sizeof(AnimClip.animduration));
	inputfile.read((char*)&AnimClip.bonesize, sizeof(AnimClip.bonesize));


	AnimClip.baked_bone_trans_datas.resize(AnimClip.keyframe_count);


	for (size_t i = 0; i < AnimClip.keyframe_count; i++)
	{
		for (size_t j = 0; j < AnimClip.bonesize; j++)
		{
			XMFLOAT4X4 matrix;

			for (size_t k = 0; k < 4; k++)
			{
				for (size_t l = 0; l < 4; l++)
				{
					inputfile.read((char*)&matrix.m[k][l], sizeof(matrix.m[k][l]));
				}
			}

			AnimClip.baked_bone_trans_datas[i].emplace_back(XMLoadFloat4x4(&matrix));
		}
	}

	inputfile.close();

	outputAddress = AnimClip;

	return true;
}

bool AnimationBaker::BakeAnim(AssimpModel::Animator* animator, const AssimpModel::AnimationClip& animation, std::wstring outputfilename)
{
	std::wstring bakedmodelfolder = L"BakedModels/";

	if (IsDebuggerPresent())
	{
#ifdef _DEBUG // Debug Mode
#ifdef _WIN64 // x64
		bakedmodelfolder = L"../x64/Debug/BakedModels/";
#else	// x86
		bakedmodelfolder = L"../Debug/BakedModels/";

#endif
#else	// Release Mode
#ifdef _WIN64
		bakedmodelfolder = L"../x64/Release/BakedModels/";
#else	// x86
		bakedmodelfolder = L"../Release/BakedModels/";
#endif
#endif
	}
	BakedAnimationClip outputclip;

	outputclip.bonesize = animator->GetBoneSize();

	float animduration = animation.duration;
	double currenttimepos = 0.0;
	outputclip.animduration = animduration;

	outputclip.keyframe_count = floor(animduration * BAKE_FPS) + 1;
	outputclip.baked_bone_trans_datas.resize(outputclip.keyframe_count);

	double timeStep = animduration / outputclip.keyframe_count;

	for (int i = 0; i < outputclip.keyframe_count; i++)
	{
		XMMATRIX matrix[256];
		animator->GetPoseOffsetTransforms(matrix, animation, currenttimepos);
		for (int j = 0; j < outputclip.bonesize; j++)
		{
			outputclip.baked_bone_trans_datas[i].emplace_back(matrix[j]);
		}

		currenttimepos += timeStep;
	}

	std::ofstream outputfile;
	outputfile.open(bakedmodelfolder + outputfilename + MyAnimFileExtension, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!outputfile.is_open())
	{
		ErrorLoger::Log("Failed to create file");
		return false;
	}
	

	//std::cin >> outputclip.keyframe_count;

	outputfile.write((char*)&outputclip.keyframe_count, sizeof(outputclip.keyframe_count));

	//std::cin >> outputclip.timestep;

	outputfile.write((char*)&outputclip.animduration, sizeof(outputclip.animduration));

	//std::cin >> outputclip.bonesize;
	outputfile.write((char*)&outputclip.bonesize, sizeof(outputclip.bonesize));


	for (size_t i = 0; i < outputclip.baked_bone_trans_datas.size(); i++)
	{
		for (size_t j = 0; j < outputclip.bonesize; j++)
		{
			XMFLOAT4X4 matrix;
			XMStoreFloat4x4(&matrix, outputclip.baked_bone_trans_datas[i][j]);
			for (size_t k = 0; k < 4; k++)
			{
				for (size_t l = 0; l < 4; l++)
				{
					//std::cin >> matrix.m[k][l];
					outputfile.write((char*)&matrix.m[k][l], sizeof(matrix.m[k][l]));
				}
			}
		}
	}

	outputfile.close();
	return true;
}


