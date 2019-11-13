#pragma once

//To allow for camera movement by using the left and right arrow key in this tutorial we create a new class to calculate and maintain the position of the viewer. 
//This class will only handle turning left and right for now but can be expanded to maintain all different movement changes.
//The movement also includes acceleration and deceleration to create a smooth camera effect.

#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


#include <math.h>

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&other);
	~PositionClass();

	void SetFrameTime(float time);
	void GetRotation(float&y);

	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);

private:
	float m_frameTime;
	float m_rotationY;
	float m_leftTurnSpeed,m_rightTurnSpeed;

};

#endif