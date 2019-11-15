#pragma once
#include<vector>
#include"Component.h"
#include<typeinfo>
class GameObject
{
public:
	GameObject();
	~GameObject();

	std::vector<Component*> components;

	template<class T>
	T* AddComponent() {
		T* result= nullptr;
		components.push_back(new T(this));
		result = dynamic_cast<T*>(components.back());
		return result;
	}


};

