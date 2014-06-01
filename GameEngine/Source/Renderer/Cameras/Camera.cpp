#include "Core.h"

#include "Camera.h"

Camera::Camera(const Frustum & frustum, Vec & position, Vec & dir, Vec & up,
	const Viewport & viewport)
	: m_frustum{ frustum }, m_bActive{ true }, m_bDebug{ false }, m_bHasTarget{ false },
	m_bVPRecalc{ false }, m_bOrthoRecalc{ false },
	m_position{ position }, m_up{ up }, m_yaw{ 0 }, m_pitch{ 0 }, m_roll{ roll }, m_viewport{ viewport }
{
	m_dir = Normalize(dir);
	m_initialDir = m_dir;

	//initialize matrices
	UpdateTransforms();
}

void Camera::UpdateTransforms()
{
	Mat4x4 rot = CreateRollPitchYae(m_roll, m_pitch, m_yaw);

	//calculate point camera is focusing
	m_dir = m_initialDir * rot;
	m_dir = Normalize(m_dir);
	m_lookAt = m_pos + m_dir;

	m_prevView = m_view;
	m_prevProjection = m_projection;

	m_view = CreateViewMatrixLH(m_pos, m_dir, m_up);
	m_projection = CreateperspectiveProjectionLH(m_frustum.GetFOV(), m_frustum.GetAspectRatio(),
		m_frustum.GetNearZ(), m_frustum.GetFarZ());
}

void Camera::Update(unsigned int elapsedMs)
{
	m_pitch = Clamp(m_pitch, PI * (-0.25), PI * 0.25);
	UpdateTransforms();
}