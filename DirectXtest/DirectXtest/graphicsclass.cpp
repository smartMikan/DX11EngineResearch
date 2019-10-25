




///////////////////////////////////
// Filename: graphicsclass.cpp
///////////////////////////////////
#include "graphicsclass.h"


//Now that we have a D3DClass member we will start to fill out some code inside the GraphicsClass to initializeand shutdown the D3DClass object.
//We will also add calls to BeginSceneand EndScene in the Render function so that we are now drawing to the window using Direct3D.

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


//The second change is in the Initialize function inside the GraphicsClass.
//Here we create the D3DClass objectand then call the D3DClass Initialize function.
//We send this function the screen width, screen height, handle to the window, and the four global variables from the Graphicsclass.h file.
//The D3DClass will use all these variables to setup the Direct3D system.
//We'll go into more detail about that once we look at the d3dclass.cpp file.

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

//The next change is in the ProgramEnd function in the GraphicsClass.
//Shut down of all graphics objects occur here so we have placed the D3DClass shutdown in this function.
//Note that I check to see if the pointer was initialized or not.
//If it wasn't we can assume it was never set up and not try to shut it down. 
//That is why it is important to set all the pointers to null in the class constructor. 
//If it does find the pointer has been initialized then it will attempt to shut down the D3DClass and then clean up the pointer afterwards.


void GraphicsClass::ProgramEnd()
{
	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

//The Frame function has been updated so that it now calls the Render function each frame.

bool GraphicsClass::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

//The final change to this class is in the Render function. 
//We call the m_Direct3D object to clear the screen to a grey color. 
//After that we call EndScene so that the grey color is presented to the window.

bool GraphicsClass::Render()
{
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);


	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();
	return true;
}

