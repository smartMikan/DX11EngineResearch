#pragma once
#include <DirectXMath.h>

struct CB_VS_MatrixBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct CB_VS_MatrixBuffer_2d
{
	DirectX::XMMATRIX wvpMatrix;
};


struct CB_PS_ShadowMatrix
{
	DirectX::XMMATRIX shadowMatrix;
};

struct CB_PS_Material
{
	DirectX::XMFLOAT4 GlobalAmbient;

	DirectX::XMFLOAT4 AmbientColor;

	DirectX::XMFLOAT4 EmissiveColor;

	DirectX::XMFLOAT4 DiffuseColor;

	DirectX::XMFLOAT4 SpecularColor;

	DirectX::XMFLOAT4 Reflectance;

	float Opacity;				//4
	float SpecularPower;		//4
	float IndexOfRefraction;	//4
	int HasAmbientTexture;		//4

	int HasEmissiveTexture;		//4
	int HasDiffuseTexture;		//4
	int HasSpecularTexture;		//4
	int HasSpecularPowerTexture;//4

	int HasNormalTexture;		//4
	int HasBumpTexture;			//4
	int HasOpacityTexture;		//4
	float BumpIntensity;		//4
	
	float SpecularScale;		//4
	float AlphaThreshold;		//4
	float padding[2];			//8

};

struct CB_PS_LightBuffer
{
	DirectX::XMFLOAT3 ambientLightColor; //12
	float ambientLightStrength;			 //4

	DirectX::XMFLOAT3 dynamicLightColor; //12
	float dynamicLightStrength;			//4

	DirectX::XMFLOAT3 lightPosition; //12
	float dynamicLightAttenuation_a; //4


	DirectX::XMFLOAT3 lightDirection; //12
	float dynamicLightAttenuation_b; //4
	float dynamicLightAttenuation_c; //4

	int LightType;				 //4
	float padding[3];				 //4
};


struct CB_PS_CameraBuffer
{
	DirectX::XMFLOAT3 cameraPosition; //12
	float padding; //4
};

static constexpr size_t MAX_BONES = 256;

struct ConstantBuffer_Bones
{
	DirectX::XMMATRIX bone_Transforms[MAX_BONES];
};

struct ConstantBuffer_BakedBones
{
	DirectX::XMMATRIX bone_Transforms_Prev[MAX_BONES];
};



//struct CB_VS_SkinBoneBumpMapMatrixBuffer
//{
//	DirectX::XMMATRIX world;
//	DirectX::XMMATRIX view;
//	DirectX::XMMATRIX projection;
//	DirectX::XMMATRIX worldInvTranspose;
//};
//
//struct CB_VS_SkinBoneBumpMapMatrixTransformBuffer
//{
//	DirectX::XMMATRIX* boneTranforms;
//};
//
//
//struct CB_VS_Light_CameraBuffer
//{
//	DirectX::XMFLOAT3 cameraPosition; //12
//	float padding; //4
//};
//
//
//struct CB_PS_pixelshader
//{
//	DirectX::XMMATRIX matrix;
//};
//
//struct CB_PS_LightBuffer
//{
//	DirectX::XMFLOAT4 ambientColor; //16
//	DirectX::XMFLOAT4 diffuseColor; //16
//	DirectX::XMFLOAT3 lightDirection; //12
//	//float padding;
//	float specularPower; //4
//	DirectX::XMFLOAT4 specularColor; //16
//};
//
//struct CB_PS_SkinBoneBumpMapMaterialBuffer
//{
//	DirectX::XMFLOAT4 ambientColor; //16
//	DirectX::XMFLOAT4 diffuseColor; //16
//	DirectX::XMFLOAT4 SpecularColor; //16
//};
//
//struct CB_PS_SkinBoneBumpMapCameraBuffer
//{
//	DirectX::XMFLOAT3 cameraPosition; //12
//	float padding; //4
//};
//
//struct CB_PS_SkinBoneBumpMapLightBuffer
//{
//	DirectX::XMFLOAT4 ambientColor; //16
//	DirectX::XMFLOAT4 diffuseColor; //16
//	DirectX::XMFLOAT3 lightDirection; //12
//	float padding;
//};