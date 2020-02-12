#pragma once

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;

#include<memory>

#include "d3dclass.h"
#include "inputclass.h"
#include "shadermanagerclass.h"
#include "texturemanagerclass.h"
#include "timerclass.h"
#include "userinterfaceclass.h"
#include "cameraclass.h"
#include "positionclass.h"
#include "GameObjectClass.h"
#include "TerrainClass.h"
//#include "modelclass.h"
#include "frustumclass.h"
#include "skydomeclass.h"
#include "skycubeclass.h"
#include "lightclass.h"
#include "particlesystemclass.h"

//#include"SkinnedModelClass.h"

#include "rendertextureclass.h"
#include "Graphic/Shaders.h"

class ZoneClass
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	ZoneClass();
	ZoneClass(const ZoneClass&);
	~ZoneClass();

	bool Initialize(D3DClass*, HWND, int, int, float);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, TextureManagerClass*, float frametime, int fps, int cpu);

	float cubeTranslation[3] = {12.0f,0.1f,33.0f};

	IVertexShader* CreateVertexShader(const std::string& filename);

	LightClass* m_Light;
	//SkinnedModelInstance mCharacterInstance2;
	GameObjectClass* m_AnimModel;
	GameObjectClass* m_UnMoveModel;
	GameObjectClass* m_Player;
	
private:
	void HandleMovementInput(InputClass*, float frameTime,float fps);
	
	bool RenderShadowMap(D3DClass* Direct3D, ShaderManagerClass*);
	
	bool Render(D3DClass*, ShaderManagerClass*, TextureManagerClass*);

	bool InitializeShaders();
	void SetLight();
private:
	ComPtr<ID3D11Device> device;
	ComPtr <ID3D11DeviceContext> deviceContext;

	UserInterfaceClass * m_UserInterface;
	CameraClass* m_Camera;
	PositionClass* m_Position;
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

	bool m_displayUI = false, m_wireFrame = false, m_cellLines = false, m_heightLocked = false;
	int m_cubemapsky = 0;

	XMMATRIX modelPosition;

	// shaders
	std::unique_ptr<D3DVertexShader> d3dvertexshader;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_animation;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_nolight;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_shadowmap;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_shadowmap_anim;
	PixelShader pixelshader;
	PixelShader pixelshader_nolight;
	PixelShader pixelshader_tonemapping;
	PixelShader pixelshader_heightmapping;
	PixelShader pixelshader_depthColor;

	bool enableToneshading = false;
	// c_buffers
	ConstantBuffer<CB_VS_MatrixBuffer> cb_vs_wvpBuffer;
	ConstantBuffer<CB_PS_LightBuffer> cb_ps_light;
	ConstantBuffer<CB_PS_CameraBuffer> cb_ps_camera;
	ConstantBuffer<CB_PS_Material> cb_ps_material;
	ConstantBuffer<CB_PS_ShadowMatrix> cb_ps_shadowMatrix;
	ConstantBuffer<ConstantBuffer_Bones> cb_bones;

};
