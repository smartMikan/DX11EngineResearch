#pragma once

//To allow for camera movement by using the left and right arrow key in this tutorial we create a new class to calculate and maintain the position of the viewer. 
//This class will only handle turning left and right for now but can be expanded to maintain all different movement changes.
//The movement also includes acceleration and deceleration to create a smooth camera effect.

#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#include <DirectXMath.h>
#include <math.h>
using namespace DirectX;



class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&other);
	~PositionClass();

	void SetFrameTime(float time);


	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&x, float&y, float&z);
	XMFLOAT3 GetPosition();
	void GetRotation(float&x, float&y, float&z);
	void GetRotation(float&y);
	float GetRotationY();

	void MoveForward(bool keydown);
	void MoveBackward(bool keydown);
	void MoveUpward(bool keydown);
	void MoveDownward(bool keydown);
	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);
	void LookUpward(bool keydown);
	void LookDownward(bool keydown);

	void Orbit(bool keydown, bool isleft, XMFLOAT3 targetpsotion);


private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;
};

#endif