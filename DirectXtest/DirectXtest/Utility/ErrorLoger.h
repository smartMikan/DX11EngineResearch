#pragma once

#include <Windows.h>
#include "COMException.h"

class ErrorLoger
{
public:
	static void Log(std::string message);
	static void Log(std::wstring message);
	static void Log(HRESULT hr,std::string message);
	static void Log(HRESULT hr, std::wstring message);
	static void Log(COMException& exception);

};
