#pragma once

#include "../Objects/IMovable.h"

class Light : public IMovable
{
public:

	__forceinline Light(const Vec & color, const Vec & pos,
		const Vec & dir, float range);

	virtual void SetWorldTransform(const Mat4x4 & transform) override;
	virtual LightType VGetType() const = 0;

	__forceinline const Vec & GetColor() const;
	__forceinline const Vec & GetPos() const;
	__forceinline const Vec & GetDir() const;
	__forceinline const float GetRange() const;

protected:
	Vec m_color;
	Vec m_pos;
	Vec m_dir;

	float m_range;
};

__forceinline const Vec & Light::GetColor() const
{
	return m_color;
}

__forceinline const Vec & Light::GetPos() const
{
	return m_pos;
}

__forceinline const Vec & Light::GetDir() const
{
	return m_dir;
}

__forceinline const float Light::GetRange() const
{
	return m_range;
}