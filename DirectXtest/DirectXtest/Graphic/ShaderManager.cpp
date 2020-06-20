#include "ShaderManager.h"

bool ShaderManager::InitializeShaders(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	m_device = device.Get();

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG // Debug Mode
#ifdef _WIN64 // x64
		shaderfolder = L"..\\x64\\Debug\\";
#else	// x86
		shaderfolder = L"..\\Debug\\";
#endif
#else	// Release Mode
#ifdef _WIN64
		shaderfolder = L"..\\x64\\Release\\";
#else	// x86
		shaderfolder = L"..\\Release\\";
#endif
#endif
	}

	d3dvertexshader = std::make_unique<D3DVertexShader>(m_device, StringHelper::WideToString(shaderfolder) + "vertexShader.cso");
	d3dvertexshader_animation = std::make_unique<D3DVertexShader>(m_device, StringHelper::WideToString(shaderfolder) + "VertexShaderAnim.cso");
	d3dvertexshader_nolight = std::make_unique<D3DVertexShader>(m_device, StringHelper::WideToString(shaderfolder) + "VS_nolight.cso");
	d3dvertexshader_shadowmap = std::make_unique<D3DVertexShader>(m_device, StringHelper::WideToString(shaderfolder) + "VS_shadowmap.cso");
	d3dvertexshader_shadowmap_anim = std::make_unique<D3DVertexShader>(m_device, StringHelper::WideToString(shaderfolder) + "VS_shadowmap_anim.cso");

	bool result;
	result = pixelshader.Initialize(device, shaderfolder + L"pixelshader.cso");
	if (!result)
	{
		return false;
	}
	result = pixelshader_nolight.Initialize(device, shaderfolder + L"pixelshader_nolight.cso");
	if (!result)
	{
		return false;
	}
	result = pixelshader_toonmapping.Initialize(device, shaderfolder + L"pixelshader_toonmapping.cso");
	if (!result)
	{
		return false;
	}
	result = pixelshader_heightmapping.Initialize(device, shaderfolder + L"PixelShader_HeightMapping.cso");
	if (!result)
	{
		return false;
	}
	result = pixelshader_depthColor.Initialize(device, shaderfolder + L"PixelShader_Depth.cso");
	if (!result)
	{
		return false;
	}
	return true;
}


IVertexShader* ShaderManager::CreateVertexShader(const std::string& filename)
{
	return new D3DVertexShader(m_device, filename);
}