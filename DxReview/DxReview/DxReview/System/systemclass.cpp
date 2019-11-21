
#include "systemclass.h"

///
///コンストラクタ
///オブジェクトポインタをNULLポインタとして初期化する
SystemClass::SystemClass()
{
	m_Application = 0;
	
}

SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}

///
///イニシャライザー
///
bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	//ウィンドウの幅と高さを0の初期値を与えて
	screenWidth = 0;
	screenHeight = 0;

	//ウィンドウを立ち上がる
	InitializeWindows(screenWidth, screenHeight);


	//アプリケーションシールクラスをnewします
	m_Application = new ApplicationClass;
	if (!m_Application)
	{
		return false;
	}

	//アプリケーションクラスをイニシャル
	result = m_Application->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	return true;
}



//クラスポインターをリリースします
void SystemClass::Release()
{
	//アプリケーションクラスをリリース
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	//ウィンドウをリリース
	ShutdownWindows();

	return;
}



//メインWindowsLoop
void SystemClass::Run()
{
	MSG msg; 
	bool done, result;


	//メモリー確保
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}


//The following Frame function is where all the processing for our application is done.

bool SystemClass::Frame()
{
	bool result;

	// Do the frame processing for the application object.
	result = m_Application->Frame();
	if (!result)
	{
		return false;
	}


	return true;
}

//We have removed the Windows keyboard reads from the MessageHandler function. 
//Direct Input handles all of this for us now.
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}





//The InitializeWindows function is where we put the code to build the window we will use to render to. 
//It returns screenWidth and screenHeight back to the calling function so we can make use of them throughout the application.
//We create the window using some default settings to initialize a plain black window with no borders.
//The function will make either a small window or make a full screen window depending on a global variable called FULL_SCREEN. 
//If this is set to true then we make the screen cover the entire users desktop window. 
//If it is set to false we just make a 800x600 window in the middle of the screen. 
//I placed the FULL_SCREEN global variable at the top of the 「graphicsclass.h」 file in case you want to modify it.
//It will make sense later why I placed the global in that file instead of the header for this file.

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName_LPCSTR = "DX11_ENGINE_RESEARCH";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(m_hinstance, /*IDI_WINLOGO*/ MAKEINTRESOURCEA(102));
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);


	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 1024x768 resolution.
		screenWidth = 1024;
		screenHeight = 768;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}


	RECT wr; // window rectangle
	wr.left = posX;
	wr.top = posY;
	wr.right = wr.left + screenWidth;
	wr.bottom = wr.top + screenHeight;
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, 
		m_applicationName_LPCSTR,
		m_applicationName_LPCSTR,
		WS_OVERLAPPEDWINDOW,/*WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,*/
		wr.left, wr.top, 
		wr.right - wr.left, wr.bottom - wr.top,
		NULL, 
		NULL, 
		m_hinstance, 
		NULL);


	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

///
//ShutdownWindows does just that.
//It returns the screen settings back to normal and releases the window and the handles associated with it.

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName.c_str(), m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

//The WndProc function is where windows sends its messages to.
//You'll notice we tell windows the name of it when we initialize the window class with wc.lpfnWndProc = WndProc in the InitializeWindows function above.
//I included it in this class file since we tie it directly into the system class by having it send all the messages to the MessageHandler function defined inside SystemClass. 
//This allows us to hook the messaging functionality straight into our class and keep the code clean.

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		// Check if the window is being closed.
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		// All other messages pass to the message handler in the system class.
		default:
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
