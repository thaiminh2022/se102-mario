#pragma once

#include <cstdint>

#include "Texture.h"

enum SpriteFlip : std::uint8_t {
	NONE = 0,         // 0000 - No flip
	HORIZONTALLY = 1, // 0001 - Horizontal flip
	VERTICALLY = 2,   // 0010 - Vertical flip
	BOTH = 3          // 0011 - Now you've got the idea
};

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
	void Draw(float x, float y, bool flipX, bool flipY);
};

