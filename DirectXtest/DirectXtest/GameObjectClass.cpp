#include "GameObjectClass.h"

GameObjectClass::GameObjectClass()
{
	m_Position = 0;
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

	result = m_Model.Initialize(filePath, device, deviceContext, wvpMatrix, cb_ps_material, pVertexShader);
	if (result == false) {
		return false;
	}
	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation.
	m_Position->SetPosition(128.0f, 10.0f, 128.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	return true;
}

bool GameObjectClass::InitAnimation(ConstantBuffer<ConstantBuffer_Bones>& cbufBones)
{
	mAnimComp = std::make_unique<AnimationComponent>(&mAnimator);
	//mAnimComp = new AnimationComponent();
	mPlayAnimtion = true;
	mAnimTimer.Start();
	return m_Model.InitAnimation(&cbufBones, &mAnimator, mAnimComp.get());
	//return m_Model.InitAnimation(&cbufBones, &mAnimator, mAnimComp);
}

void GameObjectClass::Shutdown()
{
	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}
}

void GameObjectClass::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix)
{
	//static float time;
	//time += 0.003f;
	if (mPlayAnimtion)
	{
		if ((float)mAnimTimer.GetMiliseceondsElapsed() / (1000.0f / mAnimTimeScale) >= mAnimator.GetCurrentAnimation().duration)
			mAnimTimer.Restart();
		mAnimator.SetTimpPos((float)mAnimTimer.GetMiliseceondsElapsed() / (1000.0f / mAnimTimeScale));
		/*if (time  >= mAnimator.GetCurrentAnimation().duration)
			time=0.0f;
		mAnimator.SetTimpPos(time);*/
		const AnimationChannel* channel = mAnimComp->GetCurrentChannel();
		if (channel)
		{
			mAnimator.Bind(deviceContext);
		}
	}


	m_Model.Draw(worldMatrix, viewMatrix, projectionMatrix);
}



XMMATRIX GameObjectClass::GetWorldMatrix()
{
	return worldPosition;
}

bool GameObjectClass::SetWorldMatrix(XMMATRIX world)
{
	worldPosition = world;
	return true;
}




