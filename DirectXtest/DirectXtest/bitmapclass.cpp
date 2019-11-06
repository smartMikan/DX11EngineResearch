#include "bitmapclass.h"

BitmapClass::BitmapClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}

BitmapClass::BitmapClass(const BitmapClass&)
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	bool result;
	//In the Initialize function both the screen sizeand image size are stored.
	//These will be required for generating exact vertex locations during rendering.
	//Note that the pixels of the image do not need to be exactly the same as the texture that is used, 
	//you can set this to any sizeand use any size texture you want also.

	// Store the screen size.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the size in pixels that this bitmap should be rendered at.
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	//The previous rendering location is first initialized to negative one.
	//This will be an important variable that will locate where it last drew this image.
	//If the image location hasn't changed since last frame then it won't modify the dynamic vertex buffer which will save us some cycles.

	// Initialize the previous rendering position to negative one.
	m_previousPosX = -1;
	m_previousPosY = -1;

	//The buffers are then createdand the texture for this bitmap image is also loaded in.
	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

//The Shutdown function will release the vertexand index buffers as well as the texture that was used for the bitmap image.

void BitmapClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

//Render puts the buffers of the 2D image on the video card.
//As input it takes the position of where to render the image on the screen.
//The UpdateBuffers function is called with the position parameters.
//If the position has changed since the last frame it will then update the location of the vertices in the dynamic vertex buffer to the new location.
//If not it will skip the UpdateBuffers function.
//After that the RenderBuffers function will prepare the final vertices / indices for rendering.

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result;

	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	result = UpdateBuffers(deviceContext, positionX, positionY);
	if (!result)
	{
		return false;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}

//GetIndexCount returns the number of indexes for the 2D image. This will pretty much always be six.
int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

//The GetTexture function returns a pointer to the texture resource for this 2D image.
//The shader will call this function so it has access to the image when drawing the buffers.
ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

//InitializeBuffers is the function that is used to build the vertex and index buffer that will be used to draw the 2D image.
bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//We set the vertices to six since we are making a square out of two triangles, 
	//so six points are needed.
	//The indices will be the same.
	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

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

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Load the index array with data.
	for (i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}
	//Here is the big change in comparison to the ModelClass.
	//We are now creating a dynamic vertex buffer so we can modify the data inside the vertex buffer each frame if we need to.
	//To make it dynamic we set Usage to D3D11_USAGE_DYNAMICand CPUAccessFlags to D3D11_CPU_ACCESS_WRITE in the description.
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

	//We don't need to make the index buffer dynamic since the six indices will always point to the same six vertices even though the coordinates of the vertex may change.

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

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext*, int, int)
{
	return false;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext*)
{
}

bool BitmapClass::LoadTexture(ID3D11Device*, WCHAR*)
{
	return false;
}

void BitmapClass::ReleaseTexture()
{
}
