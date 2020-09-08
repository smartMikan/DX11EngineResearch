#pragma once


const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


#include <fstream>
#include <stdio.h>
#include <unordered_map>

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
	class MainGame* m_Game;

	//hardware
	class InputClass * m_Input;
	class TimerClass* m_Timer;
	class FpsClass* m_Fps;
	class CpuClass* m_Cpu;

	class D3DClass* m_Direct3D;

	//resource
	class SoundClass* m_Sound;
	class ShaderManagerClass* m_ShaderManager;
	class TextureManagerClass* m_TextureManager;

	//extern
	ImGuiClass* m_ImGui;

};
