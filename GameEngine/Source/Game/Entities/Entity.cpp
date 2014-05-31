#include "Core.h"

#include "Entity.h"
#include "../../Scene/Representation.h"

void Entity::SetCurrentTransform(const Mat4x4 & transform, double currentTime)
{
	m_currentWorldMatrix = transform;
	if (m_pRepresentation)
		m_pRepresentation->SetCurrentTransform(m_currentWorldMatrix, currentTime);
}

void Entity::VUpdate(double time, double elapsedtime)
{
}