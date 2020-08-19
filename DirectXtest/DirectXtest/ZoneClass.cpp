#include "zoneclass.h"

ZoneClass::ZoneClass()
{
	m_UserInterface = 0;
	m_Camera = 0;
	m_Transform = 0;
	m_Light = 0;
	m_Terrain = 0;
	m_SkyDome = 0;
	m_DayLightSkyCube = 0;
	m_SunsetSkyCube = 0;
	m_DesertSkyCube = 0;
	m_NebulaSkyCube = 0;
	m_PlanetSkyCube = 0;

	m_AnimModel = 0;
	m_UnMoveModel = 0;
	m_Player = 0;

	m_Frustum = 0;
	m_ParticleSystem = 0;

	m_RenderTexture = 0;
	modelPosition = XMMatrixIdentity();
}


ZoneClass::ZoneClass(const ZoneClass& other)
{
	this->m_UserInterface = other.m_UserInterface;
	this->m_Camera = other.m_Camera;
	this->m_Transform = other.m_Transform;
	this->m_Light = other.m_Light;
	this->m_Terrain = other.m_Terrain;

	this->m_SkyDome = other.m_SkyDome;
	this->m_DayLightSkyCube = other.m_DayLightSkyCube;
	this->m_SunsetSkyCube = other.m_SunsetSkyCube;
	this->m_DesertSkyCube = other.m_DesertSkyCube;
	this->m_NebulaSkyCube = other.m_NebulaSkyCube;
	this->m_PlanetSkyCube = other.m_PlanetSkyCube;

	this->m_AnimModel = other.m_AnimModel;
	this->m_UnMoveModel = other.m_UnMoveModel;
	this->m_Player = other.m_Player;

	this->m_Frustum = other.m_Frustum;
	this->m_ParticleSystem = other.m_ParticleSystem;
	this->m_RenderTexture = other.m_RenderTexture;
	this->modelPosition = other.modelPosition;
}


