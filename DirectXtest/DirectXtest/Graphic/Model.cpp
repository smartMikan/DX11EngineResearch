#include "Model.h"

static DirectX::XMMATRIX AiToDxMatrix(const aiMatrix4x4 aimatrix) 
{
	return DirectX::XMMatrixTranspose(DirectX::XMMATRIX(&aimatrix.a1));
}


bool Model::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
	ConstantBuffer<CB_VS_MatrixBuffer>& wvpMatrix, ConstantBuffer<CB_PS_Material>& cb_ps_material, IVertexShader* pVertexShader)
{
	this->m_device = device;
	this->m_deviceContext = deviceContext;
	this->m_wvpMatrixBuffer = &wvpMatrix;
	this->m_cb_ps_material = &cb_ps_material;
	this->pVertexShader = pVertexShader;


	try
	{
		if (!this->LoadModel(filePath))
			return false;
	}
	catch (COMException & exception)
	{
		ErrorLoger::Log(exception);
		return false;
	}
	return true;
}



void Model::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix)
{
	//VsSetConstantbuffer
	this->m_deviceContext->VSSetConstantBuffers(0, 1, this->m_wvpMatrixBuffer->GetAddress());

	this->m_wvpMatrixBuffer->data.view = XMMatrixTranspose(viewMatrix);
	this->m_wvpMatrixBuffer->data.projection = XMMatrixTranspose(projectionMatrix);
	for (int i = 0; i < m_meshes.size(); i++)
	{
		this->m_wvpMatrixBuffer->data.world = XMMatrixTranspose(m_meshes[i].GetTransformMatrix() * worldMatrix); 
		this->m_wvpMatrixBuffer->ApplyChanges();
		m_meshes[i].Draw(pVertexShader);
	}
}

bool Model::InitAnimation(ConstantBuffer<ConstantBuffer_Bones>* cbufBone, Animator* animator_out, AnimationComponent* animComp)
{
	ProcessAnimation(animator_out, cbufBone, animComp);
	return true;
}

