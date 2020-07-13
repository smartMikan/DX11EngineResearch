#pragma once



#include <directxmath.h>
#include "Transform.h"

using namespace DirectX;


////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	Transform m_transform;



	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Frame(float frametime);
	void Render();
	void GetViewMatrix(XMMATRIX&);

	//We add two new functions for reflection. 
	//One is to build the reflection matrix and the other is to retrieve the reflection matrix.
	void RenderReflection(float);
	XMMATRIX GetReflectionViewMatrix();

	void RenderBaseViewMatrix();
	void GetBaseViewMatrix(XMMATRIX&);

private:
	XMMATRIX m_viewMatrix;
	XMMATRIX m_reflectionViewMatrix;
	XMMATRIX m_baseViewMatrix;
};
