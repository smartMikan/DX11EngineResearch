#include "RendererBase.h"

RendererBase::RendererBase()
{

}



RendererBase::RendererBase(const RendererBase& rhs)
{
}

RendererBase::~RendererBase()
{
}

bool RendererBase::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	m_VertexShader = std::make_unique<D3DVertexShader>(device.Get(), StringHelper::WideToString(vertexshaderpath));

	bool result;
	result = m_Pixelshader.Initialize(device, pixelshaderpath);
	if (!result)
	{
		return false;
	}
	return true;
}

IVertexShader* RendererBase::GetVertexShader()
{
	return m_VertexShader.get();
}

ID3D11PixelShader* RendererBase::GetPixelShader() {
	return m_Pixelshader.GetShader();
}




bool RendererBase::BindShader()
{

	return true;
}
