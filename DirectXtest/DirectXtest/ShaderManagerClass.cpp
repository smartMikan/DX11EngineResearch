////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "shadermanagerclass.h"


ShaderManagerClass::ShaderManagerClass()
{
	m_ColorShader = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_FontShader = 0;
	m_SkyDomeShader = 0;
	m_SkyCubeShader = 0;
	m_TerrainShader = 0;
	m_ParticleShader = 0;
}


ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass& other)
{
}


ShaderManagerClass::~ShaderManagerClass()
{
}


bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}


	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}


	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}
	
	// Create the sky dome shader object.
	m_SkyCubeShader = new SkyCubeShaderClass;
	if (!m_SkyCubeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyCubeShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	// Create the sky dome shader object.
	m_ParticleShader = new ParticleShaderClass;
	if (!m_ParticleShader)
	{
		return false;
	}


	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}


	m_SkeletalCharacterShader = new SkeletalCharacterShaderClass;
	if (!m_SkeletalCharacterShader)
	{
		return false;
	}
	result = m_SkeletalCharacterShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"failed to initialize m_SkeletalCharacterShader", NULL, MB_OK);
		return false;
	}

	return true;
}

bool ShaderManagerClass::InitializeMyShader(ID3D11Device *device, std::wstring vertexShaderCsoPath, std::wstring pixelShaderCsoPath)
{

	D3D11_INPUT_ELEMENT_DESC inputlayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0}
		/*{ "Position",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }*/
	};
	UINT elementCount = ARRAYSIZE(inputlayout);

	if (!m_vertexShader.Initialize(device, vertexShaderCsoPath, inputlayout, elementCount)) {
		return false;
	}
	

	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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

	if (!m_pixelShader.Initialize(device, pixelShaderCsoPath, &samplerDesc)) {
		return false;
	}

	//InnitiallizeMatrixBuffer
	m_MatrixBuffer.Initialize(device);

	m_Light_CameraBuffer.Initialize(device);

	m_LightBuffer.Initialize(device);


	//m_BoneMatrixBuffer.Initialize(device);
	//m_BoneTransformBuffer.Initialize(device);
	//m_BoneMaterialBuffer.Initialize(device);
	//m_BoneCameraBuffer.Initialize(device);
	//m_BoneLightBuffer.Initialize(device);

	return true;
}


void ShaderManagerClass::Shutdown()
{
	// Release the SkeletalCharacter shader object.
	if (m_SkeletalCharacterShader)
	{
		m_SkeletalCharacterShader->Shutdown();
		delete m_SkeletalCharacterShader;
		m_SkeletalCharacterShader = 0;
	}

	// Release the sky dome shader object.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}
	
	// Release the sky dome shader object.
	if (m_SkyCubeShader)
	{
		m_SkyCubeShader->Shutdown();
		delete m_SkyCubeShader;
		m_SkyCubeShader = 0;
	}

	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	
	return;
}


bool ShaderManagerClass::RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix)
{
	return m_ColorShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix);
}


bool ShaderManagerClass::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	return m_TextureShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}


//bool ShaderManagerClass::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
//	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection,
//	XMFLOAT4 diffuseColor)
//{
//
//	return false;
//}

bool ShaderManagerClass::RenderLightShader(ID3D11DeviceContext *deviceContext, int indexCount, ID3D11ShaderResourceView ** textureArray, XMMATRIX world, XMMATRIX view, XMMATRIX projection, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor)
{
	return m_LightShader->Render(deviceContext, indexCount, textureArray, world, view, projection, cameraPosition, ambientColor, diffuseColor, lightDirection,specularPower,specularColor);
}


bool ShaderManagerClass::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	return m_FontShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}



bool ShaderManagerClass::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap,
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	return m_TerrainShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, normalMap, lightDirection, diffuseColor);
}

bool ShaderManagerClass::RenderParticleShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	return m_ParticleShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManagerClass::RenderSkyDomeShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	return m_SkyDomeShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
}

bool ShaderManagerClass::RenderSkyCubeShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	return m_SkyCubeShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}
bool ShaderManagerClass::RenderSkeletalCharacterShader(ID3D11DeviceContext* d3dDeviceContext, UINT BoneNums, CXMMATRIX WorldMatrix, CXMMATRIX ViewMatrix, CXMMATRIX ProjMatrix, ID3D11ShaderResourceView* DiffuseMap,
	ID3D11ShaderResourceView* NormalMap, XMFLOAT4 AmbientLight, XMFLOAT4 diffuseLight, XMFLOAT3 LightDirection, XMFLOAT3 CameraPos, XMFLOAT4X4* BoneTransforms, Material mat)
{
	return m_SkeletalCharacterShader->Render(d3dDeviceContext, BoneNums, WorldMatrix, ViewMatrix, ProjMatrix, DiffuseMap,
		NormalMap, AmbientLight, diffuseLight, LightDirection, CameraPos, BoneTransforms, mat);
}

