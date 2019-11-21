#pragma once


#define WIN32_LEAN_AND_MEAN


#include <windows.h>
#include <tchar.h>
#include <string>


#include "applicationclass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& other);
	~SystemClass();

	bool Initialize();
	void Release();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	std::string m_applicationName;
	std::wstring m_applicationName_wide;
	LPCSTR m_applicationName_LPCSTR;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	ApplicationClass* m_Application;
};



static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

static SystemClass* ApplicationHandle = 0;


