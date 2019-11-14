////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
//The class constructor initializes the vertex and index buffer pointers to null.

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	//m_Texture = 0;
	m_TextureArray = 0;
	m_model = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}
//The Initialize function will call the initialization functions for the vertex and index buffers.

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR * modelFilename, const WCHAR * textureFilename1, const WCHAR* textureFilename2, const WCHAR * textureFilename3)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	//After the model data has been loaded we now call the new CalculateModelVectors function to calculate the tangent and binormal. 
	//It also recalculates the normal vector.
	// Calculate the normal, tangent, and binormal vectors for the model.

	CalculateModelVectors();

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	/*result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}*/

	// Load the textures for this model.
	result = LoadTextures(device, textureFilename1, textureFilename2, textureFilename3);
	if (!result)
	{
		return false;
	}

	return true;
}
//The Shutdown function will call the shutdown functions for the vertex and index buffers.

void ModelClass::Shutdown()
{

	// Release the model texture.
	//ReleaseTexture();
	
	//ReleaseTextures is called to release the texture array in the Shutdown function.
	// Release the model textures.
	ReleaseTextures();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}
//Render is called from the GraphicsClass::Render function.This function calls RenderBuffers to put the vertex and index buffers on the graphics pipeline so the color shader will be able to render them.

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

//ID3D11ShaderResourceView* ModelClass::GetTexture()
//{
//	return m_Texture->GetTexture();
//}

ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	return m_TextureArray->GetTextureArray();
}


//CalculateModelVectors generates the tangent and binormal for the model as well as a recalculated normal vector. 
//To start it calculates how many faces (triangles) are in the model. 
//Then for each of those triangles it gets the three vertices and uses that to calculate the tangent, binormal, and normal. 
//After calculating those three normal vectors it then saves them back into the model structure.
void ModelClass::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;


	// Calculate the number of faces in the model.
	faceCount = m_vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		m_model[index - 1].nx = normal.x;
		m_model[index - 1].ny = normal.y;
		m_model[index - 1].nz = normal.z;
		m_model[index - 1].tx = tangent.x;
		m_model[index - 1].ty = tangent.y;
		m_model[index - 1].tz = tangent.z;
		m_model[index - 1].bx = binormal.x;
		m_model[index - 1].by = binormal.y;
		m_model[index - 1].bz = binormal.z;

		m_model[index - 2].nx = normal.x;
		m_model[index - 2].ny = normal.y;
		m_model[index - 2].nz = normal.z;
		m_model[index - 2].tx = tangent.x;
		m_model[index - 2].ty = tangent.y;
		m_model[index - 2].tz = tangent.z;
		m_model[index - 2].bx = binormal.x;
		m_model[index - 2].by = binormal.y;
		m_model[index - 2].bz = binormal.z;

		m_model[index - 3].nx = normal.x;
		m_model[index - 3].ny = normal.y;
		m_model[index - 3].nz = normal.z;
		m_model[index - 3].tx = tangent.x;
		m_model[index - 3].ty = tangent.y;
		m_model[index - 3].tz = tangent.z;
		m_model[index - 3].bx = binormal.x;
		m_model[index - 3].by = binormal.y;
		m_model[index - 3].bz = binormal.z;
	}

	return;
}

//The CalculateTangentBinormal function takes in three vertices and then calculates and returns the tangent and binormal of those three vertices.
void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

//The CalculateNormal function takes in the tangent and binormal and then does a cross product to give back the normal vector.
void ModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}

//GetIndexCount returns the number of indexes in the model.The color shader will need this information to draw this model.
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}
//The InitializeBuffers function is where we handle creating the vertex and index buffers.Usually you would read in a model and create the buffers from that data file.For this project we will just set the points in the vertex and index buffer manually since it is only a single triangle.

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	////First create two temporary arrays to hold the vertex and index data that we will use later to populate the final buffers with.

	//// Set the number of vertices in the vertex array.
	//m_vertexCount = 3;

	//// Set the number of indices in the index array.
	//m_indexCount = 3;


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
	//Now fill both the vertex and index array with the three points of the triangle as well as the index to each of the points.Please note that I create the points in the clockwise order of drawing them.If you do this counter clockwise it will think the triangle is facing the opposite direction and not draw it due to back face culling.Always remember that the order in which you send your vertices to the GPU is very important.The color is set here as well since it is part of the vertex description.I set the color to green.


	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].tangent = XMFLOAT3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertices[i].binormal = XMFLOAT3(m_model[i].bx, m_model[i].by, m_model[i].bz);

		indices[i] = i;
	}



	// Load the vertex array with data.
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top left.
	//vertices[1].texture = XMFLOAT2(0.5f, 0.0f);
	//vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Top right.
	//vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	//
	//vertices[3].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[3].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[4].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top left.
	//vertices[4].texture = XMFLOAT2(0.5f, 0.0f);
	//vertices[4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[5].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Top right.
	//vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[5].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[3].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[3].texture = XMFLOAT2(0.0f, 1.0f);


	//// Load the index array with data.
	//indices[0] = 0;  // Top left.
	//indices[1] = 1;  // Bottom left.
	//indices[2] = 2;  // Top right.
	//
	//indices[4] = 5;  // Top left.
	//indices[5] = 4;  // Bottom left.
	//indices[6] = 3;  // Top right.


	//
	//indices[3] = 2;  // Bottom left.
	//indices[4] = 3;  // Bottom right.
	//indices[5] = 0;  // Top right.
	
