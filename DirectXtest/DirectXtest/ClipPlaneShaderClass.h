#pragma once
#include "textureshaderclass.h"
class ClipPlaneShaderClass : public TextureShaderClass
{
private:
	struct ClipPlaneBufferType
	{
		XMFLOAT4 clipPlane;
	};
public:
	ClipPlaneShaderClass();
	ClipPlaneShaderClass(const ClipPlaneShaderClass&);
	~ClipPlaneShaderClass();

	bool Initialize(ID3D11Device*, HWND) override;
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 clipPlane);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename) override;
	void ShutdownShader() override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 clipPlane);

private:
	ID3D11Buffer* m_clipPlaneBuffer;
};