bool Model::AddAnimation(const std::string & filePath, Animator * animator_out, AnimationComponent * animComp, bool disablerootTrans = false, bool disablerootRot = false, bool disablerootScale = false)
{
	this->directory = StringHelper::GetDirectoryFromPath(filePath);

	m_pScene = m_Importer.ReadFile(filePath,
		aiProcessPreset_TargetRealtime_Fast |
		aiProcess_ConvertToLeftHanded | aiProcess_TransformUVCoords/*|aiProcess_JoinIdenticalVertices*/);
	if (m_pScene == nullptr)
		return false;

	for (unsigned int anim_index = 0; anim_index < m_pScene->mNumAnimations; anim_index++)
	{
		aiAnimation* pAnimation = m_pScene->mAnimations[anim_index];

		float ticks_per_second = (float)pAnimation->mTicksPerSecond;
		if (directory.find(".gltf") != std::string::npos || directory.find(".glb") != std::string::npos)
		{
			ticks_per_second = 1000.0f;
		}
		if (ticks_per_second == 0.0f)
		{
			ticks_per_second = 1.0f;
		}

		AnimationClip animation;
		animation.name = pAnimation->mName.C_Str();
		animation.channels.resize(pAnimation->mNumChannels);
		animation.duration = (float)pAnimation->mDuration / ticks_per_second;

		for (unsigned int channel_index = 0; channel_index < pAnimation->mNumChannels; channel_index++)
		{
			aiNodeAnim* pNodeAnim = pAnimation->mChannels[channel_index];
			int node_index = FindNodeByName(pNodeAnim->mNodeName.C_Str());
			if (node_index == -1)
			{
				COM_ERROR_IF_FAILED(-1, "Missing animated node.");
				continue;
			}

			AnimationChannel& channel = animation.channels[channel_index];
			channel.node_index = node_index;
			channel.channel_name = pNodeAnim->mNodeName.C_Str();


			if (disablerootTrans && channel.channel_name == m_Bones[0].name)
			{
				//XMFLOAT4X4 temp;
				//XMStoreFloat4x4(&temp, m_AllNodeAvator[node_index].local_transform);
				channel.position_keyframes.reserve(1);
				const aiVectorKey& ai_key = pNodeAnim->mPositionKeys[0];
				PositionKeyFrame keyframe;
				keyframe.timePos = (float)ai_key.mTime / ticks_per_second;
				keyframe.value.x = ai_key.mValue.x;
				keyframe.value.y = ai_key.mValue.y;
				keyframe.value.z = ai_key.mValue.z;
				channel.position_keyframes.push_back(keyframe);
				animation.rootChannel = channel;
			}
			else
			{
				channel.position_keyframes.reserve(pNodeAnim->mNumPositionKeys);
				for (unsigned int keyframe_index = 0; keyframe_index < pNodeAnim->mNumPositionKeys; keyframe_index++)
				{
					const aiVectorKey& ai_key = pNodeAnim->mPositionKeys[keyframe_index];

					PositionKeyFrame keyframe;
					keyframe.timePos = (float)ai_key.mTime / ticks_per_second;
					keyframe.value.x = ai_key.mValue.x;
					keyframe.value.y = ai_key.mValue.y;
					keyframe.value.z = ai_key.mValue.z;

					channel.position_keyframes.push_back(keyframe);
				}
			}
			if (disablerootRot && channel.channel_name == m_Bones[0].name) {
				channel.rotation_keyframes.reserve(1);

				const aiQuatKey& ai_quatkey = pNodeAnim->mRotationKeys[0];

				RotationKeyFrame quatkeyframe;
				quatkeyframe.timePos = (float)ai_quatkey.mTime / ticks_per_second;
				quatkeyframe.value.x = ai_quatkey.mValue.x;
				quatkeyframe.value.y = ai_quatkey.mValue.y;
				quatkeyframe.value.z = ai_quatkey.mValue.z;
				quatkeyframe.value.w = ai_quatkey.mValue.w;
				channel.rotation_keyframes.push_back(quatkeyframe);
				}
			else 
			{
				channel.rotation_keyframes.reserve(pNodeAnim->mNumRotationKeys);
				for (unsigned int keyframe_index = 0; keyframe_index < pNodeAnim->mNumRotationKeys; keyframe_index++)
				{
					const aiQuatKey& ai_key = pNodeAnim->mRotationKeys[keyframe_index];

					RotationKeyFrame keyframe;
					keyframe.timePos = (float)ai_key.mTime / ticks_per_second;
					keyframe.value.x = ai_key.mValue.x;
					keyframe.value.y = ai_key.mValue.y;
					keyframe.value.z = ai_key.mValue.z;
					keyframe.value.w = ai_key.mValue.w;

					channel.rotation_keyframes.push_back(keyframe);
				}
			}
				
			if (disablerootScale && channel.channel_name == m_Bones[0].name) 
			{
				channel.scale_keyframes.reserve(1);
				
				const aiVectorKey& ai_key = pNodeAnim->mScalingKeys[0];

				ScaleKeyFrame keyframe;
				keyframe.timePos = (float)ai_key.mTime / ticks_per_second;
				keyframe.value.x = ai_key.mValue.x;
				keyframe.value.y = ai_key.mValue.y;
				keyframe.value.z = ai_key.mValue.z;

				channel.scale_keyframes.push_back(keyframe);
				
			}
			else
			{
				channel.scale_keyframes.reserve(pNodeAnim->mNumScalingKeys);
				for (unsigned int keyframe_index = 0; keyframe_index < pNodeAnim->mNumScalingKeys; keyframe_index++)
				{
					const aiVectorKey& ai_key = pNodeAnim->mScalingKeys[keyframe_index];

					ScaleKeyFrame keyframe;
					keyframe.timePos = (float)ai_key.mTime / ticks_per_second;
					keyframe.value.x = ai_key.mValue.x;
					keyframe.value.y = ai_key.mValue.y;
					keyframe.value.z = ai_key.mValue.z;

					channel.scale_keyframes.push_back(keyframe);
				}
			}
			
			animComp->AddChannel(channel.channel_name, channel_index);
		}
		


		m_Animations.push_back(animation);
		animator_out->AddAnim(animation);
	}


	return true;
}

