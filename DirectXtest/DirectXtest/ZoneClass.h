#pragma once

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;


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
#include "modelclass.h"
#include "frustumclass.h"
#include "skydomeclass.h"
#include "skycubeclass.h"
#include "lightclass.h"
#include "particlesystemclass.h"
#include"SkinnedModelClass.h"

#include "rendertextureclass.h"

class ZoneClass
{
public:

	ZoneClass();
	ZoneClass(const ZoneClass&);
	~ZoneClass();

	bool Initialize(D3DClass*, HWND, int, int, float);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, TextureManagerClass*, float frametime, int fps, int cpu);

	float cubeTranslation[3] = {12.0f,3.0f,12.0f};

	LightClass* m_Light;
	SkinnedModelInstance mCharacterInstance2;
	//GameObjectClass* m_MeshModel;

private:
	void HandleMovementInput(InputClass*, float frameTime,float fps);
	
	bool RenderSceneToTexture(D3DClass* Direct3D, ShaderManagerClass*);
	
	bool Render(D3DClass*, ShaderManagerClass*, TextureManagerClass*);


private:
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
	ModelClass* m_Model;
	


	//M3d Character Model Data
	SkinnedModelClass* mCharacterModel;

	//Character Instance
	SkinnedModelInstance mCharacterInstance1;
	
	ParticleSystemClass* m_ParticleSystem;

	RenderTextureClass* m_RenderTexture;


	//bool m_particleFollow = true;

	bool m_displayUI, m_wireFrame, m_cellLines, m_heightLocked;
	int m_cubemapsky = 0;

	XMMATRIX modelPosition;

};
