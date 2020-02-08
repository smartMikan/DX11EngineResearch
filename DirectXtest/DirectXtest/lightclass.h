#pragma once


#include <directxmath.h>
#include "positionclass.h"
#include <directxmath.h>

using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	PositionClass positionClass;

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	XMFLOAT4 GetAmbientColor();
	
	XMFLOAT4 GetDiffuseColor();


	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

	void SetDirection(float, float, float);
	void SetDirectionYawPitchRoll(float, float, float);
	void SetPosition(float, float, float);
	//void SetLookAt(float, float, float);
	
	XMFLOAT3 GetDirection();
	XMFLOAT3 GetPosition();
	void GenerateViewMatrix();
	void GetViewMatrix(XMMATRIX&);
	
	void GenerateOrthoMatrix(float, float, float);
	void GetOrthoMatrix(XMMATRIX&);


	

	void SetFrameTime(float time);

	//void TurnRight(bool keydown);

	//void TurnLeft(bool keydown);

	void Frame();


	float ambientLightStrength = 0.1f;
	float dynamicLightStrength = 1.0f;
	float dynamicLightAttenuation_a = 1.0f;
	float dynamicLightAttenuation_b = 0.0f;
	float dynamicLightAttenuation_c = 0.01f;

private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT4 m_ambientColor;

	XMFLOAT4 m_specularColor;
	float m_specularPower;


	XMFLOAT3 m_direction;
	XMFLOAT3 m_position;
	//XMFLOAT3 m_lookAt;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_orthoMatrix;

	//float m_rotationX, m_rotationY, m_rotationZ;
	
	float m_frameTime;
};

