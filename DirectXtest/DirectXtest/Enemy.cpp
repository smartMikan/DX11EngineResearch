#include "Enemy.h"
#include "Utility/RandomHelper.h"

Enemy::Enemy(GameObject* obj)
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

EnemyInstancesData Enemy::Instantiate(float pos[3], EnemyState state, float timePos)
{
	assert(EnemyCount >= RenderedEnemyCount, "RenderedEnemy bigger than EnemyCount! overhead!");
	if (EnemyCount == RenderedEnemyCount) {
		float pos1[3] = { pos[0],pos[1],pos[2] };
		int instID = EnemyCount;
		EnemyCount++;
		RenderedEnemyCount++;
		return m_instances.emplace_back(EnemyInstancesData(instID, pos1, state, timePos, instID));
	}
	else
	{

		for (int i = 0; i < EnemyCount; i++)
		{
			if (!m_instances[i].ifRender) {
				m_instances[i].ifRender = true;
				m_instances[i].m_Position[0] = pos[0];
				m_instances[i].m_Position[1] = pos[1];
				m_instances[i].m_Position[2] = pos[2];
				m_instances[i].targetPosition[0] = pos[0];
				m_instances[i].targetPosition[1] = pos[1];
				m_instances[i].targetPosition[2] = pos[2];
				m_instances[i].m_state = state;
				m_instances[i].m_TimePos = timePos;
				m_instances[i].m_Animnum = RenderedEnemyCount;
				RenderedEnemyCount++;
				break;
			}
		}
	}

}

bool Enemy::RemoveFromRender(int enemyID)
{
	if (m_instances.empty())
		return false;

	assert(enemyID < m_instances.size(), "not contain enemy ID!");

	if (RenderedEnemyCount > 0) {

		m_instances[enemyID].ifRender = false;
		RenderedEnemyCount--;
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
		if (m_instances[i].ifRender == true) {

			m_instobj->m_Transform.SetPosition(m_instances[i].m_Position);
			//m_instobj->SwitchAnim(m_instances[i].m_state);

			if (DRAW_BAKED) {
				int animnum = m_instances[i].m_Animnum % (m_instobj->GetBakedAnimCount());

				m_instances[i].rendertime = m_instobj->RenderWithBakedAnim(viewMatrix, projMatrix, animnum, m_instances[i].m_TimePos);
				drawAllEnemyTime += m_instances[i].rendertime;
			}


		}
	}
	return drawAllEnemyTime;
}

void Enemy::Frame(float frametime)
{
	m_instobj->Frame(frametime);

	for (int i = 0, ie = m_instances.size(); i < ie; i++)
	{
		if (m_instances[i].ifRender) {
			m_instances[i].MoveToRandomPoint(frametime);
		}
		m_instances[i].UpdateTime(frametime * 0.001);
	}
}

double Enemy::GetEnemyRenderTime(int enemyID)
{
	return m_instances[enemyID].rendertime;
}

void Enemy::SetInstanceAnim(int instID, int animnum)
{
	m_instances[instID].m_Animnum = animnum;
}

///temp ai
///

inline void EnemyInstancesData::AddRandomOffset()
{
	targetPosition[0] = m_Position[0] + (Utility::GetRandom(-20, 20));
	targetPosition[2] = m_Position[2] + (Utility::GetRandom(-20, 20));
}
