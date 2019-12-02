#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}


bool Model::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_device = device;
	this->m_deviceContext = deviceContext;

	try
	{
		if (!this->LoadModel(filePath))
			return false;
	}
	catch (const std::exception & exception)
	{
		return false;
	}

}


void Model::Draw(ShaderManagerClass* shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor)
{
	
	//VsSetConstantbuffer
	for (int i = 0; i < m_meshes.size(); i++)
	{
		//UpdateConstanBuffer
		shaderManager->DrawSetWithMyShader(this->m_deviceContext, m_meshes[i].GetTransformMatrix() * worldMatrix, viewMatrix, projectionMatrix, cameraPosition, ambientColor, diffuseColor, lightDirection, specularPower, specularColor);
		m_meshes[i].Draw();
	}
}

bool Model::LoadModel(const std::string& filePath)
{
	this->directory = StringHelper::GetDirectoryFromPath(filePath);

	Assimp::Importer importer;

	

	const aiScene* pScene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr)
		return false;

	this->ProcessNode(pScene->mRootNode, pScene,DirectX::XMMatrixIdentity());


	return true;
}


void Model::ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
{
	XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(this->ProcessMesh(mesh, scene, nodeTransformMatrix));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene, nodeTransformMatrix);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix) {

	std::vector<Mesh::VertexType> vertices;
	std::vector<DWORD> indices;
	std::vector<int> bongHierarchy;
	std::vector<XMFLOAT4X4> boneOffsets;
	std::map<string, AnimationClip> animations;
	AnimationClip animationclips;


	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::VertexType vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.textureCoordinate.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.textureCoordinate.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		vertices.push_back(vertex);
	}



	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	for (UINT i = 0; i < mesh->mNumBones; i++) {
		XMFLOAT4X4 offset;
		XMStoreFloat4x4(&offset,XMMatrixTranspose(XMMATRIX(&mesh->mBones[i]->mOffsetMatrix.a1)));
		boneOffsets.push_back(offset);
	}
	
	for (UINT i = 0; i < mesh->mNumAnimMeshes; i++) {
		
	}
	
	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	return Mesh(this->m_device, this->m_deviceContext, vertices, indices, textures, transformMatrix,bongHierarchy, boneOffsets, animations);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, const aiScene* scene)
{
	std::vector<Texture> materialTextures;
	TextureStorageType storetype = TextureStorageType::Invalid;
	unsigned int textureCount = material->GetTextureCount(textureType);

	//if no texture
	if (textureCount == 0)
	{
		storetype = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);

		switch (textureType)
		{
		case aiTextureType_DIFFUSE:
			material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack()) //use grey if color is black
			{
				materialTextures.push_back(Texture(this->m_device, MyColors::UnloadedTextureColor, textureType));
				return materialTextures;
			}
			materialTextures.push_back(Texture(this->m_device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
			return materialTextures;
		}
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			material->GetTexture(textureType, i, &path);
			storetype = DetermineTextureStorageType(scene, material, i, textureType);
			switch (storetype)
			{
			case TextureStorageType::EmbeddedIndexCompressed:
			{
				int index = GetTextureIndex(&path);
				Texture embeddedIndexedTexture(this->m_device, 
											   reinterpret_cast<uint8_t*>(scene->mTextures[index]->pcData),
											   scene->mTextures[index]->mWidth,
											   textureType);
				break;
			}

			case TextureStorageType::EmbeddedCompressed: 
			{
				const aiTexture* pTexture = scene->GetEmbeddedTexture(path.C_Str());
				Texture embeddedTexture(this->m_device, reinterpret_cast<uint8_t*>(pTexture->pcData),
					pTexture->mWidth, textureType);
				materialTextures.push_back(embeddedTexture);
				break;
			}

			case TextureStorageType::Disk:
			{
				std::string filename = this->directory + '/' + path.C_Str();
				Texture diskTexture(this->m_device, filename, textureType);
				materialTextures.push_back(diskTexture);
				break;
			}
			}
		}
	}

	if (materialTextures.size() == 0)
	{
		materialTextures.push_back(Texture(this->m_device, MyColors::UnhandledTextureColor, textureType));
	}
	return materialTextures;

}

TextureStorageType Model::DetermineTextureStorageType(const aiScene* scene, aiMaterial* mat, unsigned int index, aiTextureType textureType)
{
	if (mat->GetTextureCount(textureType) == 0)
	{
		return TextureStorageType::None;
	}

	aiString path;
	mat->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();

	//check if texture is an index embedded texture
	if (texturePath[0] == '*') {
		if (scene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			assert("SUPPERT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}

	//check if texture is an embedded texture but not indexed  
	if (auto texture = scene->GetEmbeddedTexture(texturePath.c_str())) {
		if (texture->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			assert("SUPPERT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}

	//check if texture is a filepath by checking for period before extension name
	if (texturePath.find('.') != std::string::npos) {
		return TextureStorageType::Disk;
	}

	return TextureStorageType::None; //No texture exists
}

int Model::GetTextureIndex(aiString* pStr)
{
	assert(pStr->length >= 2);
	return atoi(&pStr->C_Str()[1]);
}
