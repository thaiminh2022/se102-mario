#pragma once
#include <Windows.h>
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

	int GetWidth() const {
		return right - left + 1;
	}
	int GetHeight() const {
		return bottom - top + 1;
	}
};


