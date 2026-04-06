#pragma once

#include "Texture.h"

class Sprite
{
	int id;				

	int left;
	int top;
	int right;
	int bottom;

	Texture* texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;

public:
	Sprite(int id, int left, int top, int right, int bottom, Texture* tex);
	void Draw(float x, float y);
};

