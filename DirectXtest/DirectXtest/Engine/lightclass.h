#pragma once


#include <directxmath.h>
#include "Transform.h"
#include <directxmath.h>

using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	Transform m_transform;

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	XMFLOAT4 GetAmbientColor();
	
	XMFLOAT4 GetDiffuseColor();


	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

	XMFLOAT3 GetDirection();
	void GetViewMatrix(XMMATRIX&);
	
	void GenerateOrthoMatrix(float, float, float);
	void GetOrthoMatrix(XMMATRIX&);

	void GenerateProjectionMatrix(float screenDepth, float screenNear);
	void GetProjectionMatrix(XMMATRIX& projectionMatrix);

	void Frame(float frametime);


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

	XMMATRIX m_viewMatrix;
	XMMATRIX m_orthoMatrix;
	XMMATRIX m_projMatrix;
	
};

