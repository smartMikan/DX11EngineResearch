#include "ddstextureClass.h"

DDSTextureClass::DDSTextureClass()
{
	m_texture = 0;
}

DDSTextureClass::DDSTextureClass(const DDSTextureClass& other)
{
}

DDSTextureClass::~DDSTextureClass()
{
}

bool DDSTextureClass::Initialize(ID3D11Device* device,const WCHAR* filename)
{
	HRESULT result;



	// Load the texture in.
	result = CreateDDSTextureFromFile(device, filename, NULL, &m_texture);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DDSTextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* DDSTextureClass::GetTexture()
{
	return m_texture;
}
