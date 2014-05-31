#include "Core.h"

#include "DynamicRepresentation.h"

#include "Scene.h"

DynamicRepresentation::DynamicRepresentation(EntityRepresentationId id, const string & resourceMeshName) : EntityRepresentation(id, resourceMeshName)
{}

void DynamicRepresentation::SetBothTransforms(Mat4x4 & mat, double currentTime)
{
	m_prevWorldMatrix = mat;
	m_currentWorldMatrix = mat;
	m_lastUpdate = currentTime;
}

void DynamicRepresentation::SetCurrentTransform(Mat4x4 & mat, double currentTime)
{
	m_prevWorldMatrix = m_currentWorldMatrix;
	m_currentWorldMatrix = mat;

	m_lastUpdate = currentTime;
}

void DynamicRepresentation::VPushParameters(Scene *pScene, double currentTime)
{
	Mat4x4 transform;

	//new update cycle - current transform has changed
	if (m_prevWorldMatrix != m_currentWorldMatrix)
	{
		transform = Math::Lerp(m_prevWorldMatrix, m_currentWorldMatrix, Math::Min((currentTime - m_lastUpdate) / pScene->GetFixedTimeStep(), 1.0));
	}
	else
	{
		//we are not updating anything, so use current transform
		transform = m_currentWorldMatrix;
	}

	pScene->PushTransform(transform);
}