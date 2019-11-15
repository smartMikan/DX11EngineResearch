#include "texturearrayclass.h"

TextureArrayClass::TextureArrayClass()
{
	m_textures[0] = 0;
	m_textures[1] = 0;
	m_textures[2] = 0;
}

TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{
}

TextureArrayClass::~TextureArrayClass()
{
}

//Initialize takes in the two texture file names and creates two texture resources in the texture array from those files.

bool TextureArrayClass::Initialize(ID3D11Device* device, const WCHAR* filename1, const WCHAR* filename2, const WCHAR* filename3)
{
	HRESULT result;

	// Load the first texture in.
	result = CreateDDSTextureFromFile(device, filename1, NULL, &m_textures[0]);
	if (FAILED(result))
	{
		return false;
	}

	// Load the second texture in.
	result = CreateDDSTextureFromFile(device, filename2, NULL, &m_textures[1]);
	if (FAILED(result))
	{
		return false;
	}
	// Load the third texture in.
	result = CreateDDSTextureFromFile(device, filename3, NULL, &m_textures[2]);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

bool TextureArrayClass::Initialize(ID3D11Device * device, const WCHAR ** fileArray,int textureCount)
{

	HRESULT result;
	for (size_t i = 0; i < textureCount; i++)
	{
		auto sh = new ID3D11ShaderResourceView*();
		m_textures[i] = *sh;
		result = CreateDDSTextureFromFile(device, fileArray[i], NULL, &m_textures[i]);
	}
	return true;
}

//The Shutdown function releases each element in the texture array.

void TextureArrayClass::Shutdown()
{
	// Release the texture resources.
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	if (m_textures[2])
	{
		m_textures[2]->Release();
		m_textures[2] = 0;
	}
	return;
}

//GetTextureArray returns a pointer to the texture array so calling objects can have access to the textures in the texture array.
ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}
