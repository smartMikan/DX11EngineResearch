#pragma once

//ModelListClass is a new class for maintaining information about all the models in the scene. 
//For this tutorial it only maintains the size and color of the sphere models since we only have one model type. 
//This class can be expanded to maintain all the different types of models in the scene and indexes to their ModelClass but I am keeping this tutorial simple for now.

#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_

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

#endif