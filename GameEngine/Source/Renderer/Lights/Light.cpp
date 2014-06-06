#include "Core.h"

#include "Light.h"

void Light::SetWorldTransform(const Mat4x4 & transform)
{
	m_worldTransform = transform;
}