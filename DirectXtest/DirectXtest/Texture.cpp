#include "Texture.h"
#include "./Utility/ErrorLoger.h"

Texture::Texture(ID3D11Device* device, const Color& color, aiTextureType type)
{
	this->Initialize1x1ColorTexture(device,color,type);
}

Texture::Texture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->InitializeColorTexture(device, colorData, width, height, type);
}

aiTextureType Texture::GetType()
{
	return this->type;
}

ID3D11ShaderResourceView* Texture::GetTextureResourceView()
{
	return this->textureView.Get();
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
	return this->textureView.GetAddressOf();
}

void Texture::Initialize1x1ColorTexture(ID3D11Device* device, const Color& color, aiTextureType type)
{
	InitializeColorTexture(device, &color, 1, 1, type);
}

void Texture::InitializeColorTexture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->type = type;
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* p2DTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = colorData;
	initialData.SysMemPitch = width * sizeof(Color);
	HRESULT result = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
	if (FAILED(result)) {
		ErrorLoger::Log(result, "Failed to initialize texture from color data.");
	}
	texture = static_cast<ID3D11Texture2D*>(p2DTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	result = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());
	if (FAILED(result)) {
		ErrorLoger::Log(result, "Failed to create shader resource view from texture genarate from data.");
	}
}
