////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
	//m_rotationZ = 270.0f;
	//m_leftTurnSpeed = 0.0f;
	//m_rightTurnSpeed = 0.0f;
	positionClass.SetRotation(0.0f, -180.0f, 0.0f);
	SetDirection(0.0f, -1.0f, 0.0f);
	m_frameTime = 0.0f;
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}


void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}


void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

void LightClass::SetDirectionYawPitchRoll(float, float, float)
{


}


XMFLOAT3 LightClass::GetDirection()
{
	//XMMATRIX rotMATRIX = XMMatrixRotationRollPitchYaw(m_rotationX,m_rotationY,m_rotationZ);
	float m_rotationX, m_rotationY, m_rotationZ;
	float yaw, pitch, roll;
	positionClass.GetRotation(m_rotationX, m_rotationY, m_rotationZ);
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;
	XMVECTOR dir;
	dir = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	XMFLOAT3 rot;
	XMStoreFloat3(&rot, dir);
	return rot;
	//return m_direction;
}


void LightClass::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT3(x, y, z);
	return;
}

void LightClass::SetLookAt(float x, float y, float z)
{
	m_lookAt.x = x;
	m_lookAt.y = y;
	m_lookAt.z = z;
	return;
}

XMFLOAT3 LightClass::GetPosition()
{
	return m_position;
}

void LightClass::GenerateViewMatrix()
{
	XMFLOAT3 up;
	XMVECTOR upVector, positionVector, lookAtVector;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	upVector = XMLoadFloat3(&up);
	positionVector = XMLoadFloat3(&m_position);
	lookAtVector = XMLoadFloat3(&m_lookAt);

	// Create the view matrix from the three vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void LightClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void LightClass::GenerateOrthoMatrix(float width, float depthPlane, float nearPlane)
{
	// Create the orthographic matrix for the light.
	m_orthoMatrix = XMMatrixOrthographicLH(width, width, nearPlane, depthPlane);

	return;
}


void LightClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}


void LightClass::SetFrameTime(float time)
{
	m_frameTime = time;
}

void LightClass::Frame()
{


	static float lightAngle = 270.0f;
	float radians;
	static float lightPosX = 9.0f;
	//Each frame we now rotate a directional light from 270 degrees to 90 degrees to simulate sun light movement.
	// Update the position of the light each frame.
	lightPosX -= 0.002f * m_frameTime;

	// Update the angle of the light each frame.
	lightAngle -= 0.02f * m_frameTime;
	if (lightAngle < 90.0f)
	{
		lightAngle = 270.0f;

		// Reset the light position also.
		lightPosX = 9.0f;
	}
	radians = lightAngle * 0.0174532925f;

	// Update the direction of the light.
	SetDirection(sinf(radians), cosf(radians), 0.0f);


	//Also each frame we now simulate a sun light rotation using the position and lookat. As a directional light has no position we have to just create a simulated version of it by polarizing the position and lookat X coordinate. 
	//If your light needs to cover more distance then just increase the Y coordinate distance between the position and the lookat.

	// Set the position and lookat for the light.
	SetPosition(lightPosX, 18.0f, -0.1f);
	SetLookAt(-lightPosX, 0.0f, 5.0f);



}


