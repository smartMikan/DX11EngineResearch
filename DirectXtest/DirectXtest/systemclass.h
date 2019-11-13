#pragma once
///このファイルは、システムクラスを定義する
//////////////////////////
// Filename: SystemClass.h
//////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
// ここWIN32_LEAN_AND_MEANのマグロを定義によって、windows.hの一部だけ引用し、debugコンパイルのスピードを上がる
// 参考:http://dosei.hatenadiary.jp/entry/20060605/p3
// 中国語:https://blog.csdn.net/wanwancs/article/details/6065775
// 英:Here we define WIN32_LEAN_AND_MEAN. We do this to speed up the build process, it reduces the size of the Win32 header files by excluding some of the less used APIs.
#define WIN32_LEAN_AND_MEAN


#include <windows.h>
#include <tchar.h>
#include <string>
//
#include "inputclass.h"
#include "graphicsclass.h"
#include "soundclass.h"

#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "positionclass.h"
///////////////////////////
// Class name: SystemClass
///////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void ProgramOver();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	std::string m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	SoundClass* m_Sound;

	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;

	PositionClass* m_Position;

	bool m_IsWireframeMode = 0;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
//プロトタイプ宣言
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
//グロバル
static SystemClass* ApplicationHandle = 0;


#endif