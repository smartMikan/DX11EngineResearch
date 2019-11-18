#pragma once
//The header file for the FrustumClass is fairly simple. 
//The class doesn't require any initialization or shutdown. 
//Each frame the ConstructFrustum function is called after the camera has first been rendered.
//The ConstructFrustum function uses the private m_planes to calculate and store the six planes of the view frustum based on the updated viewing location. 
//From there we can call any of the four check functions to seen if either a point, cube, sphere, or rectangle are inside the viewing frustum or not.


#ifndef _FRUSTUMCLASS_H_
#define _FRUSTUMCLASS_H_


#include<directxmath.h>

using namespace DirectX;

class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass& other);
	~FrustumClass();

	void Initialize(float);

	void ConstructFrustum(XMMATRIX, XMMATRIX);

	bool CheckPoint(float,float,float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float,float,float);
	bool CheckRectangle2(float, float, float, float, float, float);

private:
	float m_screenDepth;
	float m_planes[6][4];

};

#endif
