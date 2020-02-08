#include "rendertextureclass.h"



RenderTextureClass::RenderTextureClass()
{
	/*m_renderTargetTexture = 0;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;*/
}

RenderTextureClass::RenderTextureClass(const RenderTextureClass& other)
{

}


RenderTextureClass::~RenderTextureClass()
{
}


//The Initialize function takes as input the width and height you want to make this render to texture. 
//Important: Remember that if you are rendering your screen you should keep the aspect ratio of this render to texture the same as the aspect ratio of the screen or there will be some size distortion.

//The function creates a render target texture by first setting up the description of the texture and then creating that texture.
//It then uses that texture to setup a render target view so that the texture can be drawn to as a render target.
//The third and final thing we do is create a shader resource view of that texture so that we can supply the rendered data to calling objects.

bool RenderTextureClass::Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, m_renderTargetTexture.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(m_renderTargetTexture.Get(), &renderTargetViewDesc, m_renderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(m_renderTargetTexture.Get(), &shaderResourceViewDesc, m_shaderResourceView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, NULL, m_depthStencilBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC shadowShaderResourceViewDesc;
	shadowShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shadowShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadowShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shadowShaderResourceViewDesc.Texture2D.MipLevels = depthBufferDesc.MipLevels;
	result = device->CreateShaderResourceView(m_depthStencilBuffer.Get(), &shadowShaderResourceViewDesc, shadowmap_resourceView.GetAddressOf());

	// Setup the viewport for rendering.
	/*m_viewport.Width = (float)textureWidth;
	m_viewport.Height = (float)textureHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
*/


	// Setup the viewport for rendering.
	m_viewport = std::make_unique<CD3D11_VIEWPORT>(0.0f, 0.0f, static_cast<float>(textureWidth), static_cast<float>(textureHeight));



	CD3D11_SAMPLER_DESC shadow_samplerDesc(D3D11_DEFAULT);
	shadow_samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	shadow_samplerDesc.BorderColor[0] = 0.0f;
	shadow_samplerDesc.BorderColor[1] = 1.0f;
	shadow_samplerDesc.BorderColor[2] = 0.0f;
	shadow_samplerDesc.BorderColor[3] = 1.0f;
	shadow_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadow_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadow_samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

	result = device->CreateSamplerState(&shadow_samplerDesc, shadowSampler.GetAddressOf());
	
	
	// Setup the projection matrix.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(((float)XM_PI / 4.0f), ((float)textureWidth / (float)textureHeight), screenNear, screenDepth);

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)textureWidth, (float)textureHeight, screenNear, screenDepth);



	return true;
}

//Shutdown releases the three interfaces used by the RenderTextureClass.

void RenderTextureClass::Shutdown()
{
	/*if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_shaderResourceView)
	{
		m_shaderResourceView->Release();
		m_shaderResourceView = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = 0;
	}
*/
	return;
}

void RenderTextureClass::SetDVRenderTarget(ID3D11DeviceContext *deviceContext)
{
	SetRenderTarget(deviceContext, m_depthStencilView.Get());
}

//The SetRenderTarget function sets the render target view in this class as the current rendering location for all graphics to be rendered to.
void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), depthStencilView);

	// Set the viewport.
	deviceContext->RSSetViewports(1, m_viewport.get());

	return;
}

void RenderTextureClass::ClearDVRenderTarget(ID3D11DeviceContext* deviceContext,float red, float green, float blue, float alpha)
{
	ClearRenderTarget(deviceContext, m_depthStencilView.Get(), red, green, blue, alpha);
}

//ClearRenderTarget mimics the functionality of the D3DClass::BeginScene function except for that it operates on the render target view within this class. 
//This should be called each frame before rendering to this render target view.

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView,
	float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return;
}

//The GetShaderResourceView function returns the render to texture data as a shader resource view. 
//This way whatever has been rendered to the render target view can be used as a texture in different shaders that call this function.
//Where you would normally send a texture into a shader you can instead send a call to this function in its place and the render to texture will be used.

ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return m_shaderResourceView.Get();
}

ID3D11ShaderResourceView * const* RenderTextureClass::GetShaderResourceViewAddress()
{
	return m_shaderResourceView.GetAddressOf();
}

ID3D11ShaderResourceView * const * RenderTextureClass::GetShadowShaderResourceViewAddress()
{
	return shadowmap_resourceView.GetAddressOf();
}


void RenderTextureClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void RenderTextureClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}