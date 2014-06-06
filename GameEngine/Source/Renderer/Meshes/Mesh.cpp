#include "Core.h"

#include "Mesh.h"

void Mesh::SetWorldTransform(const Mat4x4 & transform)
{
	m_worldTransform = m_objectTransform * transform;
}