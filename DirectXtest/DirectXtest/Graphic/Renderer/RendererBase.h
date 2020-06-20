#pragma once

#include"../../Utility/StringHelper.h"
#include"../ShaderModels.h"
#include"../../Engine/Component.h"



class RendererBase
{
public:
	RendererBase();
	RendererBase(const RendererBase& rhs);
	~RendererBase();


	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device);

	IVertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();



private:

	bool BindShader();

	std::unique_ptr<D3DVertexShader> m_VertexShader;
	PixelShader m_Pixelshader;

	std::wstring vertexshaderpath;
	std::wstring pixelshaderpath;


};

