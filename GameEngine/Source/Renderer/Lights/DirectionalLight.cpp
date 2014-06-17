#include "Core.h"

#include "DirectionalLight.h"

LightType DirectionalLight::VGetType() const
{
	return LT_Directional;
}