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
#include "Graphic/RasterTekObjects/textureclass.h"
#include<D3D11.h>
#include <d3dcompiler.h>
#include"Macro.h"
#include"Loader/DDSTextureLoader.h"
#include"Loader/WICTextureLoader.h"
#include<directxmath.h>
#include <wrl/client.h>
using namespace DirectX;
////////////////////////////////////////////////////////////////////////////////
// Class name: TextureManagerClass
////////////////////////////////////////////////////////////////////////////////
class TextureManagerClass
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	TextureManagerClass();
	TextureManagerClass(const TextureManagerClass&);
	~TextureManagerClass();

	bool Initialize(int count, ID3D11Device* device);
	void Shutdown();

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const WCHAR*, int);

	
	ID3D11ShaderResourceView* GetTexture(int count);

	ID3D11ShaderResourceView* const * GetToonTexture();

private:
	TextureClass * m_TextureArray;
	int m_textureCount;

	ComPtr<ID3D11ShaderResourceView> toneTexture;

};

#endif

