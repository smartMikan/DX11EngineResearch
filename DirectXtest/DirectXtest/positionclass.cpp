#include "positionclass.h"


//The class constructor initializes the private member variables to zero to start with.
PositionClass::PositionClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_forwardSpeed = 0.0f;
	m_backwardSpeed = 0.0f;
	m_upwardSpeed = 0.0f;
	m_downwardSpeed = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed = 0.0f;
	m_lookDownSpeed = 0.0f;

}

PositionClass::PositionClass(const PositionClass &other)
{
}


PositionClass::~PositionClass()
{
}

//The SetFrameTime function is used to set the frame speed in this class. 
//PositionClass will use that frame time speed to calculate how fast the viewer should be moving and rotating. 
//This function should always be called at the beginning of each frame before using this class to move the viewing position.

void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
}


//The SetPosition and SetRotation functions are used for setting the position and rotation of the viewer/camera. These functions are generally used to initialize the position of the camera other than at the origin. In this tutorial the camera will be set slightly back from the grid and in the center of it.
void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}




//The GetPosition and GetRotation functions return the current position and rotation of the camera location. In this tutorial these functions are called to provide the location and rotation of the camera for display purposes. We will draw the position/rotation as text strings on the left side of the screen. This is very useful for debugging.
void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}

XMFLOAT3 PositionClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMMATRIX PositionClass::GetPositionMatrix()
{
	return XMMatrixTranslation(m_positionX, m_positionY, m_positionZ);
}

void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}

XMFLOAT3 PositionClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

XMMATRIX PositionClass::GetRotationMatrix()
{
	return XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotationX), XMConvertToRadians(m_rotationY+180), XMConvertToRadians(m_rotationZ));
}



//GetRotation returns the Y-axis rotation of the viewer. 
void PositionClass::GetRotation(float & y)
{
	y = m_rotationY;
}

float PositionClass::GetRotationY()
{
	return m_rotationY;
}

//The following eight movement functions all work nearly the same. All eight functions are called each frame. 
//The keydown input variable to each function indicates if the user is pressing the forward key, the backward key, and so forth. 
//If they are pressing the key then each frame the speed will accelerate until it hits a maximum. 
//This way the camera speeds up similar to the acceleration in a vehicle creating the effect of smooth movement and high responsiveness.
//Likewise if the user releases the key and the keydown variable is false it will then smoothly slow down each frame until the speed hits zero.
//The speed is calculated against the frame time to ensure the movement speed remains the same regardless of the frame rate. 
//Each function then uses some basic math to calculate the new position of the viewer/camera.

//This function calculates the forward speed and movement of the viewer / camera.
void PositionClass::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_forwardSpeed += m_frameTime * 0.015f;
		if (m_forwardSpeed > (m_frameTime * 0.015f))
		{
			m_forwardSpeed = m_frameTime * 0.015f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.5f;

		if (m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}

//This function calculates the backward speed and movement of the viewer/camera.
void PositionClass::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_backwardSpeed += m_frameTime * 0.01f;

		if (m_backwardSpeed > (m_frameTime * 0.015f))
		{
			m_backwardSpeed = m_frameTime * 0.015f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.015f;

		if (m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX -= sinf(radians) * m_backwardSpeed;
	m_positionZ -= cosf(radians) * m_backwardSpeed;

	return;
}

void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_upwardSpeed += m_frameTime * 0.015f;

		if (m_upwardSpeed > (m_frameTime * 0.15f))
		{
			m_upwardSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.15f;

		if (m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY += m_upwardSpeed;

	return;
}

void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_downwardSpeed += m_frameTime * 0.015f;

		if (m_downwardSpeed > (m_frameTime * 0.15f))
		{
			m_downwardSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.15f;

		if (m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY -= m_downwardSpeed;

	return;
}



//The movement functions both work the same. 
//Both functions are called each frame. 
//The keydown input variable to each function indicates if the user is pressing the left key or the right key. 
//If they are pressing the key then each frame the speed will accelerate until it hits a maximum. 
//This way the camera speeds up similar to the acceleration in a vehicle creating the effect of smooth movement and high responsiveness.
//Likewise if the user releases the key and the keydown variable is false it will then smoothly slow down each frame until the speed hits zero. 
//The speed is calculated against the frame time to ensure the movement speed remains the same regardless of the frame rate. 
//Each function then uses some basic math to calculate the new position of the camera.
void PositionClass::TurnLeft(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
	if (keydown) {
		m_leftTurnSpeed += m_frameTime * 0.01f;
		if (m_leftTurnSpeed > (m_frameTime*0.15f)) {
			m_leftTurnSpeed = m_frameTime*0.15f;
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
	m_rotationY -= m_leftTurnSpeed;
	if (m_rotationY < 0.0f) {
		m_rotationY += 360.0f;
	}
	return;
	
}

void PositionClass::TurnRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
	if (keydown) {
		m_rightTurnSpeed += m_frameTime * 0.01f;
		if (m_rightTurnSpeed > (m_frameTime*0.15f)) {
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
	m_rotationY += m_rightTurnSpeed;
	if (m_rotationY > 0.0f) {
		m_rotationY -= 360.0f;
	}
	return;
}

void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if (m_lookUpSpeed > (m_frameTime * 0.15f))
		{
			m_lookUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime * 0.01f;

		if (m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if (m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}

	return;
}

void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if (m_lookDownSpeed > (m_frameTime * 0.15f))
		{
			m_lookDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime * 0.01f;

		if (m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if (m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}

	return;
}

void PositionClass::Orbit(bool keydown, bool isleft, XMFLOAT3 targetpsotion)
{
	if (!isleft) {
		// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
		if (keydown) {
			m_rightTurnSpeed += m_frameTime * 0.01f;
			if (m_rightTurnSpeed > (m_frameTime*0.15f)) {
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
	}
	else
	{
		// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
		if (keydown) {
			m_leftTurnSpeed += m_frameTime * 0.01f;
			if (m_leftTurnSpeed > (m_frameTime*0.15f)) {
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
	}
	// Update the rotation using the turning speed.
	m_rotationY += m_rightTurnSpeed - m_leftTurnSpeed;
	if (m_rotationY > 360.0f) {
		m_rotationY -= 360.0f;
	}
	if (m_rotationY < 0.0f) {
		m_rotationY += 360.0f;
	}
	// Update the position using the roatation.
	m_positionX = targetpsotion.x - sinf(m_rotationY * 0.0174532925f) * 8.0f;
	m_positionZ = targetpsotion.z - cosf(m_rotationY * 0.0174532925f) * 8.0f;
	m_positionY = targetpsotion.y + 6.0f;

}

