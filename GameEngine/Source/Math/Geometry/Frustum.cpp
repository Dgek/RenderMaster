#include "Core.h"

#include "Frustum.h"

void Frustum::Init(float fov, float aspect, float nearClip, float farClip)
{
	//Copy values
	m_fov = fov;
	m_aspect = aspect;
	m_near = nearClip;
	m_far = farClip;

	float halfFov = fov * 0.5f;

	//AREAL64 tanFov		= Tg(m_fov);
	double tanFov = Math::Tan(halfFov);
	Vec nearRightOffset = Vec::g_right4 * (m_near * tanFov) * m_aspect;
	Vec farRightOffset = Vec::g_right4 * (m_far  * tanFov) * m_aspect;
	Vec nearUpOffset = Vec::g_up4 * (m_near * tanFov);
	Vec farUpOffset = Vec::g_up4 * (m_far  * tanFov);

	// points start in the upper left and go around clockwise
	m_nearClip[0] = (Vec::g_forward4 * m_near) - nearRightOffset + nearUpOffset;
	m_nearClip[1] = (Vec::g_forward4 * m_near) + nearRightOffset + nearUpOffset;
	m_nearClip[2] = (Vec::g_forward4 * m_near) + nearRightOffset - nearUpOffset;
	m_nearClip[3] = (Vec::g_forward4 * m_near) - nearRightOffset - nearUpOffset;

	m_fNearPlaneWidth = 2 * nearRightOffset.x;
	m_fNearPlaneHeight = 2 * nearUpOffset.y;

	m_farClip[0] = (Vec::g_forward4 * m_far) - farRightOffset + farUpOffset;
	m_farClip[1] = (Vec::g_forward4 * m_far) + farRightOffset + farUpOffset;
	m_farClip[2] = (Vec::g_forward4 * m_far) + farRightOffset - farUpOffset;
	m_farClip[3] = (Vec::g_forward4 * m_far) - farRightOffset - farUpOffset;

	// now we have all eight points, now construct 6 planes.
	// the normals point away from you if you use counter clockwise verts.

	Vec origin{ 0.0f, 0.0f, 0.0f, 1.0f };
	m_planes[Near] = Plane{ m_nearClip[2], m_nearClip[1], m_nearClip[0] };
	m_planes[Far] = Plane{ m_farClip[0], m_farClip[1], m_farClip[2] };
	m_planes[Right] = Plane{ m_farClip[2], m_farClip[1], origin };
	m_planes[Top] = Plane{ m_farClip[1], m_farClip[0], origin };
	m_planes[Left] = Plane{ m_farClip[0], m_farClip[3], origin };
	m_planes[Bottom] = Plane{ m_farClip[3], m_farClip[2], origin };

	//fill frustum rays array
	for (int i = 0; i < 4; i++)
	{
		m_frustumRays[i] = m_farClip[i] - m_nearClip[i];
		m_frustumRays[i] = Vec::Normalize(m_frustumRays[i]);
	}
}