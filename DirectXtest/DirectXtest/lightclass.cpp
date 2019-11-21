////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
	m_rotationZ = 270.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
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


void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
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


XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}


float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

void LightClass::SetFrameTime(float time)
{
	m_frameTime = time;
}


void LightClass::TurnRight(bool keydown)
{
	float roll,dirX,dirY;
	
	// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
	if (keydown) {
		m_rightTurnSpeed += m_frameTime * 0.01f;
		if (m_rightTurnSpeed > (m_frameTime * 0.15f)) {
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 0.01f;
		if (m_rightTurnSpeed < 0.0f) {
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationZ += m_rightTurnSpeed;
	if (m_rotationZ > 0.0f) {
		m_rotationZ -= 360.0f;
	}

	roll = m_rotationZ * 0.0174532925f;

	// Update the position.
	dirY = sinf(roll);
	dirX = cosf(roll);
	SetDirection(dirX, dirY, m_direction.z);
	return;
}

void LightClass::TurnLeft(bool keydown)
{
	float roll, dirX, dirY;
	// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
	if (keydown) {
		m_leftTurnSpeed += m_frameTime * 0.01f;
		if (m_leftTurnSpeed > (m_frameTime * 0.15f)) {
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.01f;
		if (m_leftTurnSpeed < 0.0f) {
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationZ -= m_leftTurnSpeed;
	if (m_rotationZ < 0.0f) {
		m_rotationZ += 360.0f;
	}

	roll = m_rotationZ * 0.0174532925f;

	// Update the position.
	dirY = sinf(roll);
	dirX = cosf(roll);
	SetDirection(dirX, dirY, m_direction.z);
	return;

}