ZoneClass::~ZoneClass()
{
}
int ZoneClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth)
{
	bool result;

	this->device = Direct3D->GetDevice();
	this->deviceContext = Direct3D->GetDeviceContext();

	//TODO: initialze it at engine layer other than scene layer
	//Initialize Shaders
	if (!shaders.InitializeShaders(device)) {
		MessageBoxW(hwnd, L"Could not initialize the Shaders.", L"Error", MB_OK);
		return false;
	};


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
	m_Transform = new Transform;
	if (!m_Transform)
	{
		return false;
	}

	// Set the initial position and rotation.
	m_Transform->SetPosition(10.0f, 30.0f, 10.0f);
	m_Transform->SetRotation(0.0f, 0.0f, 0.0f);


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
	m_DayLightSkyCube = new SkyCubeClass;
	if (!m_DayLightSkyCube)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_DayLightSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), L"./Resources/Texture/daylight.jpg");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sky cube object.", L"Error", MB_OK);
		return false;
	}


	m_SunsetSkyCube = new SkyCubeClass;
	if (!m_SunsetSkyCube)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SunsetSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), std::vector<std::wstring>{
		L"./Resources/Texture/sunset_posX.bmp", L"./Resources/Texture/sunset_negX.bmp",
			L"./Resources/Texture/sunset_posY.bmp", L"./Resources/Texture/sunset_negY.bmp",
			L"./Resources/Texture/sunset_posZ.bmp", L"./Resources/Texture/sunset_negZ.bmp", });
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sunset sky cube object.", L"Error", MB_OK);
		return false;
	}

	m_DesertSkyCube = new SkyCubeClass;
	if (!m_DesertSkyCube)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_DesertSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), L"./Resources/Texture/desertcube1024.dds");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the desert sky cube object.", L"Error", MB_OK);
		return false;
	}

	m_NebulaSkyCube = new SkyCubeClass;
	if (!m_NebulaSkyCube)
	{
		return false;
	}
	// Initialize the sky dome object.
	result = m_NebulaSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), std::vector<std::wstring>{
		L"./Resources/SkyBoxVolume2/Stars01/rightImage.png", L"./Resources/SkyBoxVolume2/Stars01/leftImage.png",
			L"./Resources/SkyBoxVolume2/Stars01/upImage.png", L"./Resources/SkyBoxVolume2/Stars01/downImage.png",
			L"./Resources/SkyBoxVolume2/Stars01/backImage.png", L"./Resources/SkyBoxVolume2/Stars01/frontImage.png", });
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sunset sky cube object.", L"Error", MB_OK);
		return false;
	}

	m_PlanetSkyCube = new SkyCubeClass;
	if (!m_PlanetSkyCube)
	{
		return false;
	}
	// Initialize the sky dome object.
	result = m_PlanetSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), std::vector<std::wstring>{
		L"./Resources/SkyBoxVolume2/DeepSpaceBlueWithPlanet/leftImage.png", L"./Resources/SkyBoxVolume2/DeepSpaceBlueWithPlanet/rightImage.png",
			L"./Resources/SkyBoxVolume2/DeepSpaceBlueWithPlanet/upImage.png", L"./Resources/SkyBoxVolume2/DeepSpaceBlueWithPlanet/downImage.png",
			L"./Resources/SkyBoxVolume2/DeepSpaceBlueWithPlanet/frontImage.png", L"./Resources/SkyBoxVolume2/DeepSpaceBlueWithPlanet/backImage.png", });
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sunset sky cube object.", L"Error", MB_OK);
		return false;
	}


	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(Direct3D->GetDevice(), "./Resources/Terrain/Setup.txt");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}



	// Create the model object.
	m_sword = new GameObjectClass;
	if (!m_sword)
	{
		return false;
	}

	//Initialize the model object
	result = m_sword->Initialize("./3DModel/tianyi_idle.fbx", Direct3D->GetDevice(), Direct3D->GetDeviceContext(), cb_vs_wvpBuffer, cb_ps_material, shaders.d3dvertexshader_animation.get());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize tianyi model object.", L"Error", MB_OK);
		return false;
	}

	m_sword->InitAnimation(cb_bones);
	m_sword->AddAnimation("./3DModel/tianyi_idle.fbx");
	m_sword->SwitchAnim(1);
	m_sword->m_Transform.SetScale(2.02f, 2.02f, 2.02f);
	m_sword->m_Transform.SetRotation(90.0f, 180.0f, 0.0f);

	// Create the model object.
	m_tianyi = new GameObjectClass;
	if (!m_tianyi)
	{
		return false;
	}
	//Initialize the model object
	result = m_tianyi->Initialize("./3DModel/fighter.fbx", Direct3D->GetDevice(), Direct3D->GetDeviceContext(), cb_vs_wvpBuffer, cb_ps_material, shaders.d3dvertexshader_animation.get());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize tianyi model object.", L"Error", MB_OK);
		return false;
	}

	//Initialize animatior
	m_tianyi->InitAnimation(cb_bones);
	m_tianyi->AddAnimation("./3DModel/fighter.fbx");
	m_tianyi->SwitchAnim(1);
	m_tianyi->m_Transform.SetScale(0.02f, 0.02f, 0.02f);


	// Create the model object.
	m_AnimModel = new GameObjectClass;
	if (!m_AnimModel)
	{
		return false;
	}
	//Initialize the model object.
	result = m_AnimModel->Initialize("./3DModel/Hip_Hop_Dancing.fbx", Direct3D->GetDevice(), Direct3D->GetDeviceContext(), cb_vs_wvpBuffer, cb_ps_material, shaders.d3dvertexshader_animation.get());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the hiphop dancing mesh model object.", L"Error", MB_OK);
		return false;
	}

	//Initialize animatior
	m_AnimModel->InitAnimation(cb_bones);
	m_AnimModel->AddAnimation("./3DModel/Bellydancing.fbx");
	m_AnimModel->AddAnimation("./3DModel/Boxing.fbx");
	m_AnimModel->AddAnimation("./3DModel/Hip_Hop_Dancing.fbx");
	m_AnimModel->AddAnimation("./3DModel/SwordPack/swordandshieldidle.fbx");
	m_AnimModel->AddAnimation("./3DModel/SwordPack/swordandshieldrun.fbx", true);
	m_AnimModel->AddAnimation("./3DModel/SwordPack/swordandshieldslash.fbx", true);


	m_AnimModel->m_Transform.SetScale(0.02f, 0.02f, 0.02f);
	m_AnimModel->m_Transform.SetRotation(0, 180, 0);

	// Create the model object.
	m_Player = new Player;
	if (!m_Player)
	{
		return false;
	}
	//Initialize the model object.
	result = m_Player->Initialize("./3DModel/SwordPack/xbot.fbx", Direct3D->GetDevice(), Direct3D->GetDeviceContext(), cb_vs_wvpBuffer, cb_ps_material, shaders.d3dvertexshader_animation.get());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the player model object.", L"Error", MB_OK);
		return false;
	}
	//Initialize animatior
	result = m_Player->InitialAnimations(cb_bones);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the player Animation!.", L"Error", MB_OK);
		return false;
	}


	m_Player->m_Transform.SetScale(0.02f, 0.02f, 0.02f);


	m_UnMoveModel = new GameObjectClass;
	if (!m_UnMoveModel)
	{
		return false;
	}
	result = m_UnMoveModel->Initialize("./3DModel/brick_wall/brick_wall.obj", Direct3D->GetDevice(), Direct3D->GetDeviceContext(), cb_vs_wvpBuffer, cb_ps_material, shaders.d3dvertexshader.get());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the brick wall model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Light->m_transform.SetPosition(12.0, 8.0f, 12.0f);
	m_Light->m_transform.SetRotation(40.0f, 0.0f, 0.0f);

	m_Light->ambientLightStrength = 0.4f;
	m_Light->dynamicLightStrength = 5.0f;

	m_Light->dynamicLightAttenuation_a = 1.0f;
	m_Light->dynamicLightAttenuation_b = 0.1f;
	m_Light->dynamicLightAttenuation_c = 0.002f;

	m_Light->GenerateOrthoMatrix(100.0f, screenDepth, 1.0f);
	m_Light->GenerateProjectionMatrix(screenDepth, 1.0f);
	m_Light->Frame(0.0f);



	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SHADOWMAP_DEPTH, SHADOWMAP_NEAR);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}



	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), L"./Resources/star2.tga");
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


	//Initialize Enemies
	m_enemies = new Enemy(m_AnimModel);
	if (!m_enemies)
	{
		return false;
	}



	HRESULT hr;
	//initialize constant buffer
	hr = this->cb_vs_wvpBuffer.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_vs_wvpBuffer.SetDebugName("cb_vs_world");

	hr = this->cb_ps_light.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_ps_light.SetDebugName("cb_ps_light");


	hr = this->cb_ps_shadowMatrix.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");


	hr = this->cb_ps_camera.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_ps_camera.SetDebugName("cb_ps_common");

	hr = this->cb_ps_material.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_ps_material.SetDebugName("ps_material");

	hr = this->cb_bones.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_bones.SetDebugName("vs_bone_transforms");

	hr = this->cb_baked_bones.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_bones.SetDebugName("vs_Baked_bone_transforms");






	this->cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->cb_ps_light.data.ambientLightStrength = 0.3f;



	if (BakeModelMode)
	{
		AnimationBaker baker;

		for (size_t i = 0; i < m_AnimModel->GetAnimCount(); i++)
		{
			baker.BakeAnim(m_AnimModel->GetAnimator(), m_AnimModel->GetAnimator()->GetAnimation(i), L"mAnim" + to_wstring(i));
		}

		return 2;
	}

	m_AnimModel->InitBakedAnim(cb_baked_bones);
	for (size_t i = 0; i < m_AnimModel->GetAnimCount(); i++)
	{
		result = m_AnimModel->LoadBakedAnim(L"mAnim" + to_wstring(i) + L".anim");
		if (!result)
		{
			MessageBoxW(hwnd, L"Failed to load animation", L"Error", MB_OK);
			return false;
		}
	}


	return 1;
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

	// Release the sky cube object.
	if (m_DayLightSkyCube)
	{
		m_DayLightSkyCube->Shutdown();
		delete m_DayLightSkyCube;
		m_DayLightSkyCube = 0;
	}
	if (m_SunsetSkyCube)
	{
		m_SunsetSkyCube->Shutdown();
		delete m_SunsetSkyCube;
		m_SunsetSkyCube = 0;
	}
	if (m_DesertSkyCube)
	{
		m_DesertSkyCube->Shutdown();
		delete m_DesertSkyCube;
		m_DesertSkyCube = 0;
	}
	if (m_NebulaSkyCube)
	{
		m_NebulaSkyCube->Shutdown();
		delete m_NebulaSkyCube;
		m_NebulaSkyCube = 0;
	}
	if (m_PlanetSkyCube)
	{
		m_PlanetSkyCube->Shutdown();
		delete m_PlanetSkyCube;
		m_PlanetSkyCube = 0;
	}
	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}
	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
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
	if (m_AnimModel) {
		m_AnimModel->Shutdown();
		delete m_AnimModel;
		m_AnimModel = 0;
	}
	// Release the Meshmodel object.
	if (m_UnMoveModel) {
		m_UnMoveModel->Shutdown();
		delete m_UnMoveModel;
		m_UnMoveModel = 0;
	}
	// Release the Meshmodel object.
	if (m_Player) {
		m_Player->Shutdown();
		delete m_Player;
		m_Player = 0;
	}
	if (m_tianyi) {
		m_tianyi->Shutdown();
		delete m_tianyi;
		m_tianyi = 0;
	}

	if (m_sword) {
		m_sword->Shutdown();
		delete m_sword;
		m_sword = 0;
	}

	// Release the position object.
	if (m_Transform)
	{
		delete m_Transform;
		m_Transform = 0;
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



	if (m_enemies)
	{
		m_enemies->ShutDown();
		delete m_enemies;
		m_enemies = nullptr;
	}


	return;
}
bool ZoneClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, float frameTime, int fps, int cpu)
{
	bool result, foundHeight;
	float posX, posY, posZ, rotX, rotY, rotZ, height;
	// Set the frame time for calculating the updated position.
	m_Transform->SetFrameTime(frameTime);

	//TODO: update playerdata in gameObject class & need an engine work flow to do this
	m_Light->Frame(frameTime);
	m_Player->Frame(frameTime);

	// Get the view point position/rotation.
	m_Player->m_Transform.GetPosition(posX, posY, posZ);
	m_Transform->GetRotation(rotX, rotY, rotZ);

	// Do the frame processing for the user interface.
	result = m_UserInterface->Frame(Direct3D->GetDeviceContext(), fps, cpu, posX, posY, posZ, rotX, rotY, rotZ);
	if (!result)
	{
		return false;
	}

	// Do the terrain frame processing.
	m_Terrain->Frame();

	// If the height is locked to the terrain then position the camera on top of it.
	if (m_heightLocked)
	{
		// Get the height of the triangle that is directly underneath the given camera position.
		foundHeight = m_Terrain->GetHeightAtPosition(posX, posZ, height);
		if (foundHeight)
		{
			// If there was a triangle under the camera then position the camera just above it by one meter.
			//m_Camera->SetPosition(posX, height, posZ);
			m_Player->m_Transform.SetPosition(posX, height + m_Player->GetJumpHeight(), posZ);
		}

	}


	// Do the frame input processing.
	HandleMovementInput(Input, fps);

	//EnemyFrame
	m_enemies->Frame(frameTime);

	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(frameTime, Direct3D->GetDeviceContext());


	XMMATRIX lightViewMatrix, lightOrthoMatrix, lightProjMatrix;
	SetLight(m_lightType);

	m_Light->GetViewMatrix(lightViewMatrix);
	m_Light->GetOrthoMatrix(lightOrthoMatrix);
	m_Light->GetProjectionMatrix(lightProjMatrix);

	m_Camera->Frame(frameTime);



	// Render ShadowMap
	result = RenderShadowMap(Direct3D, lightViewMatrix, m_lightType == 0 ? lightOrthoMatrix : lightProjMatrix);
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render(Direct3D, ShaderManager, TextureManager);
	if (!result)
	{
		return false;
	}

	return true;
}

