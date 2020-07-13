#pragma once
#include "./Engine/GameObjectClass.h"
#include "Utility/RandomHelper.h"

//const int MAX_ENEMY = 100;
const bool DRAW_BAKED = true;


enum EnemyState
{
	idle = 4,
	move = 5,
	attack = 6,
};

typedef struct EnemyInstancesData
{
	float m_Position[3] = { 0.0f,0.0f,0.0f };
	EnemyState m_state = idle;
	float m_TimePos = 0.0f;

	//temp ai
	float targetPosition[3] = { 0.0f,0.0f,0.0f };
	//
	int m_InstanceID;
	bool ifRender = true;
	double rendertime = 0.0;
	int m_Animnum = 0;


	EnemyInstancesData(int instanceID)
	{
		m_state = idle;
		targetPosition[0] = m_Position[0];
		targetPosition[1] = m_Position[1];
		targetPosition[2] = m_Position[2];
		m_InstanceID = instanceID;
	}

	EnemyInstancesData(int instanceID, float pos[3])
	{
		m_state = idle;
		m_Position[0] = pos[0];
		m_Position[1] = pos[1];
		m_Position[2] = pos[2];

		targetPosition[0] = pos[0];
		targetPosition[1] = pos[1];
		targetPosition[2] = pos[2];
		m_InstanceID = instanceID;
	}

	EnemyInstancesData(int instanceID, float pos[3], EnemyState state = idle)
	{

		m_state = state;

		m_Position[0] = pos[0];
		m_Position[1] = pos[1];
		m_Position[2] = pos[2];
		targetPosition[0] = pos[0];
		targetPosition[1] = pos[1];
		targetPosition[2] = pos[2];
		m_InstanceID = instanceID;
	}

	EnemyInstancesData(int instanceID, float pos[3], EnemyState state = idle, float timepos = 0.0f)
	{

		m_state = state;

		m_Position[0] = pos[0];
		m_Position[1] = pos[1];
		m_Position[2] = pos[2];
		targetPosition[0] = pos[0];
		targetPosition[1] = pos[1];
		targetPosition[2] = pos[2];
		m_InstanceID = instanceID;
		m_TimePos = timepos;
	}

	EnemyInstancesData(int instanceID, float pos[3], EnemyState state = idle, float timepos = 0.0f, int animnum = 0)
	{

		m_state = state;

		m_Position[0] = pos[0];
		m_Position[1] = pos[1];
		m_Position[2] = pos[2];
		targetPosition[0] = pos[0];
		targetPosition[1] = pos[1];
		targetPosition[2] = pos[2];
		m_InstanceID = instanceID;
		m_TimePos = timepos;
		m_Animnum = animnum;
	}

	void UpdateTime(float time) {
		m_TimePos += time;
	}

	void SwitchState(EnemyState state) {
		m_state = state;
	}

	bool MoveTowardsPoint(float x, float y, float z, float frametime)
	{
		bool reached = false;
		float delta_x = x - m_Position[0];
		float delta_y = y - m_Position[1];
		float delta_z = z - m_Position[2];


		XMVECTOR dir = XMLoadFloat3(&XMFLOAT3(delta_x, delta_y, delta_z));
		XMFLOAT3 dirData;
		XMStoreFloat3(&dirData, XMVector3LengthEst(dir));

		if (dirData.x <= 1.0f) {
			reached = true;
			return reached;
		}

		XMStoreFloat3(&dirData, XMVector3Normalize(dir));

		m_Position[0] += frametime * dirData.x * 0.015f;
		m_Position[1] += frametime * dirData.y * 0.015f;
		m_Position[2] += frametime * dirData.z * 0.015f;

		return reached;
	}

	bool MoveTowardsPoint(XMFLOAT3 targetpsotion, float frametime)
	{
		return MoveTowardsPoint(targetpsotion.x, targetpsotion.y, targetpsotion.z, frametime);
	}


	///temp ai
	///
	void AddRandomOffset()
	{
		targetPosition[0] = m_Position[0] + (Utility::GetRandom(-20, 20));
		targetPosition[2] = m_Position[2] + (Utility::GetRandom(-20, 20));
	}

	void MoveToRandomPoint(float frametime)
	{
		if (MoveTowardsPoint(targetPosition[0], m_Position[1], targetPosition[2], frametime))
		{
			AddRandomOffset();
		}
	}

	

}EnemyInst;


class Enemy
{
public:
	Enemy(GameObjectClass* obj);
	Enemy(const Enemy& rhs);
	~Enemy();

	//
	EnemyInstancesData Instantiate(float pos[3], EnemyState state = idle, float timePos = 0.0f);

	bool RemoveFromRender(int enemyID);

	int GetRenderedEnemyCounts() { return RenderedEnemyCount; }
	int GetAllEnemyCounts() { return EnemyCount; }
	int GetHideEnemyCounts() { return EnemyCount - RenderedEnemyCount; }

	//ฝKมห
	void ShutDown();

	//
	void SetPosition(int enemyID, const float pos[3]);


	double Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

	void Frame(float frametime);

	double GetEnemyRenderTime(int enemyID);

	void SetInstanceAnim(int instID, int animnum);

private:
	Enemy(const Enemy&& rhs) {}
	int EnemyCount = 0, RenderedEnemyCount = 0;

	std::vector<EnemyInstancesData> m_instances;
	GameObjectClass* m_instobj;

};

