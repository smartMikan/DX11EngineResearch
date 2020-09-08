#pragma once
#include <list>
#include "Transform.h"
#include "../Graphic/Model.h"
#include "Timer.h"
#include "Animator/BakedAnimator.h"





class GameObject
{
public:
	//Object State
	enum State
	{
		GActive,
		GPaused,
		GDead
	};


	GameObject(class MainGame* game);
	GameObject(const GameObject& other);
	virtual ~GameObject();





	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_MatrixBuffer>& wvpMatrix, ConstantBuffer<CB_PS_Material>& cb_ps_material, IVertexShader* pVertexShader);
	bool InitAnimation(ConstantBuffer<ConstantBuffer_Bones>& cbufBones);
	bool AddAnimation(const std::string& filePath, bool disablerootTrans = false, bool disablerootRot = false, bool disablerootScale= false);
	int GetAnimCount();


	bool InitBakedAnim(ConstantBuffer<ConstantBuffer_BakedBones>& cbufBones);
	bool LoadBakedAnim(const std::wstring& filename);
	int GetBakedAnimCount();


	void Shutdown();
	
	void Frame(float deltatime);
	void UpdateComponents(float deltatime);
	virtual void UpdateGameObject(float deltatime);
	
	State GetState() const { return m_State; };
	void SetState(State state) { m_State = state; }

	class MainGame* GetGame() { return m_Game; }

	//add/remove component
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);


	double Render(const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, bool sameanim = false);
	
	double RenderWithBakedAnim(const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,int animnum,float& timepos);

	double Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,bool sameanim = false);
	void SwitchAnim(int index);
	void StartAnim(int index);



	Animator* GetAnimator() { return &m_Animator; }



	XMMATRIX GetWorldMatrix();
	bool SetWorldMatrix(XMMATRIX world);

	Transform m_Transform;
	Model* m_Model;

	
	
	int m_animnum;

private:
	State m_State;
	class MainGame* m_Game;


	std::vector<Component*> m_Components;

	ID3D11DeviceContext* deviceContext;
	std::vector<Timer*>  mAnimTimers;
	
	//Timer mAnimTimer;
	float mAnimTimeScale = 1.0f;
	bool mPlayAnimtion = false;
	Animator m_Animator;
	std::unique_ptr<AnimationComponent> mAnimComp;
	//AnimationComponent* mAnimComp;
	
	
	BakedAnimator m_baked_Animator;
};

