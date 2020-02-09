#include "GameObjectClass.h"

GameObjectClass::GameObjectClass()
{
	m_Model = 0;
}

GameObjectClass::GameObjectClass(const GameObjectClass& other)
{
}

GameObjectClass::~GameObjectClass()
{
}

bool GameObjectClass::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	ConstantBuffer<CB_VS_MatrixBuffer>& wvpMatrix, ConstantBuffer<CB_PS_Material>& cb_ps_material, IVertexShader* pVertexShader)
{
	bool result;
	this->deviceContext = deviceContext;
	m_Model = new Model;
	if (!m_Model) {
		return false;
	}

	result = m_Model->Initialize(filePath, device, deviceContext, wvpMatrix, cb_ps_material, pVertexShader);
	if (result == false) {
		return false;
	}
	

	// Set the initial position and rotation.
	m_Position.SetPosition(10.0f, 0.0f, 10.0f);
	m_Position.SetRotation(0.0f, 0.0f, 0.0f);
	return true;
}

bool GameObjectClass::InitAnimation(ConstantBuffer<ConstantBuffer_Bones>& cbufBones)
{
	mAnimComp = std::make_unique<AnimationComponent>(&mAnimator);
	
	mPlayAnimtion = true;
	mAnimTimers.emplace_back(new Timer);
	mAnimTimers[0]->Start();
	//mAnimTimer.Start();
	return m_Model->InitAnimation(&cbufBones, &mAnimator, mAnimComp.get());
}

bool GameObjectClass::AddAnimation(const std::string & filePath, ConstantBuffer<ConstantBuffer_Bones>& cbufBone)
{
	mAnimTimers.emplace_back(new Timer);
	mAnimTimers[mAnimator.GetNumAnimations()]->Start();
	return m_Model->AddAnimation(filePath, &cbufBone, &mAnimator, mAnimComp.get());
}



void GameObjectClass::Shutdown()
{
	// Release the position object.
	if (m_Model)
	{
		delete m_Model;
		m_Model = 0;
	}
}

void GameObjectClass::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix)
{
	int index = mAnimator.GetCurrentAnimationIndex();
	if (mPlayAnimtion)
	{
		if ((float)mAnimTimers[index]->GetMiliseceondsElapsed() / (1000.0f / mAnimTimeScale) >= mAnimator.GetCurrentAnimation().duration)
			mAnimTimers[index]->Restart();
		mAnimator.SetTimpPos((float)mAnimTimers[index]->GetMiliseceondsElapsed() / (1000.0f / mAnimTimeScale));
		
		const AnimationChannel* channel = mAnimComp->GetCurrentChannel();
		if (channel)
		{
			mAnimator.Bind(deviceContext);
		}
	}
	m_Model->Draw(worldMatrix, viewMatrix, projectionMatrix);
}

void GameObjectClass::SwitchAnim(int index)
{
	mAnimator.SetCurrentAnimationIndex(index);
}



XMMATRIX GameObjectClass::GetWorldMatrix()
{
	return XMMatrixTranslation(m_Position.GetPosition().x, m_Position.GetPosition().y, m_Position.GetPosition().z);
}

bool GameObjectClass::SetWorldMatrix(XMMATRIX world)
{
	XMFLOAT4X4 pos;
	XMStoreFloat4x4(&pos, world);
	m_Position.SetPosition(pos._14,pos._24,pos._34);
	return true;
}






