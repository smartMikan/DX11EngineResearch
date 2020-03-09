#pragma once


template<class T>
class Componet
{
public:
	 Componet();
	~Componet();

	Componet<T>& operator = (const Componet<T>& a) {
		this->Value = a.Value;
		return *this;
	}
	
	T Value;

};