bool Model::LoadModel(const std::string& filePath)
{
	this->directory = StringHelper::GetDirectoryFromPath(filePath);

	
	m_pScene = new aiScene;
	m_pScene = m_Importer.ReadFile(filePath,
		aiProcessPreset_TargetRealtime_Fast |
		aiProcess_ConvertToLeftHanded | aiProcess_TransformUVCoords/*|aiProcess_JoinIdenticalVertices*/);
	if (m_pScene == nullptr)
		return false;
	

	m_meshes.reserve(m_pScene->mNumMeshes);
	for (UINT i = 0; i < m_pScene->mNumMeshes; i++)
	{
		aiMesh* pMesh = m_pScene->mMeshes[i];
		for (UINT boneIndex = 0; boneIndex < pMesh->mNumBones; boneIndex++)
		{
			aiBone* pBone = pMesh->mBones[boneIndex];
			AddAiBone(pBone);
		}
	}

	BuildAvator(m_pScene->mRootNode, -1);

	this->ProcessNode(m_pScene->mRootNode, m_pScene,DirectX::XMMatrixIdentity());


	return true;
}




void Model::ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
{
	XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.emplace_back(this->ProcessMesh(mesh, scene, nodeTransformMatrix));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene, nodeTransformMatrix);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix) 
{
	MeshParameters params;
	std::vector<DWORD> indices;
	Material material;

	if (mesh->HasPositions())
		params.position.reserve(mesh->mNumVertices);
	if (mesh->HasVertexColors(0))
		params.color.reserve(mesh->mNumVertices);
	if (mesh->HasNormals())
		params.normal.reserve(mesh->mNumVertices);
	if (mesh->HasTextureCoords(0))
		params.texcoord.reserve(mesh->mNumVertices);
	if (mesh->HasTangentsAndBitangents())
		params.tangent.reserve(mesh->mNumVertices);
	if (mesh->HasTangentsAndBitangents())
		params.bitangent.reserve(mesh->mNumVertices);
	if (mesh->HasBones())
		params.bone_indice.resize(mesh->mNumVertices, { 0, 0, 0, 0 });
	if (mesh->HasBones())
		params.bone_weights.resize(mesh->mNumVertices, { 0.0f, 0.0f, 0.0f, 0.0f });
	
	
	
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		DirectX::XMFLOAT3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		params.position.emplace_back(position);

		if (mesh->HasNormals()) 
		{
			DirectX::XMFLOAT3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			params.normal.emplace_back(normal);
		}

		DirectX::XMFLOAT2 texcoord;
		if (mesh->mTextureCoords[0]) {
			texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}
		else
		{
			texcoord = { 0.0f, 0.0f };
		}
		params.texcoord.emplace_back(texcoord);


		if (mesh->HasTangentsAndBitangents()) 
		{
			DirectX::XMFLOAT3 tangent;
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
			params.tangent.emplace_back(tangent);

			DirectX::XMFLOAT3 bitangent;
			bitangent.x = mesh->mBitangents[i].x;
			bitangent.y = mesh->mBitangents[i].y;
			bitangent.z = mesh->mBitangents[i].z;
			params.bitangent.emplace_back(bitangent);
		}

		if (mesh->HasVertexColors(0))
		{
			DirectX::XMFLOAT4 color;
			color.x = mesh->mColors[0]->r;
			color.y = mesh->mColors[0]->g;
			color.z = mesh->mColors[0]->b;
			color.w = mesh->mColors[0]->a;
			params.color.emplace_back(color);
		}
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


	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		aiBone* pBone = mesh->mBones[i];
		int bone_index = FindBoneByName(pBone->mName.C_Str());
		assert(bone_index != -1, "referencing invalid bone");
		for (unsigned int weight_index = 0; weight_index < pBone->mNumWeights; weight_index++)
		{
			const aiVertexWeight& pWeight = pBone->mWeights[weight_index];
			AddBoneWeight(&params.bone_indice, &params.bone_weights, pWeight.mVertexId, bone_index, pWeight.mWeight);
		}
	}

	//std::vector<Texture> textures;
	//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
	//textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	aiMaterial* pMaterial = scene->mMaterials[mesh->mMaterialIndex];
	LoadMaterialTextures(material, pMaterial, aiTextureType::aiTextureType_DIFFUSE, scene);
	LoadMaterialTextures(material, pMaterial, aiTextureType::aiTextureType_NORMALS, scene);
	LoadMaterialTextures(material, pMaterial, aiTextureType::aiTextureType_SPECULAR, scene);
	LoadMaterialTextures(material, pMaterial, aiTextureType::aiTextureType_EMISSIVE, scene);
	LoadMaterialTextures(material, pMaterial, aiTextureType::aiTextureType_AMBIENT, scene);

	return Mesh(this->m_device, this->m_deviceContext, *m_cb_ps_material,params, indices, material, transformMatrix);
}

