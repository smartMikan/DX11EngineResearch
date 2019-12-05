#pragma once
#ifndef _SKELETAL_CHARACTER_SHADER_CLASS_H
#define _SKELETAL_CHARACTER_SHADER_CLASS_H

#include"Macro.h"

#include<d3d11.h>
//#include<xnamath.h>
//#include<D3DX11.h> //含编译Shader程序的函数
#include<d3dcompiler.h>
#include<fstream>
#include"CommonVertexFormat.h"
#include<directxmath.h>
using namespace DirectX;

class SkeletalCharacterShaderClass
{

private:
	//变换矩阵常量缓存结构体
	struct CBMatrix
	{
		XMMATRIX mWorldMatrix;
		XMMATRIX mViewMatrix;
		XMMATRIX mProjMatrix;
		XMMATRIX mWorldInvTranposeMatirx;
	};

	struct CBMaterial
	{
		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular; // w = SpecPower
	};

	//灯光常量缓存结构体,注意类变量用XMFLOAT而不是XMVECTOR,16个字节的倍数
	struct CBLight   //传到shader里的常量缓存对应位置的值相对应
	{
		XMFLOAT4 AmbientLight;
		XMFLOAT4 DiffuseLight;
		XMFLOAT3 LightDireciton;
		float pad; //填充的系数
	};

	struct CBSkeletal
	{
		XMMATRIX BoneTransform[80];
	};


	struct CBCamera
	{
		XMFLOAT3 CameraPos;
		float pad1;
	};

private:
		ID3D11VertexShader* md3dVertexShader;
		ID3D11PixelShader* md3dPixelShader;
		ID3D11InputLayout* md3dInputLayout; //这与VertexShader相关联,因此要放在ColorShaderClass里,而不是D3DClass
		ID3D11Buffer* mCBMatrixBuffer; //变换矩阵(常量)缓存,顶点索引也是用这个类型
		ID3D11Buffer* mCBLightBuffer;  //灯光常量缓存
		ID3D11Buffer* mCBBoneTransformBuffer; //骨头变换数组常量缓存
		ID3D11Buffer* mCBMaterialBuffer; //材质常量缓存
		ID3D11Buffer* mCBCameraBuffer; //相机常量缓存
		ID3D11SamplerState *mSamplerState; //采样状态(采样方式)

private:

	//初始化Shader,用于创建InputLayout,VertexShader,PixelShader,常量缓存
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);

	//释放Shader
	bool ShutdownShader();

	//输出Shader编译文件的错误信息
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	//返回一个矩阵的逆矩阵的转置
	XMMATRIX GetInvenseTranspose(CXMMATRIX ma);

	bool SetShaderParameter(ID3D11DeviceContext*, UINT BoneNums, CXMMATRIX WorldMatrix, CXMMATRIX ViewMatrix, CXMMATRIX ProjMatrix, ID3D11ShaderResourceView* DiffuseMap,
		ID3D11ShaderResourceView* NormalMap, XMFLOAT4 AmbientLight, XMFLOAT4 diffuseLight, XMFLOAT3 LightDirection, XMFLOAT3 CameraPos, XMFLOAT4X4* BoneTransforms, Material mat);
	
	void RenderShader(ID3D11DeviceContext*);

public:
	SkeletalCharacterShaderClass();
	SkeletalCharacterShaderClass(const SkeletalCharacterShaderClass&);
	~SkeletalCharacterShaderClass();

public:
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, UINT BoneNums, CXMMATRIX WorldMatrix, CXMMATRIX ViewMatrix, CXMMATRIX ProjMatrix,  ID3D11ShaderResourceView* DiffuseMap,
		ID3D11ShaderResourceView* NormalMap, XMFLOAT4 AmbientLight, XMFLOAT4 diffuseLight, XMFLOAT3 LightDirection, XMFLOAT3 CameraPos, XMFLOAT4X4* BoneTransforms, Material mat);
	//
};

#endif // !_SKELETAL_CHARACTER_H
