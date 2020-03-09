#pragma once

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
	void SetPosition(PositionClass other);
	void SetRotation(float, float, float);
	void SetScale(float, float, float);

	void GetPosition(float&x, float&y, float&z);
	XMFLOAT3 GetPosition();
	XMMATRIX GetPositionMatrix();
	void GetRotation(float&x, float&y, float&z);
	XMFLOAT3 GetRotation();
	XMMATRIX GetRotationMatrix();
	void GetRotation(float&y);
	float GetRotationY();

	void GetScale(float&x, float&y, float&z);
	XMFLOAT3 GetScale();
	XMMATRIX GetScaleMatrix();

	XMMATRIX GetWorldMatrix();

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
	float m_scaleX, m_scaleY, m_scaleZ;

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;

	

};