void Model::LoadMaterialTextures(Material& material, aiMaterial* aimaterial, aiTextureType textureType, const aiScene* scene)
{
	Resource<Texture> materialTextures = nullptr;
	TextureStorageType storetype = TextureStorageType::Invalid;
	unsigned int textureCount = aimaterial->GetTextureCount(textureType);

	//if no texture
	if (textureCount == 0)
	{
		storetype = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);

		switch (textureType)
		{
		//case aiTextureType_DIFFUSE:
		//	aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
		//	if (aiColor.IsBlack()) //use grey if color is black
		//	{
		//		materialTextures.push_back(Texture(this->m_device, MyColors::UnloadedTextureColor, textureType));
		//		return materialTextures;
		//	}
		//	materialTextures.push_back(Texture(this->m_device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
		//	return materialTextures;
		case aiTextureType_AMBIENT:
			aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, aiColor);
			material.SetAmbientColour(aiColor.r, aiColor.g, aiColor.b);
			break;
		case aiTextureType_DIFFUSE:
			aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			material.SetDiffuseColour(aiColor.r, aiColor.g, aiColor.b);
			break;
		case aiTextureType_SPECULAR:
			aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiColor);
			if (aiColor.IsBlack())
			{
				material.SetSpecularColour(1.0f, 1.0f, 1.0f);
			}
			else
			{
				material.SetSpecularColour(aiColor.r, aiColor.g, aiColor.b);
			}
			break;
		case aiTextureType_EMISSIVE:
			aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiColor);
			material.SetEmissiveColour(aiColor.r, aiColor.g, aiColor.b);
			break;
		case aiTextureType_NORMALS:
		case aiTextureType_HEIGHT:
			return;
		default:
			assert(false, "Unknown texture type");
		}
		
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			aimaterial->GetTexture(textureType, i, &path);
			storetype = DetermineTextureStorageType(scene, aimaterial, i, textureType);
			switch (storetype)
			{
				
				case TextureStorageType::EmbeddedIndexCompressed:
				{
					int index = GetTextureIndex(&path);
					materialTextures = std::make_shared<Texture>(this->m_device,
						reinterpret_cast<const char*>(scene->mTextures[index]->pcData),
						scene->mTextures[index]->mWidth,
						textureType);
					break;
				}
				
				case TextureStorageType::EmbeddedIndexNonCompressed:
				{
					int index = GetTextureIndex(&path);
					materialTextures = std::make_shared<Texture>(this->m_device,
						reinterpret_cast<char*>(scene->mTextures[index]->pcData),
						scene->mTextures[index]->mWidth* scene->mTextures[index]->mHeight,
						textureType);
					break;
				}
				
				case TextureStorageType::EmbeddedCompressed: 
				{
					const aiTexture* pTexture = scene->GetEmbeddedTexture(path.C_Str());
					materialTextures = std::make_shared<Texture>(this->m_device,
						reinterpret_cast<const char*>(pTexture->pcData),
						pTexture->mWidth,
						textureType);
					break;
				}
				case TextureStorageType::EmbeddedNonCompressed:
				{
					const aiTexture* pAiTex = scene->GetEmbeddedTexture(path.C_Str());
					materialTextures = std::make_shared<Texture>(this->m_device,
						reinterpret_cast<char*>(pAiTex->pcData),
						pAiTex->mWidth* pAiTex->mHeight,
						textureType);
					break;
				}
				case TextureStorageType::Disk:
				{
					std::string filename = this->directory + '/' + path.C_Str();
					materialTextures = ResourceManager::GetTexture(this->m_device, filename, textureType);
					break;
				}
			}

		}
		switch (textureType)
		{
		case aiTextureType_AMBIENT:
			material.SetAmbientTexture(std::move(materialTextures));
			break;
		case aiTextureType_DIFFUSE:
			material.SetDiffuseTexture(std::move(materialTextures));
			break;
		case aiTextureType_SPECULAR:
			material.SetSpecularTexture(std::move(materialTextures));
			break;
		case aiTextureType_EMISSIVE:
			material.SetEmissiveTexture(std::move(materialTextures));
			break;
		case aiTextureType_NORMALS:
		case aiTextureType_HEIGHT:
			material.SetNormalTexture(std::move(materialTextures));
			break;
		default:
			assert(false, "Unknown texture type");
		}
		
	}
	
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


