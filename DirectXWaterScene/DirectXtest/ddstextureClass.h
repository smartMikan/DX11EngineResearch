#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#include "./Loader/DDSTextureLoader.h"
using namespace DirectX;
using namespace std;

class DDSTextureClass
{
public:
	DDSTextureClass();
	DDSTextureClass(const DDSTextureClass&);
	~DDSTextureClass();
	
	//The first two functions will load a texture from a given file nameand unload that texture when it is no longer needed.

	bool Initialize(ID3D11Device*device, const WCHAR* filename);
	void Shutdown();
	//The GetTexture function returns a pointer to the texture resource so that it can be used for rendering by shaders.

	ID3D11ShaderResourceView* GetTexture();

private:
	//This is the private texture resource.

	ID3D11ShaderResourceView* m_texture;
};

