#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


//const bool FULL_SCREEN = full;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


#include <fstream>
#include <stdio.h>
#include "Engine/inputclass.h"
#include "Engine/d3dclass.h"
#include "Graphic/RasterTekObjects/ShaderManagerClass.h"
#include "texturemanagerclass.h"
#include "Engine/timerclass.h"
#include "Engine/fpsclass.h"
#include "Engine/cpuclass.h"
#include "zoneclass.h"
#include "Engine/soundclass.h"
#include "ImGuiClass.h"

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
	InputClass * m_Input;
	D3DClass* m_Direct3D;
	ShaderManagerClass* m_ShaderManager;
	TextureManagerClass* m_TextureManager;
	TimerClass* m_Timer;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	ZoneClass* m_Zone;
	SoundClass* m_Sound;
	ImGuiClass* m_ImGui;
};

#endif