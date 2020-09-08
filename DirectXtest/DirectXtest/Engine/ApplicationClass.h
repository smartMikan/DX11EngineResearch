#pragma once


const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


#include <fstream>
#include <stdio.h>
#include <unordered_map>

#include "inputclass.h"
#include "timerclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "soundclass.h"
#include "d3dclass.h"

#include "../Graphic/RasterTekObjects/ShaderManagerClass.h"
#include "../Graphic/texturemanagerclass.h"
#include "../MainGame.h"
#include "../ImGuiClass.h"

/// <summary>
/// Application interface
/// </summary>
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND,bool, int, int);
	void Shutdown();
	bool Frame();


private:
	//Game logic
	MainGame* m_Game;

	//hardware
	InputClass * m_Input;
	D3DClass* m_Direct3D;
	TimerClass* m_Timer;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;

	//resource
	SoundClass* m_Sound;
	ShaderManagerClass* m_ShaderManager;
	TextureManagerClass* m_TextureManager;

	//extern
	ImGuiClass* m_ImGui;

};
