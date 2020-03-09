#pragma once

#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <memory>

using namespace DirectX;

class RenderTextureClass
{
public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass&);
	~RenderTextureClass();

	bool Initialize(ID3D11Device*, int, int, float, float);
	void Shutdown();

	void SetDVRenderTarget(ID3D11DeviceContext*);
	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearDVRenderTarget(ID3D11DeviceContext*, float, float, float, float);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11ShaderResourceView* GetShadowShaderResourceView();
	ID3D11ShaderResourceView* const * GetShaderResourceViewAddress();
	ID3D11ShaderResourceView* const * GetShadowShaderResourceViewAddress();
	void GetProjectionMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	Microsoft::WRL::ComPtr<ID3D11SamplerState> shadowSampler;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTargetTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shadowmap_resourceView;


	std::unique_ptr<CD3D11_VIEWPORT> m_viewport;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;

};

#endif
