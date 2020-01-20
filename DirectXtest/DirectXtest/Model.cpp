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
		//vector<XMMATRIX> Transforms;
		
		//BoneTransform(RunningTime, Transforms,&m_meshes[i]);

		//UpdateConstanBuffer
		shaderManager->DrawSetWithMyShader(this->m_deviceContext, m_meshes[i].GetTransformMatrix() * worldMatrix, viewMatrix, projectionMatrix, cameraPosition, ambientColor, diffuseColor, lightDirection, XMFLOAT4(1,1,1,1),XMFLOAT4(1,1,1,1), specularColor);
		m_meshes[i].Draw();
	}
}

bool Model::LoadModel(const std::string& filePath)
{
	this->directory = StringHelper::GetDirectoryFromPath(filePath);

	//Assimp::Importer importer;

	//const aiScene* pScene = importer.ReadFile(filePath,
	//	aiProcess_Triangulate |
	//	aiProcess_ConvertToLeftHanded);
	m_pScene = new aiScene;
	m_pScene = m_Importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (m_pScene == nullptr)
		return false;
	/*const aiScene* pScene = m_Importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr)
		return false;*/

	this->ProcessNode(m_pScene->mRootNode, m_pScene,DirectX::XMMatrixIdentity());


	bool hasAnim = m_pScene->HasAnimations();
	if (hasAnim == true) 
	{
		numOfAnim = m_pScene->mNumAnimations;
		this->ProcessAnimation(skinnedData, m_pScene);
	}
	
	

	

	//// all using the results from the previous code snippet
	//std::vector<aiVector3D> resultPos(mesh->mNumVertices);
	//std::vector<aiVector3D> resultNorm(mesh->mNumVertices);
	//// loop through all vertex weights of all bones
	//for (size_t a = 0; a < mesh->mNumBones; ++a)
	//{
	//	
	//	const aiBone* bone = mesh->mBones[a];
	//	const aiMatrix4x4& posTrafo = boneMatrices[a];
	//	aiMatrix3x3 normTrafo = aiMatrix3x3(posTrafo); // 3x3 matrix, contains the bone matrix without the translation, only with rotation and possibly scaling
	//	for (size_t b = 0; b < bone->mNumWeights; ++b)
	//	{
	//		const aiVertexWeight& weight = bone->mWeights[b];
	//		size_t vertexId = weight.mVertexId;
	//		const aiVector3D& srcPos = mesh->mVertices[vertexId];
	//		const aiVector3D& srcNorm = mesh->mNormals[vertexId];
	//		resultPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
	//		resultNorm[vertexId] += weight.mWeight * (normTrafo * srcNorm);
	//	}
	//}


	//m_skindata.Set(bongHierarchy, boneOffsets, animations);
	return true;
}

//void Model::BoneTransform(float TimeInSeconds, vector<XMMATRIX>& Transforms,Mesh* mesh)
//{
//	XMMATRIX Identity = XMMatrixIdentity();
//	//Identity.InitIdentity();
//
//	float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 30.0f);
//	float TimeInTicks = TimeInSeconds * TicksPerSecond;
//	float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);
//
//	ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity, mesh);
//
//	UINT num = mesh->GetNumBones();
//	Transforms.resize(num);
//
//	/*for (UINT i = 0; i < mesh->GetNumBones(); i++) {
//		Transforms[i] = mesh->m_BoneInfo[i].FinalTransformation;
//	}*/
//}


