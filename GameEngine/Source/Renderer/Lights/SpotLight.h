#pragma once

#include "Light.h"

class SpotLight : public Light
{
public:

	__forceinline SpotLight(const Vec & color, const Vec & pos, const Vec & dir,
		float range, float innerAngleInRadians, float outerAngleInRadians);

	virtual LightType VGetType() const override;

protected:

	float m_innerAngleInRadians;
	float m_outerAngleInRadians;
};

__forceinline SpotLight::SpotLight(const Vec & color, const Vec & pos, const Vec & dir,
	float range, float innerAngleInRadians, float outerAngleInRadians)
	: Light{ color, pos, dir, range }, m_innerAngleInRadians{ innerAngleInRadians },
	m_outerAngleInRadians{ outerAngleInRadians }
{}