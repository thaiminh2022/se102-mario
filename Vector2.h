#pragma once

struct Vector2Int
{
	int x;
	int y;

	explicit Vector2Int(const int x = 0, const int y = 0)
	{
		this->x = x;
		this->y = y;
	}


	Vector2Int operator+(const Vector2Int& other) const
	{
		return Vector2Int{ x + other.x, y + other.y };
	}


	Vector2Int operator-(const Vector2Int& other) const
	{
		return Vector2Int{ x - other.x, y - other.y };
	}
	Vector2Int operator*(const int& other) const
	{
		return Vector2Int{ x * other, y * other };
	}
	Vector2Int operator/(const int& other) const
	{
		return Vector2Int{ x / other, y / other };
	}

	static Vector2Int Right()
	{
		return Vector2Int(1, 0);
	}
	static Vector2Int Left()
	{
		return Vector2Int(-1, 0);
	}

	static Vector2Int Down()
	{
		return Vector2Int(0, 1);
	}
	static Vector2Int Up()
	{
		return Vector2Int(0, -1);
	}
};


struct Vector2
{
	float x;
	float y;

	explicit Vector2(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(const Vector2Int& vec)
	{
		this->x = static_cast<float>(vec.x);
		this->y = static_cast<float>(vec.y);
	}

	Vector2 operator+(const Vector2& other) const 
	{
		return Vector2{ x + other.x, y + other.y };
	}
	Vector2& operator+=(const Vector2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}


	Vector2 operator-(const Vector2& other) const
	{
		return Vector2{ x - other.x, y - other.y };
	}
	Vector2 operator*(const float& other) const
	{
		return Vector2{ x * other, y * other };
	}
	Vector2 operator/(const float& other) const
	{
		return Vector2{ x / other, y / other };
	}


};


