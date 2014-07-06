#pragma once

#include "Light.h"

class PointLight : public Light
{
public:

	PointLight(const Vec & color, const Vec & pos, float range);

	virtual LightType VGetType() const override;
};

__forceinline PointLight::PointLight(const Vec & color, const Vec & pos, float range)
	: Light{ color, pos, Vec{ 0.0, -1.0f, 0.0f, 1.0f }, range }
{}