#include"SkeletalCharacterShaderClass.h"

SkeletalCharacterShaderClass::SkeletalCharacterShaderClass()
{
	md3dVertexShader = 0;
	md3dPixelShader = 0;
	md3dInputLayout = 0;
	mCBMatrixBuffer = 0;
	mCBLightBuffer = 0;	
	mCBMaterialBuffer = 0L;
	mCBCameraBuffer = 0;
}


SkeletalCharacterShaderClass::SkeletalCharacterShaderClass(const SkeletalCharacterShaderClass&)
{

}

SkeletalCharacterShaderClass::~SkeletalCharacterShaderClass()
{

}


bool SkeletalCharacterShaderClass::Initialize(ID3D11Device* d3dDevice, HWND hwnd, ID3D11DeviceContext* deviceContext)
{
	bool result;
	result = InitializeShader(d3dDevice, hwnd, L"Shader/SkeletalCharacterShader.fx", L"Shader/SkeletalCharacterShader.fx");
	if (!result)
		return false;

	return true;
}

void SkeletalCharacterShaderClass::Shutdown()
{
	ShutdownShader();
}



bool SkeletalCharacterShaderClass::Render(ID3D11DeviceContext* d3dDeviceContext, UINT BoneNums, CXMMATRIX WorldMatrix, CXMMATRIX ViewMatrix, CXMMATRIX ProjMatrix,  ID3D11ShaderResourceView* DiffuseMap,
	ID3D11ShaderResourceView* NormalMap, XMFLOAT4 AmbientLight, XMFLOAT4 diffuseLight, XMFLOAT3 LightDirection, XMFLOAT3 CameraPos, XMFLOAT4X4* BoneTransforms, M3d::Material mat)
{
	bool result;
	//设置Shader常量缓存和纹理资源
	result = SetShaderParameter(d3dDeviceContext, BoneNums, WorldMatrix, ViewMatrix, ProjMatrix,DiffuseMap,NormalMap,AmbientLight,diffuseLight,LightDirection, CameraPos, BoneTransforms, mat);
	if (!result)
		return false;

	//设置VertexShader PixelShader InputLayout SamplerState
	RenderShader(d3dDeviceContext);

	return true;
}

