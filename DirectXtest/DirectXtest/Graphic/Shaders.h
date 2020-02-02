#pragma once
#include "../Utility/ErrorLoger.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <unordered_map>
#include <atomic>
#pragma comment(lib,"D3DCompiler.lib")



class AttributeInfo
{
public:
	enum class VertexAttribute
	{
		Invalid = -1,
		Position,
		Color,
		Normal,
		TexCoord,
		Tangent,
		Bitangent,
		BoneId,
		BoneWeight,
		TotalAttributes
	};

	static VertexAttribute SemanticToAttribute(const std::string& semantic)
	{
		static std::unordered_map<std::string, VertexAttribute> s_mapConvert;
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			s_mapConvert["POSITION"] = VertexAttribute::Position;
			s_mapConvert["COLOR"] = VertexAttribute::Color;
			s_mapConvert["NORMAL"] = VertexAttribute::Normal;
			s_mapConvert["TEXCOORD"] = VertexAttribute::TexCoord;
			s_mapConvert["TANGENT"] = VertexAttribute::Tangent;
			s_mapConvert["BITANGENT"] = VertexAttribute::Bitangent;
			s_mapConvert["BONEID"] = VertexAttribute::BoneId;
			s_mapConvert["BONEWEIGHT"] = VertexAttribute::BoneWeight;
		}

		auto it = s_mapConvert.find(semantic);
		if (it == s_mapConvert.end())
		{
			return VertexAttribute::Invalid;
		}

		return it->second;
	}

	static std::string AttributeToSemantic(const VertexAttribute attribute)
	{
		static std::unordered_map<VertexAttribute, std::string> s_mapConvert;
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			s_mapConvert[VertexAttribute::Position] = "POSITION";
			s_mapConvert[VertexAttribute::Color] = "COLOR";
			s_mapConvert[VertexAttribute::Normal] = "NORMAL";
			s_mapConvert[VertexAttribute::TexCoord] = "TEXCOORD";
			s_mapConvert[VertexAttribute::Tangent] = "TANGENT";
			s_mapConvert[VertexAttribute::Bitangent] = "BITANGENT";
			s_mapConvert[VertexAttribute::BoneId] = "BONEID";
			s_mapConvert[VertexAttribute::BoneWeight] = "BONEWEIGHT";
		}

		auto it = s_mapConvert.find(attribute);
		if (it == s_mapConvert.end())
		{
			return "";
		}

		return it->second;
	}
};


class IVertexShader
{
public:
	virtual ~IVertexShader() = default;

	virtual std::string GetName() const = 0;
	virtual bool RequiresVertexAttribute(AttributeInfo::VertexAttribute attribute) const = 0;
};

class D3DVertexShader : public IVertexShader
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	D3DVertexShader(ID3D11Device* pDevice, const std::string& filename);
	~D3DVertexShader();

	virtual std::string GetName() const override { return m_szName; }
	virtual bool RequiresVertexAttribute(AttributeInfo::VertexAttribute attribute) const override { return m_bUsesAttribute[(int)attribute]; }
	ID3D11VertexShader* GetShader(ID3D11Device* pDevice);
	ID3D11InputLayout* GetLayout() { return m_pInputLayout.Get(); }
	const ID3D11InputLayout* GetLayout() const { return m_pInputLayout.Get(); }
private:
	void CreateInputLayoutDescFromVertexShaderSignature(ID3D11Device* pDevice, const void* pCodeBytes, const size_t size);
	void LoadFromFile(ID3D11Device* pDevice, const std::string& filename, bool crash_on_error);
	void OnFileChanged(const std::string& filename);
	bool IsDirty() const { return m_bDirty; }
	void SetDirty(bool dirty) { m_bDirty = dirty; }
private:
	std::string m_szName;
	ComPtr<ID3D11InputLayout>	m_pInputLayout;
	bool m_bUsesAttribute[(int)AttributeInfo::VertexAttribute::TotalAttributes];
	std::atomic_bool m_bDirty = true;
	ComPtr<ID3D11VertexShader> m_pShader;

};

class VertexShader
{
public:
	bool Initialize(ID3D11Device* device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutdesc,UINT elementCount);
	ID3D11VertexShader* GetShder();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout() {
		return this->m_inputlayout.Get();
	}
	

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputlayout = nullptr;
};




class PixelShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderPath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
	
	/*ID3D11SamplerState* GetSamplerState() {
		return this->m_samplerState.Get();
	}*/
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer = nullptr;
	//Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState = nullptr;
};
