#include "frustumclass.h"



FrustumClass::FrustumClass()
{
}


FrustumClass::~FrustumClass()
{
}

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	float zMinimum, r;
	XMMATRIX matrix;

	matrix = DirectX::XMMatrixIdentity();

	//Caculate the minimum Z distance in the frustum
	zMinimum = -projectionMatrix.r[3].m128_f32[2] / projectionMatrix.r[2].m128_f32[2];

	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix.r[2].m128_f32[2] = r;
	projectionMatrix.r[3].m128_f32[2] = -r * zMinimum;

	XMMatrixMultiply(matrix, XMMatrixMultiply(viewMatrix, projectionMatrix));

	//near
	m_planes[0].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[2];
	m_planes[0].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[2];
	m_planes[0].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[2];
	m_planes[0].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[2];

	XMPlaneNormalize(m_planes[0]);
	
	//far
	m_planes[1].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[2];
	m_planes[1].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[2];
	m_planes[1].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[2];
	m_planes[1].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[2];

	XMPlaneNormalize(m_planes[1]);
	//left
	m_planes[2].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[0];
	m_planes[2].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[0];
	m_planes[2].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[0];
	m_planes[2].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[0];

	XMPlaneNormalize(m_planes[2]);
	//right
	m_planes[3].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[0];
	m_planes[3].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[0];
	m_planes[3].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[0];
	m_planes[3].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[0];

	XMPlaneNormalize(m_planes[3]);
	
	//top
	m_planes[4].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[1];
	m_planes[4].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[1];
	m_planes[4].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[1];
	m_planes[4].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[1];

	XMPlaneNormalize(m_planes[4]);
	//bottom
	m_planes[5].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[1];
	m_planes[5].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[1];
	m_planes[5].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[1];
	m_planes[5].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[1];

	XMPlaneNormalize(m_planes[5]);
	
	return;

}

bool FrustumClass::CheckPoint(float x, float y, float z) {
	int i;
	XMVECTOR vectror;
	vectror.m128_f32[0] = x;
	vectror.m128_f32[1] = y;
	vectror.m128_f32[2] = z;


	for (int i = 0; i < 6; i++)
	{

		if (XMPlaneDotCoord(m_planes[i], vectror).m128_f32[0] < 0.0f) {
			return false;
		}
	}
	return true;
}


bool FrustumClass :: CheckCube(float xCenter,float yCenter,float zCenter,float radius) {

	int i;
	XMVECTOR vectror;
	vectror.m128_f32[0] = xCenter;
	vectror.m128_f32[1] = yCenter;
	vectror.m128_f32[2] = zCenter;
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