bool SkeletalCharacterShaderClass::InitializeShader(ID3D11Device* d3dDevice, HWND hwnd, const WCHAR* VSFileName, const WCHAR* PSFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* VertexShaderBuffer;
	ID3D10Blob* PixelShaderBuffer;

	//第一,初始化参数
	errorMessage = NULL;
	VertexShaderBuffer = NULL;
	PixelShaderBuffer = NULL;

	//第二,编译VertexShader代码,并创建VertexShader
	result = D3DCompileFromFile(VSFileName, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &VertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//存在错误信息
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, VSFileName);
		}
		//不存在错误信息,也就是没有找到Shader文件
		else
		{
			MessageBoxW(hwnd, L"can not find VS file", L"error", MB_OK);
		}
	}



	//第三,编译PixelShader,并创建PixelShader
	result = D3DCompileFromFile(PSFileName, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//存在错误信息
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, PSFileName);
		}
		//不存在错误信息,也就是没有找到Shader文件
		else
		{
			MessageBoxW(hwnd, L"can not find PS file", L"error", MB_OK);
		}
	}

	result = d3dDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &md3dVertexShader);
	if (FAILED(result))
	{
		return false;
	}
	result = d3dDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &md3dPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//第四,填充输入布局形容结构体,创建输入布局
	D3D11_INPUT_ELEMENT_DESC VertexInputLayout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, // 96位即12个字节
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "WEIGHTS",0,DXGI_FORMAT_R32G32B32_FLOAT,0,48,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,60,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	unsigned int numElements = sizeof(VertexInputLayout) / sizeof(VertexInputLayout[0]);         //布局数量

	HR(d3dDevice->CreateInputLayout(VertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &md3dInputLayout));

	//第五,释放VertexShaderBuffer和PixelShaderBuffer
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;

	//第六,设置(变换矩阵常量)缓存形容结构体,并创建矩阵常量缓存
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDesc.ByteWidth = sizeof(CBMatrix);   //结构体大小,必须为16字节倍数
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = 0;

	HR(d3dDevice->CreateBuffer(&matrixBufferDesc, NULL, &mCBMatrixBuffer));


	//第七,设置（灯光常量缓存）缓存形容结构体，并创建灯光常量缓存,复制粘贴的时候别把LightBufferDesc和matrixBufferDesc搞错了

	D3D11_BUFFER_DESC LightBufferDesc;
	ZeroMemory(&LightBufferDesc, sizeof(LightBufferDesc));
	LightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	LightBufferDesc.ByteWidth = sizeof(CBLight);   //结构体大小,必须为16字节倍数
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightBufferDesc.CPUAccessFlags = 0;

	HR(d3dDevice->CreateBuffer(&LightBufferDesc, NULL, &mCBLightBuffer));


	//第八,设置（骨头变换矩阵数组）缓存形容结构体，并创建灯光常量缓存

	D3D11_BUFFER_DESC BoneTransformBufferDesc;
	ZeroMemory(&BoneTransformBufferDesc, sizeof(BoneTransformBufferDesc));
	BoneTransformBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BoneTransformBufferDesc.ByteWidth = sizeof(CBSkeletal);   //结构体大小,必须为16字节倍数
	BoneTransformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BoneTransformBufferDesc.CPUAccessFlags = 0;

	HR(d3dDevice->CreateBuffer(&BoneTransformBufferDesc, NULL, &mCBBoneTransformBuffer));


	//第九,设置材质常量缓存结构体，并创建材质常量缓存
	D3D11_BUFFER_DESC MaterialBufferDesc;
	ZeroMemory(&MaterialBufferDesc, sizeof(MaterialBufferDesc));
	MaterialBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	MaterialBufferDesc.ByteWidth = sizeof(CBMaterial);   //结构体大小,必须为16字节倍数
	MaterialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MaterialBufferDesc.CPUAccessFlags = 0;
	HR(d3dDevice->CreateBuffer(&MaterialBufferDesc, NULL, &mCBMaterialBuffer));

	//第十，设置相机常量缓存结构体，并创建相机常量缓存
	D3D11_BUFFER_DESC CameraBufferDesc;
	ZeroMemory(&CameraBufferDesc, sizeof(CameraBufferDesc));
	CameraBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	CameraBufferDesc.ByteWidth = sizeof(CBCamera);   //结构体大小,必须为16字节倍数
	CameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CameraBufferDesc.CPUAccessFlags = 0;
	HR(d3dDevice->CreateBuffer(&CameraBufferDesc, NULL, &mCBCameraBuffer));


	//第十一,填充采样形容结构体,并且创建采样状态
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  //都是线性插值(三种方式,点过滤,线性过滤,异性过滤)
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(d3dDevice->CreateSamplerState(&samplerDesc, &mSamplerState));

	return true;
}

bool SkeletalCharacterShaderClass::ShutdownShader()
{
	ReleaseCOM(mCBCameraBuffer);
	ReleaseCOM(mCBMaterialBuffer);
	ReleaseCOM(mCBBoneTransformBuffer);
	ReleaseCOM(mCBMatrixBuffer);
	ReleaseCOM(mCBLightBuffer);
	ReleaseCOM(md3dInputLayout);
	ReleaseCOM(md3dPixelShader);
	ReleaseCOM(md3dVertexShader);
	return true;
}

void SkeletalCharacterShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// 获取指向错误信息文本的指针
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// 获取错误信息文本的长度
	bufferSize = errorMessage->GetBufferSize();

	// 创建一个txt,用于写入错误信息
	fout.open("shader-error.txt");

	//想txt文件写入错误信息
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// 关闭文件
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	//弹出提醒的小窗口
	MessageBoxW(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

}

