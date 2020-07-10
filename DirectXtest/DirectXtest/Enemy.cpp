#include "Enemy.h"

Enemy::Enemy(GameObjectClass* obj)
{
	m_instobj = obj;
}



Enemy::Enemy(const Enemy& rhs)
{
	m_instances = rhs.m_instances;
	m_instobj = rhs.m_instobj;
}



Enemy::~Enemy()
{
	ShutDown();
}

EnemyInstancesData Enemy::Instantiate(float pos[3], EnemyState state, float m_TimePos)
{
	float pos1[3] = { pos[0],pos[1],pos[2] };
	int instID = m_instances.size();
	EnemyCount++;
	RenderedEnemyCount++;
	return m_instances.emplace_back(EnemyInstancesData(instID, pos1, state, m_TimePos));
}

bool Enemy::RemoveFromRender(int enemyID)
{
	if (enemyID < m_instances.size()) {

		m_instances[enemyID].ifRender = false;
		return true;
	}
	return false;
}




void Enemy::ShutDown()
{
	m_instobj = nullptr;

}


void Enemy::SetPosition(int enemyID, const float pos[3])
{
	m_instances[enemyID].m_Position[0] = pos[0];
	m_instances[enemyID].m_Position[1] = pos[1];
	m_instances[enemyID].m_Position[2] = pos[2];
}

double Enemy::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix)
{
	double drawAllEnemyTime = 0.0;
	
	for (int i = 0, ie = m_instances.size(); i < ie; i++)
	{
		if (m_instances[i].ifRender) {
			m_instobj->m_Position.SetPosition(m_instances[i].m_Position);
			m_instobj->SwitchAnim(m_instances[i].m_state);
			drawAllEnemyTime += m_instances[i].rendertime = m_instobj->Render(viewMatrix, projMatrix,SameAnim);
		}
	}
	return drawAllEnemyTime;
}

void Enemy::Frame(float frametime)
{
	for (int i = 0, ie = m_instances.size(); i < ie; i++)
	{
		if (m_instances[i].ifRender) {
			m_instances[i].MoveToRandomPoint(frametime);
		}
	}
}

double Enemy::GetEnemyRenderTime(int enemyID)
{
	return m_instances[enemyID].rendertime;
}



