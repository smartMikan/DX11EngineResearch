#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: texturemanagerclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREMANAGERCLASS_H_
#define _TEXTUREMANAGERCLASS_H_

#include<string>
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include<D3D11.h>
#include <d3dcompiler.h>
#include"Macro.h"
#include"Loader/DDSTextureLoader.h"
#include<directxmath.h>
using namespace DirectX;
////////////////////////////////////////////////////////////////////////////////
// Class name: TextureManagerClass
////////////////////////////////////////////////////////////////////////////////
class TextureManagerClass
{
public:
	TextureManagerClass();
	TextureManagerClass(const TextureManagerClass&);
	~TextureManagerClass();

	bool Initialize(int);
	void Shutdown();

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const WCHAR*, int);

	
	ID3D11ShaderResourceView* GetTexture(int);

private:
	TextureClass * m_TextureArray;
	int m_textureCount;
};

#endif

