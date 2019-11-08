#pragma once

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

