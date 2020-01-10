#include "ImGuiClass.h"



ImGuiClass::ImGuiClass()
{
}


ImGuiClass::~ImGuiClass()
{
}

bool ImGuiClass::Initialize(HWND hwnd, D3DClass* Direct3D)
{

	//SetUP ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Direct3D->GetDevice(), Direct3D->GetDeviceContext());
	ImGui::StyleColorsClassic();
	return true;
}


void ImGuiClass::ShutDown()
{


}


bool ImGuiClass::Frame(ZoneClass* zone)
{

	static int counter = 0;


	//Start the Dear ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Create ImGui Test Window
	ImGui::Begin("Test");
	

	ImGui::Text("MyFirst ImGui!");
	if (ImGui::Button("Click Me!!")) 
	{
		counter += 1;
	}

	ImGui::SameLine();
	std::string clickCount = "Click Count :" + std::to_string(counter);
	ImGui::Text(clickCount.c_str());


	ImGui::DragFloat3("Translation X/Y/Z", zone->cubeTranslation, 0.1f);











	ImGui::End();

	//Assemble Together Draw Data
	ImGui::Render();

	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());




	return true;
}