void ShaderManagerClass::DrawSetWithMyShader(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX* boneTransformOffSet, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, XMFLOAT4 materialAmbientColor, XMFLOAT4 materialDiffuseColor, XMFLOAT4 SpecularColor)
{
	//update ConstantBuffer

	//update ConstantBuffer
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	m_MatrixBuffer.data.world = worldMatrix;
	m_MatrixBuffer.data.view = viewMatrix;
	m_MatrixBuffer.data.projection = projectionMatrix;
	m_MatrixBuffer.ApplyChanges(deviceContext);

	// Now set the matrix constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(0, 1, m_MatrixBuffer.GetAddress());

	m_Light_CameraBuffer.data.cameraPosition = cameraPosition;
	m_Light_CameraBuffer.data.padding = 0.0f;

	m_Light_CameraBuffer.ApplyChanges(deviceContext);

	// Now set the camera constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(1, 1, m_Light_CameraBuffer.GetAddress());

	m_LightBuffer.data.ambientColor = ambientColor;
	m_LightBuffer.data.diffuseColor = diffuseColor;
	m_LightBuffer.data.lightDirection = lightDirection;
	//m_LightBuffer->data.padding = 0.0f;
	m_LightBuffer.data.specularColor = SpecularColor;
	m_LightBuffer.data.specularPower = 32.0f;

	m_LightBuffer.ApplyChanges(deviceContext);

	deviceContext->PSSetConstantBuffers(0, 1, m_LightBuffer.GetAddress());

	//Update BoneMatrixBuffer;
	//m_BoneMatrixBuffer.data.worldInvTranspose = worldMatrix;
	//worldMatrix = XMMatrixTranspose(worldMatrix);
	//viewMatrix = XMMatrixTranspose(viewMatrix);
	//projectionMatrix = XMMatrixTranspose(projectionMatrix);
	//m_BoneMatrixBuffer.data.world = worldMatrix;
	//m_BoneMatrixBuffer.data.view = viewMatrix;
	//m_BoneMatrixBuffer.data.projection = projectionMatrix;
	//// Now set the matrix constant buffer in the vertex shader with the updated values.
	//deviceContext->VSSetConstantBuffers(0, 1, m_BoneMatrixBuffer.GetAddress());

	////Update BoneTransformBuffer;
	//m_BoneTransformBuffer.data.boneTranforms = boneTransformOffSet;
	//m_BoneTransformBuffer.ApplyChanges(deviceContext);
	//// Now set the camera constant buffer in the vertex shader with the updated values.
	//deviceContext->VSSetConstantBuffers(1, 1, m_BoneTransformBuffer.GetAddress());


	////Update BoneLightBuffer;
	//m_BoneLightBuffer.data.ambientColor = ambientColor;
	//m_BoneLightBuffer.data.diffuseColor = diffuseColor;
	//m_BoneLightBuffer.data.lightDirection = lightDirection;
	//m_BoneLightBuffer.ApplyChanges(deviceContext);
	//deviceContext->PSSetConstantBuffers(0, 1, m_BoneLightBuffer.GetAddress());

	////Update BoneMaterialBuffer;
	//m_BoneMaterialBuffer.data.ambientColor = materialAmbientColor;
	//m_BoneMaterialBuffer.data.diffuseColor = materialDiffuseColor;
	//m_BoneMaterialBuffer.data.SpecularColor = SpecularColor;
	//m_BoneMaterialBuffer.ApplyChanges(deviceContext);
	//deviceContext->PSSetConstantBuffers(1, 1, m_BoneMaterialBuffer.GetAddress());


	////Update BoneCameraBuffer;
	//m_BoneCameraBuffer.data.cameraPosition = cameraPosition;
	//m_BoneCameraBuffer.ApplyChanges(deviceContext);
	//deviceContext->PSSetConstantBuffers(2, 1, m_BoneCameraBuffer.GetAddress());

	deviceContext->IASetInputLayout(this->m_vertexShader.GetInputLayout());
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(m_vertexShader.GetShder(), NULL, 0);
	deviceContext->PSSetShader(m_pixelShader.GetShder(), NULL, 0);


}

