#include "lightclass.h"


LightClass::LightClass()
{
	m_transform.SetRotation(0.0f, 0.0f, 0.0f);
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


//void LightClass::SetDirection(float x, float y, float z)
//{
//	//m_direction = XMFLOAT3(x, y, z);
//	
//	return;
//}




XMFLOAT3 LightClass::GetDirection()
{
	float m_rotationX, m_rotationY, m_rotationZ;
	float yaw, pitch, roll;
	XMFLOAT3 up, lookAt;
	XMVECTOR lookatvec;

	
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;


	m_transform.GetRotation(m_rotationX, m_rotationY, m_rotationZ);
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	lookatvec = XMLoadFloat3(&lookAt);


	XMMATRIX rotMATRIX = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	lookatvec = XMVector3TransformCoord(lookatvec, rotMATRIX);

	XMStoreFloat3(&lookAt, lookatvec);
	return lookAt;
}






//void LightClass::GenerateViewMatrix()
//{
//	XMFLOAT3 up;
//	XMVECTOR upVector, positionVector, lookAtVector;
//
//	// Setup the vector that points upwards.
//	up.x = 0.0f;
//	up.y = 1.0f;
//	up.z = 0.0f;
//	upVector = XMLoadFloat3(&up);
//	positionVector = XMLoadFloat3(&position.GetPosition());
//	lookAtVector = XMLoadFloat3(&m_direction);
//
//	// Translate the rotated camera position to the location of the viewer.
//	lookAtVector = XMVectorAdd(positionVector, lookAtVector);
//
//	// Create the view matrix from the three vectors.
//	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
//
//	return;
//}

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


void LightClass::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;


	// Setup field of view and screen aspect for a square light source.
	fieldOfView = (float)XM_PI * 0.5f;
	screenAspect = 1.0f;

	// Create the projection matrix for the light.
	m_projMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	return;
}

void LightClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projMatrix;
	return;
}


void LightClass::Frame(float frametime)
{
	m_transform.SetFrameTime(frametime);


	XMFLOAT3 up, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&m_transform.GetPosition());

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);
	m_transform.GetRotation(pitch, yaw, roll);
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch *= 0.0174532925f;
	yaw *= 0.0174532925f;
	roll *=  0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}