void Model::ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
{
	XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(this->ProcessMesh(mesh, scene, nodeTransformMatrix,i));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene, nodeTransformMatrix);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix, UINT meshID) 
{

	std::vector<Mesh::VertexType> vertices;
	std::vector<DWORD> indices;
	std::vector<int> bongHierarchy;
	std::vector<XMFLOAT4X4> boneOffsets;
	//std::map<string, AnimationClip> animations;
	//AnimationClip animationclips;
	

	
	/*for (size_t i = 0; i < mesh->mNumBones; i++)
	{
		scene->mRootNode->FindNode(mesh->mBones[i]->mName);
	}*/
	

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

		if (mesh->mTangents != nullptr) {
			vertex.tangent.x = mesh->mTangents->x;
			vertex.tangent.y = mesh->mTangents->y;
			vertex.tangent.z = mesh->mTangents->z;
			vertex.tangent.w = 1;
		}

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
	

	// calculate bone matrices
	//std::vector<aiMatrix4x4> boneMatrices(mesh->mNumBones);
	//for (size_t i = 0; i < mesh->mNumBones; ++i)
	//{
	//	const aiBone* bone = mesh->mBones[i];

	//  find the corresponding node by again looking recursively through the node hierarchy for the same name
	//	const aiNode* node = FindNodeRecursivelyByName(scene->mRootNode, bone->mName);

	//	// start with the mesh-to-bone matrix 
	//	boneMatrices[i] = bone->mOffsetMatrix;
	//	// and now append all node transformations down the parent chain until we're back at mesh coordinates again
	//	const aiNode* tempNode = node;
	//	while (tempNode)
	//	{
	//		boneMatrices[i] *= tempNode->mTransformation;   // check your matrix multiplication order here!!!
	//		tempNode = tempNode->mParent;
	//	}

	//	XMFLOAT4X4 offset;
	//	XMStoreFloat4x4(&offset, XMMatrixTranspose(XMMATRIX(&boneMatrices[i].a1)));
	//	boneOffsets.push_back(offset);
	//}
	//
	
	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	return Mesh(this->m_device, this->m_deviceContext, vertices, indices, textures, transformMatrix);
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



//void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const XMMATRIX& ParentTransform,Mesh* mesh)
//{
//	string NodeName(pNode->mName.data);
//
//	const aiAnimation* pAnimation = m_pScene->mAnimations[0];
//
//	XMMATRIX NodeTransformation = XMMatrixTranspose(XMMATRIX(&pNode->mTransformation.a1));
//
//	//XMMATRIX NodeTransformation(pNode->mTransformation);
//
//	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
//
//	if (pNodeAnim) {
//		// Interpolate scaling and generate scaling transformation matrix
//		aiVector3D Scaling;
//		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
//		XMMATRIX ScalingM;
//		ScalingM = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
//		//ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);
//
//		// Interpolate rotation and generate rotation transformation matrix
//		aiQuaternion RotationQ;
//		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
//		aiMatrix3x3 Rot = RotationQ.GetMatrix();
//		XMMATRIX RotationM = XMMatrixTranspose(XMMATRIX(&Rot.a1));
//		//XMMATRIX RotationM = XMMATRIX(RotationQ.GetMatrix());
//
//		// Interpolate translation and generate translation transformation matrix
//		aiVector3D Translation;
//		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
//		XMMATRIX TranslationM;
//		TranslationM = XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
//		//TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);
//
//		// Combine the above transformations
//		//NodeTransformation = TranslationM * RotationM * ScalingM;
//		NodeTransformation = TranslationM * ScalingM;
//
//	}
//
//	XMMATRIX GlobalTransformation = ParentTransform * NodeTransformation;
//
//	//if (mesh->m_BoneMapping.find(NodeName) != mesh->m_BoneMapping.end()) {
//	//	UINT BoneIndex = mesh->m_BoneMapping[NodeName];
//	//	mesh->m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation *
//	//		mesh->m_BoneInfo[BoneIndex].BoneOffset;
//	//}
//
//	for (UINT i = 0; i < pNode->mNumChildren; i++) {
//		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation,mesh);
//	}
//}
//
//void Model::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	if (pNodeAnim->mNumScalingKeys == 1) {
//		Out = pNodeAnim->mScalingKeys[0].mValue;
//		return;
//	}
//
//	UINT ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
//	UINT NextScalingIndex = (ScalingIndex + 1);
//	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
//	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
//	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
//	assert(Factor >= 0.0f && Factor <= 1.0f);
//	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
//	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
//	aiVector3D Delta = End - Start;
//	Out = Start + Factor * Delta;
//}
//
//void Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	// we need at least two values to interpolate...
//	if (pNodeAnim->mNumRotationKeys == 1) {
//		Out = pNodeAnim->mRotationKeys[0].mValue;
//		return;
//	}
//
//	UINT RotationIndex = FindRotation(AnimationTime, pNodeAnim);
//	UINT NextRotationIndex = (RotationIndex + 1);
//	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
//	float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
//	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
//	assert(Factor >= 0.0f && Factor <= 1.0f);
//	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
//	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
//	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
//	Out = Out.Normalize();
//}
//
//void Model::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	if (pNodeAnim->mNumPositionKeys == 1) {
//		Out = pNodeAnim->mPositionKeys[0].mValue;
//		return;
//	}
//
//	UINT PositionIndex = FindPosition(AnimationTime, pNodeAnim);
//	UINT NextPositionIndex = (PositionIndex + 1);
//	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
//	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
//	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
//	assert(Factor >= 0.0f && Factor <= 1.0f);
//	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
//	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
//	aiVector3D Delta = End - Start;
//	Out = Start + Factor * Delta;
//}
//
//UINT Model::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	assert(pNodeAnim->mNumScalingKeys > 0);
//
//	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
//		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
//			return i;
//		}
//	}
//
//	assert(0);
//
//	return 0;
//}
//
//UINT Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	assert(pNodeAnim->mNumRotationKeys > 0);
//
//	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
//		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
//			return i;
//		}
//	}
//
//	assert(0);
//
//	return 0;
//}
//
//UINT Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
//		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
//			return i;
//		}
//	}
//
//	assert(0);
//
//	return 0;
//}
//
//
//
//const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
//{
//	for (UINT i = 0; i < pAnimation->mNumChannels; i++) {
//		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
//
//		if (string(pNodeAnim->mNodeName.data) == NodeName) {
//			return pNodeAnim;
//		}
//	}
//
//	return NULL;
//}
//

//Give A RootNode And Return an Node By name;
const aiNode* Model::FindNodeRecursivelyByName(const aiNode* node, aiString nodeName) {

	if (node->mName == nodeName) 
	{
		return node;
	}

	for (UINT i = 0; i < node->mNumChildren;i++) 
	{
		FindNodeRecursivelyByName(node->mChildren[i], nodeName);
	}
}


void Model::ProcessAnimation(AssimpSkinnedData skindata, const aiScene* modelScene)
{

	vector<XMFLOAT4X4> boneOffets;
	vector<int> boneToParentIndex;

	int mNumNode = 0;

	map<aiString, int> boneNameToNumberIndexMap;
	map<int, aiString> boneNumberToNameIndexMap;

	map<string, AnimationClip> animations;

	//LoadBoneToParentIndex;
	//Load All Node And Give The Node A IndexNumber And Save The Node's ParentIndexNumber In  boneToParentIndex
	//And Save the Node's offset Matrix


	ProcessNodeMap(boneNameToNumberIndexMap, boneNumberToNameIndexMap, modelScene->mRootNode, mNumNode);

	boneToParentIndex.clear();

	ProcessBoneToParentIndex(boneToParentIndex, boneNameToNumberIndexMap, modelScene->mRootNode);

	
	//Process All Node, Check if it has An Animation,if it has,Read The Animation message by ReadBoneKeyFrame(),
	//if not , create a Non-Moving AnimationClip.BoneAnimations for it thus the BoneHirachy tree Wont be disrupt by it;



	//LoadAnimationClip
	for (size_t i = 0; i < modelScene->mNumAnimations; i++)
	{


		const aiAnimation* anim = modelScene->mAnimations[i];
		

		nameOfAnim = anim->mName.C_Str();
		string clipName = nameOfAnim;
		AnimationClip clip;
		//UINT numBones = anim->mNumChannels;
		UINT numBones = mNumNode;
		clip.BoneAnimations.resize(numBones);

		//FindNodeAnim(modelScene->mRootNode, anim, clip);

		for (UINT boneIndex = 0; boneIndex < numBones; boneIndex++)
		{
			//FindNode
			const aiNode* targetnode = modelScene->mRootNode->FindNode(boneNumberToNameIndexMap[boneIndex]);

			//checkifNodeIsAnimNode
			FindNodeAnim(targetnode,anim,clip);

			//ReadBoneKeyFrame(anim->mChannels[boneIndex], clip.BoneAnimations[boneNameToNumberIndexMap[modelScene->mRootNode->FindNode(anim->mChannels[boneIndex]->mNodeName)->mName]],anim->mDuration);
		}

		animations[clipName] = clip;
	}

	skindata.SetAnimations(animations);

}


void Model::FindNodeAnim(const aiNode* node, const aiAnimation* anim, AssimpModel::AnimationClip& clip) {

	bool isAnimNode = false;


	for (UINT i = 0; i < anim->mNumChannels; i++)
	{
		BoneAnimation boneAnim;

		if (anim->mChannels[i]->mNodeName == node->mName) {
			//is
			isAnimNode = true;
			//read KeyFrameOftheNode

			anim->mChannels[i];

			ReadBoneKeyFrame(anim->mChannels[i], boneAnim, anim->mDuration);
		}
		else
		{
			CreateBoneKeyFrame(boneAnim);
		}
		clip.BoneAnimations.push_back(boneAnim);
			
	}
	

}

void Model::CreateBoneKeyFrame(AssimpModel::BoneAnimation&boneAnimation) {

	boneAnimation.TranslationKeyFrames.resize(2);
	boneAnimation.TranslationKeyFrames[0].TimePos = 0.0f;
	boneAnimation.TranslationKeyFrames[0].Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	boneAnimation.TranslationKeyFrames[1].TimePos = 1.0f;
	boneAnimation.TranslationKeyFrames[1].Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);


	boneAnimation.RotationQuatKeyFrames.resize(2);
	boneAnimation.RotationQuatKeyFrames[0].TimePos = 0.0f;
	boneAnimation.RotationQuatKeyFrames[0].Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	boneAnimation.RotationQuatKeyFrames[1].TimePos = 1.0f;
	boneAnimation.RotationQuatKeyFrames[1].Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	boneAnimation.ScaleKeyFrames.resize(2);
	boneAnimation.ScaleKeyFrames[0].TimePos = 0.0f;
	boneAnimation.ScaleKeyFrames[0].Value = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	boneAnimation.ScaleKeyFrames[1].TimePos = 1.0f;
	boneAnimation.ScaleKeyFrames[1].Value = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}


