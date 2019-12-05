////////////////////////////////////////////////////////////////////////////////
// Filename: zoneclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "zoneclass.h"

ZoneClass::ZoneClass()
{
	m_UserInterface = 0;
	m_Camera = 0;
	m_Position = 0;
	m_Light = 0;
	m_Terrain = 0;
	m_SkyDome = 0;
	m_SkyCube = 0;
	m_Model = 0;
	m_MeshModel = 0;
	m_Frustum = 0;
	m_ParticleSystem = 0;
}


ZoneClass::ZoneClass(const ZoneClass& other)
{
}


ZoneClass::~ZoneClass()
{
}
bool ZoneClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth)
{
	bool result;


	// Create the user interface object.
	m_UserInterface = new UserInterfaceClass;
	if (!m_UserInterface)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_UserInterface->Initialize(Direct3D, screenHeight, screenWidth);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	// Set the UI to display by default.
	m_displayUI = true;

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();

	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation.
	m_Position->SetPosition(10.0f, 30.0f,10.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);


	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	// Initialize the frustum object.
	m_Frustum->Initialize(screenDepth);

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(Direct3D->GetDevice());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome object.
	m_SkyCube = new SkyCubeClass;
	if (!m_SkyCube)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext() ,L"./3DModel/Texture/daylight.jpg");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sky cube object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(Direct3D->GetDevice(), ".//Terrain/Setup.txt");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), L"./3DModel/Cube.txt", L"./3DModel/Texture/stone01.dds", L"./3DModel/Texture/bump01.dds", L"./3DModel/Texture/light01.dds");

	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_MeshModel = new GameObjectClass;
	if (!m_MeshModel)
	{
		return false;
	}
	// Initialize the model object.
	result = m_MeshModel->Initialize("./3DModel/Hip_Hop_Dancing.fbx", Direct3D->GetDevice(), Direct3D->GetDeviceContext());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the mesh model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);


	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), L"./Resources/star.tga");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the particle object.", L"Error", MB_OK);
		return false;
	}


	// Set wire frame rendering initially to disabled.
	m_wireFrame = false;

	// Set the rendering of cell lines initially to disabled.
	m_cellLines = false;

	// Set the user locked to the terrain height for movement.
	m_heightLocked = true;

	//Set the cubemap sky to to disabled.
	m_cubemapsky = false;

	return true;
}

