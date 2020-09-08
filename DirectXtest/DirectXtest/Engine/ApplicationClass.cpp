#include "applicationclass.h"

#include "inputclass.h"
#include "timerclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "soundclass.h"
#include "d3dclass.h"


#include "../Graphic/RasterTekObjects/ShaderManagerClass.h"
#include "../Graphic/texturemanagerclass.h"
#include "../MainGame.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Timer = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_ShaderManager = 0;
	m_TextureManager = 0;
	m_Game = 0;
	m_Sound = 0;
	m_ImGui = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
	m_Input = other.m_Input;
	m_Direct3D = other.m_Direct3D;
	m_Timer = other.m_Timer;
	m_Fps = other.m_Fps;
	m_Cpu = other.m_Cpu;
	m_ShaderManager = other.m_ShaderManager;
	m_TextureManager = other.m_TextureManager;
	m_Game = other.m_Game;
	m_Sound = other.m_Sound;
	m_ImGui = other.m_ImGui;
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, bool fullscreen, int screenWidth, int screenHeight)
{
	bool result;

	//Initialize DirectInput.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	//Initialize system timer.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	//Initialize fps observer.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}
	m_Fps->Initialize();


	//Initialize cpu observer.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}
	m_Cpu->Initialize();

	//Initialize D3D
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, fullscreen, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}


	//Initialize Sound
	m_Sound = new SoundClass();
	if (!m_Sound) {
		return false;
	}
	result = m_Sound->Initialize(hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize Sound Object.", L"Error", MB_OK);
		return false;
	}


	//Initialize shader manager.
	m_ShaderManager = new ShaderManagerClass;
	if (!m_ShaderManager)
	{
		return false;
	}

	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd, m_Direct3D->GetDeviceContext());
	result = m_ShaderManager->InitializeMyShader(m_Direct3D->GetDevice(), L"./Shader/LightVertexShader.cso", L"./Shader/LightPixelShader.cso");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	//Initialize texture manager.
	m_TextureManager = new TextureManagerClass;
	if (!m_TextureManager)
	{
		return false;
	}

	result = m_TextureManager->Initialize(10, m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the texture manager object.", L"Error", MB_OK);
		return false;
	}

	//Load textures into the texture manager.
	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"./Resources/Terrain/Texture/dirt01d.tga", 0);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not Load the texture.", L"Error", MB_OK);
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"./Resources/Terrain/Texture/dirt01n.tga", 1);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not Load the texture.", L"Error", MB_OK);
		return false;
	}

	//Initialize Main Game.
	m_Game = new MainGame;
	if (!m_Game)
	{
		return false;
	}

	int zresult = m_Game->Initialize(m_Direct3D, hwnd, screenWidth, screenHeight, SCREEN_DEPTH, m_Input, m_ShaderManager, m_TextureManager);
	switch (zresult)
	{
	case 0:
		//Initial failure
		MessageBoxW(hwnd, L"Could not initialize the zone object.", L"Error", MB_OK);
		return false;
	case 1:
		//normal mode success
		break;
	case 2:

		///bakemodel mode success
		MessageBoxW(hwnd, L"BakeModelOver.", L"Error", MB_OK);
		return false;
	default:
		break;
	}

	


	//Initialize ImGui
	m_ImGui = new ImGuiClass();
	if (!m_ImGui) {
		return false;
	}
	result = m_ImGui->Initialize(hwnd, m_Direct3D);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize ImGUI Object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown()
{
	//Release ImGui object.
	if (m_ImGui)
	{
		m_ImGui->ShutDown();
		delete m_ImGui;
		m_ImGui = 0;
	}

	//Release Main Game.
	if (m_Game)
	{
		m_Game->Shutdown();
		delete m_Game;
		m_Game = 0;
	}

	//Release the Sound Class
	if (m_Sound) {
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	//Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	//Release the cpu object.
	if (m_Cpu)
	{
		delete m_Cpu;
		m_Cpu = 0;
	}

	//Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	//Release the texture manager object.
	if (m_TextureManager)
	{
		m_TextureManager->Shutdown();
		delete m_TextureManager;
		m_TextureManager = 0;
	}

	//Release the shader manager object.
	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	//Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}


	//Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}

bool ApplicationClass::Frame()
{
	bool result;
	//Update the system stats.
	m_Fps->Frame();
	m_Cpu->Frame();
	m_Timer->Frame();

	//Do the input frame processing.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	//Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	//Do Main Game Loop.
	result = m_Game->Frame(m_Timer->GetDeltaTime(), m_Fps->GetFps(), m_Cpu->GetCpuPercentage());
	if (!result)
	{
		return false;
	}

	//Do ImGui Loop
	result = m_ImGui->Frame(m_Game);
	if (!result)
	{
		return false;
	}

	//Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return result;
}



