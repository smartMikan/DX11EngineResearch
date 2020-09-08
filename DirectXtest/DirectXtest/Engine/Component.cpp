#include "Component.h"

#include"GameObjectClass.h"


Component::Component(GameObject* owner, int updateOrder)
	:m_Owner(owner)
	,m_UpdateOrder(updateOrder)
{
	m_Owner->AddComponent(this);

}

Component::~Component()
{
	m_Owner->RemoveComponent(this);
}

void Component::Frame(float deltatime)
{

}
