#pragma once
//The graphics class is the other object that is created by the system class.
//All the graphics functionality in this application will be encapsulated in this class.
//I will also use the header in this file for all the graphics related global settings that we may want to change such as full screen or windowed mode.
//As you can see the D3DClass will be located inside the GraphicsClass.
//The previous project mentioned that all new graphics related classes 
//will be encapsulated in the GraphicsClassand that is why it is the best location 
//for the new D3DClass.






/////////////////////////////
// Filename: graphicsclass.h
/////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


//////////////
// INCLUDES //
//////////////
//#include <windows.h>

//Here is the first change.
//We have taken out the include for windows.h
//and instead included the new d3dclass.h.
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <math.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "rendertextureclass.h"
#include "lightshaderclass.h"
#include "refractionshaderclass.h"
#include "watershaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//We'll need these four globals to start with.


/////////////////////////////
// Class name: GraphicsClass
/////////////////////////////


using namespace DirectX;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND hwnd);
	void ProgramEnd();
	bool Frame();
	void SetWireframeMode(bool isWireframeMod);

private:
	bool Render();
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();
	bool RenderScene();

private:

	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	//We will need four different models for this tutorial. A model for the ground, a model for the wall, and model for the bath, and a model for the water.
	ModelClass* m_GroundModel, * m_WallModel, * m_BathModel, * m_WaterModel;
	LightClass* m_Light;

	//We will also need two render to texture objects for the reflection texture and the refraction texture.
	RenderTextureClass* m_RefractionTexture, * m_ReflectionTexture;

	//The light shader, refraction shader, and water shader are needed for this tutorial.
	LightShaderClass* m_LightShader;
	RefractionShaderClass* m_RefractionShader;
	WaterShaderClass* m_WaterShader;

	//We also need a couple variables to keep track of the water position and water height.
	float m_waterHeight, m_waterTranslation;
};

#endif
