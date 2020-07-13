#include "Transform.h"


//The class constructor initializes the private member variables to zero to start with.
Transform::Transform()
{
	scale = Vector3(1, 1, 1);

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

Transform::Transform(const Transform& other)
{
	positon = other.positon;
	rotation = other.rotation;
	scale = other.scale;

	m_frameTime = other.m_frameTime;

	m_forwardSpeed = other.m_forwardSpeed;
	m_backwardSpeed = other.m_backwardSpeed;
	m_upwardSpeed = other.m_upwardSpeed;
	m_downwardSpeed = other.m_downwardSpeed;
	m_leftTurnSpeed = other.m_leftTurnSpeed;
	m_rightTurnSpeed = other.m_rightTurnSpeed;
	m_lookUpSpeed = other.m_lookUpSpeed;
	m_lookDownSpeed = other.m_lookDownSpeed;
}


Transform::~Transform()
{
}

//The SetFrameTime function is used to set the frame speed in this class. 
//Transform will use that frame time speed to calculate how fast the viewer should be moving and rotating. 
//This function should always be called at the beginning of each frame before using this class to move the viewing position.

void Transform::SetFrameTime(float time)
{
	m_frameTime = time;
}


//The SetPosition and SetRotation functions are used for setting the position and rotation of the viewer/camera. These functions are generally used to initialize the position of the camera other than at the origin. In this tutorial the camera will be set slightly back from the grid and in the center of it.
void Transform::SetPosition(float x, float y, float z)
{
	positon.pos3D.x = x;
	positon.pos3D.y = y;
	positon.pos3D.z = z;
	return;
}

void Transform::SetPosition(float pos[3])
{
	positon.pos3D.x = pos[0];
	positon.pos3D.y = pos[1];
	positon.pos3D.z = pos[2];
}

void Transform::SetPosition(Transform other)
{
	positon = other.positon;
	return;
}


void Transform::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	return;
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	return;
}




//The GetPosition and GetRotation functions return the current position and rotation of the camera location. In this tutorial these functions are called to provide the location and rotation of the camera for display purposes. We will draw the position/rotation as text strings on the left side of the screen. This is very useful for debugging.
void Transform::GetPosition(float& x, float& y, float& z)
{
	x = positon.pos3D.x;
	y = positon.pos3D.y;
	z = positon.pos3D.z;
	return;
}

XMFLOAT3 Transform::GetPosition()
{
	return XMFLOAT3(positon.pos3D.x, positon.pos3D.y, positon.pos3D.z);
}

XMMATRIX Transform::GetPositionMatrix()
{
	return XMMatrixTranslation(positon.pos3D.x, positon.pos3D.y, positon.pos3D.z);
}

void Transform::GetRotation(float& x, float& y, float& z)
{
	x = rotation.x;
	y = rotation.y;
	z = rotation.z;
	return;
}

XMFLOAT3 Transform::GetRotation()
{
	return XMFLOAT3(rotation.x, rotation.y, rotation.z);
}

XMMATRIX Transform::GetRotationMatrix()
{
	return XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
}



//GetRotation returns the Y-axis rotation of the viewer. 
void Transform::GetRotation(float& y)
{
	y = rotation.y;
}

float Transform::GetRotationY()
{
	return rotation.y;
}

float Transform::GetRotationYRaid()
{
	return rotation.y * (XM_PI / 180.0f);
}

void Transform::GetScale(float& x, float& y, float& z)
{
	x = scale.x;
	y = scale.y;
	z = scale.z;
}

XMFLOAT3 Transform::GetScale()
{
	return XMFLOAT3(scale.x, scale.y, scale.z);
}

