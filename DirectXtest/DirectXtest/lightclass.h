#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////

#include <directxmath.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();


	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);


	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

	void SetFrameTime(float time);

	void TurnRight(bool keydown);

	void TurnLeft(bool keydown);


private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT4 m_ambientColor;
	XMFLOAT3 m_direction;
	
	XMFLOAT4 m_specularColor;
	float m_specularPower;

	float m_frameTime;
	float m_rotationZ;
	float m_leftTurnSpeed, m_rightTurnSpeed;
};

#endif
