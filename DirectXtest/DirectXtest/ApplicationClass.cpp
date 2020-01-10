////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Timer = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_ShaderManager = 0;
	m_TextureManager = 0;
	m_Zone = 0;
	m_Sound = 0;
	m_ImGui - 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd,bool fullscreen,int screenWidth, int screenHeight)
{
	bool result;

	

	// Create the input object.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, fullscreen, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	if (!m_ShaderManager)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	result = m_ShaderManager->InitializeMyShader(m_Direct3D->GetDevice(), L"./Shader/LightVertexShader.cso", L"./Shader/LightPixelShader.cso");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}



	// Create the texture manager object.
	m_TextureManager = new TextureManagerClass;
	if (!m_TextureManager)
	{
		return false;
	}

	// Initialize the texture manager object.
	result = m_TextureManager->Initialize(10);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the texture manager object.", L"Error", MB_OK);
		return false;
	}

	// Load textures into the texture manager.
	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"./Terrain/Texture/dirt01d.tga", 0);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"./Terrain/Texture/dirt01n.tga", 1);
	if (!result)
	{
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();


	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the fps object.
	m_Cpu->Initialize();

	// Create the zone object.
	m_Zone = new ZoneClass;
	if (!m_Zone)
	{
		return false;
	}


	// Initialize the zone object.
	result = m_Zone->Initialize(m_Direct3D, hwnd, screenWidth, screenHeight, SCREEN_DEPTH);
  	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the zone object.", L"Error", MB_OK);
		return false;
	}


	//Create the sound object
	m_Sound = new SoundClass();
	if (!m_Sound) {
		return false;
	}

	//Initialize the Sound object
	result = m_Sound->Initialize(hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize Sound Object.", L"Error", MB_OK);
		return false;
	}


	//Create ImGui object
	m_ImGui = new ImGuiClass();
	if (!m_ImGui) {
		return false;
	}

	//Initialize ImGui object
	result = m_ImGui->Initialize(hwnd,m_Direct3D);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize ImGUI Object.", L"Error", MB_OK);
		return false;
	}



	return true;
}

void ApplicationClass::Shutdown()
{
	// Release ImGui object.
	if (m_ImGui)
	{
		m_ImGui->ShutDown();
		delete m_ImGui;
		m_ImGui = 0;
	}



	// Release the zone object.
	if (m_Zone)
	{
		m_Zone->Shutdown();
		delete m_Zone;
		m_Zone = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the texture manager object.
	if (m_TextureManager)
	{
		m_TextureManager->Shutdown();
		delete m_TextureManager;
		m_TextureManager = 0;
	}

	// Release the shader manager object.
	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	//Release the Sound Class
	if (m_Sound) {
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}


	// Release the input object.
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


	// Update the system stats.
	m_Fps->Frame();
	m_Cpu->Frame();
	m_Timer->Frame();

	// Do the input frame processing.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}


	// Do the zone frame processing.
	result = m_Zone->Frame(m_Direct3D, m_Input, m_ShaderManager, m_TextureManager, m_Timer->GetTime(), m_Fps->GetFps(),m_Cpu->GetCpuPercentage());
	if (!result)
	{
		return false;
	}

	

	result = m_ImGui->Frame(m_Zone);




	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return result;
}



