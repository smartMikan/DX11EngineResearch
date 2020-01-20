#include"MeshGeometryClass.h"


MeshGeometryClass::MeshGeometryClass():mVertexBuffer(NULL),mIndexBuffer(NULL),mIndexBufferFormat(DXGI_FORMAT_R32_UINT),mVertexStride(0)
{
	
}


MeshGeometryClass::~MeshGeometryClass()
{
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);
}


bool MeshGeometryClass::SetIndices(ID3D11Device* device, const unsigned long* indices, UINT count)
{
	D3D11_BUFFER_DESC  indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	HR(device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer));

	return true;
}

void MeshGeometryClass::SetSubsetTable(vector<Subset>& SubsetTable)
{
	mSubsetTable = SubsetTable;
}

void MeshGeometryClass::Draw(ID3D11DeviceContext* deviceContext, UINT subsetId)
{

	UINT offset=0;
	//设置拓扑方式
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mVertexStride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer, mIndexBufferFormat,0);
	deviceContext->DrawIndexed(mSubsetTable[subsetId].FaceCount * 3, mSubsetTable[subsetId].FaceStart * 3, 0);
}
