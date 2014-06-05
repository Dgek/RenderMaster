#pragma once

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"
#include "../../Math/Geometry/Frustum.h"

#include "../../Graphics/Other/Viewport.h"

class Entity;
class Camera
{
protected:
	/**
	View and Projection matrices
	**/
	Mat4x4 m_view;
	Mat4x4 m_prevView;
	Mat4x4 m_projection;
	Mat4x4 m_prevProj;

	Mat4x4 m_viewProj;
	Mat4x4 m_orthoProj;
	bool m_bVPRecalc;
	bool m_bOrthoRecalc;

	/**
	General Data
	**/

	Vec	m_pos;		//position
	Vec m_dir;		//direction of the camera
	Vec m_initialDir;
	Vec	m_lookAt;	//point camera is looking at
	Vec m_up;		//direction of up vector

	float m_yaw;
	float m_pitch;
	float m_roll;

	/**
	Projection Data
	**/
	Frustum	m_frustum;

	//Viewport to render to
	Viewport m_viewport;

	/**
	Other Data
	**/
	bool m_bActive;
	bool m_bDebug;

	/** ===============================
	Methods
	=============================== **/
	void UpdateTransforms();

	//Target camera is following
	shared_ptr<Entity> m_pTarget;
	bool m_bHasTarget;

public:

	//Constructor and destructor
	Camera(const Frustum & frustum, const Vec & position, const Vec & dir, 
		const Vec & up, const Viewport & viewport);

	/*==
	Mutators
	==*/
	__forceinline void SetActive(bool isActive);

	__forceinline void SetPosition(const Vec & pos);
	__forceinline void SetLookAt(const Vec & lookAt);
	__forceinline void SetDir(const Vec & dir);

	__forceinline void SetRoll(float roll);
	__forceinline void SetPitch(float pitch);
	__forceinline void SetYaw(float yaw);

	__forceinline void SetTarget(shared_ptr<Entity> pTarget);
	/*==
	Accessors
	==*/

	__forceinline bool IsActive() const;

	//AVIRTUAL HRESULT SetViewTransform(Scene *pScene);

	__forceinline const Frustum &GetFrustum();

	__forceinline const Mat4x4 & GetProjection() const;
	__forceinline const Mat4x4 & GetView() const;
	__forceinline const Mat4x4 & GetViewProjection();
	__forceinline const Mat4x4 & GetPrevView() const;
	__forceinline const Mat4x4 & GetPrevProjection() const;

	__forceinline const Mat4x4 & GetOrthoProjection();

	__forceinline Vec GetPosition() const;
	__forceinline Vec GetLookAt() const;
	__forceinline Vec GetDir() const;

	__forceinline float	GetRoll() const;
	__forceinline float	GetPitch() const;
	__forceinline float	GetYaw() const;

	__forceinline float	GetNearZ() const;
	__forceinline float	GetFarZ() const;

	__forceinline float GetViewportWidth() const;
	__forceinline float GetViewportHeight() const;

	__forceinline shared_ptr<Entity> GetTarget() const;
	__forceinline bool HasTarget() const;

	/*==
	Methods
	==*/
	virtual void VUpdate(unsigned int elapsedMs);
	__forceinline void BindViewport();
};

__forceinline void Camera::SetActive(bool isActive)
{
	m_bActive = isActive;
}

__forceinline void Camera::SetPosition(const Vec & pos)
{
	m_pos = pos;
}

__forceinline void Camera::SetLookAt(const Vec & lookAt)
{
	m_lookAt = lookAt;
}

__forceinline void Camera::SetDir(const Vec & dir)
{
	m_dir = dir;
}

__forceinline void Camera::SetRoll(float roll)
{
	m_roll = roll;
}

__forceinline void Camera::SetPitch(float pitch)
{
	m_pitch = pitch;
}

__forceinline void Camera::SetYaw(float yaw)
{
	m_yaw = yaw;
}

__forceinline void Camera::SetTarget(shared_ptr<Entity> pTarget)
{
	if (pTarget)
	{
		m_bHasTarget = true;
	}
	else
	{
		m_bHasTarget = false;
	}

	m_pTarget = pTarget;
}

__forceinline bool Camera::IsActive() const
{
	return m_bActive;
}

__forceinline const Frustum & Camera::GetFrustum()
{
	return m_frustum;
}

__forceinline const Mat4x4 & Camera::GetProjection() const
{
	return m_projection;
}

__forceinline const Mat4x4 & Camera::GetView() const
{
	return m_view;
}

__forceinline const Mat4x4 & Camera::GetViewProjection()
{
	if (!m_bVPRecalc)
	{
		m_viewProj = m_view * m_projection;
		m_bVPRecalc = true;
	}

	return m_viewProj;
}

__forceinline const Mat4x4 & Camera::GetPrevView() const
{
	return m_prevView;
}

__forceinline const Mat4x4 & Camera::GetPrevProjection() const
{
	return m_prevProj;
}

__forceinline const Mat4x4 & Camera::GetOrthoProjection()
{
	if (!m_bOrthoRecalc)
	{
		m_orthoProj = Mat4x4::CreateOrthoProjectionLH(SCREEN_WIDTH, SCREEN_HEIGHT, m_frustum.GetNearZ(), m_frustum.GetFarZ());
		m_bOrthoRecalc = true;
	}

	return m_orthoProj;
}

__forceinline Vec Camera::GetPosition() const
{
	return m_pos;
}

__forceinline Vec Camera::GetLookAt() const
{
	return m_lookAt;
}

__forceinline Vec Camera::GetDir() const
{
	return m_dir;
}

__forceinline float	Camera::GetRoll() const
{
	return m_roll;
}

__forceinline float	Camera::GetPitch() const
{
	return m_pitch;
}

__forceinline float	Camera::GetYaw() const
{
	return m_yaw;
}

__forceinline float	Camera::GetNearZ() const
{
	return m_frustum.GetNearZ();
}

__forceinline float	Camera::GetFarZ() const
{
	return m_frustum.GetFarZ();
}

__forceinline float Camera::GetViewportWidth() const
{
	return m_viewport.Width;
}

__forceinline float Camera::GetViewportHeight() const
{
	return m_viewport.Height;
}

__forceinline shared_ptr<Entity> Camera::GetTarget() const
{
	return m_pTarget;
}

__forceinline bool Camera::HasTarget() const
{
	return m_bHasTarget;
}

__forceinline void Camera::BindViewport()
{
	m_viewport.Bind();
}