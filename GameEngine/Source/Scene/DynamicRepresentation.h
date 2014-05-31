#pragma once

#include "Representation.h"

class DynamicRepresentation : public EntityRepresentation
{
protected:
	Mat4x4 m_prevWorldMatrix;

public:

	__forceinline DynamicRepresentation(EntityRepresentationId id);

	DynamicRepresentation(EntityRepresentationId id, const string & resourceMeshName);
	
	//set current and previous matrices to initial state
	void SetBothTransforms(Mat4x4 & mat, double currentTime);
	void SetCurrentTransform(Mat4x4 & mat, double currentTime);

protected:

	virtual void VPushParameters(Scene *pScene, double currentTime) override;
};

__forceinline DynamicRepresentation::DynamicRepresentation(EntityRepresentationId id) : EntityRepresentation(id)
{}