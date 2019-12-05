#pragma once
#ifndef _SKINNED_MODEL_CLASS
#define _SKINNED_MODEL_CLASS

#include"MeshGeometryClass.h"
#include"SkinnedDataClass.h"
#include"TextureManagerClass.h"
#include"CommonVertexFormat.h"
#include"M3DLoaderClass.h"
#include<D3Dcommon.h>  
//#include<D3DX11.h>
#include <d3dcompiler.h>
class SkinnedModelClass
{

public:
	UINT SubsetCount;
	vector<Material> Mat; //材质
	vector<ID3D11ShaderResourceView*> DiffuseMapSRV; //漫反射
	vector<ID3D11ShaderResourceView*> NormalMapSRV; //法线贴图

	//骨骼顶点数据
	vector<PosNormalTexTanSkinnedVertex> Vertices;

	vector<unsigned long> Indices;

	vector<MeshGeometryClass::Subset> Subsets;


	SkinnedDataClass SkinnedData;
	MeshGeometryClass ModelMesh; 

public:
	SkinnedModelClass(ID3D11Device* device, const string& modelFileName, wstring texturePath);
	~SkinnedModelClass();

	ID3D11ShaderResourceView* CreateTexture(ID3D11Device* device, std::wstring FileName);
};

struct SkinnedModelInstance
{
	SkinnedModelClass* Model;
	float TimePos;
	std::string ClipName;
	XMFLOAT4X4 World;
	std::vector<XMFLOAT4X4> FinalTransforms;

	void Update(float dt);
};

#endif // !_SKINNED_MODEL_CLASS
