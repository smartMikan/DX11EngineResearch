#pragma once

//The TextClass handles all the 2D text drawing that the application will need. 
//It renders 2D text to the screen and uses the FontClass and FontShaderClass to assist it in doing so.
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "shadermanagerclass.h"
//#include "fontshaderclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass
{
private:
	//SentenceType is the structure that holds the rendering information for each text sentence.

	struct SentenceType
	{
		ID3D11Buffer* vertexBuffer, * indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};
	//The VertexType must match the one in the FontClass.

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	//bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, XMMATRIX baseViewMatrix);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, int, bool, FontClass*, const char*, int, int, float, float, float);

	void Shutdown();
	//bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix);
	void Render(ID3D11DeviceContext*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

	//bool SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext);
	//bool SetRenderCount(int count, ID3D11DeviceContext* deviceContext);
	//bool SetFps(int, ID3D11DeviceContext*);
	//bool SetCpu(int, ID3D11DeviceContext*);
	//bool UpdateSentence(SentenceType* sentence, const char * text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext);
	bool UpdateSentence(ID3D11DeviceContext*, FontClass*, const char*, int, int, float, float, float);


private:
	//bool InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);
	bool InitializeSentence(ID3D11Device*, ID3D11DeviceContext*, FontClass*, const char*, int, int, float, float, float);

	void ReleaseSentence(SentenceType** sentence);
	//bool RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX worldMatrix, XMMATRIX orthoMatrix);
	void RenderSentence(ID3D11DeviceContext*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	//FontClass* m_Font;
	//FontShaderClass* m_FontShader;
	
	//XMMATRIX m_baseViewMatrix;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_vertexBuffer2, *m_indexBuffer2;
	int m_screenWidth, m_screenHeight, m_maxLength, m_vertexCount, m_indexCount;
	bool m_shadow;
	XMFLOAT4 m_pixelColor;

	//We will use two sentences in this project.

	/*SentenceType* m_sentence1;
	SentenceType* m_sentence2;
	SentenceType* m_sentence3;
	SentenceType* m_sentence4;
	SentenceType* m_sentence5;
	SentenceType* m_sentence6;*/
};

#endif
