#pragma once
#include "Windows.h"

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

	Rect(const RECT& r) {
		left = r.left;
		right = r.right;
		bottom = r.bottom;
		top = r.top;
	}

	int GetWidth() const {
		return right - left + 1;
	}
	int GetHeight() const {
		return bottom - top + 1;
	}
};

