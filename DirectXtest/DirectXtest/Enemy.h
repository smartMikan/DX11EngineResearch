#pragma once
#include "./Engine/GameObjectClass.h"

enum EnemyState
{
	idle = 4,
	move = 5,
	attack = 6,
};




class Enemy
{
public:
	Enemy(GameObjectClass* obj);
	Enemy(GameObjectClass* obj,int instID);
	Enemy(GameObjectClass* obj, const float* pos);
	Enemy(GameObjectClass* obj, const float* pos,int instID);
	Enemy(const Enemy& rhs);
	~Enemy();

	void Instantiate(GameObjectClass* obj);
	void Instantiate(GameObjectClass* obj,int instanceID);

	void ShutDown();


	void SetPosition(const float* pos);


	void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

	void Frame(float frametime);

	bool MoveTowardsPoint(float x, float y, float z, float frametime)
	{
		bool reached;
		float delta_x = x - m_Position[0];
		float delta_y = y - m_Position[1];
		float delta_z = z - m_Position[2];


		XMVECTOR dir = XMLoadFloat3(&XMFLOAT3(delta_x, delta_y, delta_z));
		XMFLOAT3 dirData;
		XMStoreFloat3(&dirData, XMVector3LengthEst(dir));

		if (dirData.x <= 1.0f) {

			return true;
		}

		XMStoreFloat3(&dirData, XMVector3Normalize(dir));

		m_Position[0] += frametime * dirData.x * 0.015f;
		m_Position[1] += frametime * dirData.y * 0.015f;
		m_Position[2] += frametime * dirData.z * 0.015f;

		return false;
	}

	bool MoveTowardsPoint(XMFLOAT3 targetpsotion, float frametime)
	{
		return MoveTowardsPoint(targetpsotion.x, targetpsotion.y, targetpsotion.z, frametime);
	}

	void AddRandomOffset();

	void MoveToRandomPoint(float* target,float frametime);

	void SwitchState(EnemyState state) {
		Animnum = state;
	}

	float m_Position[3] = { 12.0f,0.1f,33.0f };
	int Animnum = 0;
	EnemyState state = idle;
	GameObjectClass* m_object;
	
	float targetPosition[3] = { 0.0f,0.0f,0.0f };
	int m_InstanceID;

private:
	Enemy(const Enemy&& rhs){}
};

