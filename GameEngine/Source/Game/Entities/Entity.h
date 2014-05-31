#pragma once

#include "../../Math/Matrix.h"

class EntityRepresentation;
class Entity
{
protected:

	EntityId m_id;

	//entity transformation matrix
	Mat4x4		m_prevWorldMatrix;
	Mat4x4		m_currentWorldMatrix;

	shared_ptr<EntityRepresentation> m_pRepresentation;

public:

	/*
	* Constructor
	*
	* @param id unique entity id
	*/
	__forceinline Entity(EntityId id);

	__forceinline EntityId GetId() const;
	__forceinline Mat4x4 GetCurrentTransform() const;
	
	__forceinline void SetRepresentation(shared_ptr<EntityRepresentation>);
	void SetCurrentTransform(const Mat4x4 & transform, double currentTime);

	virtual void VUpdate(double time, double elapsedtime);
};

__forceinline Entity::Entity(EntityId id)
	: m_id{ id }
{}

__forceinline EntityId Entity::GetId() const
{
	return m_id;
}
__forceinline Mat4x4 Entity::GetCurrentTransform() const
{
	return m_currentWorldMatrix;
}

__forceinline void Entity::SetRepresentation(shared_ptr<EntityRepresentation> pRep)
{
	m_pRepresentation = pRep;
}