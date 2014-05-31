#pragma once

#include "../Vector.h"

class Plane
{
private:
	Vec m_p;

public:

	__forceinline Plane() = default;
	__forceinline Plane(const Plane & other) = default;

	/* 
	* Constructor
	*
	* @param coeff Vector of plane coefficients
	*/
	__forceinline Plane(const Vec & coeff);

	/*
	* Constructor
	*
	* @param p1 point one
	* @param p2 point two
	* @param p2 point three
	*/
	Plane(const Vec & p1, const Vec & p2, const Vec & p3);

	__forceinline void Normalize();

	__forceinline bool Inside(const Vec & point) const;
	__forceinline bool Inside(const Vec & point, float radius) const;

	__forceinline Vec GetNormal() const;
	__forceinline float GetD() const;
};

__forceinline Plane::Plane(const Vec & coeff)
	: m_p{ coeff }
{
}

__forceinline Plane::Plane(const Vec & p1, const Vec & p2, const Vec & p3)
{
	auto v21 = p2 - p1;
	auto v31 = p3 - p1;

	auto normal = v21 ^ v31;
	float D = Dot3(-normal, p1);

	m_p = Vec{ normal, D };
	Normalize();
}

__forceinline void Plane::Normalize()
{
	m_p.Normalize();
}

__forceinline Vec Plane::GetNormal() const
{
	return Vec{ m_p.x, m_p.y, m_p.z, 0.0f };
}

__forceinline float Plane::GetD() const
{
	return m_p.w;
}

__forceinline bool Plane::Inside(const Vec & point) const
{
	auto dist = Dot3(Vec{ m_p.x, m_p.y, m_p.z, 0.0f }, point) + m_p.w;
	return dist >= 0.0f;
}

__forceinline bool Plane::Inside(const Vec & point, float radius) const
{
	auto dist = Dot3(Vec{ m_p.x, m_p.y, m_p.z, 0.0f }, point) + m_p.w;
	return dist >= -radius;
}