#pragma once

#include "Plane.h"

class Frustum
{
public:
	enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };

	Plane m_planes[NumPlanes];	// planes of the frusum in camera space
	Vec m_nearClip[4];			// verts of the near clip plane in camera space
	Vec m_farClip[4];			// verts of the far clip plane in camera space
	Vec m_frustumRays[4];		// frustum rays 

	float m_fov;				// field of view in radians
	float m_aspect;				// aspect ratio - width divided by height
	float m_near;				// near clipping distance
	float m_far;				// far clipping distance

	float m_fNearPlaneWidth;
	float m_fNearPlaneHeight;

public:

	//constructor
	__forceinline Frustum();

	void Init(float fov, float aspect, float near, float far);

	/**
	Methods
	**/
	__forceinline bool Inside(const Vec & point) const;
	__forceinline bool Inside(const Vec & point, float radius) const;

	/**
	Getters
	**/
	__forceinline const Plane & Get(Side side) const;
	__forceinline float GetFOV() const;
	__forceinline float GetAspectRatio() const;
	__forceinline float GetNearZ() const;
	__forceinline float GetFarZ() const;
	__forceinline float GetNearWidth() const;
	__forceinline float GetNearHeight() const;
	__forceinline const Vec & GetLowerLeftRay() const;
	__forceinline const Vec & GetLowerRightRay() const;
	__forceinline const Vec & GetUpperLeftRay() const;
	__forceinline const Vec & GetUpperRightRay() const;

	/**
	Mutators
	**/
	__forceinline void SetFOV(float fov);
	__forceinline void SetAspect(float aspect);
	__forceinline void SetNear(float nearClip);
	__forceinline void SetFar(float farClip);

	void Render();
};

__forceinline Frustum::Frustum()
{
	m_fov = static_cast<float>(PI) / 4.0f;										//default field of view is 90 degrees
	m_aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);//default aspect ratio 
	m_near = 0.1f;										//default near clip plane is 1m away from the camera
	m_far = 1000.0f;									//default near clip plane is 100m away from the camera
}

__forceinline bool Frustum::Inside(const Vec & point) const
{
	//point is inside the frustum only if it is inside each of the plain
	for (int i = 0; i<NumPlanes; i++)
	{
		if (!m_planes[i].Inside(point))
			return false;
	}

	return true;
}
__forceinline bool Frustum::Inside(const Vec & point, float radius) const
{
	//point is inside the frustum only if it is inside each of the plain
	for (int i = 0; i < NumPlanes; ++i)
	{
		if (!m_planes[i].Inside(point, radius))
			return false;
	}

	//we are fully in view
	return true;
}

__forceinline const Plane & Frustum::Get(Side side) const 
{ 
	return m_planes[side]; 
}

__forceinline float Frustum::GetFOV() const 
{ 
	return m_fov; 
}

__forceinline float Frustum::GetAspectRatio() const 
{
	return m_aspect;
}

__forceinline float Frustum::GetNearZ() const 
{ 
	return m_near; 
}

__forceinline float Frustum::GetFarZ() const
{ 
	return m_far; 
}

__forceinline float Frustum::GetNearWidth() const 
{ 
	return m_fNearPlaneWidth; 
}

__forceinline float Frustum::GetNearHeight() const 
{ 
	return m_fNearPlaneHeight;
}

__forceinline const Vec & Frustum::GetLowerLeftRay() const 
{ 
	return m_frustumRays[3];
}

__forceinline const Vec & Frustum::GetLowerRightRay() const 
{ 
	return m_frustumRays[2];
}

__forceinline const Vec & Frustum::GetUpperLeftRay() const 
{ 
	return m_frustumRays[0]; 
}

__forceinline const Vec & Frustum::GetUpperRightRay() const 
{ 
	return m_frustumRays[1];
}

__forceinline void Frustum::SetFOV(float fov) 
{ 
	m_fov = fov;
	Init(m_fov, m_aspect, m_near, m_far); 
}

__forceinline void Frustum::SetAspect(float aspect) 
{ 
	m_aspect = aspect;
	Init(m_fov, m_aspect, m_near, m_far); 
}

__forceinline void Frustum::SetNear(float nearClip) 
{ 
	m_near = nearClip;
	Init(m_fov, m_aspect, m_near, m_far); 
}

__forceinline void Frustum::SetFar(float farClip) 
{ 
	m_far = farClip;
	Init(m_fov, m_aspect, m_near, m_far); 
}