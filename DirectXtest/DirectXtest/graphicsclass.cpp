




///////////////////////////////////
// Filename: graphicsclass.cpp
///////////////////////////////////
#include "graphicsclass.h"
#include <math.h>

//Now that we have a D3DClass member we will start to fill out some code inside the GraphicsClass to initializeand shutdown the D3DClass object.
//We will also add calls to BeginSceneand EndScene in the Render function so that we are now drawing to the window using Direct3D.

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;

	m_ColorShader = 0;
	m_TextureShader = 0;

	m_LightShader = 0;
	m_Light = 0;
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


	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_Direct3D->GetDevice(),  m_Direct3D->GetDeviceContext(),"./3DModel/mayacube.txt", "./3DModel/Texture/pic8026.tga");
	//result = m_Model->Initialize(m_Direct3D->GetDevice(),  m_Direct3D->GetDeviceContext(), "hoge.tga");

	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}



	//// Create the color shader object.
	//m_ColorShader = new ColorShaderClass;
	//if (!m_ColorShader)
	//{
	//	return false;
	//}

	// Initialize the color shader object.
	//result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	/*if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	*/

	//// Create the texture shader object.
	//m_TextureShader = new TextureShaderClass;
	//if (!m_TextureShader)
	//{
	//	return false;
	//}

	//// Initialize the texture shader object.
	//result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBoxW(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
	//	return false;
	//}
	//


	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	
	//The new light object is created here.

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	
	//The color of the light is set to purple and the light direction is set to point down the positive Z axis.

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);




	framesincestart = 0;


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

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

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

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	

	result = Update();
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

//The final change to this class is in the Render function. 
//We call the m_Direct3D object to clear the screen to a grey color. 
//After that we call EndScene so that the grey color is presented to the window.

bool GraphicsClass::Render(float rotation)
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	//// Clear the buffers to begin the scene.
	//m_Direct3D->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//m_Camera->SetPosition(sinf((float)framesincestart/180.0f), 0, cosf((float)framesincestart/180.0f));


	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = XMMatrixRotationY(rotation);


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	//// Render the model using the color shader.
	//result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);


	// Render the model using the texture shader.
	//result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());


	// Render the model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	


	if (!result)
	{
		return false;
	}



	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();
	return true;
}

bool GraphicsClass::Update() {

	/*if (framesincestart == NULL)
		return false;*/

	framesincestart += 1;

	return true;
}

