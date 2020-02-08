#pragma once
#include <list>
#include "positionclass.h"
#include "Graphic/Model.h"
#include "Engine/Timer.h"

class GameObjectClass
{
public:
	GameObjectClass();
	GameObjectClass(const GameObjectClass& other);
	~GameObjectClass();

	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_MatrixBuffer>& wvpMatrix, ConstantBuffer<CB_PS_Material>& cb_ps_material, IVertexShader* pVertexShader);
	bool InitAnimation(ConstantBuffer<ConstantBuffer_Bones>& cbufBones);
	bool AddAnimation(const std::string& filePath, ConstantBuffer<ConstantBuffer_Bones>& cbufBone);


	void Shutdown();

	void Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix);
	void SwitchAnim(int index);


	XMMATRIX GetWorldMatrix();
	bool SetWorldMatrix(XMMATRIX world);

	PositionClass* m_Position;
	Model m_Model;

private:
	XMMATRIX worldPosition = XMMatrixIdentity();
	ID3D11DeviceContext* deviceContext;

	std::vector<Timer*>  mAnimTimers;
	//Timer mAnimTimer;
	float mAnimTimeScale = 1.0f;
	bool mPlayAnimtion = false;
	Animator mAnimator;
	std::unique_ptr<AnimationComponent> mAnimComp;
	//AnimationComponent* mAnimComp;

};

