#pragma once

#include "../Objects/IMovable.h"

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

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

	__forceinline Mat4x4 GetShadowViewProj() const;

protected:
	Vec m_color;
	Vec m_pos;
	Vec m_dir;

	float m_range;
};

__forceinline Light::Light(const Vec & color, const Vec & pos, const Vec & dir, float range)
	: m_color{ color }, m_pos{ pos }, m_dir{ dir }, m_range{ range }
{}

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

__forceinline Mat4x4 Light::GetShadowViewProj() const
{
	Mat4x4 view = Mat4x4::CreateViewMatrixLH(m_pos, m_dir, Vec(0.0f, 1.0f, 0.0f, 0.0f));
	Mat4x4 ortho = Mat4x4::CreateOrthoProjectionLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.5f, m_range);

	return view * ortho;
}