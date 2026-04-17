#pragma once
#include <Windows.h>
#include  "Vector2.h"
#include <cmath>

struct RectF
{
	float top, left, right, bottom;

	explicit RectF(float left = 0, float top = 0, float right = 0, float bottom = 0) {
		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;
	}
};


struct Rect
{
	int left;
	int top;
	int right;
	int bottom;

	explicit Rect(int left = 0, int top = 0, int right = 0, int bottom = 0) {
		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;
	}

	static Rect FromXYWH(int left, int top, int width, int height)
	{
		return Rect{ left, top, left + width, top + height};
	}
	static Rect FromXYWH(float left, float top, int width, int height)
	{
		return Rect{ static_cast<int>(left), static_cast<int>(top), static_cast<int>(left) + width, static_cast<int>(top) + height };
	}

	Rect(const RECT& r) {
		left = r.left;
		right = r.right;
		bottom = r.bottom;
		top = r.top;
	}
	Rect(const RectF& r) {
		left = static_cast<int>(roundf(r.left));
		right = static_cast<int>(roundf(r.right));
		bottom = static_cast<int>(roundf(r.bottom));
		top = static_cast<int>(roundf(r.top));
	}
	RECT GetRECT() const
	{
		RECT r;
		r.top = top;
		r.right = right;
		r.left = left;
		r.bottom = bottom;
		return r;
	}

	bool IsColliding(const Rect& other) const
	{
		return (other.left <= right &&
			other.right >= left &&
			other.top <= bottom &&     // FIX
			other.bottom >= top);      // FIX
	}

	Vector2Int GetPushDir(const Rect& other, float& pushAmount) const
	{
		if (!IsColliding(other))
			return Vector2Int(0, 0);

		float pushLeft = static_cast<float>(right - other.left);   // move other left
		float pushRight = static_cast<float>(other.right - left);   // move other right
		float pushUp = static_cast<float>(bottom - other.top);   // move other up
		float pushDown = static_cast<float>(other.bottom - top);   // move other down
		pushAmount = pushLeft;

		Vector2Int dir = Vector2Int::Right();

		if (pushRight < pushAmount)
		{
			pushAmount = pushRight;
			dir = Vector2Int::Left();
		}

		if (pushUp < pushAmount)
		{
			pushAmount = pushUp;
			dir = Vector2Int::Down();
		}

		if (pushDown < pushAmount)
		{
			pushAmount = pushDown;
			dir = Vector2Int::Up();
		}

		return dir;
	}

	int GetWidth() const {
		return right - left + 1;
	}
	int GetHeight() const {
		return bottom - top + 1;
	}
};


