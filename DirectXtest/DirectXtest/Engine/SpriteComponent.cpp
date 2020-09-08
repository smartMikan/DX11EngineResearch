#include "SpriteComponent.h"
#include "GameObjectClass.h"
#include "../MainGame.h"
#include "../Graphic/Texture.h"



SpriteComponent::SpriteComponent(GameObject* owner, int draworder)
	:Component(owner)
	,m_Texture(nullptr)
	,m_DrawOrder(draworder)
	,m_TextureHeight(0)
	,m_TextureWidth(0)
{
	m_Owner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	m_Owner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{

}

void SpriteComponent::SetTexture(Texture* texture)
{
	m_Texture = texture;
	m_TextureHeight = texture->Get()->GetHeight();
	m_TextureWidth = texture->Get()->GetWidth();
}


