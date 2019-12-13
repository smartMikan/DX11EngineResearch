#include "AssimpLoader.h"


bool AssimpLoader::LoadAssimp(const string & FileName, vector<PosNormalTexTanVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats)
{
	this->directory = StringHelper::GetDirectoryFromPath(FileName);

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(FileName,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);


	if (pScene == nullptr)
		return false;

	UINT numMaterials = 0;
	UINT numVertices = 0;
	UINT numTriangles = 0;
	UINT numBones = 0;
	UINT numAnimationsClips = 0;

	ReadMaterials(pScene, numMaterials, mats);
	ReadSubsetTable(pScene, numMaterials, subsets);
	ReadVertices(pScene, numVertices, vertices);
	ReadTriangles(pScene, numTriangles, indices);
	return true;
}

bool AssimpLoader::LoadAssimp(const string & FileName, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats, SkinnedDataClass & skinInfo)
{
	this->directory = StringHelper::GetDirectoryFromPath(FileName);

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(FileName,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);


	if (pScene == nullptr)
		return false;


	UINT numMaterials = 0;
	UINT numVertices = 0;
	UINT numTriangles = 0;
	UINT numBones = 0;
	UINT numAnimationsClips = 0;

	vector<XMFLOAT4X4> boneOffets;
	vector<int> boneToParentIndex;
	map<string, AnimationClip> animations;

	ProcessNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity(),vertices,indices,subsets,mats,skinInfo);




	

	ReadMaterials(pScene, numMaterials, mats);
	ReadSubsetTable(pScene, numMaterials, subsets);
	ReadSkinnedVertices(pScene, numVertices, vertices);
	ReadTriangles(pScene, numTriangles, indices);
	ReadBoneOffets(pScene, numBones, boneOffets);
	ReadBoneHierarchy(pScene, numBones, boneToParentIndex);
	ReadAnimationClips(pScene, numBones, numAnimationsClips, animations);

	skinInfo.Set(boneToParentIndex, boneOffets, animations);
	return true;
}



void AssimpLoader::ReadMaterials(const aiScene * aiscene, UINT numMaterials, vector<aiMaterial>& mats)
{


	
}

void AssimpLoader::ReadSubsetTable(const aiScene* aiscene, UINT numSubsets, vector<MeshGeometryClass::Subset>& subsets)
{
}

void AssimpLoader::ReadVertices(const aiScene* aicene, UINT numVertices, vector<PosNormalTexTanVertex>& vertices)
{
}

void AssimpLoader::ReadSkinnedVertices(const aiScene* aicene, UINT numVertices, vector<PosNormalTexTanSkinnedVertex>& vertices)
{

}

void AssimpLoader::ReadTriangles(const aiScene* aicene, UINT numTriangles, vector<unsigned long>& indices)
{
}

void AssimpLoader::ReadBoneOffets(const aiScene* aicene, UINT numBones, vector<XMFLOAT4X4>& boneOffsets)
{
}

void AssimpLoader::ReadBoneHierarchy(const aiScene* aicene, UINT numBones, vector<int>& boneIndexToParentIndex)
{
}

void AssimpLoader::ReadAnimationClips(const aiScene* aicene, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations)
{
}

void AssimpLoader::ReadBoneKeyFrame(const aiScene* aicene, UINT numBones, BoneAnimation & boneAnimation)
{
}



void AssimpLoader::ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats, SkinnedDataClass & skinInfo)
{
	XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, nodeTransformMatrix, i, vertices, indices, subsets, mats, skinInfo);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene, nodeTransformMatrix, vertices, indices, subsets, mats, skinInfo);
	}
}




void AssimpLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix, UINT meshID, vector<PosNormalTexTanSkinnedVertex>& vertices, vector<unsigned long>& indices, vector<MeshGeometryClass::Subset>& subsets, vector<aiMaterial>& mats, SkinnedDataClass & skinInfo)
{
	



	//for (size_t i = 0; i < mesh->mNumBones; i++)
	//{
	//	scene->mRootNode->FindNode(mesh->mBones[i]->mName);
	//}
	//

	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		PosNormalTexTanSkinnedVertex vertex;
		vertex.Pos.x = mesh->mVertices[i].x;
		vertex.Pos.y = mesh->mVertices[i].y;
		vertex.Pos.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.Tex.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.Tex.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertex.TangentU.x = mesh->mTangents[i].x;
		vertex.TangentU.y = mesh->mTangents[i].y;
		vertex.TangentU.z = mesh->mTangents[i].z;
		vertex.TangentU.w = -1;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;


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
	/*for (UINT i = 0; i < mesh->mNumBones; i++) {
	XMFLOAT4X4 offset;
	XMStoreFloat4x4(&offset,XMMatrixTranspose(XMMATRIX(&mesh->mBones[i]->mOffsetMatrix.a1)));
	boneOffsets.push_back(offset);
	}*/
	/*const aiMesh* mesh = mesh;*/
	// calculate bone matrices
	std::vector<aiMatrix4x4> boneMatrices(mesh->mNumBones);
	for (size_t a = 0; a < mesh->mNumBones; ++a)
	{
		const aiBone* bone = mesh->mBones[a];

		// find the corresponding node by again looking recursively through the node hierarchy for the same name
		aiNode* node = FindNodeRecursivelyByName(modelScene->mRootNode, bone->mName);

		// start with the mesh-to-bone matrix 
		boneMatrices[a] = bone->mOffsetMatrix;
		// and now append all node transformations down the parent chain until we're back at mesh coordinates again
		const aiNode* tempNode = node;
		while (tempNode)
		{
			boneMatrices[a] *= tempNode->mTransformation;   // check your matrix multiplication order here!!!
			tempNode = tempNode->mParent;
		}
	}


	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
	

}

std::vector<Texture> AssimpLoader::LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, const aiScene* scene)
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

TextureStorageType AssimpLoader::DetermineTextureStorageType(const aiScene* scene, aiMaterial* mat, unsigned int index, aiTextureType textureType)
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

int AssimpLoader::GetTextureIndex(aiString* pStr)
{
	assert(pStr->length >= 2);
	return atoi(&pStr->C_Str()[1]);
}




