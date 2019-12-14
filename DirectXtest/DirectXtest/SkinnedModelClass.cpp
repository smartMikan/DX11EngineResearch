#include"SkinnedModelClass.h"


SkinnedModelClass::SkinnedModelClass(ID3D11Device* device, const string& modelFileName, wstring texturePath)
{
	bool result;
	vector<M3dMaterial> mats;
	M3dLoaderClass m3dLoader;
	m3dLoader.LoadM3d(modelFileName, Vertices, Indices, Subsets, mats,SkinnedData);
	
	result=ModelMesh.SetVertices(device,&Vertices[0],Vertices.size());
	if (!result)
	{
		MessageBoxW(NULL, L"ModelMesh.SetVertices failure", NULL, MB_OK);
	}
	result=ModelMesh.SetIndices(device,&Indices[0],Indices.size());
	if (!result)
	{
		MessageBoxW(NULL, L"ModelMesh.SetIndices failure", NULL, MB_OK);
	}

	ModelMesh.SetSubsetTable(Subsets);

	SubsetCount = mats.size();

	for (UINT i = 0; i < SubsetCount; ++i)
	{
		Mat.push_back(mats[i].Mat);

		ID3D11ShaderResourceView* diffuseMapSRV = CreateTexture(device, texturePath + mats[i].DiffuseMapName);
		DiffuseMapSRV.push_back(diffuseMapSRV);

		ID3D11ShaderResourceView* normalMapSRV= CreateTexture(device, texturePath + mats[i].NormalMapName);
		NormalMapSRV.push_back(normalMapSRV);
	}
}


SkinnedModelClass::~SkinnedModelClass()
{

}

ID3D11ShaderResourceView* SkinnedModelClass::CreateTexture(ID3D11Device* device, std::wstring FileName)
{
	ID3D11ShaderResourceView* SRV = NULL;

	//if (FAILED(D3DX11CreateShaderResourceViewFromFile(md3dDevice, FileName.c_str(), 0, 0, &SRV, 0)))
	if (FAILED(CreateDDSTextureFromFile(device, FileName.c_str(), NULL, &SRV)))
	{
		MessageBoxW(NULL, L"D3DX11CreateShaderResourceViewFromFile failure", L"ERROR", MB_OK);
	}


	return SRV;
}

void SkinnedModelInstance::Update(float dt)
{
	TimePos += dt;
	Model->SkinnedData.GetFinalTransforms(ClipName, TimePos, FinalTransforms);

	
	if (TimePos > Model->SkinnedData.GetClipEndTime(ClipName))
		TimePos = 0.0f;
	if (TimePos < 0.0f) {
		TimePos = Model->SkinnedData.GetClipEndTime(ClipName);
	}
}

void SkinnedModelInstance::DeUpdate(float dt) {
	if (TimePos == 0.0f) {
		return;
	}
	if (TimePos > Model->SkinnedData.GetClipEndTime(ClipName) / 2) {

		TimePos += dt;
	}
	else
	{
		TimePos -= dt;
	}
	if (TimePos > Model->SkinnedData.GetClipEndTime(ClipName)) {

		TimePos = 0.0f;
	}
	if (TimePos < 0.0f) {
		TimePos = Model->SkinnedData.GetClipEndTime(ClipName);
	}

}



