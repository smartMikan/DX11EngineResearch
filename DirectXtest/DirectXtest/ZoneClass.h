#pragma once

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;

#include "ipch.h"

const bool BakeModelMode = false;


class ZoneClass
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	ZoneClass();
	ZoneClass(const ZoneClass&);
	~ZoneClass();

	int Initialize(D3DClass*, HWND, int, int, float);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, TextureManagerClass*, float frametime, int fps, int cpu);


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
	//SkinnedModelInstance mCharacterInstance2;
	GameObjectClass* m_AnimModel;
	GameObjectClass* m_UnMoveModel;
	Player* m_Player;
	GameObjectClass* m_tianyi;
	GameObjectClass* m_sword;

	Enemy* m_enemies;

	
private:
	void HandleMovementInput(InputClass*,int fps);
	
	bool RenderShadowMap(D3DClass* Direct3D, const XMMATRIX&, const XMMATRIX&);
	
	bool Render(D3DClass*, ShaderManagerClass*, TextureManagerClass*);
	
	bool RenderSky(D3DClass*, ShaderManagerClass*, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);
	
	bool RenderParticles(D3DClass*, ShaderManagerClass*, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);

	bool RenderTerrain(D3DClass*, ShaderManagerClass*, TextureManagerClass* , const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);

	bool RenderAnimationGameObjects(D3DVertexShader*, const XMMATRIX& ,const XMMATRIX&);
	bool RenderNonAnimationGameObjects(D3DVertexShader*, const XMMATRIX& ,const XMMATRIX&);
	
	//bool InitializeShaders();
	void SetLight(int lightType);
private:
	ComPtr<ID3D11Device> device;
	ComPtr <ID3D11DeviceContext> deviceContext;

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
	


	//M3d Character Model Data
	//SkinnedModelClass* mCharacterModel;

	//Character Instance
	//SkinnedModelInstance mCharacterInstance1;
	
	ParticleSystemClass* m_ParticleSystem;

	RenderTextureClass* m_RenderTexture;


	//bool m_particleFollow = true;

	bool m_wireFrame = false, m_cellLines = false, m_heightLocked = false;
	int m_cubemapsky = 0;

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

};
