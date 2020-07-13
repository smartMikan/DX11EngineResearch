#include "cameraclass.h"
//The class constructor will initialize the position and rotation of the camera to be at the origin of the scene.

CameraClass::CameraClass()
{	
}


CameraClass::CameraClass(const CameraClass& other)
{
	m_transform = other.m_transform;
	m_baseViewMatrix = other.m_baseViewMatrix;
	m_reflectionViewMatrix = other.m_reflectionViewMatrix;
	m_viewMatrix = other.m_viewMatrix;
}


CameraClass::~CameraClass()
{
}
//The SetPosition and SetRotation functions are used for setting up the position and rotation of the camera.

void CameraClass::SetPosition(float x, float y, float z)
{
	return m_transform.SetPosition(x, y, z);
}



void CameraClass::SetRotation(float x, float y, float z)
{
	
	return m_transform.SetRotation(x,y,z);
}
//The GetPosition and GetRotation functions return the location and rotation of the camera to calling functions.

XMFLOAT3 CameraClass::GetPosition()
{
	return m_transform.GetPosition();
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_transform.GetRotation();
}
//The Render function uses the position and rotation of the camera to build and update the view matrix.We first setup our variables for up, position, rotation, and so forth.Then at the origin of the scene we first rotate the camera based on the x, y, and z rotation of the camera.Once it is properly rotated when then translate the camera to the position in 3D space.With the correct values in the position, lookAt, and up we can then use the XMMatrixLookAtLH function to create the view matrix to represent the current camera rotation and translation.

void CameraClass::Frame(float frametime)
{
	m_transform.SetFrameTime(frametime);
}

void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
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

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.

	XMFLOAT3 rotation = m_transform.GetRotation();
	pitch = XMConvertToRadians(rotation.x);
	yaw = XMConvertToRadians(rotation.y);
	roll = XMConvertToRadians(rotation.z);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


//After the Render function has been called to create the view matrix we can provide the updated view matrix to calling functions using this GetViewMatrix function.
//The view matrix will be one of the three main matrices used in the HLSL vertex shader.

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

//The new RenderReflection function builds a reflection view matrix the same way as the regular Render function builds a view matrix. 
//The only difference is that we take as input the height of the object that will act as the Y axis plane and then we use that height to invert the position.y variable for reflection. 
//This will build the reflection view matrix that we can then use in the shader.
//Note that this function only works for the Y axis plane.

void CameraClass::RenderReflection(float height)
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float radians;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	position = m_transform.GetPosition();
	position.y = -position.y + (height * 2.0f);

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);
	
	// Calculate the rotation in radians.
	radians = m_transform.GetRotationYRaid();

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + position.x;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + position.z;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Create the view matrix from the three vectors.
	m_reflectionViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


void CameraClass::RenderBaseViewMatrix()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position = m_transform.GetPosition();

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);


	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = m_transform.GetRotationMatrix();


	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_baseViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


void CameraClass::GetBaseViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_baseViewMatrix;
	return;
}


XMMATRIX CameraClass::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
}