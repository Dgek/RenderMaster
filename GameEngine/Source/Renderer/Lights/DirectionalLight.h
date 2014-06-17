#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:

	__forceinline DirectionalLight(const Vec & color, const Vec & pos, const Vec & dir,
		float range = INFINITY);

	virtual LightType VGetType() const override;
};

__forceinline DirectionalLight::DirectionalLight(const Vec & color, const Vec & pos, const Vec & dir, float range)
	: Light{ color, pos, dir, range }
{
}