//Add a aiBone to our AiboneDictionary to find it later
void Model::AddAiBone(aiBone* pBone)
{
	m_mapBoneNameToAiBone[pBone->mName.C_Str()] = pBone;
}

//Get the aibone by its name
aiBone* Model::GetAiBoneByName(const std::string& name)
{
	auto it = m_mapBoneNameToAiBone.find(name);
	if (it == m_mapBoneNameToAiBone.end())
	{
		return nullptr;
	}
	return it->second;
}

//Add a Node to our current avator's last index(whether is a aiBone it should be a BoneNode In our Avator )
int Model::AddAvatorNode(aiNode* node, int parent_index)
{
	int index = (int)m_Avator.size();
	//Add this Node name to our current avator's last index
	m_mapNodeNameToIndex[node->mName.C_Str()] = index;

	//whether is a aiBone it should be a BoneNode In our Avator 
	BoneNode boneNode;
	//load this node's toparent transform
	boneNode.local_transform = AiToDxMatrix(node->mTransformation);

	boneNode.parent_index = parent_index;
	//add this node to avator
	m_Avator.push_back(boneNode);

	//return this node's index in our Avator
	return index;
}

///Add A Bone Data to m_Bones and m_mapBoneNameToIndex(Pair the AiNode with same AiBones)
int Model::AddBone(aiNode* node, aiBone* bone, int parent_index)
{
	//First Add a Node To Avator and get this Node's index in our avator
	int node_index = AddAvatorNode(node, parent_index);

	BoneData boneData;
	//Save the index
	boneData.index = node_index;
	//save the name
	boneData.name = bone->mName.C_Str();
	//
	boneData.inverse_transform = AiToDxMatrix(bone->mOffsetMatrix);
	
	m_mapBoneNameToIndex[boneData.name] = (int)m_Bones.size();

	m_Bones.push_back(boneData);

	return node_index;
}

//Build our own avator map similar to process node,after this we can find the actual index of our bones
void Model::BuildAvator(aiNode* pNode, int parent_index)
{
	//check if this node is an aiBone
	aiBone* pBone = GetAiBoneByName(pNode->mName.C_Str());

	int index;
	if (!pBone)
	{
		//Add a Node to our current avator's last index(whether is a aiBone it should be a BoneNode In our Avator )
		index = AddAvatorNode(pNode, parent_index);
	}
	else
	{
		///Add A Bone Data to m_Bones and m_mapBoneNameToIndex(and Pair the AiNode and AiBones with same index)
		index = AddBone(pNode, pBone, parent_index);
	}
	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		//recursively build hole avator tree
		BuildAvator(pNode->mChildren[i], index);
	}
}


//Get bone's index in our bonedata
int Model::FindBoneByName(const std::string& name) const
{
	auto it = m_mapBoneNameToIndex.find(name);
	if (it == m_mapBoneNameToIndex.end())
	{
		return -1;
	}
	return it->second;
}

//Get a node's index in our nodedata
int Model::FindNodeByName(const std::string& name) const
{
	auto it = m_mapNodeNameToIndex.find(name);
	if (it == m_mapNodeNameToIndex.end())
		return -1;
	return it->second;
}


//add Boneweight message into meshparam
void Model::AddBoneWeight(std::vector<XMUINT4>* boneindices, std::vector<XMFLOAT4>* weights, unsigned int vertex_id, unsigned int bone_id, float weight)
{
	//find the vertex's target parm's boneindices vector
	XMUINT4& curr_boneindices = (*boneindices)[vertex_id];

	//find this vertex's target parm's current weight
	XMFLOAT4& curr_weight = (*weights)[vertex_id];

	//give this vertex its bone data
	if (curr_weight.x == 0.0f)
	{
		curr_boneindices.x = bone_id;
		curr_weight.x = weight;
	}
	else if (curr_weight.y == 0.0f)
	{
		curr_boneindices.y = bone_id;
		curr_weight.y = weight;
	}
	else if (curr_weight.z == 0.0f)
	{
		curr_boneindices.z = bone_id;
		curr_weight.z = weight;
	}
	else if (curr_weight.w == 0.0f)
	{
		curr_boneindices.w = bone_id;
		curr_weight.w = weight;
	}
	else
	{
		assert(false, "bone weights only support 0~4");
	}
}





