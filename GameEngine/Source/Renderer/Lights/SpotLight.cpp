#include "Core.h"

#include "SpotLight.h"

LightType SpotLight::VGetType() const
{
	return LT_Spot;
}