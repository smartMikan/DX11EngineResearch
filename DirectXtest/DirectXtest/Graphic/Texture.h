#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Color.h"
#include <assimp/material.h>
#include "../../Utility/StringHelper.h"
#include<memory>


enum TexFormat
{
	TEX_FORMAT_UNKNOWN = 0,
	TEX_FORMAT_R32G32B32A32_TYPELESS = 1,
	TEX_FORMAT_R32G32B32A32_FLOAT = 2,
	TEX_FORMAT_R32G32B32A32_UINT = 3,
	TEX_FORMAT_R32G32B32A32_SINT = 4,
	TEX_FORMAT_R32G32B32_TYPELESS = 5,
	TEX_FORMAT_R32G32B32_FLOAT = 6,
	TEX_FORMAT_R32G32B32_UINT = 7,
	TEX_FORMAT_R32G32B32_SINT = 8,
	TEX_FORMAT_R16G16B16A16_TYPELESS = 9,
	TEX_FORMAT_R16G16B16A16_FLOAT = 10,
	TEX_FORMAT_R16G16B16A16_UNORM = 11,
	TEX_FORMAT_R16G16B16A16_UINT = 12,
	TEX_FORMAT_R16G16B16A16_SNORM = 13,
	TEX_FORMAT_R16G16B16A16_SINT = 14,
	TEX_FORMAT_R32G32_TYPELESS = 15,
	TEX_FORMAT_R32G32_FLOAT = 16,
	TEX_FORMAT_R32G32_UINT = 17,
	TEX_FORMAT_R32G32_SINT = 18,
	TEX_FORMAT_R32G8X24_TYPELESS = 19,
	TEX_FORMAT_D32_FLOAT_S8X24_UINT = 20,
	TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
	TEX_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
	TEX_FORMAT_R10G10B10A2_TYPELESS = 23,
	TEX_FORMAT_R10G10B10A2_UNORM = 24,
	TEX_FORMAT_R10G10B10A2_UINT = 25,
	TEX_FORMAT_R11G11B10_FLOAT = 26,
	TEX_FORMAT_R8G8B8A8_TYPELESS = 27,
	TEX_FORMAT_R8G8B8A8_UNORM = 28,
	TEX_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
	TEX_FORMAT_R8G8B8A8_UINT = 30,
	TEX_FORMAT_R8G8B8A8_SNORM = 31,
	TEX_FORMAT_R8G8B8A8_SINT = 32,
	TEX_FORMAT_R16G16_TYPELESS = 33,
	TEX_FORMAT_R16G16_FLOAT = 34,
	TEX_FORMAT_R16G16_UNORM = 35,
	TEX_FORMAT_R16G16_UINT = 36,
	TEX_FORMAT_R16G16_SNORM = 37,
	TEX_FORMAT_R16G16_SINT = 38,
	TEX_FORMAT_R32_TYPELESS = 39,
	TEX_FORMAT_D32_FLOAT = 40,
	TEX_FORMAT_R32_FLOAT = 41,
	TEX_FORMAT_R32_UINT = 42,
	TEX_FORMAT_R32_SINT = 43,
	TEX_FORMAT_R24G8_TYPELESS = 44,
	TEX_FORMAT_D24_UNORM_S8_UINT = 45,
	TEX_FORMAT_R24_UNORM_X8_TYPELESS = 46,
	TEX_FORMAT_X24_TYPELESS_G8_UINT = 47,
	TEX_FORMAT_R8G8_TYPELESS = 48,
	TEX_FORMAT_R8G8_UNORM = 49,
	TEX_FORMAT_R8G8_UINT = 50,
	TEX_FORMAT_R8G8_SNORM = 51,
	TEX_FORMAT_R8G8_SINT = 52,
	TEX_FORMAT_R16_TYPELESS = 53,
	TEX_FORMAT_R16_FLOAT = 54,
	TEX_FORMAT_D16_UNORM = 55,
	TEX_FORMAT_R16_UNORM = 56,
	TEX_FORMAT_R16_UINT = 57,
	TEX_FORMAT_R16_SNORM = 58,
	TEX_FORMAT_R16_SINT = 59,
	TEX_FORMAT_R8_TYPELESS = 60,
	TEX_FORMAT_R8_UNORM = 61,
	TEX_FORMAT_R8_UINT = 62,
	TEX_FORMAT_R8_SNORM = 63,
	TEX_FORMAT_R8_SINT = 64,
	TEX_FORMAT_A8_UNORM = 65,
	TEX_FORMAT_R1_UNORM = 66,
	TEX_FORMAT_R9G9B9E5_SHAREDEXP = 67,
	TEX_FORMAT_R8G8_B8G8_UNORM = 68,
	TEX_FORMAT_G8R8_G8B8_UNORM = 69,
	TEX_FORMAT_BC1_TYPELESS = 70,
	TEX_FORMAT_BC1_UNORM = 71,
	TEX_FORMAT_BC1_UNORM_SRGB = 72,
	TEX_FORMAT_BC2_TYPELESS = 73,
	TEX_FORMAT_BC2_UNORM = 74,
	TEX_FORMAT_BC2_UNORM_SRGB = 75,
	TEX_FORMAT_BC3_TYPELESS = 76,
	TEX_FORMAT_BC3_UNORM = 77,
	TEX_FORMAT_BC3_UNORM_SRGB = 78,
	TEX_FORMAT_BC4_TYPELESS = 79,
	TEX_FORMAT_BC4_UNORM = 80,
	TEX_FORMAT_BC4_SNORM = 81,
	TEX_FORMAT_BC5_TYPELESS = 82,
	TEX_FORMAT_BC5_UNORM = 83,
	TEX_FORMAT_BC5_SNORM = 84,
	TEX_FORMAT_B5G6R5_UNORM = 85,
	TEX_FORMAT_B5G5R5A1_UNORM = 86,
	TEX_FORMAT_B8G8R8A8_UNORM = 87,
	TEX_FORMAT_B8G8R8X8_UNORM = 88,
	TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
	TEX_FORMAT_B8G8R8A8_TYPELESS = 90,
	TEX_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
	TEX_FORMAT_B8G8R8X8_TYPELESS = 92,
	TEX_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
	TEX_FORMAT_BC6H_TYPELESS = 94,
	TEX_FORMAT_BC6H_UF16 = 95,
	TEX_FORMAT_BC6H_SF16 = 96,
	TEX_FORMAT_BC7_TYPELESS = 97,
	TEX_FORMAT_BC7_UNORM = 98,
	TEX_FORMAT_BC7_UNORM_SRGB = 99,
	TEX_FORMAT_AYUV = 100,
	TEX_FORMAT_Y410 = 101,
	TEX_FORMAT_Y416 = 102,
	TEX_FORMAT_NV12 = 103,
	TEX_FORMAT_P010 = 104,
	TEX_FORMAT_P016 = 105,
	TEX_FORMAT_420_OPAQUE = 106,
	TEX_FORMAT_YUY2 = 107,
	TEX_FORMAT_Y210 = 108,
	TEX_FORMAT_Y216 = 109,
	TEX_FORMAT_NV11 = 110,
	TEX_FORMAT_AI44 = 111,
	TEX_FORMAT_IA44 = 112,
	TEX_FORMAT_P8 = 113,
	TEX_FORMAT_A8P8 = 114,
	TEX_FORMAT_B4G4R4A4_UNORM = 115,