void Model::ReadBoneKeyFrame(const aiNodeAnim* mSingleBone, AssimpModel::BoneAnimation& boneAnimation, double Animduration)
{
	UINT numPosKeyframes = 0;
	UINT numRotKeyframes = 0;
	UINT numScaleKeyframes = 0;
	const aiNodeAnim* channel = mSingleBone;
	numPosKeyframes = channel->mNumPositionKeys;
	numRotKeyframes = channel->mNumRotationKeys;
	numScaleKeyframes = channel->mNumScalingKeys;

	boneAnimation.TranslationKeyFrames.resize(numPosKeyframes);
	boneAnimation.RotationQuatKeyFrames.resize(numRotKeyframes);
	boneAnimation.ScaleKeyFrames.resize(numScaleKeyframes);

	
	 


	for (UINT KeyFrameIndex = 0; KeyFrameIndex < numPosKeyframes; ++KeyFrameIndex) {
		XMFLOAT4 Translation;


		Translation.x = channel->mPositionKeys[KeyFrameIndex].mValue.x;
		Translation.y = channel->mPositionKeys[KeyFrameIndex].mValue.y;
		Translation.z = channel->mPositionKeys[KeyFrameIndex].mValue.z;
		Translation.w = 1.0f;

		boneAnimation.TranslationKeyFrames[KeyFrameIndex].TimePos = channel->mPositionKeys[KeyFrameIndex].mTime;
		boneAnimation.TranslationKeyFrames[KeyFrameIndex].Value = Translation;
	}

	if (numPosKeyframes = 0) {
		boneAnimation.TranslationKeyFrames.resize(2);
		boneAnimation.TranslationKeyFrames[0].TimePos = 0.0f;
		boneAnimation.TranslationKeyFrames[0].Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		boneAnimation.TranslationKeyFrames[1].TimePos = 1.0f;
		boneAnimation.TranslationKeyFrames[1].Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	for (UINT KeyFrameIndex = 0; KeyFrameIndex < numRotKeyframes; ++KeyFrameIndex) {
		XMFLOAT4 Rotation;


		Rotation.w = channel->mRotationKeys[KeyFrameIndex].mValue.w;
		Rotation.x = channel->mRotationKeys[KeyFrameIndex].mValue.x;
		Rotation.y = channel->mRotationKeys[KeyFrameIndex].mValue.y;
		Rotation.z = channel->mRotationKeys[KeyFrameIndex].mValue.z;

		boneAnimation.RotationQuatKeyFrames[KeyFrameIndex].TimePos = channel->mRotationKeys[KeyFrameIndex].mTime;
		boneAnimation.RotationQuatKeyFrames[KeyFrameIndex].Value = Rotation;
	}

	if (numRotKeyframes = 0) {
		boneAnimation.RotationQuatKeyFrames.resize(2);
		boneAnimation.RotationQuatKeyFrames[0].TimePos = 0.0f;
		boneAnimation.RotationQuatKeyFrames[0].Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		boneAnimation.RotationQuatKeyFrames[1].TimePos = 1.0f;
		boneAnimation.RotationQuatKeyFrames[1].Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	}

	for (UINT KeyFrameIndex = 0; KeyFrameIndex < numScaleKeyframes; ++KeyFrameIndex) {
		XMFLOAT4 Scale;


		Scale.x = channel->mScalingKeys[KeyFrameIndex].mValue.x;
		Scale.y = channel->mScalingKeys[KeyFrameIndex].mValue.y;
		Scale.z = channel->mScalingKeys[KeyFrameIndex].mValue.z;
		Scale.w = 1.0f;

		boneAnimation.ScaleKeyFrames[KeyFrameIndex].TimePos = channel->mScalingKeys[KeyFrameIndex].mTime;
		boneAnimation.ScaleKeyFrames[KeyFrameIndex].Value = Scale;
	}

	if (numScaleKeyframes = 0) {
		boneAnimation.ScaleKeyFrames.resize(2);
		boneAnimation.ScaleKeyFrames[0].TimePos = 0.0f;
		boneAnimation.ScaleKeyFrames[0].Value = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		boneAnimation.ScaleKeyFrames[1].TimePos = 1.0f;
		boneAnimation.ScaleKeyFrames[1].Value = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

}

void Model::ProcessNodeMap(map<aiString,int> &boneNameToNumberIndexMap, map<int, aiString> &boneNumberToNameIndexMap,const aiNode* node,int &currentNodeNumber)
{
	boneNameToNumberIndexMap[node->mName] = currentNodeNumber;
	boneNumberToNameIndexMap[currentNodeNumber] = node->mName;
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		currentNodeNumber++;
		ProcessNodeMap(boneNameToNumberIndexMap, boneNumberToNameIndexMap ,node->mChildren[i], currentNodeNumber);
	}
}

void Model::ProcessBoneToParentIndex(vector<int> &boneToParentIndex, map<aiString, int> boneNameToNumberIndexMap, const aiNode* node) {
	
	if (node->mParent != NULL) {
		boneToParentIndex.push_back(boneNameToNumberIndexMap[node->mParent->mName]);
	}
	else
	{
		boneToParentIndex.push_back(0);
	}
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessBoneToParentIndex(boneToParentIndex,boneNameToNumberIndexMap, node->mChildren[i]);
	}
}
