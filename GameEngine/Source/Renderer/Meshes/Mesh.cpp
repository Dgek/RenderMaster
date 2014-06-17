#include "Core.h"

#include "Mesh.h"

Mesh::~Mesh()
{}

void Mesh::SetWorldTransform(const Mat4x4 & transform)
{
	m_worldTransform = m_objectTransform * transform;
}