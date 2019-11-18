#pragma once


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <stdio.h>
//#include "System/VertexBuffer.h"
//#include "System/IndexBuffer.h"
#include "terraincellclass.h"
#include "frustumclass.h"
using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
		XMFLOAT3 color;
	};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
		float r, g, b;
	};

	struct VectorType
	{
		float x, y, z;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass& other);
	~TerrainClass();

	bool Initialize(ID3D11Device* device, const char* setupFilename);
	void Shutdown();
	void Frame();
	//bool Render(ID3D11DeviceContext*);

	//int GetIndexCount();
	bool RenderCell(ID3D11DeviceContext* deviceContext, int cellId, FrustumClass* Frustum);
	void RenderCellLines(ID3D11DeviceContext* deviceContext, int cellId);

	int GetCellIndexCount(int cellId);
	int GetCellLinesIndexCount(int cellId);
	int GetCellCount();

	int GetRenderCount();
	int GetCellsDrawn();
	int GetCellsCulled();

	bool GetHeightAtPosition(float inputX, float inputZ, float& height);

private:
	bool LoadSetupFile(const char* filename);
	bool LoadBitmapHeightMap();
	bool LoadRawHeightMap();
	void ShutdownHeightMap();
	void SetTerrainCoordinates();
	bool CalculateNormals();
	bool LoadColorMap();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();

	void CalculateTerrainVectors();
	void CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent, VectorType& binormal);

	bool LoadTerrainCells(ID3D11Device* device);
	void ShutdownTerrainCells();
	bool CheckHeightOfTriangle(float x, float z, float& height, float v0[3], float v1[3], float v2[3]);
	//bool InitializeBuffers(ID3D11Device*);
	//void ShutdownBuffers();
	//void RenderBuffers(ID3D11DeviceContext*);

private:
	//ID3D11Buffer /** m_vertexBuffer,*/ *m_indexBuffer;
	//IndexBuffer* m_indexBuffer;
	//VertexBuffer<VertexType> *m_vertexBuffer;
	int m_vertexCount;
		//, m_indexCount;

	int m_terrainHeight, m_terrainWidth;
	float m_heightScale;
	char *m_terrainFilename, *m_colorMapFilename;

	HeightMapType* m_heightMap;
	ModelType* m_terrainModel;
	
	TerrainCellClass* m_TerrainCells;

	int m_cellCount, m_renderCount, m_cellsDrawn, m_cellsCulled;
};

