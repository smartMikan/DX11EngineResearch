#pragma once

#include <memory>
#include <intsafe.h>
#include <string>

enum ComponentType
{

};


template<typename T>
class Component
{
public:
	//Component<T>();
	//~Component<T>();


	//Block non-T
	Component();
	~Component();

	bool operator==(const Component& rhs) 
	{

		if (this->m_type != rhs.m_type) 
		{
			return false;
		}

		if (this->m_name != rhs.m_name) 
		{
			return false;
		}

		if (this->uuid != rhs.uuid) 
		{
			return false;
		}
		
		return true;
	}

	

	Component<T>& operator = (const Component<T>& rhs) {
		this->Value = a.Value;
		return *this;
	}
	
	T Value;
	virtual void Frame();

private:

	ComponentType m_type;
	std::string uuid;
	std::string m_name;
};

