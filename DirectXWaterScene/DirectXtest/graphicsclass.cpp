




///////////////////////////////////
// Filename: graphicsclass.cpp
///////////////////////////////////
#include "graphicsclass.h"


//Now that we have a D3DClass member we will start to fill out some code inside the GraphicsClass to initializeand shutdown the D3DClass object.
//We will also add calls to BeginSceneand EndScene in the Render function so that we are now drawing to the window using Direct3D.

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_GroundModel = 0;
	m_WallModel = 0;
	m_BathModel = 0;
	m_WaterModel = 0;
	m_Light = 0;
	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_LightShader = 0;
	m_RefractionShader = 0;
	m_WaterShader = 0;
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

	// Create the ground model object.
	m_GroundModel = new ModelClass;
	if (!m_GroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = m_GroundModel->Initialize(m_Direct3D->GetDevice(), L"./3DModel/ground.txt", L"./3DModel/Texture/ground01.dds");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	// Create the wall model object.
	m_WallModel = new ModelClass;
	if (!m_WallModel)
	{
		return false;
	}

	// Initialize the wall model object.
	result = m_WallModel->Initialize(m_Direct3D->GetDevice(), L"./3DModel/wall.txt", L"./3DModel/Texture/wall01.dds");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the wall model object.", L"Error", MB_OK);
		return false;
	}

	// Create the bath model object.
	m_BathModel = new ModelClass;
	if (!m_BathModel)
	{
		return false;
	}

	// Initialize the bath model object.
	result = m_BathModel->Initialize(m_Direct3D->GetDevice(), L"./3DModel/bath.txt", L"./3DModel/Texture/marble01.dds");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the bath model object.", L"Error", MB_OK);
		return false;
	}

	// Create the water model object.
	m_WaterModel = new ModelClass;
	if (!m_WaterModel)
	{
		return false;
	}

	// Initialize the water model object.
	result = m_WaterModel->Initialize(m_Direct3D->GetDevice(), L"./3DModel/water.txt", L"./3DModel/Texture/water01.dds");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the water model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 0.5f);

	//Setup the two render to textures for the scene reflection and refraction.
	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	//Setup the three shaders that we will be using.
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

	// Create the refraction shader object.
	m_RefractionShader = new RefractionShaderClass;
	if (!m_RefractionShader)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_RefractionShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}

	//Setup the height of the water planeand initialize the position of the water translation.
	// Set the height of the water.
	m_waterHeight = 2.75f;

	// Initialize the position of the water.
	m_waterTranslation = 0.0f;

	return true;
}

