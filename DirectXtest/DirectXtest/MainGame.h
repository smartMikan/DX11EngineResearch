#pragma once

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;

#include "ipch.h"

const bool BakeModelMode = false;

/// <summary>
/// Main Scene Interface
/// </summary>
class MainGame
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	MainGame();
	MainGame(const MainGame& other);
	~MainGame();

	int Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth, InputClass* Input, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager);
	void Shutdown();
	bool Frame(float frameTime, int fps, int cpu);
	



	void AddGameobject(class GameObject* gameobject);
	void RemoveGameobject(class GameObject* gameobject);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);


	bool CreateEnemy();
	bool CreateEnemyAtPositon(float pos[3]);
	bool RemoveEnemyFromRender(int enemyID);



public:
	float wallTranslation[3] = { 12.0f,0.1f,33.0f };
	float wallRotation[3] = { 0.0f, 0.0f, 0.0f };
	float wallScaling[3] = { 20.0f, 20.0f, 1.0f };
	
	int m_lightType = 0;
	bool toonShading = true;
	bool m_displayUI = false;
	double rendertime;
	

	LightClass* m_Light;

	GameObject* m_AnimModel;
	GameObject* m_UnMoveModel;
	GameObject* m_tianyi;
	GameObject* m_sword;
	Player* m_Player;
	Enemy* m_enemies;

	
private:
	void HandleMovementInput(int fps);
	
	bool UpdateGameObjects(float frameTime, int fps, int cpu);


	bool RenderShadowMap(D3DClass* Direct3D, const XMMATRIX& lightViewMatrix, const XMMATRIX& lightProjMatrix);
	
	bool Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager);
	
	bool RenderSky(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);
	
	bool RenderParticles(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);

	bool RenderTerrain(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);

	bool RenderAnimationGameObjects(D3DVertexShader* vertexshader, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);
	bool RenderNonAnimationGameObjects(D3DVertexShader* vertexshader, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);
	
	//bool InitializeShaders();
	void SetLight(int lightType);


private:

	class D3DClass* Sys_Direct3D;
	ComPtr<ID3D11Device> device;
	ComPtr <ID3D11DeviceContext> deviceContext;

	class InputClass* Sys_Input;
	class ShaderManagerClass* Sys_ShaderManager;
	class TextureManagerClass* Sys_TextureManager;

	UserInterfaceClass * m_UserInterface;
	CameraClass* m_Camera;
	Transform* m_Transform;
	TerrainClass* m_Terrain;
	FrustumClass* m_Frustum;
	SkyDomeClass* m_SkyDome;
	SkyCubeClass* m_DayLightSkyCube;
	SkyCubeClass* m_SunsetSkyCube;
	SkyCubeClass* m_DesertSkyCube;
	SkyCubeClass* m_NebulaSkyCube;
	SkyCubeClass* m_PlanetSkyCube;
	//ModelClass* m_Model;
	


	std::vector<GameObject*> m_GameObjects;
	std::vector<GameObject*> m_PendingGameObjects;

	std::vector<SpriteComponent*> m_Sprites;

	//M3d Character Model Data
	//SkinnedModelClass* mCharacterModel;

	//Character Instance
	//SkinnedModelInstance mCharacterInstance1;
	
	ParticleSystemClass* m_ParticleSystem;

	RenderTextureClass* m_RenderTexture;


	//bool m_particleFollow = true;

	bool m_wireFrame = false, m_cellLines = false, m_heightLocked = false;
	int m_cubemapsky = 0;

	
	bool m_UpdatingGameobjects = false;
	
	XMMATRIX modelPosition;

	
	ShaderManager shaders;
	
	// c_buffers
	ConstantBuffer<CB_VS_MatrixBuffer> cb_vs_wvpBuffer;
	ConstantBuffer<CB_PS_LightBuffer> cb_ps_light;
	ConstantBuffer<CB_PS_CameraBuffer> cb_ps_camera;
	ConstantBuffer<CB_PS_Material> cb_ps_material;
	ConstantBuffer<CB_PS_ShadowMatrix> cb_ps_shadowMatrix;
	ConstantBuffer<ConstantBuffer_Bones> cb_bones;
	ConstantBuffer<ConstantBuffer_BakedBones> cb_baked_bones;


	float m_maxdeltatime = 50.0f;



};
