#include "Mesh.h"
#include "Shaders.h"

Mesh::Mesh(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<CB_PS_Material>& cb_ps_material,
	MeshParameters& params,
	std::vector<DWORD>& indices,
	Material& material,
	const DirectX::XMMATRIX& transformMatrix
	)
{
	this->m_device = device;
	this->m_deviceContext = deviceContext;
	this->cb_ps_material = &cb_ps_material;
	this->material = material;
	this->transformMatrix = transformMatrix;
	SetData(params);

	HRESULT result = this->m_indexBuffer.Initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(result, "Failed to initialize index buffer for mesh.");

}

Mesh::Mesh(const Mesh& other)
{
	this->m_device = other.m_device;
	this->m_deviceContext = other.m_deviceContext;
	this->m_indexBuffer = other.m_indexBuffer;
	this->m_Position = other.m_Position;
	this->m_Color = other.m_Color;
	this->m_Normal = other.m_Normal;
	this->m_TexCoord = other.m_TexCoord;
	this->m_Tangent = other.m_Tangent;
	this->m_Bitangent = other.m_Bitangent;
	this->m_BoneIndice = other.m_BoneIndice;
	this->m_BoneWeights = other.m_BoneWeights;
	this->material =other.material;
	this->cb_ps_material = other.cb_ps_material;
	this->transformMatrix = other.transformMatrix;
}

void Mesh::Bind(IVertexShader* pVertexShader) const
{
	size_t slot = 0;

	if (pVertexShader->RequiresVertexAttribute(AttributeInfo::VertexAttribute::Position))
	{
		assert(m_Position.VertexCount() > 0, "Shader requires model to have vertex position data");

		ID3D11Buffer* pVertexBuffer = m_Position.Get();
		UINT stride = (UINT)m_Position.Stride();
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers((UINT)slot, 1, &pVertexBuffer, &stride, &offset);
		slot++;
	}
	if (pVertexShader->RequiresVertexAttribute(AttributeInfo::VertexAttribute::BoneId))
	{
		assert(m_BoneIndice.VertexCount() > 0, "Shader requires model to have bone name data");
		ID3D11Buffer* pVertexBuffer = m_BoneIndice.Get();
		UINT stride = (UINT)m_BoneIndice.Stride();
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers((UINT)slot, 1, &pVertexBuffer, &stride, &offset);
		slot++;
	}
	if (pVertexShader->RequiresVertexAttribute(AttributeInfo::VertexAttribute::BoneWeight))
	{
		assert(m_BoneWeights.VertexCount() > 0, "Shader requires model to have bone weight data");
		ID3D11Buffer* pVertexBuffer = m_BoneWeights.Get();
		UINT stride = (UINT)m_BoneWeights.Stride();
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers((UINT)slot, 1, &pVertexBuffer, &stride, &offset);
		slot++;
	}
	if (pVertexShader->RequiresVertexAttribute(AttributeInfo::VertexAttribute::Color))
	{
		assert(m_Color.VertexCount() > 0, "Shader requires model to have vertex color data");
		ID3D11Buffer* pVertexBuffer = m_Color.Get();
		UINT stride = (UINT)m_Color.Stride();
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers((UINT)slot, 1, &pVertexBuffer, &stride, &offset);
		slot++;
	}
	if (pVertexShader->RequiresVertexAttribute(AttributeInfo::VertexAttribute::Normal))
	{
		assert(m_Normal.VertexCount() > 0, "Shader requires model to have vertex normal data");
		ID3D11Buffer* pVertexBuffer = m_Normal.Get();
		UINT stride = (UINT)m_Normal.Stride();
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers((UINT)slot, 1, &pVertexBuffer, &stride, &offset);
		slot++;
	}
	if (pVertexShader->RequiresVertexAttribute(AttributeInfo::VertexAttribute::TexCoord))
	{
		assert(m_TexCoord.VertexCount() > 0, "Shader requires model to have vertex UV data");
		ID3D11Buffer* pVertexBuffer = m_TexCoord.Get();
		UINT stride = (UINT)m_TexCoord.Stride();
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers((UINT)slot, 1, &pVertexBuffer, &stride, &offset);
		slot++;
	}
	if (pVertexShader->RequiresVertexAttribute(AttributeInfo::VertexAttribute::Tangent))
	{
		assert(m_Tangent.VertexCount() > 0, "Shader requires model to have vertex tangent data");
		ID3D11Buffer* pVertexBuffer = m_Tangent.Get();
		UINT stride = (UINT)m_Tangent.Stride();
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers((UINT)slot, 1, &pVertexBuffer, &stride, &offset);
		slot++;
	}
	if (pVertexShader->RequiresVertexAttribute(AttributeInfo::VertexAttribute::Bitangent))
	{
		assert(m_Bitangent.VertexCount() > 0, "Shader requires model to have vertex bitangent data");
		ID3D11Buffer* pVertexBuffer = m_Bitangent.Get();
		UINT stride = (UINT)m_Bitangent.Stride();
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers((UINT)slot, 1, &pVertexBuffer, &stride, &offset);
		slot++;
	}

}

