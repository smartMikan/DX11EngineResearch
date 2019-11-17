﻿#pragma once
#include "StringConverter.h"
#include <Windows.h>
#include <comdef.h>
class ErrorLoger
{
public:
	static void Log(std::string message);
	static void Log(HRESULT hr,std::string message);
	static void Log(HRESULT hr, std::wstring message);


};
