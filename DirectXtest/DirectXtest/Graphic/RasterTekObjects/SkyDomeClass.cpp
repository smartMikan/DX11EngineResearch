#include "SkyDomeClass.h"



SkyDomeClass::SkyDomeClass()
{
	m_model = 0;
}



SkyDomeClass::~SkyDomeClass()
{
}

bool SkyDomeClass::Initialize(ID3D11Device* device)
{
	bool result;


	// Load in the sky dome model.
	result = LoadSkyDomeModel(L"./3DModel/skydome.txt");
	if (!result)
	{
		return false;
	}

	// Load the sky dome into a vertex and index buffer for rendering.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Set the color at the top of the sky dome.
	m_apexColor = XMFLOAT4(0.0f, 0.05f, 0.6f, 1.0f);

	// Set the color at the center of the sky dome.
	m_centerColor = XMFLOAT4(0.0f, 0.5f, 0.8f, 1.0f);

	return true;
}

void SkyDomeClass::Shutdown()
{
	// Release the vertex and index buffer that were used for rendering the sky dome.
	ReleaseBuffers();

	// Release the sky dome model.
	ReleaseSkyDomeModel();

	return;
}

void SkyDomeClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Render the sky dome.
	RenderBuffers(deviceContext);

	return;
}

int SkyDomeClass::GetIndexCount()
{
	return m_indexCount;
}

XMFLOAT4 SkyDomeClass::GetApexColor()
{
	return m_apexColor;
}

XMFLOAT4 SkyDomeClass::GetCenterColor()
{
	return m_centerColor;
}

bool SkyDomeClass::LoadSkyDomeModel(const WCHAR* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void SkyDomeClass::ReleaseSkyDomeModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool SkyDomeClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		indices[i] = i;
	}




	result = m_vertexBuffer.Initialize(device, vertices, m_vertexCount);
	if (FAILED(result))
	{
		return false;
	}



	result = m_indexBuffer.Initialize(device, indices, m_indexCount);
	if (FAILED(result))
	{
		return false;
	}
	

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void SkyDomeClass::ReleaseBuffers()
{

	return;
}

void SkyDomeClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{

	unsigned int offset;

	// Set vertex buffer stride and offset.
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddress(), m_vertexBuffer.StridePtr(), &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}