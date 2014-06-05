#pragma once

#include "Math.h"

class Vec
{
public:
	//coordinates
	float x, y, z, w;

	static const Vec g_up4;
	static const Vec g_right4;
	static const Vec g_left4;
	static const Vec g_forward4;
	static const Vec g_back4;

	__forceinline static Vec Normalize(const Vec & v);

public:
	/*
	*Default Constructor
	*/
	__forceinline Vec() = default;

	/*
	*Constructor
	*
	* @param inX x 
	* @param inY y
	* @param inZ z
	* @param inW w
	*/
	__forceinline Vec(float inX, float inY, float inZ, float inW);

	/*
	*Constructor
	*
	* @param v gives xyz
	* @param w gives w
	*/
	__forceinline Vec(const Vec & v, float w);

	__forceinline float & operator[](int index);

	__forceinline float operator[](int index) const;

	__forceinline void Set(float inX, float inY, float inZ, float inW);

	__forceinline Vec operator-() const;

	__forceinline Vec & operator+=(const Vec & v);

	__forceinline Vec & operator-=(const Vec & v);

	__forceinline Vec & operator*=(const Vec & v);

	__forceinline Vec & operator/=(const Vec & v);

	__forceinline Vec operator*(float scale) const;

	__forceinline Vec & operator*=(float scale);

	__forceinline Vec operator/(float scale) const;


	bool operator==(const Vec & v) const;

	bool operator!=(const Vec & v) const;

	__forceinline const Vec & Normalize();

	__forceinline float Length() const;

	__forceinline Vec Reflect(const Vec & normal) const;
};

const Vec g_up4 = Vec{ 0.0f, 1.0f, 0.0f, 0.0f };
const Vec g_right4 = Vec{ 1.0f, 0.0f, 0.0f, 0.0f };
const Vec g_left4 = Vec{ -1.0f, 0.0f, 0.0f, 0.0f };
const Vec g_forward4 = Vec{ 0.0f, 0.0f, 1.0f, 0.0f };
const Vec g_back4 = Vec{ 0.0f, 0.0f, -1.0f, 0.0f };

__forceinline Vec Vec::Normalize(const Vec & v)
{
	const float scale = Math::InvSqrt(v.x*v.x + v.y*v.y + v.z*v.z);

	return Vec{ v.x * scale, v.y * scale, v.z * scale, v.w * scale };
}

__forceinline Vec::Vec(float inX, float inY, float inZ, float inW)
	: x{ inX }, y{ inY }, z{ inZ }, w{ inW }
{}

__forceinline Vec::Vec(const Vec & v, float w)
	: x{ v.x }, y{ v.y }, z{ v.z }, w{ w }
{}

__forceinline float & Vec::operator[](int index)
{
	return (&x)[index];
}

__forceinline float Vec::operator[](int index) const
{
	return (&x)[index];
}

__forceinline void Vec::Set(float inX, float inY, float inZ, float inW)
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

__forceinline Vec Vec::operator-() const
{
	return Vec{ -x, -y, -z, -w };
}

__forceinline Vec & Vec::operator+=(const Vec & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

__forceinline Vec & Vec::operator-=(const Vec & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

__forceinline Vec & Vec::operator*=(const Vec & v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

__forceinline Vec & Vec::operator/=(const Vec & v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

__forceinline Vec Vec::operator*(float scale) const
{
	return Vec{ x * scale, y * scale, z * scale, w * scale };
}

__forceinline Vec & Vec::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;

	return *this;
}

__forceinline Vec Vec::operator/(float scale) const
{
	float rscale = 1.f / scale;
	return Vec{ x * scale, y * scale, z * scale, w * scale };
}


bool Vec::operator==(const Vec & v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

bool Vec::operator!=(const Vec & v) const
{
	return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w));
}

__forceinline const Vec & Vec::Normalize()
{
	const float scale = Math::InvSqrt(x*x + y*y + z*z);

	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;

	return *this;
}

__forceinline float Vec::Length() const
{
	return Math::Sqrt(x*x + y*y + z*z);
}

__forceinline Vec operator+(Vec v1, const Vec & v2)
{
	v1 += v2;
	return v1;
}

__forceinline Vec operator-(Vec v1, const Vec & v2)
{
	v1 -= v2;
	return v1;
}

__forceinline Vec operator*(Vec v1, const Vec & v2)
{
	v1 *= v2;
	return v1;
}

__forceinline Vec operator/(Vec v1, const Vec & v2)
{
	v1 /= v2;
	return v1;
}

__forceinline Vec operator^(Vec v1, const Vec & v2)
{
	return Vec
		{
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x,
			0.0f
		};
}

//__forceinline Vec Vec::Reflect(const Vec & normal)
//{
//	return 2.0f * 
//}

__forceinline float Dot3(const Vec & v1, const Vec & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

__forceinline float Dot4(const Vec & v1, const Vec & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}