void GraphicsClass::ProgramEnd()
{
	// Release the water shader object.
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the refraction shader object.
	if (m_RefractionShader)
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the reflection render to texture object.
	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	// Release the refraction render to texture object.
	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the water model object.
	if (m_WaterModel)
	{
		m_WaterModel->Shutdown();
		delete m_WaterModel;
		m_WaterModel = 0;
	}

	// Release the bath model object.
	if (m_BathModel)
	{
		m_BathModel->Shutdown();
		delete m_BathModel;
		m_BathModel = 0;
	}

	// Release the wall model object.
	if (m_WallModel)
	{
		m_WallModel->Shutdown();
		delete m_WallModel;
		m_WallModel = 0;
	}

	// Release the ground model object.
	if (m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
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
	//Each frame update the position of the water to simulation motion.
	// Update the position of the water to simulate motion.
	m_waterTranslation += 0.001f;
	if (m_waterTranslation > 1.0f)
	{
		m_waterTranslation -= 1.0f;
	}

	// Set the position and rotation of the camera.
	m_Camera->SetPosition(-10.0f, 6.0f, -10.0f);
	m_Camera->SetRotation(0.0f, 45.0f, 0.0f);


	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

//First we render the refraction of the scene to a texture. 
//Then we render the reflection of the scene to a texture.
//And then finally we render the scene normally and use the reflection and refraction texture to create the water effect. 
//Remember if you want to reduce the cost of rendering to texture all the time you can instead only do so every 15-30 frames and reuse the previous textures.

bool GraphicsClass::Render()
{
	bool result;


	// Render the refraction of the scene to a texture.
	result = RenderRefractionToTexture();
	if (!result)
	{
		return false;
	}

	// Render the reflection of the scene to a texture.
	result = RenderReflectionToTexture();
	if (!result)
	{
		return false;
	}

	// Render the scene as normal to the back buffer.
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	return true;
}

//The RenderRefractionToTexture function renders the refraction of the scene to a render to texture.
//As the refraction of the scene is everything beneath the water and only the bath model is visible underneath the model we can be selective and just render the bath model.
bool GraphicsClass::RenderRefractionToTexture()
{
	XMFLOAT4 clipPlane;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	//Now as expected we use a clip plane to clip everything above the water plane and only render what is beneath it.
	//However you will notice that I am translating the clip plane up just slightly above the water height. 
	//The reason being is that this technique has edge issues and to reduce the visibility of the dark edges that appear once translated by the normal map sampling location we just force the sampling to occur in a range where there is more color data available (by moving the clip plane slightly up). 
	//When you run this program try just using the regular water height so you can see the artifacting that does occur normally.
	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Translate to where the bath model will be rendered.
	worldMatrix = XMMatrixTranslation(0.0f, 2.0f, 0.0f);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_BathModel->Render(m_Direct3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_RefractionShader->Render(m_Direct3D->GetDeviceContext(), m_BathModel->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_BathModel->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
	return false;
}

//RenderReflectionToTexture renders the reflection of the scene to a render to texture. 
//As the reflection of the scene is everything above the water we only need to render the wall model since it is the only thing that can reflect in the water.
bool GraphicsClass::RenderReflectionToTexture()
{
	XMMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	bool result;


	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);
	
	//The reflection matrix is setup using the water height.
	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_waterHeight);

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Get the world and projection matrices from the d3d object.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Translate to where the wall model will be rendered.
	worldMatrix = XMMatrixTranslation(0.0f, 6.0f, 8.0f);

	// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WallModel->Render(m_Direct3D->GetDeviceContext());


	// Render the wall model using the light shader and the reflection view matrix.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_WallModel->GetIndexCount(), m_WallModel->GetTexture(),
		m_LightShader->GenarateMatrixBuffer(worldMatrix, reflectionViewMatrix,projectionMatrix),
		m_LightShader->GenerateCameraBuffer(m_Camera->GetPosition(), 0.0f),
		m_LightShader->GenerateLightBuffer(m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection()));
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

//The RenderScene function is where we render the scene to the back buffer and use the reflection and refraction render to textures to render the water.
bool GraphicsClass::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//Render the ground model first.
	// Translate to where the ground model will be rendered.
	worldMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);

	// Put the ground model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_GroundModel->Render(m_Direct3D->GetDeviceContext());

	// Render the ground model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), m_GroundModel->GetTexture(),
		m_LightShader->GenarateMatrixBuffer(worldMatrix,viewMatrix, projectionMatrix),
		m_LightShader->GenerateCameraBuffer(m_Camera->GetPosition(), 0.0f),
		m_LightShader->GenerateLightBuffer(m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection()));
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	//Render the wall model next.
	// Translate to where the wall model will be rendered.
	worldMatrix = XMMatrixTranslation(0.0f, 6.0f, 8.0f);

	// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WallModel->Render(m_Direct3D->GetDeviceContext());

	// Render the wall model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_WallModel->GetIndexCount(), m_WallModel->GetTexture(),
		m_LightShader->GenarateMatrixBuffer(worldMatrix, viewMatrix, projectionMatrix),
		m_LightShader->GenerateCameraBuffer(m_Camera->GetPosition(), 0.0f),
		m_LightShader->GenerateLightBuffer(m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection()));
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	//Render the bath model.
	// Translate to where the bath model will be rendered.
	worldMatrix = XMMatrixTranslation(0.0f, 2.0f, 0.0f);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_BathModel->Render(m_Direct3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_BathModel->GetIndexCount(), m_BathModel->GetTexture(),
		m_LightShader->GenarateMatrixBuffer(worldMatrix, viewMatrix, projectionMatrix),
		m_LightShader->GenerateCameraBuffer(m_Camera->GetPosition(), 0.0f),
		m_LightShader->GenerateLightBuffer(m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection()));
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	//Finally render the water model using the reflection matrix, render to textures, normal map, translation value, and the reflectRefractScale (0.01f).
	// Get the camera reflection view matrix.
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	// Translate to where the water model will be rendered.
	worldMatrix = XMMatrixTranslation(0.0f, m_waterHeight, 0.0f);

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WaterModel->Render(m_Direct3D->GetDeviceContext());

	// Render the water model using the water shader.
	result = m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_WaterModel->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
		m_RefractionTexture->GetShaderResourceView(), m_WaterModel->GetTexture(),
		m_waterTranslation, 0.01f);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


void GraphicsClass::SetWireframeMode(bool isWireframeMod)
{
	
	m_Direct3D->SetRasterizerState(isWireframeMod);
	
}

