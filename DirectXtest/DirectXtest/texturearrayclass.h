#pragma once
//The TextureArrayClass replaces the TextureClass that was used before. 
//Instead of having just a single texture it can now have multiple textures and give calling objects access to those textures. 
//For this tutorial it just handles two textures but it can easily be expanded.

#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_

#include <d3d11.h>
#include "ddstextureClass.h"
#include "textureclass.h"
#include <d3dcompiler.h>
#include <vector>

#include "./Loader/DDSTextureLoader.h"
#include "./Loader/WICTextureLoader.h"

class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass& other);
	~TextureArrayClass();

	bool Initialize(ID3D11Device* device, const WCHAR* filename1, const WCHAR* filename2, const WCHAR* filename3);
	bool Initialize(ID3D11Device* device, const WCHAR** fileArray , int textureCount);
	void Shutdown();

	vector<ID3D11ShaderResourceView*> GetTextureVector();

private:
	//This is the two element texture array private variable.
	std::vector<ID3D11ShaderResourceView*> m_textures;
};

#endif

