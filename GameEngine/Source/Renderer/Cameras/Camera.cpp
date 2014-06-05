#include "Core.h"

#include "Camera.h"

Camera::Camera(const Frustum & frustum, const Vec & position, const Vec & dir, const Vec & up, const Viewport & viewport)
	: m_frustum{ frustum }, m_bActive{ true }, m_bDebug{ false }, m_bHasTarget{ false },
	m_bVPRecalc{ false }, m_bOrthoRecalc{ false }, m_pos{ position }, m_up{ up },
	m_yaw{ 0 }, m_pitch{ 0 }, m_roll{ 0 }, m_viewport{ viewport }
{
	m_dir = Vec::Normalize(dir);
	m_initialDir = m_dir;

	//initialize matrices
	UpdateTransforms();
}

void Camera::UpdateTransforms()
{
	Mat4x4 rot = Mat4x4::CreateRollPitchYaw(m_roll, m_pitch, m_yaw);

	//calculate point camera is focusing
	m_dir = m_initialDir * rot;
	m_dir = Vec::Normalize(m_dir);
	m_lookAt = m_pos + m_dir;

	m_prevView = m_view;
	m_prevProj = m_projection;

	m_view = Mat4x4::CreateViewMatrixLH(m_pos, m_dir, m_up);
	m_projection = Mat4x4::CreatePerspectiveProjectionLH(m_frustum.GetFOV(), m_frustum.GetAspectRatio(),
		m_frustum.GetNearZ(), m_frustum.GetFarZ());
}

void Camera::VUpdate(unsigned int elapsedMs)
{
	m_pitch = Math::Clamp(static_cast<float>(m_pitch), static_cast<float>(PI * (-0.25)),
		static_cast<float>(PI * 0.25));
	UpdateTransforms();
}