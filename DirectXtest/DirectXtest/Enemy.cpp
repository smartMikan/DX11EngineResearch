#include "Enemy.h"


Enemy::Enemy(GameObjectClass* obj)
{
	Instantiate(obj);
}

Enemy::Enemy(GameObjectClass* obj, int instID)
{
	Instantiate(obj,instID);
}

Enemy::Enemy(GameObjectClass* obj, const float* pos)
{
	Instantiate(obj);
	SetPosition(pos);
}

Enemy::Enemy(GameObjectClass* obj, const float* pos, int instID)
{
	Instantiate(obj,instID);
	SetPosition(pos);
}

Enemy::Enemy(const Enemy& rhs)
{
	m_object = rhs.m_object;
	m_Position[0] = rhs.m_Position[0];
	m_Position[1] = rhs.m_Position[1];
	m_Position[2] = rhs.m_Position[2];
	Animnum = rhs.Animnum;
	state = rhs.state;
	m_InstanceID = rhs.m_InstanceID;
}



Enemy::~Enemy()
{
	m_object = nullptr;
}

void Enemy::Instantiate(GameObjectClass* obj)
{
	m_object = obj;
	state = idle;
	Animnum = state;
	targetPosition[0] = m_Position[0];
	targetPosition[1] = m_Position[1];
	targetPosition[2] = m_Position[2];
}

void Enemy::Instantiate(GameObjectClass* obj, int instanceID)
{
	Instantiate(obj);
	m_InstanceID = instanceID;
}

void Enemy::ShutDown()
{
	m_object = nullptr;
}

void Enemy::SetPosition(const float* pos)
{
	m_Position[0] = pos[0];
	m_Position[1] = pos[1];
	m_Position[2] = pos[2];

}

void Enemy::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix)
{
	m_object->m_Position.SetPosition(m_Position[0], m_Position[1], m_Position[2]);
	m_object->SwitchAnim(Animnum);
	m_object->Render(viewMatrix, projMatrix);
}

void Enemy::Frame(float frametime)
{
	MoveToRandomPoint(targetPosition, frametime);
}

void Enemy::AddRandomOffset()
{
	srand(time(NULL));
	targetPosition[0] = m_Position[0] + ((rand() % 20) - 10);
	targetPosition[2] = m_Position[2] + ((rand() % 20) - 10);
}

void Enemy::MoveToRandomPoint(float* target, float frametime)
{
	if (MoveTowardsPoint(targetPosition[0], m_Position[1], targetPosition[2], frametime))
	{
		AddRandomOffset();
	}
}



