#pragma once
///システムクラス;
// ここWIN32_LEAN_AND_MEANのマグロを定義によって、windows.hの一部だけ引用し、debugコンパイルのスピードを上がる;
// 参考:http://dosei.hatenadiary.jp/entry/20060605/p3
// 中国語:https://blog.csdn.net/wanwancs/article/details/6065775
// 英:Here we define WIN32_LEAN_AND_MEAN. We do this to speed up the build process, it reduces the size of the Win32 header files by excluding some of the less used APIs.;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../Utility/StringHelper.h"
#include "Engine/applicationclass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize(const wstring& configFile);
	void ProgramOver();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
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
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//グロバル;
static SystemClass* ApplicationHandle = 0;
