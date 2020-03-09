///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "fontclass.h"

FontClass::FontClass()
{
	m_Font = 0;
	m_DDSTexture = 0;
}
FontClass::FontClass(const FontClass& other)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, const WCHAR * fontFilename, const WCHAR * textureFilename, float fontHeight, int spaceSize)
{
	bool result;


	// Store the height of the font.
	m_fontHeight = fontHeight;

	// Store the size of spaces in pixel size.
	m_spaceSize = spaceSize;

	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();

	return;
}

//The LoadFontData function is where we load the fontdata.txt file which contains the indexing information for the texture.
bool FontClass::LoadFontData(const WCHAR * filename)
{
	ifstream fin;
	int i;
	char temp;
	
	//First we create an array of the FontType structure. 
	//The size of the array is set to 95 as that is the number of characters in the texture and hence the number of indexes in the fontdata.txt file.

	// Create the font spacing buffer.
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	//Now we open the fileand read each line into the array m_Font.
	//We only need to read in the texture TU leftand right coordinates as well as the pixel size of the character.
	
	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

//The ReleaseFontData function releases the array that holds the texture indexing data.
void FontClass::ReleaseFontData()
{
	// Release the font data array.
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}

	return;
}

//The LoadTexture function reads in the font.dds file into the texture shader resource.
//This will be the texture we take the characters fromand write them to their own square polygons for rendering.

bool FontClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_DDSTexture = new DDSTextureClass;
	if (!m_DDSTexture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_DDSTexture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_DDSTexture)
	{
		m_DDSTexture->Shutdown();
		delete m_DDSTexture;
		m_DDSTexture = 0;
	}

	return;
}

//GetTexture returns the font texture interface so the font graphics can be rendered.

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_DDSTexture->GetTexture();
}

//BuildVertexArray will be called by the TextClass to build vertex buffers out of the text sentences it sends to this function as input.
//This way each sentence in the TextClass that needs to be drawn has its own vertex buffer that can be rendered easily after being created.
//The vertices input is the pointer to the vertex array that will be returned to the TextClass once it is built.
//The sentence input is the text sentence that will be used to create the vertex array.
//The drawX and drawY input variables are the screen coordinates of where to draw the sentence.
void FontClass::BuildVertexArray(void* vertices, const char * sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;
	
	//The following loop will now build the vertexand index arrays.
	//It takes each character from the sentenceand creates two triangles for it.
	//It then maps the character from the font texture onto those two triangles using the m_Font array which has the TU texture coordinatesand pixel size.
	//Once the polygon for that character has been created it then updates the X coordinate on the screen of where to draw the next character.

	// Draw each letter onto a quad.
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);

			vertexPtr[index+1].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index+1].texture = XMFLOAT2(m_Font[letter].right, 1.0f);

			vertexPtr[index+2].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index+2].texture = XMFLOAT2(m_Font[letter].left, 1.0f);

			// Second triangle in quad.
			vertexPtr[index+3].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index+3].texture = XMFLOAT2(m_Font[letter].left, 0.0f);

			vertexPtr[index+4].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index+4].texture = XMFLOAT2(m_Font[letter].right, 0.0f);

			vertexPtr[index+5].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index+5].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			
			index+=6;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}

	return;
}

int FontClass::GetSentencePixelLength(char* sentence)
{
	int pixelLength, numLetters, i, letter;


	pixelLength = 0;
	numLetters = (int)strlen(sentence);

	for (i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then count it as three pixels.
		if (letter == 0)
		{
			pixelLength += m_spaceSize;
		}
		else
		{
			pixelLength += (m_Font[letter].size + 1);
		}
	}

	return pixelLength;
}

int FontClass::GetFontHeight()
{
	return (int)m_fontHeight;
}