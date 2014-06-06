#pragma once

#include "../../Math/Matrix.h"

class IMovable
{
protected:

	Mat4x4 m_worldTransform;

public:

	virtual void SetWorldTransform(const Mat4x4 & transform) = 0;
	__forceinline const Mat4x4 & GetWorldTransform() const;
};

__forceinline const Mat4x4 & IMovable::GetWorldTransform() const
{
	return m_worldTransform;
}