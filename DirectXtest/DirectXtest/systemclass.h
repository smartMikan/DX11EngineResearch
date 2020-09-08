#pragma once

///Windows Application Wrap Class

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../Utility/StringHelper.h"
#include "Engine/applicationclass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& other);
	~SystemClass();

	bool Initialize(const wstring& configFile);
	void ProgramOver();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

	bool LoadConfigFile(const wstring& filename);
private:
	std::string m_applicationName;
	std::wstring m_applicationName_wide;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	ApplicationClass* m_Application;
	bool FULLSCREEN = false;
	
};



//プロトタイプ宣言;
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);


//グロバル;
static SystemClass* ApplicationHandle = 0;
