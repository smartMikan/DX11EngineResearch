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
	float lightdir[3];
	float lightvec[3];
	zone->m_Light->m_position.GetRotation(lightdir[0], lightdir[1], lightdir[2]);

	lightvec[0] = zone->m_Light->GetDirection().x;
	lightvec[1] = zone->m_Light->GetDirection().y;
	lightvec[2] = zone->m_Light->GetDirection().z;


	float ambientcolor[3];
	ambientcolor[0] = zone->m_Light->GetAmbientColor().x;
	ambientcolor[1] = zone->m_Light->GetAmbientColor().y;
	ambientcolor[2] = zone->m_Light->GetAmbientColor().z;

	float diffusecolor[3];
	diffusecolor[0] = zone->m_Light->GetDiffuseColor().x;
	diffusecolor[1] = zone->m_Light->GetDiffuseColor().y;
	diffusecolor[2] = zone->m_Light->GetDiffuseColor().z;

	float timepos = zone->mCharacterInstance2.TimePos;




	//Start the Dear ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Create ImGui Test Window
	ImGui::Begin("CubePos");
	if (ImGui::Button("Click Me!!")) 
	{
		counter += 1;
	}

	ImGui::SameLine();
	std::string clickCount = "Click Count :" + std::to_string(counter);
	ImGui::Text(clickCount.c_str());


	ImGui::Text("CubePosition:");

	ImGui::DragFloat3("Translation X/Y/Z", zone->cubeTranslation, 0.1f);

	ImGui::End();

	//Create ImGui Test Window
	ImGui::Begin("Light");

	ImGui::DragFloat3("Light Direction", lightdir, 0.1f);
	ImGui::DragFloat3("Light Vec", lightvec, 0.1f);
	ImGui::ColorEdit3("Light Ambient", ambientcolor);
	ImGui::ColorEdit3("Light Diffuse", diffusecolor);
	zone->m_Light->m_position.SetRotation(lightdir[0], lightdir[1], lightdir[2]);
	zone->m_Light->SetAmbientColor(ambientcolor[0], ambientcolor[1], ambientcolor[2],1.0f);
	zone->m_Light->SetDiffuseColor(diffusecolor[0],diffusecolor[1],diffusecolor[2],1.0f);

	ImGui::End();

	ImGui::Begin("Anim Model Instance");
	ImGui::Text("Animation Clip: ");
	ImGui::SameLine();
	ImGui::Text(zone->mCharacterInstance2.ClipName.c_str());
	ImGui::SliderFloat("AnimClipTime", &timepos, 0.0f, zone->mCharacterInstance2.Model->SkinnedData.GetClipEndTime(zone->mCharacterInstance2.ClipName));
	zone->mCharacterInstance2.Update(timepos - zone->mCharacterInstance2.TimePos);

	ImGui::End();


	//Assemble Together Draw Data
	ImGui::Render();

	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	return true;
}

