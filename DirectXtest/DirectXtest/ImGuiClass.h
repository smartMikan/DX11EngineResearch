#pragma once


#include "Utility/StringHelper.h"
#include "d3dclass.h"
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_dx11.h"
#include "ImGUI\imgui_impl_win32.h"
#include "ZoneClass.h"

class ImGuiClass
{
public:
	ImGuiClass();
	~ImGuiClass();

	bool Initialize(HWND hwnd, D3DClass* Direct3D);

	void ShutDown();


	bool Frame(ZoneClass* zone);

};

