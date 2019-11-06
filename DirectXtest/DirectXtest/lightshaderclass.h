#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include<directxmath.h>
#include<d3dcompiler.h>
#include <fstream>
using namespace std;
using namespace DirectX;

//////////////////////////////////////////////////
// Class name: LightShaderClass
//////////////////////////////////////////////////
class LightShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	

	//We add a new camera buffer structure to match the new camera constant buffer in the vertex shader.
	//Note we add a padding to make the structure size a multiple of 16 to prevent CreateBuffer failing when using sizeof with this structure.
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	//The new LightBufferType structure will be used for holding lighting information.
	//This typedef is the same as the new typedef in the pixel shader.
	//Do note that I add a extra float for size padding to ensure the structure is a multiple of 16. 
	//Since the structure without an extra float is only 28 bytes CreateBuffer would have failed 
	//if we used a sizeof(LightBufferType) because it requires sizes that are a multiple of 16 to succeed.

	//The LightBufferType has been modified to hold a specular color and specular power to match the light constant buffer in the pixel shader.
	//Pay attention to the fact that I placed the specular power by the light direction to form a 4 float slot instead of using padding 
	//so that the structure could be kept in multiples of 16 bytes.
	//Also had specular power been placed last in the structure and no padding used beneath light direction then the shader would not have functioned correctly.
	//This is because even though the structure was a multiple of 16 the individual slots themselves were not aligned logically to 16 bytes each.

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		
		//float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
		float specularPower;
		XMFLOAT4 specularColor;
	};

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, ID3D11ShaderResourceView*, MatrixBufferType, CameraBufferType, LightBufferType);

	MatrixBufferType GenarateMatrixBuffer(XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	CameraBufferType GenerateCameraBuffer(XMFLOAT3 cameraPosition, float padding);
	LightBufferType GenerateLightBuffer(XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, MatrixBufferType, ID3D11ShaderResourceView*, CameraBufferType, LightBufferType);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	
	//There is a new private constant buffer for the light information(color and direction).
	//The light buffer will be used by this class to set the global light variables inside the HLSL pixel shader.

	ID3D11Buffer* m_cameraBuffer;

	ID3D11Buffer* m_lightBuffer;
};

#endif



///Specular lighting
//Specular lighting is most commonly used to give light reflection off of metallic surfaces such as mirrors and highly polished / reflective metal surfaces.It is also used on other materials such as reflecting sunlight off of water.Used well it can add a degree of photo realism to most 3D scenes.
//
//The equation for specular lighting is the following :
//
//SpecularLighting = SpecularColor * (SpecularColorOfLight * ((NormalVector dot HalfWayVector) power SpecularReflectionPower) * Attentuation * Spotlight)
//We will modify the equation to produce just the basic specular lighting effect as follows :
//
//SpecularLighting = SpecularLightColor * (ViewingDirection dot ReflectionVector) power SpecularReflectionPower
//The reflection vector in this equation has to be produced by multiplying double the light intensity by the vertex normal.The direction of the light is subtracted which then gives the reflection vector between the light source and the viewing angle :
//
//ReflectionVector = 2 * LightIntensity * VertexNormal - LightDirection
//The viewing direction in the equation is produced by subtracting the location of the camera by the position of the vertex :
//
//ViewingDirection = CameraPosition - VertexPosition