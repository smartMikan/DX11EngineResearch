#pragma once


#include "textclass.h"
#include "minimapclass.h"
//#include "debugwindowclass.h"

class UserInterfaceClass
{
public:
	UserInterfaceClass();
	UserInterfaceClass(const UserInterfaceClass& other);
	~UserInterfaceClass();

	bool Initialize(D3DClass* Direct3D, int screenHeight, int screenWidth);
	bool Initialize(D3DClass* Direct3D, int screenHeight, int screenWidth,ID3D11ShaderResourceView* renderTexture);
	void Shutdown();

	bool Frame(int fps,int cpu, float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);
	bool Render(ShaderManagerClass* ShaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX orthoMatrix);

	bool UpdateRenderCounts(int renderCount, int nodesDrawn, int nodesCulled);

private:
	bool UpdateFpsString(int fps);
	bool UpdateCpuString(int cpu);
	bool UpdatePositionStrings(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);

private:
	FontClass * m_Font1;
	TextClass *m_FpsString,*m_CpuString , *m_VideoStrings, *m_PositionStrings;
	int m_previousFps;
	int m_previousPosition[6];
	TextClass* m_RenderCountStrings;
	MiniMapClass* m_MiniMap;
	//DebugWindowClass* m_DebugWindow;

	class D3DClass* Sys_D3d;

};