//Load Animations in assimp scene
void Model::ProcessAnimation(Animator* animator_out, ConstantBuffer<ConstantBuffer_Bones>* cbufBone, AnimationComponent* animComp)
{

	for (unsigned int anim_index = 0; anim_index < m_pScene->mNumAnimations; anim_index++)
	{
		aiAnimation* pAnimation = m_pScene->mAnimations[anim_index];

		float ticks_per_second = (float)pAnimation->mTicksPerSecond;
		if (directory.find(".gltf") != std::string::npos || directory.find(".glb") != std::string::npos)
		{
			ticks_per_second = 1000.0f;
		}
		if (ticks_per_second == 0.0f)
		{
			ticks_per_second = 1.0f;
		}

		AnimationClip animation;
		animation.name = pAnimation->mName.C_Str();
		animation.channels.resize(pAnimation->mNumChannels);
		animation.duration = (float)pAnimation->mDuration / ticks_per_second;

		for (unsigned int channel_index = 0; channel_index < pAnimation->mNumChannels; channel_index++)
		{
			aiNodeAnim* pNodeAnim = pAnimation->mChannels[channel_index];
			int node_index = FindNodeByName(pNodeAnim->mNodeName.C_Str());
			if (node_index == -1)
			{
				COM_ERROR_IF_FAILED(-1, "Missing animated node.");
				continue;
			}

			AnimationChannel& channel = animation.channels[channel_index];
			channel.node_index = node_index;

			channel.position_keyframes.reserve(pNodeAnim->mNumPositionKeys);
			for (unsigned int keyframe_index = 0; keyframe_index < pNodeAnim->mNumPositionKeys; keyframe_index++)
			{
				const aiVectorKey& ai_key = pNodeAnim->mPositionKeys[keyframe_index];

				PositionKeyFrame keyframe;
				keyframe.timePos = (float)ai_key.mTime / ticks_per_second;
				keyframe.value.x = ai_key.mValue.x;
				keyframe.value.y = ai_key.mValue.y;
				keyframe.value.z = ai_key.mValue.z;

				channel.position_keyframes.push_back(keyframe);
			}

			channel.rotation_keyframes.reserve(pNodeAnim->mNumRotationKeys);
			for (unsigned int keyframe_index = 0; keyframe_index < pNodeAnim->mNumRotationKeys; keyframe_index++)
			{
				const aiQuatKey& ai_key = pNodeAnim->mRotationKeys[keyframe_index];

				RotationKeyFrame keyframe;
				keyframe.timePos = (float)ai_key.mTime / ticks_per_second;
				keyframe.value.x = ai_key.mValue.x;
				keyframe.value.y = ai_key.mValue.y;
				keyframe.value.z = ai_key.mValue.z;
				keyframe.value.w = ai_key.mValue.w;

				channel.rotation_keyframes.push_back(keyframe);
			}

			channel.scale_keyframes.reserve(pNodeAnim->mNumScalingKeys);
			for (unsigned int keyframe_index = 0; keyframe_index < pNodeAnim->mNumScalingKeys; keyframe_index++)
			{
				const aiVectorKey& ai_key = pNodeAnim->mScalingKeys[keyframe_index];

				ScaleKeyFrame keyframe;
				keyframe.timePos = (float)ai_key.mTime / ticks_per_second;
				keyframe.value.x = ai_key.mValue.x;
				keyframe.value.y = ai_key.mValue.y;
				keyframe.value.z = ai_key.mValue.z;

				channel.scale_keyframes.push_back(keyframe);
			}


			animComp->AddChannel(animation.name, channel_index);
		}
		m_Animations.push_back(animation);
	}
	*animator_out = Animator(m_Avator, m_Bones, std::move(m_Animations), cbufBone);
}
