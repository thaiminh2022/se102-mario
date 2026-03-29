#include "Sprite.h"
#include "Game.h"

Sprite::Sprite(int id, int left, int top, int right, int bottom, Texture* tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->GetShaderResourceView();

	sprite.TexCoord.x = this->left / (float)tex->GetWidth();
	sprite.TexCoord.y = this->top / (float)tex->GetHeight();

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	sprite.TexSize.x = spriteWidth / (float)tex->GetWidth();
	sprite.TexSize.y = spriteHeight / (float)tex->GetHeight();

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);
}

void Sprite::Draw(float x, float y)
{
	Game* g = Game::GetInstance();

	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, x, (g->GetBackBufferHeight() - y), 0.1f);
	this->sprite.matWorld = (this->matScaling * matTranslation);

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}
