#include "GameObjectClass.h"
#include "../MainGame.h"
#include "Component.h"

GameObject::GameObject(MainGame* game)
	:m_State(GActive)
	,m_Game(game)
{

	m_Model = 0;

	m_Game->AddGameobject(this);
}

GameObject::GameObject(const GameObject& other)
{

}

GameObject::~GameObject()
{
	m_Game->RemoveGameobject(this);
	//delete component
	while (!m_Components.empty()) 
	{
		delete m_Components.back();
	}

	Shutdown();

}

bool GameObject::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
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
	m_Transform.SetPosition(10.0f, 0.0f, 10.0f);
	m_Transform.SetRotation(0.0f, 0.0f, 0.0f);
	return true;
}

bool GameObject::InitAnimation(ConstantBuffer<ConstantBuffer_Bones>& cbufBones)
{
	mAnimComp = std::make_unique<AnimationComponent>(&m_Animator);
	
	mPlayAnimtion = true;
	mAnimTimers.emplace_back(new Timer);
	mAnimTimers[0]->Start();
	//mAnimTimer.Start();

	return m_Model->InitAnimation(&cbufBones, &m_Animator, mAnimComp.get());
}

bool GameObject::AddAnimation(const std::string & filePath, bool disablerootTrans, bool disablerootRot, bool disablerootScale)
{
	assert(mAnimComp.get() != nullptr);

	mAnimTimers.emplace_back(new Timer);
	
	mAnimTimers.back()->Start();

	return m_Model->AddAnimation(filePath, &m_Animator, mAnimComp.get(),disablerootTrans, disablerootRot, disablerootScale);
}

int GameObject::GetAnimCount()
{
	return m_Animator.GetNumAnimations();
}

bool GameObject::InitBakedAnim(ConstantBuffer<ConstantBuffer_BakedBones>& cbufBones)
{
	m_baked_Animator = BakedAnimator(&cbufBones);

	return true;
}

bool GameObject::LoadBakedAnim(const std::wstring& filename)
{

	return m_baked_Animator.LoadAnimFromFile(filename);
}

int GameObject::GetBakedAnimCount()
{
	return m_baked_Animator.bakedclips.size();
}



void GameObject::Shutdown()
{
	// Release the position object.
	if (m_Model)
	{
		delete m_Model;
		m_Model = 0;
	}

	if (mAnimComp) {
		mAnimComp.release();
		
	}

}

void GameObject::Frame(float deltatime)
{
	m_Transform.SetFrameTime(deltatime);
	if (m_State == GActive) 
	{
		UpdateComponents(deltatime);
		UpdateGameObject(deltatime);
	}
}

void GameObject::UpdateComponents(float deltatime)
{
	for (auto comp : m_Components)
	{
		comp->Frame(deltatime);
	}
}

void GameObject::UpdateGameObject(float deltatime)
{
	//virtual
}



void GameObject::AddComponent(Component* component)
{
	int myOrder = component->GetUpdateOrder();
	auto iter = m_Components.begin();
	//search for first component has higher order 
	for (; iter!=m_Components.end();iter++)
	{
		if (myOrder<(*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	//insert component
	m_Components.insert(iter, component);
}

void GameObject::RemoveComponent(Component* component)
{
	auto iter = std::find(m_Components.begin(), m_Components.end(), component);
	if (iter != m_Components.end()) 
	{
		m_Components.erase(iter);
	}
}

double GameObject::Render(const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,bool sameanim)
{	
	return Draw(m_Transform.GetWorldMatrix(), viewMatrix, projectionMatrix,sameanim);
}


double GameObject::RenderWithBakedAnim(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, int animnum, float& timepos)
{
	double drawTime, calanimtime;
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);

	if (mPlayAnimtion)
	{
		if (timepos >= m_baked_Animator.bakedclips[animnum].animduration)
		{
			timepos = 0;
		}
	}
	

	m_baked_Animator.Bind(deviceContext, animnum, timepos);



	QueryPerformanceCounter(&t2);
	calanimtime = (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;



	drawTime = m_Model->Draw(m_Transform.GetWorldMatrix(), viewMatrix, projectionMatrix);

	return calanimtime + drawTime;
}

double GameObject::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,bool sameanim)
{
	double drawTime, calanimtime;

	LARGE_INTEGER t1,t2,tc;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

	int index = m_Animator.GetCurrentAnimationIndex();
	if (!sameanim) {
		if (mPlayAnimtion)
		{
			if ((float)mAnimTimers[index]->GetMiliseceondsElapsed() / (1000.0f / mAnimTimeScale) >= m_Animator.GetCurrentAnimation().duration)
				mAnimTimers[index]->Restart();
			m_Animator.SetTimpPos((float)mAnimTimers[index]->GetMiliseceondsElapsed() / (1000.0f / mAnimTimeScale));

			const BoneChannel* channel = mAnimComp->GetCurrentBoneChannel();
			if (channel)
			{
				m_Animator.Bind(deviceContext);
			}
		}
	}

    QueryPerformanceCounter(&t2);
	calanimtime =(double)(t2.QuadPart-t1.QuadPart)/(double)tc.QuadPart;
    
	drawTime = m_Model->Draw(worldMatrix, viewMatrix, projectionMatrix);

	return calanimtime > drawTime ? calanimtime: drawTime;
}

void GameObject::SwitchAnim(int index)
{
	assert(index < m_Animator.GetNumAnimations(), "AnimationOverHeads!");
	if (m_Animator.GetCurrentAnimationIndex() == index) {
		return;
	}
	else
	{
		m_Animator.SetCurrentAnimationIndex(index);
	}
}

void GameObject::StartAnim(int index)
{
	SwitchAnim(index);
	mAnimTimers[index]->Restart();
}


XMMATRIX GameObject::GetWorldMatrix()
{
	return XMMatrixTranslation(m_Transform.GetPosition().x, m_Transform.GetPosition().y, m_Transform.GetPosition().z);
}

bool GameObject::SetWorldMatrix(XMMATRIX world)
{
	XMFLOAT4X4 pos;
	XMStoreFloat4x4(&pos, world);
	m_Transform.SetPosition(pos._14,pos._24,pos._34);
	return true;
}