//	
//	//Cube
//	/*		   
//	  (-1,1,1) 6 ____________  4(1,1,1)
//				/|			/|
//			   / |		   / |
//			  /	 |		  /  |
//(-1,1,-1) 1/___|_______/2  | (1,1,-1)
//			 |   |_______|___|
// (-1,-1,1) | 7 /       |   /  5(1,-1,1)
//			 |	/    	 |  /
//			 | /		 | /
//			 |/__________|/
//		0(-1,-1,-1)			3(1,-1,-1)
//
//
//	*/
//	// Load the vertex array with data.
//	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);  // Bottom left.
//	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
//
//	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  // Top left.
//	vertices[1].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
//
//	vertices[2].position = XMFLOAT3(1.0f, 1.0f, -1.0f);  // Top right.
//	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
//
//	vertices[3].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  // Bottom right.
//	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
//	
//	vertices[4].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  // Bottom left.
//	vertices[4].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
//
//	vertices[5].position = XMFLOAT3(1.0f, -1.0f, 1.0f);  // Top left.
//	vertices[5].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
//
//	vertices[6].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);  // Top right.
//	vertices[6].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
//
//	vertices[7].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);  // Bottom right.
//	vertices[7].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
//
//
//	// Load the index array with data.
//	//Front
//	indices[0] = 0;  // Bottom left.
//	indices[1] = 1;  // Top left.
//	indices[2] = 2;  // Top right.
//	
//	indices[3] = 2;  // Top right.
//	indices[4] = 3;  // Bottom right.
//	indices[5] = 0;  // Bottom left.
//
//	
//	//Right
//	indices[6] = 3;  // Bottom left.
//	indices[7] = 2;  // Top left.
//	indices[8] = 4;  // Top right.
//
//	indices[9] = 4;  // Top right.
//	indices[10] = 5;  // Bottom right.
//	indices[11] = 3;  // Bottom left.
//
//	//Back
//	indices[12] = 5;  // Bottom left.
//	indices[13] = 4;  // Top left.
//	indices[14] = 6;  // Top right.
//
//	indices[15] = 6;  // Top right.
//	indices[16] = 7;  // Bottom right.
//	indices[17] = 5;  // Bottom left.
//
//	//Left
//	indices[18] = 7;  // Bottom left.
//	indices[19] = 6;  // Top left.
//	indices[20] = 1;  // Top right.
//
//	indices[21] = 1;  // Top right.
//	indices[22] = 0;  // Bottom right.
//	indices[23] = 7;  // Bottom left.
//
//	//Top
//	indices[24] = 1;  // Bottom left.
//	indices[25] = 6;  // Top left.
//	indices[26] = 4;  // Top right.
//
//	indices[27] = 4;  // Top right.
//	indices[28] = 2;  // Bottom right.
//	indices[29] = 1;  // Bottom left.
//
//	//Bottom
//	indices[30] = 7;  // Bottom left.
//	indices[31] = 0;  // Top left.
//	indices[32] = 3;  // Top right.
//
//	indices[33] = 0;  // Top right.
//	indices[34] = 3;  // Bottom right.
//	indices[35] = 5;  // Bottom left.
//
//
//


	//With the vertex array and index array filled out we can now use those to create the vertex buffer and index buffer.Creating both buffers is done in the same fashion.First fill out a description of the buffer.In the description the ByteWidth(size of the buffer) and the BindFlags(type of buffer) are what you need to ensure are filled out correctly.After the description is filled out you need to also fill out a subresource pointer which will point to either your vertex or index array you previously created.With the description and subresource pointer you can call CreateBuffer using the D3D device and it will return a pointer to your new buffer.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	//After the vertex buffer and index buffer have been created you can delete the vertex and index arrays as they are no longer needed since the data was copied into the buffers.

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}





//The ShutdownBuffers function just releases the vertex buffer and index buffer that were created in the InitializeBuffers function.

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}
//RenderBuffers is called from the Render function.The purpose of this function is to set the vertex buffer and index buffer as active on the input assembler in the GPU.Once the GPU has an active vertex buffer it can then use the shader to render that buffer.This function also defines how those buffers should be drawn such as triangles, lines, fans, and so forth.In this project we set the vertex buffer and index buffer as active on the input assembler and tell the GPU that the buffers should be drawn as triangles using the IASetPrimitiveTopology DirectX function.

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}



//bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR * filename)
//{
//	bool result;
//
//
//	// Create the texture object.
//	m_Texture = new TextureClass;
//	if (!m_Texture)
//	{
//		return false;
//	}
//
//	// Initialize the texture object.
//	result = m_Texture->Initialize(device, deviceContext, filename);
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}

bool ModelClass::LoadTextures(ID3D11Device* device, const WCHAR* filename1, const WCHAR* filename2, const WCHAR * filename3)
{
	bool result;


	// Create the texture array object.
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}



	// Initialize the texture array object.
	result = m_TextureArray->Initialize(device, filename1, filename2, filename3);
	if (!result)
	{
		return false;
	}

	return true;
}

//void ModelClass::ReleaseTexture()
//{
//	// Release the texture object.
//	if (m_Texture)
//	{
//		m_Texture->Shutdown();
//		delete m_Texture;
//		m_Texture = 0;
//	}
//
//	return;
//}

void ModelClass::ReleaseTextures()
{
	// Release the texture array object.
	if (m_TextureArray)
	{
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}

	return;
}

bool ModelClass::LoadModel(const WCHAR * filename)
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

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}