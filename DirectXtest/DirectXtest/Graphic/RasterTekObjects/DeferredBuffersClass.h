#pragma once


const int BUFFER_COUNT = 2;


#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class DeferredBuffersClass
{
public:
	DeferredBuffersClass();
	DeferredBuffersClass(const DeferredBuffersClass& other);
	~DeferredBuffersClass();

	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear);
	void Shutdown();

	void SetRenderTargets(ID3D11DeviceContext*deviceContext);
	void ClearRenderTargets(ID3D11DeviceContext*deviceContext, float red, float green, float blue, float alpha);

	ID3D11ShaderResourceView* GetShaderResourceView(int view);

private:
	int m_textureWidth, m_textureHeight;

	ID3D11Texture2D* m_renderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* m_renderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* m_shaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
};


