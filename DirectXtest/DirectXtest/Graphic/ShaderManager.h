#pragma once

#include"../../Utility/StringHelper.h"
#include"ShaderModels.h"

class ShaderManager
{

public:
	bool InitializeShaders(Microsoft::WRL::ComPtr<ID3D11Device>& device);
	IVertexShader* CreateVertexShader(const std::string& filename);


	// shaders
	std::unique_ptr<D3DVertexShader> d3dvertexshader;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_animation;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_nolight;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_shadowmap;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_shadowmap_anim;
	PixelShader pixelshader;
	PixelShader pixelshader_nolight;
	PixelShader pixelshader_toonmapping;
	PixelShader pixelshader_heightmapping;
	PixelShader pixelshader_depthColor;

private:
	ID3D11Device* m_device;

};