void ZoneClass::Shutdown()
{

	// Release the particle system object.
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the sky dome object.
	if (m_SkyCube)
	{
		m_SkyCube->Shutdown();
		delete m_SkyCube;
		m_SkyCube = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the frustum object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the Meshmodel object.
	if (m_MeshModel) {
		m_MeshModel->Shutdown();
		delete m_MeshModel;
		m_MeshModel = 0;
	}

	// Release the model object.
	if (m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the user interface object.
	if (m_UserInterface)
	{
		m_UserInterface->Shutdown();
		delete m_UserInterface;
		m_UserInterface = 0;
	}

	return;
}
bool ZoneClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, float frameTime, int fps, int cpu, TimerClass* TimeClass)
{
	bool result, foundHeight;
	float posX, posY, posZ, rotX, rotY, rotZ, height;


	// Do the frame input processing.
	HandleMovementInput(Input, frameTime);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Do the frame processing for the user interface.
	result = m_UserInterface->Frame(Direct3D->GetDeviceContext(), fps, cpu, posX, posY, posZ, rotX, rotY, rotZ);
	if (!result)
	{
		return false;
	}

	// Do the terrain frame processing.
	m_Terrain->Frame();

	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(frameTime, Direct3D->GetDeviceContext());

	// If the height is locked to the terrain then position the camera on top of it.
	if (m_heightLocked)
	{
		// Get the height of the triangle that is directly underneath the given camera position.
		foundHeight = m_Terrain->GetHeightAtPosition(posX, posZ, height);
		if (foundHeight)
		{
			// If there was a triangle under the camera then position the camera just above it by one meter.
			m_Position->SetPosition(posX, height + 1.0f, posZ);
			m_Camera->SetPosition(posX, height + 1.0f, posZ);
		}
	}


	// Render the graphics.
	result = Render(Direct3D, ShaderManager, TextureManager,TimeClass);
	if (!result)
	{
		return false;
	}

	return true;
}
void ZoneClass::HandleMovementInput(InputClass* Input, float frameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;
	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);
	m_Light->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = Input->IsLeftPressed() /*|| Input->GetHorizontal() < 0*/;
	m_Position->TurnLeft(keyDown);

	keyDown = Input->IsRightPressed() /*|| Input->GetHorizontal() > 0*/;
	m_Position->TurnRight(keyDown);

	keyDown = Input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = Input->IsAPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = Input->IsZPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = Input->IsPgUpPressed() /*|| Input->GetVertical() < 0*/;
	m_Position->LookUpward(keyDown);

	keyDown = Input->IsPgDownPressed() /*|| Input->GetVertical() > 0*/;
	m_Position->LookDownward(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Set the rotation of the light.
	keyDown = Input->IsQPressed();
	m_Light->TurnLeft(keyDown);

	keyDown = Input->IsWPressed();
	m_Light->TurnRight(keyDown);

	// Determine if the user interface should be displayed or not.
	if (Input->IsF1Toggled())
	{
		m_displayUI = !m_displayUI;
	}

	// Determine if the terrain should be rendered in wireframe or not.
	if (Input->IsF2Toggled())
	{
		m_wireFrame = !m_wireFrame;
	}
	
	// Determine if we should render the lines around each terrain cell.
	if (Input->IsF3Toggled())
	{
		m_cellLines = !m_cellLines;
	}

	// Determine if we should be locked to the terrain height when we move around or not.
	if (Input->IsF4Toggled())
	{
		m_heightLocked = !m_heightLocked;
	}

	if (Input->IsF5Toggled())
	{
		m_ParticleSystem->SetParticleProperty(500, 10, 500, -3, 1, 0.2, 1000);
	}

	if (Input->IsF6Toggled())
	{
		m_cubemapsky = !m_cubemapsky;
	}

	return;
}

bool ZoneClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, TimerClass* TimeClass)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;
	XMFLOAT3 cameraPosition;
	int i;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);
	
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();
	// Construct the frustum.
	m_Frustum->ConstructFrustum(projectionMatrix, viewMatrix);

	// Clear the buffers to begin the scene.
	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Turn off back face culling.
	Direct3D->TurnOffCulling();

	// Translate the sky dome to be centered around the camera position.
	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	if (m_cubemapsky) {
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_SkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_SkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_SkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
	}
	else
	{
		Direct3D->TurnZBufferOff();
		// Render the sky dome using the sky dome shader.
		m_SkyDome->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyDomeShader(Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
		if (!result)
		{
			return false;
		}
	}

	// Reset the world matrix.
	Direct3D->GetWorldMatrix(worldMatrix);

	// Turn the Z buffer back and back face culling on.
	Direct3D->TurnZBufferOn();
	Direct3D->TurnOnCulling();

	// Turn on wire frame rendering of the terrain if needed.
	if (m_wireFrame)
	{
		Direct3D->EnableWireframe();
	}

	// Render the terrain cells (and cell lines if needed).
	for (i = 0; i < m_Terrain->GetCellCount(); i++)
	{
		// Render each terrain cell if it is visible only.
		result = m_Terrain->RenderCell(Direct3D->GetDeviceContext(), i, m_Frustum);
		if (result)
		{
			// Render the cell buffers using the terrain shader.
			result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i), worldMatrix, viewMatrix,
				projectionMatrix, TextureManager->GetTexture(0), TextureManager->GetTexture(1),
				m_Light->GetDirection(), m_Light->GetDiffuseColor());
			if (!result)
			{
				return false;
			}

			// If needed then render the bounding box around this terrain cell using the color shader. 
			if (m_cellLines)
			{
				m_Terrain->RenderCellLines(Direct3D->GetDeviceContext(), i);
				ShaderManager->RenderColorShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellLinesIndexCount(i), worldMatrix,
					viewMatrix, projectionMatrix);
				if (!result)
				{
					return false;
				}
			}
		}
	}


	//// Render the terrain grid using the color shader.
	//m_Terrain->Render(Direct3D->GetDeviceContext());
	//result = result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
	//	projectionMatrix, TextureManager->GetTexture(0), TextureManager->GetTexture(1),
	//	m_Light->GetDirection(), m_Light->GetDiffuseColor());
	//if (!result)
	//{
	//	return false;
	//}

	Direct3D->GetWorldMatrix(worldMatrix);
	XMMATRIX modelPosition;
	modelPosition = worldMatrix;
	modelPosition = XMMatrixTranslation(128.0f, 1.5f, 128.0f);

	/*m_Model->Render(Direct3D->GetDeviceContext());
	result = ShaderManager->RenderLightShader(Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), m_Model->GetTextureVector(), modelPosition, viewMatrix,
		projectionMatrix,m_Camera->GetPosition(),m_Light->GetAmbientColor(),m_Light->GetDiffuseColor(),m_Light->GetDirection(),m_Light->GetSpecularPower(),m_Light->GetSpecularColor());*/

	modelPosition = worldMatrix;
	modelPosition = XMMatrixTranslation(10.0f, 0.0f, 128.0f);
	m_MeshModel->Draw(ShaderManager, modelPosition, viewMatrix, projectionMatrix,m_Camera->GetPosition(),m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularPower(), m_Light->GetSpecularColor(),TimeClass->GetTime());
	

	//Direct3D->TurnOnParticleBlending();
	//// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_ParticleSystem->Render(Direct3D->GetDeviceContext());
	//
	//XMMATRIX particlePosition;
	//particlePosition = worldMatrix;
	//particlePosition = XMMatrixTranslation(255.0f, 3.0f, 255.0f);
	//
	//// Render the particle using the texture shader.
	//result = ShaderManager->RenderParticleShader(Direct3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), particlePosition, viewMatrix, projectionMatrix,
	//	m_ParticleSystem->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}
	//
	//// Turn off alpha blending.
	//Direct3D->TurnOffParticleBlending();


	// Turn off the wire frame rendering once the terrain rendering is complete so we don't render anything else such as the UI in wire frame.
	// Turn off wire frame rendering of the terrain if it was on.
	if (m_wireFrame)
	{
		Direct3D->DisableWireframe();
	}

	// Update the render counts in the UI.
	result = m_UserInterface->UpdateRenderCounts(Direct3D->GetDeviceContext(), m_Terrain->GetRenderCount(), m_Terrain->GetCellsDrawn(),
		m_Terrain->GetCellsCulled());
	if (!result)
	{
		return false;
	}

	// Render the user interface.
	if (m_displayUI)
	{
		result = m_UserInterface->Render(Direct3D, ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	Direct3D->EndScene();

	return true;
}

