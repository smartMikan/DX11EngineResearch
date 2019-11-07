#pragma once
//The graphics class is the other object that is created by the system class.
//All the graphics functionality in this application will be encapsulated in this class.
//I will also use the header in this file for all the graphics related global settings that we may want to change such as full screen or windowed mode.
//As you can see the D3DClass will be located inside the GraphicsClass.
//The previous tutorial mentioned that all new graphics related classes 
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
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"

#include "lightshaderclass.h"
#include "lightclass.h"

#include "bitmapclass.h"

#include "textclass.h"
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
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND hwnd);
	void ProgramEnd();
	bool Frame(int mouseX, int mouseY);

private:
	bool Render(float rotation, int mouseX, int mouseY);
	bool Update();
private:
	//And the second change is the new private pointer to the D3DClass which we have called m_Direct3D.
	//In case you were wondering I use the prefix 「m_」on all class variables.
	//That way when I'm coding I can remember quickly which variables are members of the class and which are not.
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;

	ColorShaderClass* m_ColorShader;

	TextureShaderClass* m_TextureShader;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	int framesincestart;

	BitmapClass* m_Bitmap;

	TextClass* m_Text;
};

#endif
