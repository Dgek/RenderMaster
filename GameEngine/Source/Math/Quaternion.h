#pragma once

#include "float3.h"
#include "Matrix.h"

class Quaternion
{
public:

	float x, y, z, w;

	__forceinline Quaternion();

	__forceinline Quaternion(const Quaternion & q);

	__forceinline Quaternion(float inX, float inY, float inZ, float inW);

	__forceinline Quaternion(float3 axis, float angleInRad);

	static Quaternion CreateFromEuler(const Vec & euler);

	Vec Euler() const;

	/*
	* Constructor
	* 
	* @param M a rotation matrix
	*/
	__forceinline explicit Quaternion(const Mat4x4 & M);

	__forceinline float & operator[](int index);

	__forceinline float operator[](int index) const;

	__forceinline void Set(float inX, float inY, float inZ, float inW);

	__forceinline Quaternion operator-() const;

	__forceinline Quaternion & operator*=(const Quaternion & v);

	__forceinline Quaternion & operator^=(const Quaternion & v);

	__forceinline Quaternion operator*(float scale) const;

	__forceinline Quaternion & operator*=(float scale);

	__forceinline Quaternion operator/(float scale) const;


	__forceinline bool operator==(const Quaternion & v) const;

	__forceinline bool operator!=(const Quaternion & v) const;

	__forceinline const Quaternion & Normalize();

	__forceinline const Quaternion & Inverse();

	__forceinline bool ContainsNan() const;
};

__forceinline Quaternion::Quaternion()
	: x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 1.0f }
{}

__forceinline Quaternion::Quaternion(float inX, float inY, float inZ, float inW)
	: x{ inX }, y{ inY }, z{ inZ }, w{ inW }
{}

__forceinline Quaternion::Quaternion(float3 axis, float angleInRad)
{
	const auto halfAngle = 0.5f * angleInRad;
	const auto s = Math::Sin(halfAngle);
	const auto c = Math::Cos(halfAngle);

	x = s*axis.x;
	y = s*axis.y;
	z = s*axis.z;
	w = c;
}

__forceinline float & Quaternion::operator[](int index)
{
	return (&x)[index];
}

__forceinline float Quaternion::operator[](int index) const
{
	return (&x)[index];
}

__forceinline void Quaternion::Set(float inX, float inY, float inZ, float inW)
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

__forceinline Quaternion Quaternion::operator-() const
{
	return{ -x, -y, -z, -w };
}

__forceinline Quaternion & Quaternion::operator*=(const Quaternion & v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

__forceinline Quaternion & Quaternion::operator^=(const Quaternion & q)
{
	x = w*q.x + x*q.w + z*q.y - y*q.z;
	y = w*q.y + y*q.w + x*q.z - z*q.x;
	z = w*q.z + z*q.w + y*q.x - x*q.y;
	w = w*q.w - x*q.x - y*q.y - z*q.z;

	return *this;
}

__forceinline Quaternion operator*(Quaternion q1, const Quaternion & q2)
{
	q1 *= q2;
	return q1;
}

__forceinline Quaternion operator^(Quaternion q1, const Quaternion & q2)
{
	q1 ^= q2;
	return q1;
}

__forceinline Quaternion Quaternion::operator*(float scale) const
{
	return Quaternion{ x*scale, y*scale, z*scale, w*scale };
}

__forceinline Quaternion & Quaternion::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;

	return *this;
}

__forceinline Quaternion Quaternion::operator/(float scale) const
{
	const auto inv = 1.0f / scale;

	return Quaternion{ x*inv, y*inv, z*inv, w*inv };
}

__forceinline bool Quaternion::operator==(const Quaternion & v) const
{
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}

__forceinline bool Quaternion::operator!=(const Quaternion & v) const
{
	return (x != v.x || y != v.y || z != v.z || w != v.w);
}

__forceinline const Quaternion & Quaternion::Inverse()
{
	return Quaternion{ -x, -y, -z, w };
}

__forceinline bool Quaternion::ContainsNan() const
{
	return (_isnan(x) || _finite(x)
		|| _isnan(y) || _finite(y)
		|| _isnan(z) || _finite(z)
		|| _isnan(w) || _finite(w));
}