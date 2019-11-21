#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include "textureclass.h"
using namespace DirectX;

class ParticleSystemClass
{
public:
	struct ParticleState
	{
		float particleDeviationX, particleDeviationY, particleDeviationZ;
		float particleVelocity, particleVelocityVariation;
		float particleSize, particlesPerSecond;
	};

private:
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};
public:
	ParticleSystemClass();
	ParticleSystemClass(const ParticleSystemClass&);
	~ParticleSystemClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const WCHAR*);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

	bool SetParticleProperty(float particleDeviationX, float particleDeviationY, float particleDeviationZ, float particleVelocity, float particleVelocityVariation, float particleSize, float m_particlesPerSecond);
	bool SetParticleProperty(ParticleState* state);
	bool SetParticleProperty();

private:
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const WCHAR*);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*);

	void RenderBuffers(ID3D11DeviceContext*);

private:
	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;
	int m_currentParticleCount;
	float m_accumulatedTime;

	TextureClass* m_Texture;

	ParticleType* m_particleList;
	
	int m_vertexCount, m_indexCount;
	VertexType* m_vertices;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;


	ParticleState* m_DefaultState;
};

