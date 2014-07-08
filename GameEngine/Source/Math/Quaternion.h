#pragma once

#include "float3.h"
#include "Matrix.h"

class Quaternion
{
public:

	float x, y, z, w;

	Quaternion();

	Quaternion(const Quaternion & q);

	Quaternion(float inX, float inY, float inZ, float inW);

	Quaternion(float3 axis, float angleInRad);

	static Quaternion CreateFromEuler(const Vec & euler);
	static Quaternion Slerp(const Quaternion & q0, Quaternion & q1, float t);

	Vec Euler() const;

	/*
	* Constructor
	* 
	* @param M a rotation matrix
	*/
	explicit Quaternion(const Mat4x4 & M);

	float & operator[](int index);

	float operator[](int index) const;

	void Set(float inX, float inY, float inZ, float inW);

	Quaternion operator-() const;

	Quaternion & operator*=(const Quaternion & v);

	Quaternion & operator^=(const Quaternion & v);

	Quaternion operator*(float scale) const;

	Quaternion & operator*=(float scale);

	Quaternion operator/(float scale) const;


	bool operator==(const Quaternion & v) const;

	bool operator!=(const Quaternion & v) const;

	const Quaternion & operator=(const Quaternion & q);

	const Quaternion & Normalize();

	const Quaternion & Inverse();

	const Quaternion & Power(float t);

	bool ContainsNan() const;
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

__forceinline const Quaternion & Quaternion::operator=(const Quaternion & q)
{
	if (this != &q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}

	return *this;
}

__forceinline const Quaternion & Quaternion::Inverse()
{
	return Quaternion{ -x, -y, -z, w };
}

__forceinline const Quaternion & Quaternion::Power(float t)
{
	//if identity then protect against zero divide
	if (Math::Abs(w) < 0.9999f)
	{
		//alpha = theta / 2
		auto alpha = Math::ACos(w);

		auto newAlpha = alpha * t;

		w = Math::Cos(newAlpha);

		auto mult = Math::Sin(newAlpha) / Math::Sin(alpha);
		x *= mult;
		y *= mult;
		z *= mult;
	}
}

__forceinline bool Quaternion::ContainsNan() const
{
	return (_isnan(x) || _finite(x)
		|| _isnan(y) || _finite(y)
		|| _isnan(z) || _finite(z)
		|| _isnan(w) || _finite(w));
}

__forceinline float Dot4(const Quaternion & v1, const Quaternion & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

__forceinline Quaternion Quaternion::Slerp(const Quaternion & q0, Quaternion & q1, float t)
{
	auto cosOmega = Dot4(q0, q1);

	if (cosOmega < 0.0f)
	{
		q1 = -q1;
		cosOmega *= -1;
	}

	//if very close just use linear interpolation
	float k0, k1;
	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		auto sinOmega = Math::Sqrt(1.0f - cosOmega * cosOmega);

		auto omega = Math::Atan2(sinOmega, cosOmega);

		auto invSinOmega = 1.0f / sinOmega;

		k0 = Math::Sin((1.0f - t)*omega)*invSinOmega;
		k1 = Math::Sin(t*omega) *invSinOmega;
	}

	return Quaternion{ q0.x*k0 + q1.x*k1, q0.y*k0 + q1.y*k1, q0.z*k0 + q1.z*k1, q0.w*k0 + q1.w*k1 };
}