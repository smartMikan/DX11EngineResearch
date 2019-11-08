#include "frustumclass.h"



FrustumClass::FrustumClass()
{
}

FrustumClass::FrustumClass(const FrustumClass & other)
{
}


FrustumClass::~FrustumClass()
{
}


//ConstructFrustum is called every frame by the GraphicsClass. 
//It passes in the the depth of the screen, the projection matrix, and the view matrix. 
//We then use these input variables to calculate the matrix of the view frustum at that frame. 
//With the new frustum matrix we then calculate the six planes that form the view frustum.

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	float zMinimum, r;
	XMMATRIX matrix;

	//Caculate the minimum Z distance in the frustum
	zMinimum = -projectionMatrix.r[3].m128_f32[2] / projectionMatrix.r[2].m128_f32[2];

	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix.r[2].m128_f32[2] = r;
	projectionMatrix.r[3].m128_f32[2] = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.

	matrix = XMMatrixMultiply(viewMatrix, projectionMatrix);

	// Calculate near plane of frustum.
	m_planes[0].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[2];
	m_planes[0].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[2];
	m_planes[0].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[2];
	m_planes[0].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[2];

	XMPlaneNormalize(m_planes[0]);
	
	// Calculate far plane of frustum.
	m_planes[1].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[2];
	m_planes[1].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[2];
	m_planes[1].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[2];
	m_planes[1].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[2];

	XMPlaneNormalize(m_planes[1]);
	
	// Calculate left plane of frustum.
	m_planes[2].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[0];
	m_planes[2].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[0];
	m_planes[2].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[0];
	m_planes[2].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[0];

	XMPlaneNormalize(m_planes[2]);
	
	// Calculate right plane of frustum.
	m_planes[3].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[0];
	m_planes[3].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[0];
	m_planes[3].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[0];
	m_planes[3].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[0];

	XMPlaneNormalize(m_planes[3]);
	
	// Calculate top plane of frustum.
	m_planes[4].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[1];
	m_planes[4].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[1];
	m_planes[4].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[1];
	m_planes[4].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[1];

	XMPlaneNormalize(m_planes[4]);
	
	// Calculate bottom plane of frustum.
	m_planes[5].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[1];
	m_planes[5].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[1];
	m_planes[5].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[1];
	m_planes[5].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[1];

	XMPlaneNormalize(m_planes[5]);
	
	return;

}

//CheckPoint checks if a single point is inside the viewing frustum. 
//This is the most general of the four checking algorithms but can be very efficient if used correctly in the right situation over the other checking methods. 
//It takes the point and checks to see if it is inside all six planes. 
//If the point is inside all six then it returns true, otherwise it returns false if not.

bool FrustumClass::CheckPoint(float x, float y, float z) {
	int i;
	XMVECTOR vectror;
	vectror.m128_f32[0] = x;
	vectror.m128_f32[1] = y;
	vectror.m128_f32[2] = z;


	for (int i = 0; i < 6; i++)
	{
		// Check if the point is inside all six planes of the view frustum.
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			return false;
		}
	}
	return true;
}

//CheckCube checks if any of the eight corner points of the cube are inside the viewing frustum.
//It only requires as input the center point of the cube and the radius, it uses those to calculate the 8 corner points of the cube.
//It then checks if any one of the corner points are inside all 6 planes of the viewing frustum. 
//If it does find a point inside all six planes of the viewing frustum it returns true, otherwise it returns false.
bool FrustumClass :: CheckCube(float xCenter,float yCenter,float zCenter,float radius) {

	int i;
	XMVECTOR vectror;
	vectror.m128_f32[0] = xCenter;
	vectror.m128_f32[1] = yCenter;
	vectror.m128_f32[2] = zCenter;

	// Check if any one point of the cube is in the view frustum.
	for (i = 0; i < 6; i++) {
		vectror.m128_f32[0] = xCenter - radius;
		vectror.m128_f32[1] = yCenter - radius;
		vectror.m128_f32[2] = zCenter - radius;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter + radius;
		vectror.m128_f32[1] = yCenter - radius;
		vectror.m128_f32[2] = zCenter - radius;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter - radius;
		vectror.m128_f32[1] = yCenter + radius;
		vectror.m128_f32[2] = zCenter - radius;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter + radius;
		vectror.m128_f32[1] = yCenter + radius;
		vectror.m128_f32[2] = zCenter - radius;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter - radius;
		vectror.m128_f32[1] = yCenter - radius;
		vectror.m128_f32[2] = zCenter + radius;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter + radius;
		vectror.m128_f32[1] = yCenter - radius;
		vectror.m128_f32[2] = zCenter + radius;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter - radius;
		vectror.m128_f32[1] = yCenter + radius;
		vectror.m128_f32[2] = zCenter + radius;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter + radius;
		vectror.m128_f32[1] = yCenter + radius;
		vectror.m128_f32[2] = zCenter + radius;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		return false;
	}
	
	return true;
	
}

//CheckSphere checks if the radius of the sphere from the center point is inside all six planes of the viewing frustum.
//If it is outside any of them then the sphere cannot be seen and the function will return false.
//If it is inside all six the function returns true that the sphere can be seen.

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius) {
	int i;
	XMVECTOR vectror;
	vectror.m128_f32[0] = xCenter;
	vectror.m128_f32[1] = yCenter;
	vectror.m128_f32[2] = zCenter;

	//Check if any of the 6 planes of the rectangle are inside the view frustum
	for (i = 0; i<6; i++)
	{
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < -radius)
		{
			return false;
		}
	}

	return true;
}


//CheckRectangle works the same as CheckCube except that that it takes as input the x radius, y radius, and z radius of the rectangle instead of just a single radius of a cube. 
//It can then calculate the 8 corner points of the rectangle and do the frustum checks similar to the CheckCube function.

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter,float xSize,float ySize,float zSize) {

	int i;
	XMVECTOR vectror;
	vectror.m128_f32[0] = xCenter;
	vectror.m128_f32[1] = yCenter;
	vectror.m128_f32[2] = zCenter;
	for (i = 0; i < 6; i++) {
		vectror.m128_f32[0] = xCenter - xSize;
		vectror.m128_f32[1] = yCenter - ySize;
		vectror.m128_f32[2] = zCenter - zSize;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter + xSize;
		vectror.m128_f32[1] = yCenter - ySize;
		vectror.m128_f32[2] = zCenter - zSize;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter - xSize;
		vectror.m128_f32[1] = yCenter + ySize;
		vectror.m128_f32[2] = zCenter - zSize;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter - xSize;
		vectror.m128_f32[1] = yCenter - ySize;
		vectror.m128_f32[2] = zCenter + zSize;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter + xSize;
		vectror.m128_f32[1] = yCenter + ySize;
		vectror.m128_f32[2] = zCenter - zSize;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter + xSize;
		vectror.m128_f32[1] = yCenter - ySize;
		vectror.m128_f32[2] = zCenter + zSize;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter - xSize;
		vectror.m128_f32[1] = yCenter + ySize;
		vectror.m128_f32[2] = zCenter + zSize;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		vectror.m128_f32[0] = xCenter + xSize;
		vectror.m128_f32[1] = yCenter + ySize;
		vectror.m128_f32[2] = zCenter + zSize;
		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			continue;
		}

		return false;
	}

	return true;

}