/// <summary>
/// Add a Enemy
/// </summary>
bool ZoneClass::CreateEnemy()
{
	float pos[3] = { 0, 0, 0 };
	m_enemies->Instantiate(pos);
	return true;
}

bool ZoneClass::CreateEnemyAtPositon(float pos[3])
{
	//Initialize the model object.
	m_enemies->Instantiate(pos);

	//
	//m_enemies->SetInstanceAnim(m_enemies->GetAllEnemyCounts() - 1, m_enemies->GetAllEnemyCounts());

	return true;
}

/// <summary>
/// InGui§À π§¶
/// </summary>
/// <param name="enemyID"></param>
/// <returns></returns>
bool ZoneClass::RemoveEnemyFromRender(int enemyID)
{
	return m_enemies->RemoveFromRender(enemyID);
}




void ZoneClass::HandleMovementInput(InputClass* Input, int fps)
{
	//TODO: Update Inpot in a List of GameObject rather than in scene layer
	// Determine if the user interface should be displayed or not.
	if (Input->IsKeyToggled(DIK_F1))
	{
		m_displayUI = !m_displayUI;
	}

	// Determine if the terrain should be rendered in wireframe or not.
	if (Input->IsKeyToggled(DIK_F2))
	{
		m_wireFrame = !m_wireFrame;
	}

	// Determine if we should render the lines around each terrain cell.
	if (Input->IsKeyToggled(DIK_F3))
	{
		m_cellLines = !m_cellLines;
	}

	// Determine if we should be locked to the terrain height when we move around or not.
	if (Input->IsKeyToggled(DIK_F4))
	{
		m_heightLocked = !m_heightLocked;
	}



	//Switch Sky Cube
	if (Input->IsKeyToggled(DIK_F6))
	{
		m_cubemapsky = (m_cubemapsky + 1) % 6;
	}

	//Player Input
	bool keyDown;
	//float posX, posY, posZ, rotX, rotY, rotZ;
	XMFLOAT3 orbitposition = m_Player->m_Transform.GetPosition();

	// Handle player input.
	m_Player->HandleInput(Input, &m_Camera->m_transform);


	//Camera Input
	keyDown = Input->IsKeyPressed(DIK_PGUP) || Input->IsKeyPressed(DIK_I);
	m_Camera->m_transform.LookUpward(keyDown);

	keyDown = Input->IsKeyPressed(DIK_PGDN) || Input->IsKeyPressed(DIK_K);
	m_Camera->m_transform.LookDownward(keyDown);

	

	//Sync the model rotation with camera
	if (!m_Player->isCyclone) {
		m_Player->m_Transform.SetRotation(0, m_Camera->m_transform.GetRotationY() + 180, 0);
	}

	//if is Directionlight set lightpos with camera pos
	//TODO: smarter global directional shadowmap
	if (m_lightType == 0) {
		m_Light->m_transform.SetPosition(orbitposition.x, orbitposition.y + 35, orbitposition.z - 35);
	}
	if (m_lightType == 1)
	{
		XMFLOAT3 pos = m_Player->m_Transform.GetPosition();
		m_Light->m_transform.SetPosition(pos.x, pos.y + 1.0f, pos.z - 10.0f);
	}


	return;
}

