#pragma once
#include <list>
#include "inputclass.h"
#include "Transform.h"
#include "../Graphic/Model.h"
#include "Timer.h"
#include "Component.h"
#include "Animator/BakedAnimator.h"

class GameObjectClass
{
public:
	GameObjectClass();
	GameObjectClass(const GameObjectClass& other);
	~GameObjectClass();

	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_MatrixBuffer>& wvpMatrix, ConstantBuffer<CB_PS_Material>& cb_ps_material, IVertexShader* pVertexShader);
	bool InitAnimation(ConstantBuffer<ConstantBuffer_Bones>& cbufBones);
	bool AddAnimation(const std::string& filePath, bool disablerootTrans = false, bool disablerootRot = false, bool disablerootScale= false);
	int GetAnimCount();


	bool InitBakedAnim(ConstantBuffer<ConstantBuffer_BakedBones>& cbufBones);
	bool LoadBakedAnim(const std::string& filename);
	int GetBakedAnimCount();


	void Shutdown();
	
	void Frame(float frametime);

	double Render(const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, bool sameanim = false);
	
	double RenderWithBakedAnim(const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,int animnum,float& timepos);

	double Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,bool sameanim = false);
	void SwitchAnim(int index);
	void StartAnim(int index);



	Animator* GetAnimComp() { return &mAnimator; }



	XMMATRIX GetWorldMatrix();
	bool SetWorldMatrix(XMMATRIX world);

	Transform m_Transform;
	Model* m_Model;

	bool isJump;
	bool isGround;
	bool isAttack;
	float attackDuration;
	float GetJumpHeight();
	float Jump(float startSpeed, float acce);
	
	int m_animnum;

private:

	ID3D11DeviceContext* deviceContext;
	std::vector<Timer*>  mAnimTimers;
	

	
	Timer m_jumpTimer;
	//Timer mAnimTimer;
	float mAnimTimeScale = 1.0f;
	bool mPlayAnimtion = false;
	Animator mAnimator;
	std::unique_ptr<AnimationComponent> mAnimComp;
	//AnimationComponent* mAnimComp;
	float m_jumpAcce = 0;
	float m_jumpSpeed = 0;

	
	BakedAnimator m_baked_Animator;
};

