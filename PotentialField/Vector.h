#pragma once
#pragma once
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

struct Vector2
{
	float x = 0;
	float y = 0;


	float GetLength() const
	{
		return sqrt(x * x + y * y);
	}
	float GetLengthSq() const
	{
		return x * x + y * y;
	}

	void Normalize()
	{
		operator/=(GetLength());
	}
	Vector2 GetNormalized() const
	{
		Vector2 v{ x,y };
		v.Normalize();
		return v;
	}

	void Rotate(float angle)
	{
		Vector2 v{ x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(y) };
		(*this) = v;
	}
	Vector2 GetRotated(float angle) const
	{
		return { x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(y) };

	}


	Vector2 operator+(Vector2 other) const
	{
		return { x + other.x, y + other.y };
	}
	Vector2 operator-(Vector2 other) const
	{
		return { x - other.x, y - other.y };
	}
	Vector2 operator*(float other) const
	{
		return { x * other, y * other };
	}
	Vector2 operator/(float other) const
	{
		return { x / other, y / other };
	}

	void operator+=(Vector2 other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(Vector2 other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator*=(float other)
	{
		x *= other;
		y *= other;
	}
	void operator/=(float other)
	{
		x /= other;
		y /= other;
	}
};

inline float ToRadian(float degree)
{
	return 3.14159265359 / 180.0f * degree;
}
inline float ToDegree(float radian)
{
	return 180.0f / 3.14159265359 * radian;
}