//, FXMVECTOR AmbientLight, FXMVECTOR diffuseLight, FXMVECTOR LightDirection, CXMVECTOR CameraPos, XMFLOAT4X4* BoneTransforms, Material mat
bool SkeletalCharacterShaderClass::SetShaderParameter(ID3D11DeviceContext* d3dDeviceContext, UINT BoneNums, CXMMATRIX WorldMatrix, CXMMATRIX ViewMatrix, CXMMATRIX ProjMatrix, ID3D11ShaderResourceView* DiffuseMap,
	ID3D11ShaderResourceView* NormalMap, XMFLOAT4 AmbientLight, XMFLOAT4 diffuseLight, XMFLOAT3 LightDirection, XMFLOAT3 CameraPos, XMFLOAT4X4* BoneTransforms, M3d::Material mat)
{
	
	//第一，更新变换矩阵常量缓存的值
	//将矩阵转置,在传入常量缓存前进行转置,因为GPU对矩阵数据会自动进行一次转置
	CBMatrix cbMatrix;
	XMMATRIX worldMa = XMMatrixTranspose(WorldMatrix);
	XMMATRIX viewMa= XMMatrixTranspose(ViewMatrix);
	XMMATRIX projMa = XMMatrixTranspose(ProjMatrix);
	XMMATRIX worldInvTranspose = XMMatrixTranspose(GetInvenseTranspose(WorldMatrix));  //世界矩阵的逆矩阵的转置
	cbMatrix.mWorldMatrix = worldMa;
	cbMatrix.mViewMatrix = viewMa;
	cbMatrix.mProjMatrix = projMa;
	cbMatrix.mWorldInvTranposeMatirx = worldInvTranspose;
	d3dDeviceContext->UpdateSubresource(mCBMatrixBuffer, 0, NULL, &cbMatrix, 0, 0);


	//第二，更新灯光常量缓存的值
	//设置在顶点缓存中常量缓存的位置,注册号
    CBLight cbLight;
	cbLight.AmbientLight = AmbientLight;
	cbLight.DiffuseLight = diffuseLight;
	cbLight.LightDireciton = LightDirection;
	//XMStoreFloat4(&cbLight.AmbientLight, AmbientLight);
	//XMStoreFloat4(&cbLight.DiffuseLight, diffuseLight);
	//XMStoreFloat3(&cbLight.LightDireciton, LightDirection);
	cbLight.pad = 0.0f;
	d3dDeviceContext->UpdateSubresource(mCBLightBuffer, 0, NULL, &cbLight, 0, 0);

	//第三,更新骨头变换常量缓存的值
	CBSkeletal cbBoneTransform;
	for (UINT i = 0; i < BoneNums; ++i)
	{
		cbBoneTransform.BoneTransform[i] = XMMatrixTranspose(XMLoadFloat4x4(&BoneTransforms[i]));
	}
	d3dDeviceContext->UpdateSubresource(mCBBoneTransformBuffer, 0, NULL, &cbBoneTransform, 0, 0);

	//第四，更新材质常量缓存
	CBMaterial cbMaterial;
	cbMaterial.Ambient = mat.Ambient;
	cbMaterial.Diffuse = mat.Diffuse;
	cbMaterial.Specular = mat.Specular;
	d3dDeviceContext->UpdateSubresource(mCBMaterialBuffer, 0, NULL, &cbMaterial, 0, 0);

	//第五,更新相机常量缓存
	CBCamera cbCamera;
	cbCamera.CameraPos = CameraPos;
	//XMStoreFloat3(&cbCamera.CameraPos, CameraPos);
	d3dDeviceContext->UpdateSubresource(mCBCameraBuffer, 0, NULL, &cbCamera, 0, 0);

	//第六,设置在VertexShader的常量缓存的值(带着更新的值)
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &mCBMatrixBuffer);
	d3dDeviceContext->VSSetConstantBuffers(2, 1, &mCBBoneTransformBuffer);

	//第七,设置在PixelShader的常量缓存的值
	d3dDeviceContext->PSSetConstantBuffers(1, 1, &mCBLightBuffer);
	d3dDeviceContext->PSSetConstantBuffers(3, 1, &mCBMaterialBuffer);
	d3dDeviceContext->PSSetConstantBuffers(4, 1, &mCBCameraBuffer);

	//第八,设置在PixelShader的纹理资源
	d3dDeviceContext->PSSetShaderResources(0, 1, &DiffuseMap);
	d3dDeviceContext->PSSetShaderResources(1, 1, &NormalMap);

	return true;
}

void SkeletalCharacterShaderClass::RenderShader(ID3D11DeviceContext* deviceContext)
{
	//设置顶点输入布局
	deviceContext->IASetInputLayout(md3dInputLayout);

	//设置VertexShader和PixelShader
	deviceContext->VSSetShader(md3dVertexShader, NULL, 0);
	deviceContext->PSSetShader(md3dPixelShader, NULL, 0);

	//设置采样状态
	deviceContext->PSSetSamplers(0, 1, &mSamplerState);  //S0注册 对应0

}


//返回一个矩阵的逆矩阵的转置
XMMATRIX SkeletalCharacterShaderClass::GetInvenseTranspose(CXMMATRIX ma)
{
	XMMATRIX A = ma;

	//将矩阵A的第四行置零,因为向量是无法进行平移的
	A.r[3] = { 0.0f,0.0f,0.0f,1.0f };

	XMVECTOR det = XMMatrixDeterminant(A);
	XMMATRIX MaInvense = XMMatrixInverse(&det, A);
	return XMMatrixTranspose(MaInvense);
}