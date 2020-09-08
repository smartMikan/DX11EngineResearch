#pragma once
#include "Component.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class GameObject* owner, int draworder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return m_DrawOrder; }
	int GetTextureHeight() const { return m_TextureHeight; }
	int GetTextureWidth() const{ return m_TextureWidth; }

protected:
	class Texture* m_Texture;
	int m_DrawOrder;
	int m_TextureHeight;
	int m_TextureWidth;
};

