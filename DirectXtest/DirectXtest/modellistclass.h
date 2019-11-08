#pragma once


#include <directxmath.h>
#include <stdlib.h>
#include <time.h>
using namespace DirectX;


class ModelListClass
{
private:
	struct ModelInfoType {
		XMFLOAT4 color;
		float positionX, positionY, positionZ;
	};


public:
	ModelListClass();
	ModelListClass(const ModelListClass& other);
	~ModelListClass();

	bool Initialize(int numModels);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, XMFLOAT4&);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};

