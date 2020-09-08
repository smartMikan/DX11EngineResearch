#pragma once

//#include <memory>
//#include <intsafe.h>
//#include <string>

class Component
{
public:
	
	Component(class GameObject* owner, int updateOrder = 100);
	virtual ~Component();


	Component& operator = (const Component& rhs) {
		
		return *this;
	}
	
	virtual void Frame(float deltatime);
	int GetUpdateOrder() const { return m_UpdateOrder; }


protected:

	class GameObject* m_Owner;
	int m_UpdateOrder;

	//std::string uuid;
	//std::string m_name;
};
