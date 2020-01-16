#pragma once
#include "Animation.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//アニメーションをプレイ、現在アニメーション時間を管理するクラスです。
class CAnimationController
{
public:
	CAnimationController();
	~CAnimationController(){}

	/////////////// Controller Part ///////////////

	void UpdateTime(float fElapsedTime);

	void Play(std::string _AnimationName);
	void SetAnimationData(std::string _AnimationName, float _fCurrentAnimLength);
	void Stop();
	void Pause();

	GET_SET_FUNC_IMPL(float, CurrentAnimationTime, m_fCurrentAnimTime);
	GET_SET_FUNC_IMPL(string, CurrentAnimationName, m_CurrentPlayingAnimationName);

private:
	std::string						m_CurrentPlayingAnimationName;
	float						m_fCurrentAnimTime;
	float						m_fCurrentAnimLength;
	bool						m_bIsPlaying;

	std::vector<std::pair<std::string, float>>	m_vAnimationTimeData;
};