bool ZoneClass::RenderShadowMap(D3DClass* Direct3D, const XMMATRIX& lightViewMatrix, const XMMATRIX& lightProjMatrix)
{
	//TODO: create static lightmap for terrain to get shadow on terrain 

	// Set the render target to be the render to DepthStencilView.
	m_RenderTexture->SetDVRenderTarget(Direct3D->GetDeviceContext());
	// Clear the render to texture.
	m_RenderTexture->ClearDVRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	cb_ps_shadowMatrix.data.shadowMatrix = XMMatrixTranspose(lightViewMatrix * lightProjMatrix);
	cb_ps_shadowMatrix.ApplyChanges();

	deviceContext->PSSetShader(NULL, NULL, 0);
	//animModels
	RenderAnimationGameObjects(shaders.d3dvertexshader_shadowmap_anim.get(), lightViewMatrix, lightProjMatrix);
	//noanimModels
	RenderNonAnimationGameObjects(shaders.d3dvertexshader_shadowmap.get(), lightViewMatrix, lightProjMatrix);


	// Reset the render target back to the original back buffer and not the render to texture anymore.
	Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	Direct3D->ResetViewport();

	return true;
}

bool ZoneClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	//TODO: throw bool error log
	bool result;
	XMFLOAT3 cameraPosition;

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

	// Translate the sky dome to be centered around the camera position.
	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	//RenderSkyCube or SkyDome (using Camera position)
	result = RenderSky(Direct3D, ShaderManager, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	// Reset the world matrix.
	Direct3D->GetWorldMatrix(worldMatrix);

	// Turn on wire frame rendering of the terrain if needed.
	if (m_wireFrame)
	{
		Direct3D->EnableWireframe();
	}

	//render terrain
	result = RenderTerrain(Direct3D, ShaderManager, TextureManager, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	//check toonshader
	if (toonShading) {
		deviceContext->PSSetShader(shaders.pixelshader_toonmapping.GetShader(), NULL, 0);
		deviceContext->PSSetShaderResources(5, 1, TextureManager->GetToonTexture());
	}
	else
	{
		deviceContext->PSSetShader(shaders.pixelshader.GetShader(), NULL, 0);
	}
	deviceContext->PSSetSamplers(1, 1, m_RenderTexture->shadowSampler.GetAddressOf());
	deviceContext->PSSetShaderResources(4, 1, m_RenderTexture->GetShadowShaderResourceViewAddress());
	deviceContext->PSSetConstantBuffers(3, 1, cb_ps_shadowMatrix.GetAddress());
	//update lightbuffer
	SetLight(m_lightType);

	//WithAnim
	RenderAnimationGameObjects(shaders.d3dvertexshader_animation.get(), viewMatrix, projectionMatrix);

	//NoAnim
	RenderNonAnimationGameObjects(shaders.d3dvertexshader.get(), viewMatrix, projectionMatrix);

	


	//reset worldmatrix
	Direct3D->GetWorldMatrix(worldMatrix);

	result = RenderParticles(Direct3D, ShaderManager, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off the wire frame rendering once the terrain rendering is complete so we don't render anything else such as the UI in wire frame.
	// Turn off wire frame rendering of the terrain if it was on.
	if (m_wireFrame)
	{
		Direct3D->DisableWireframe();
	}


	Direct3D->GetWorldMatrix(worldMatrix);
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

	return true;
}

bool ZoneClass::RenderSky(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	bool result;
	//TODO:use one skycube object to hold different cubemaps rather than create skycube class for each different cubemap

	// Turn off back face culling.
	Direct3D->TurnOffCulling();

	//switch texture
	switch (m_cubemapsky)
	{
	case 0:
		Direct3D->TurnZBufferOff();
		// Render the sky dome using the sky dome shader.
		m_SkyDome->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyDomeShader(Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
		if (!result)
		{
			return false;
		}
		break;
	case 1:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_DayLightSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_DayLightSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_DayLightSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	case 2:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_SunsetSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_SunsetSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_SunsetSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	case 3:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_DesertSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_DesertSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_DesertSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	case 4:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_NebulaSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_NebulaSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_NebulaSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	case 5:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_PlanetSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_PlanetSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_PlanetSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	default:
		break;
	}


	// Turn the Z buffer back and back face culling on.
	Direct3D->TurnZBufferOn();
	Direct3D->TurnOnCulling();

	return true;
}

bool ZoneClass::RenderParticles(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	bool result;
	Direct3D->TurnOnParticleBlending();
	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(Direct3D->GetDeviceContext());

	XMMATRIX particlePosition;

	float camrotation = m_Camera->GetRotation().y;

	particlePosition = worldMatrix;
	particlePosition *= XMMatrixRotationY(camrotation);
	particlePosition = XMMatrixTranslation(255.0f, 3.0f, 255.0f);

	// Render the particle using the texture shader.
	result = ShaderManager->RenderParticleShader(Direct3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), particlePosition, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending.
	Direct3D->TurnOffParticleBlending();


	return true;
}

bool ZoneClass::RenderTerrain(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	bool result;

	// Render the terrain cells (and cell lines if needed).
	for (int i = 0; i < m_Terrain->GetCellCount(); i++)
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
	return true;
}

bool ZoneClass::RenderAnimationGameObjects(D3DVertexShader* vertexshader, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix)
{
	deviceContext->VSSetShader(vertexshader->GetShader(device.Get()), NULL, 0);
	deviceContext->IASetInputLayout(vertexshader->GetLayout());

	//rander at pos 1;
	m_AnimModel->m_Transform.SetPosition(10.0f, 0.0f, 30.0);
	m_AnimModel->SwitchAnim(3);
	m_AnimModel->Render(viewMatrix, projMatrix);

	//rander at pos 2;
	m_AnimModel->m_Transform.SetPosition(15.0f, 0.0f, 30.0f);
	m_AnimModel->SwitchAnim(1);
	m_AnimModel->Render(viewMatrix, projMatrix);

	//rander at pos 3;
	m_AnimModel->m_Transform.SetPosition(20.0f, 0.0f, 30.0f);
	m_AnimModel->SwitchAnim(2);
	m_AnimModel->Render(viewMatrix, projMatrix);

	//rander at pos 4;
	m_tianyi->m_Transform.SetPosition(25.0f, 0.0f, 30.0f);
	m_tianyi->Render(viewMatrix, projMatrix);

	//rander at pos 5;
	m_sword->m_Transform.SetPosition(30.0f, 0.2f, 30.0f);
	m_sword->Render(viewMatrix, projMatrix);


	//rander player;
	m_Player->Render(viewMatrix, projMatrix);



	rendertime = m_enemies->Draw(viewMatrix, projMatrix);
	rendertime *= 2;

	return true;
}

bool ZoneClass::RenderNonAnimationGameObjects(D3DVertexShader* vertexshader, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix)
{
	deviceContext->VSSetShader(vertexshader->GetShader(device.Get()), NULL, 0);
	deviceContext->IASetInputLayout(vertexshader->GetLayout());

	//rander floor;
	m_UnMoveModel->m_Transform.SetPosition(50.0f, 0.1f, 50.0f);
	m_UnMoveModel->m_Transform.SetRotation(90.0f, 0.0f, 0.0f);
	m_UnMoveModel->m_Transform.SetScale(50.0f, 50.0f, 1.0f);
	m_UnMoveModel->Render(viewMatrix, projMatrix);

	//rander brick front;
	m_UnMoveModel->m_Transform.SetPosition(wallTranslation[0], wallTranslation[1], wallTranslation[2]);
	m_UnMoveModel->m_Transform.SetRotation(wallRotation[0], wallRotation[1], wallRotation[2]);
	m_UnMoveModel->m_Transform.SetScale(wallScaling[0], wallScaling[1], wallScaling[2]);
	m_UnMoveModel->Render(viewMatrix, projMatrix);

	//TODO: change the wall object with a cube rather than a rectangle XD;
	//rander brick back;
	m_UnMoveModel->m_Transform.SetPosition(wallTranslation[0], wallTranslation[1], wallTranslation[2] + 0.001f);
	m_UnMoveModel->m_Transform.SetRotation(wallRotation[0], wallRotation[1] + 180, wallRotation[2]);
	m_UnMoveModel->m_Transform.SetScale(wallScaling[0], wallScaling[1], wallScaling[2]);
	m_UnMoveModel->Render(viewMatrix, projMatrix);


	return true;
}





//update light PS shader buffers 
void ZoneClass::SetLight(int lightType)
{
	cb_ps_camera.data.cameraPosition = m_Camera->GetPosition();
	cb_ps_camera.ApplyChanges();
	deviceContext->PSSetConstantBuffers(1, 1, cb_ps_camera.GetAddress());

	cb_ps_light.data.ambientLightColor = XMFLOAT3(m_Light->GetAmbientColor().x, m_Light->GetAmbientColor().y, m_Light->GetAmbientColor().z);
	cb_ps_light.data.ambientLightStrength = m_Light->ambientLightStrength;
	cb_ps_light.data.dynamicLightColor = XMFLOAT3(m_Light->GetDiffuseColor().x, m_Light->GetDiffuseColor().y, m_Light->GetDiffuseColor().z);
	cb_ps_light.data.dynamicLightStrength = m_Light->dynamicLightStrength;
	cb_ps_light.data.lightPosition = m_Light->m_transform.GetPosition();
	cb_ps_light.data.dynamicLightAttenuation_a = m_Light->dynamicLightAttenuation_a;
	cb_ps_light.data.dynamicLightAttenuation_b = m_Light->dynamicLightAttenuation_b;
	cb_ps_light.data.dynamicLightAttenuation_c = m_Light->dynamicLightAttenuation_c;
	cb_ps_light.data.LightType = lightType;
	cb_ps_light.data.lightDirection = m_Light->GetDirection();
	cb_ps_light.ApplyChanges();

	deviceContext->PSSetConstantBuffers(0, 1, cb_ps_light.GetAddress());
}