void Mesh::BindMaterial() const
{
	cb_ps_material->data.GlobalAmbient = { 1.0f, 1.0f, 1.0f, 1.0f };
	cb_ps_material->data.AmbientColor = { 0.005f, 0.005f, 0.005f, 1.0f };
	cb_ps_material->data.DiffuseColor = DirectX::XMFLOAT4(material.GetDiffuseColor().x, material.GetDiffuseColor().y, material.GetDiffuseColor().z, 1.0f);
	cb_ps_material->data.SpecularColor = DirectX::XMFLOAT4(material.GetSpecularColor().x, material.GetSpecularColor().y, material.GetSpecularColor().z, 1.0f);
	cb_ps_material->data.EmissiveColor = DirectX::XMFLOAT4(material.GetEmissiveColor().x, material.GetEmissiveColor().y, material.GetEmissiveColor().z, 1.0f);
	cb_ps_material->data.Opacity = material.GetOpacity();
	cb_ps_material->data.HasAmbientTexture = (material.GetAmbientTexture() != nullptr);
	cb_ps_material->data.HasDiffuseTexture = (material.GetDiffuseTexture() != nullptr);
	cb_ps_material->data.HasSpecularTexture = (material.GetSpecularTexture() != nullptr);
	cb_ps_material->data.HasEmissiveTexture = (material.GetEmissiveTexture() != nullptr);
	cb_ps_material->data.HasNormalTexture = (material.GetNormalTexture() != nullptr);
	cb_ps_material->data.HasBumpTexture = false;
	cb_ps_material->data.SpecularPower = material.GetShininess();

	cb_ps_material->ApplyChanges();
	m_deviceContext->PSSetConstantBuffers(2, 1, cb_ps_material->GetAddress());

	if (material.GetDiffuseTexture() != nullptr)
		this->m_deviceContext->PSSetShaderResources(0, 1, material.GetDiffuseTexture()->GetShaderResourceView());
	if (material.GetNormalTexture() != nullptr)
		this->m_deviceContext->PSSetShaderResources(1, 1, material.GetNormalTexture()->GetShaderResourceView());
	if (material.GetSpecularTexture() != nullptr)
		this->m_deviceContext->PSSetShaderResources(2, 1, material.GetSpecularTexture()->GetShaderResourceView());
	if (material.GetEmissiveTexture() != nullptr)
		this->m_deviceContext->PSSetShaderResources(3, 1, material.GetEmissiveTexture()->GetShaderResourceView());
}

void Mesh::SetData(MeshParameters& params)
{
	if (!params.position.empty())
	{
		m_Position.Initialize(m_device, params.position.data(), params.position.size());
	}
	if (!params.color.empty())
	{
		m_Color.Initialize(m_device, params.color.data(), params.color.size());
	}
	if (!params.normal.empty())
	{
		m_Normal.Initialize(m_device, params.normal.data(), params.normal.size());
	}
	if (!params.texcoord.empty())
	{
		m_TexCoord.Initialize(m_device, params.texcoord.data(), params.texcoord.size());
	}
	if (!params.tangent.empty())
	{
		m_Tangent.Initialize(m_device, params.tangent.data(), params.tangent.size());
	}
	if (!params.bitangent.empty())
	{
		m_Bitangent.Initialize(m_device, params.bitangent.data(), params.bitangent.size());
	}
	if (!params.bone_indice.empty())
	{
		m_BoneIndice.Initialize(m_device, params.bone_indice.data(), params.bone_indice.size());
	}
	if (!params.bone_weights.empty())
	{
		m_BoneWeights.Initialize(m_device, params.bone_weights.data(), params.bone_weights.size());
	}
}

void Mesh::Draw(IVertexShader* pVertexShader)
{
	BindMaterial();
	Bind(pVertexShader);
	this->m_deviceContext->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	this->m_deviceContext->DrawIndexed(this->m_indexBuffer.IndexCount(), 0, 0);
}

int Mesh::GetIndexSize()
{
	return m_indexBuffer.IndexCount();
}


const DirectX::XMMATRIX& Mesh::GetTransformMatrix()
{
	return this->transformMatrix;
}

