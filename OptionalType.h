#pragma once

template <typename T>
struct Optional
{
	bool hasValue;
	T value;

	Optional() : hasValue(false), value() {}
	Optional(const T& v) : hasValue(true), value(v) {}

	void Reset()
	{
		hasValue = false;
		value = T();
	}

	void Set(const T& v)
	{
		hasValue = true;
		value = v;
	}
};