XMMATRIX Transform::GetScaleMatrix()
{
	return  XMMatrixScaling(scale.x, scale.y, scale.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
	return GetScaleMatrix() * GetRotationMatrix() * GetPositionMatrix();
}



//The following eight movement functions all work nearly the same. All eight functions are called each frame. 
//The keydown input variable to each function indicates if the user is pressing the forward key, the backward key, and so forth. 
//If they are pressing the key then each frame the speed will accelerate until it hits a maximum. 
//This way the camera speeds up similar to the acceleration in a vehicle creating the effect of smooth movement and high responsiveness.
//Likewise if the user releases the key and the keydown variable is false it will then smoothly slow down each frame until the speed hits zero.
//The speed is calculated against the frame time to ensure the movement speed remains the same regardless of the frame rate. 
//Each function then uses some basic math to calculate the new position of the viewer/camera.

//This function calculates the forward speed and movement of the viewer / camera.
void Transform::MoveForward(bool keydown)
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
	radians = rotation.y * 0.0174532925f;

	// Update the position.
	positon.pos3D.x -= sinf(radians) * m_forwardSpeed;
	positon.pos3D.z -= cosf(radians) * m_forwardSpeed;

	return;
}

//This function calculates the backward speed and movement of the viewer/camera.
void Transform::MoveBackward(bool keydown)
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
	radians = rotation.y * 0.0174532925f;

	// Update the position.
	positon.pos3D.x += sinf(radians) * m_backwardSpeed;
	positon.pos3D.z += cosf(radians) * m_backwardSpeed;

	return;
}

void Transform::MoveUpward(bool keydown)
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
	positon.pos3D.y += m_upwardSpeed;

	return;
}

void Transform::MoveDownward(bool keydown)
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
	positon.pos3D.y -= m_downwardSpeed;

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
void Transform::TurnLeft(bool keydown)
{
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
	rotation.y -= m_leftTurnSpeed;
	if (rotation.y < 0.0f) {
		rotation.y += 360.0f;
	}
	return;

}

void Transform::TurnRight(bool keydown)
{
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
	rotation.y += m_rightTurnSpeed;
	if (rotation.y > 360.0f) {
		rotation.y -= 360.0f;
	}
	return;
}

double Transform::TurnRight(double speed)
{
	double deltaY = speed * m_frameTime * 0.015;
	rotation.y += deltaY;
	if (rotation.y > 360.0f) {
		rotation.y -= 360.0f;
	}
	return deltaY;
}

void Transform::LookUpward(bool keydown)
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
	rotation.x -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if (rotation.x > 90.0f)
	{
		rotation.x = 90.0f;
	}

	return;
}

void Transform::LookDownward(bool keydown)
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
	rotation.x += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if (rotation.x < -90.0f)
	{
		rotation.x = -90.0f;
	}

	return;
}

void Transform::Orbit(bool keydown, bool isleft, XMFLOAT3 targetpsotion)
{
	if (!isleft) {
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
	}
	else
	{
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
	}
	// Update the rotation using the turning speed.
	rotation.y += m_rightTurnSpeed - m_leftTurnSpeed;
	if (rotation.y > 360.0f) {
		rotation.y -= 360.0f;
	}
	if (rotation.y < 0.0f) {
		rotation.y += 360.0f;
	}
	// Update the position using the roatation.
	positon.pos3D.x = targetpsotion.x - sinf(rotation.y * 0.0174532925f) * 8.0f;
	positon.pos3D.y = targetpsotion.y + 6.0f;
	positon.pos3D.z = targetpsotion.z - cosf(rotation.y * 0.0174532925f) * 8.0f;

}

bool Transform::MoveTowardsPoint(float x, float y, float z)
{
	bool reached;
	float delta_x = x - positon.pos3D.x;
	float delta_y = y - positon.pos3D.y;
	float delta_z = z - positon.pos3D.z;


	XMVECTOR dir = XMLoadFloat3(&XMFLOAT3(delta_x, delta_y, delta_z));
	XMFLOAT3 dirData;
	XMStoreFloat3(&dirData, XMVector3LengthEst(dir));

	if (dirData.x <= 1.0f) {

		return true;
	}

	XMStoreFloat3(&dirData, XMVector3Normalize(dir));

	positon.pos3D.x += m_frameTime * dirData.x;
	positon.pos3D.y += m_frameTime * dirData.y;
	positon.pos3D.z += m_frameTime * dirData.z;

	return false;
}

bool Transform::MoveTowardsPoint(XMFLOAT3 targetpsotion)
{
	return MoveTowardsPoint(targetpsotion.x, targetpsotion.y, targetpsotion.z);
}








