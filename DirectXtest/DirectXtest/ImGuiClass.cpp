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
	static float createEnemyPos[3] = { 0,0,0 };
	int EnemyCount;


	static bool lighttype = false;
	float lightRot[3];
	float lightpos[3];

	zone->m_Light->position.GetRotation(lightRot[0], lightRot[1], lightRot[2]);

	lightpos[0] = zone->m_Light->position.GetPosition().x;
	lightpos[1] = zone->m_Light->position.GetPosition().y;
	lightpos[2] = zone->m_Light->position.GetPosition().z;
	

	float ambientcolor[3];
	ambientcolor[0] = zone->m_Light->GetAmbientColor().x;
	ambientcolor[1] = zone->m_Light->GetAmbientColor().y;
	ambientcolor[2] = zone->m_Light->GetAmbientColor().z;

	float diffusecolor[3];
	diffusecolor[0] = zone->m_Light->GetDiffuseColor().x;
	diffusecolor[1] = zone->m_Light->GetDiffuseColor().y;
	diffusecolor[2] = zone->m_Light->GetDiffuseColor().z;

	

	//Start the Dear ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	////Create ImGui Test Window
	ImGui::Begin("WallPos");

	ImGui::Text("WallPosition:");
	ImGui::DragFloat3("Position X/Y/Z", zone->wallTranslation, 0.1f);
	ImGui::Text("WallRotation:");
	ImGui::DragFloat3("Rotation X/Y/Z", zone->wallRotation, 0.1f);
	ImGui::Text("WallScaling:");
	ImGui::DragFloat3("Scale X/Y/Z", zone->wallScaling, 0.1f);

	ImGui::End();


	//Create Light Window
	ImGui::Begin("Light");

	ImGui::DragFloat3("Light Rotation", lightRot, 0.1f);
	ImGui::DragFloat3("Light Pos", lightpos, 0.1f);
	ImGui::ColorEdit3("Light Ambient", ambientcolor);
	ImGui::ColorEdit3("Light Diffuse", diffusecolor);
	zone->m_Light->position.SetRotation(lightRot[0], lightRot[1], lightRot[2]);
	zone->m_Light->position.SetPosition(lightpos[0],lightpos[1],lightpos[2]);
	zone->m_Light->SetAmbientColor(ambientcolor[0], ambientcolor[1], ambientcolor[2],1.0f);
	zone->m_Light->SetDiffuseColor(diffusecolor[0],diffusecolor[1],diffusecolor[2],1.0f);
	ImGui::DragFloat("AmbientStrength", &zone->m_Light->ambientLightStrength, 0.01, 0.0f, 1.0f);
	ImGui::DragFloat("Dynamic Light Strength", &zone->m_Light->dynamicLightStrength, 0.01, 0.0f, 10.0f);
	ImGui::DragFloat("Dynamic Light Attenuation Base", &zone->m_Light->dynamicLightAttenuation_a, 0.001, 0.1f, 1.0f);
	ImGui::DragFloat("Dynamic Light Attenuation Distance", &zone->m_Light->dynamicLightAttenuation_b, 0.001, 0.0f, 1.0f);
	ImGui::DragFloat("Dynamic Light Attenuation DistancePow", &zone->m_Light->dynamicLightAttenuation_c, 0.001, 0.0f, 1.0f);
	ImGui::Checkbox("PointLight(Test)", &lighttype);
	zone->m_lightType = !lighttype ? 0 : 1;
	ImGui::Checkbox("ToonShader(Test)", &zone->toonShading);
	ImGui::End();


	////Create ImGui Test Window
	ImGui::Begin("InputList");

	ImGui::Text("UpArrow(or W):Move Forward");
	ImGui::Text("DownArrow(or S):Move Back");
	ImGui::Text("LeftArrow(or A):TurnLeft");
	ImGui::Text("RightArrow(or D):TurnRight");
	ImGui::Text("I (or Page Up):Look Up");
	ImGui::Text("K (or Page Down):Look Down");
	ImGui::Text("Space: Jump");
	ImGui::Text("Left Control(or Left Mouse Button) :Attack");
	ImGui::Text("");
	ImGui::Text("F1:Hide/DisPlay  UI");
	ImGui::Text("F2:Hide/DisPlay WireFrame");
	ImGui::Text("F3:Hide/DisPlay TerrainCell Frame");
	ImGui::Text("F6:Change SkyBox");

	ImGui::End();

	

	////Create Enemy!
	ImGui::Begin("CreateEnemy");
	ImGui::DragFloat3("Position X/Y/Z", createEnemyPos, 0.1f);
	if (ImGui::Button("CreateEnemy!")) 
	{
		zone->CreateEnemyAtPositon(createEnemyPos);
	}
	EnemyCount = zone->m_enemies.size();
	ImGui::Text("EnemyCount: ");
	ImGui::Text(to_string(EnemyCount).c_str());
	if (ImGui::Button("RemoveEnemy!"))
	{
		zone->RemoveEnemy(EnemyCount-1);
	}


	ImGui::End();


	
	//Assemble Together Draw Data
	ImGui::Render();

	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	


	return true;
}

