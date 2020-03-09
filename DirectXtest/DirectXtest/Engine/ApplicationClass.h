#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


//const bool FULL_SCREEN = full;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


#include <fstream>
#include <stdio.h>

#include "inputclass.h"
#include "timerclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "soundclass.h"
#include "d3dclass.h"

#include "../Graphic/RasterTekObjects/ShaderManagerClass.h"
#include "../texturemanagerclass.h"
#include "../zoneclass.h"
#include "../ImGuiClass.h"

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