	TEX_FORMAT_P208 = 130,
	TEX_FORMAT_V208 = 131,
	TEX_FORMAT_V408 = 132
};

class ITexture
{
public:
	virtual ~ITexture() = default;

	virtual size_t GetWidth() const = 0;
	virtual size_t GetHeight() const = 0;
	virtual TexFormat GetFormat() const = 0;
	virtual aiTextureType GetType() const = 0;
	virtual bool IsTranslucent() const = 0;
	virtual ID3D11ShaderResourceView* const* GetShaderResourceView() const = 0;
};


class D3DTexture : public ITexture
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	D3DTexture(ID3D11Device* pDevice, const std::wstring& filename, aiTextureType type);
	D3DTexture(ID3D11Device* pDevice, const char* pData, size_t size, aiTextureType type);
	D3DTexture(ID3D11Device* pDevice, const Color& color, aiTextureType type);
	D3DTexture(ID3D11Device* pDevice, const Color* colorData, UINT width, UINT height, aiTextureType type);

	virtual size_t GetWidth() const override { return m_iWidth; }
	virtual size_t GetHeight() const override { return m_iHeight; }
	virtual TexFormat GetFormat() const override { return m_Format; }
	virtual aiTextureType GetType() const override { return m_Type; }
	virtual bool IsTranslucent() const override { return m_bIsTranslucent; }
	virtual ID3D11ShaderResourceView* const* GetShaderResourceView() const override { return m_pTextureView.GetAddressOf(); }
private:
	void Initialize1x1ColorTexture(ID3D11Device* device, const Color& colorData, aiTextureType type);
	void InitializeColorTexture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type);
private:
	ComPtr<ID3D11Resource>				m_pTexture = nullptr;
	ComPtr<ID3D11ShaderResourceView>	m_pTextureView = nullptr;

	TexFormat m_Format;
	aiTextureType m_Type = aiTextureType::aiTextureType_UNKNOWN;
	size_t m_iWidth = 0;
	size_t m_iHeight = 0;
	bool m_bIsTranslucent = false;
};

class Texture
{
public:
	Texture() = default;

	Texture(ID3D11Device* device, const Color& color, aiTextureType type);
	Texture(ID3D11Device* device, const Color* colorData,UINT width,UINT height, aiTextureType type);

	Texture(ID3D11Device* device, const std::wstring & filePath, aiTextureType type);
	Texture(ID3D11Device* device, const std::string& filePath, aiTextureType type) :
		Texture(device, StringHelper::StringToWide(filePath), type)
	{};
	Texture(ID3D11Device* device, const char* pData, size_t size, aiTextureType type);

	aiTextureType GetType();
	ID3D11ShaderResourceView* const * GetShaderResourceView() const;
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();
	ITexture* Get() { return m_pTexture.get(); }
	const ITexture* Get() const { return m_pTexture.get(); }

	operator ITexture*();
	ITexture* operator->();
	const ITexture* operator->() const;

private:
	std::unique_ptr<ITexture> m_pTexture = nullptr;
	void Initialize1x1ColorTexture(ID3D11Device* device, const Color& color, aiTextureType type);
	void InitializeColorTexture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type);
	Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
};

