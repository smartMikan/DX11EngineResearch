#pragma once

#include <DirectXMath.h>
#include <math.h>



struct Vector2
{
	float x, y;

	Vector2() {
		x = 0; y = 0;
	}
	Vector2(float _x, float _y) {
		x = _x, y = _y;
	}
};

struct Vector3
{
	float x, y, z;
	Vector3() {
		x = 0, y = 0, z = 0;
	}
	Vector3(float _x, float _y) :x(_x), y(_y), z(0) {}
	Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
	Vector3(Vector2 _vector2) :x(_vector2.x), y(_vector2.y), z(0) {}
};


union Positon
{
	Vector2 pos2D;
	Vector3 pos3D;

	Positon() {
		pos2D = Vector2();
		pos3D = pos2D;
	}
}; 




using namespace DirectX;

class Transform
{
public:
	Transform();
	Transform(const Transform& other);
	~Transform();

	void SetFrameTime(float time);


	void SetPosition(float, float, float);
	void SetPosition(float pos[3]);
	void SetPosition(Transform other);
	void SetRotation(float, float, float);
	void SetScale(float, float, float);

	void GetPosition(float& x, float& y, float& z);
	XMFLOAT3 GetPosition();
	XMMATRIX GetPositionMatrix();
	void GetRotation(float& x, float& y, float& z);
	XMFLOAT3 GetRotation();
	XMMATRIX GetRotationMatrix();
	void GetRotation(float& y);
	float GetRotationY();
	float GetRotationYRaid();

	void GetScale(float& x, float& y, float& z);
	XMFLOAT3 GetScale();
	XMMATRIX GetScaleMatrix();

	XMMATRIX GetWorldMatrix();

	void MoveForward(bool keydown);
	void MoveBackward(bool keydown);
	void MoveUpward(bool keydown);
	void MoveDownward(bool keydown);
	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);
	double TurnRight(double speed);
	void LookUpward(bool keydown);
	void LookDownward(bool keydown);


	void Orbit(bool keydown, bool isleft, XMFLOAT3 targetpsotion);


	bool MoveTowardsPoint(float, float, float);
	bool MoveTowardsPoint(XMFLOAT3 targetpsotion);


private:
	Positon positon;
	Vector3 rotation;
	Vector3 scale;
	

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;

	Transform* parent = nullptr;
	Transform* last = nullptr;
	Transform* next = nullptr;
	Transform* firstchild = nullptr;
	int childcount = 0;

};
