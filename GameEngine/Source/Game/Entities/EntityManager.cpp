#include "Core.h"

#include "EntityManager.h"
#include "Entity.h"

shared_ptr<Entity> EntityManager::VGetEntity(EntityId id)
{
	//iterate the list and extract our entity
	for (auto ent = begin(m_entities); ent != end(m_entities); ++ent)
		//the entity with such id found
	if ((*ent)->GetId() == id)
		return (*ent);

	assert(0 && "No entity with such Id!");
	return nullptr;
}

void EntityManager::VAddEntity(shared_ptr<Entity> pEntity)
{
	//just insert it to the list
	m_entities.push_back(pEntity);
}

bool EntityManager::VRemoveEntity(EntityId id)
{
	//iterate the list and extract our entity
	auto ent = begin(m_entities);
	for (; ent != end(m_entities); ++ent)
	{
		//the entity with such id found
		if ((*ent)->GetId() == id)
			break;
	}

	if (ent != m_entities.end())
	{
		m_entities.erase(ent);
		return true;
	}

	return false;
}

void EntityManager::VUpdate(double time, double elapsedTime)
{
	for (auto ent = begin(m_entities); ent != end(m_entities); ++ent)
	{
		(*ent)->VUpdate(time, elapsedTime);
	}
}

void EntityManager::VUpdateEntity(EntityId id, const Mat4x4 & transform, double currentTime)
{
	//iterate the list and extract our entity
	for (auto ent = begin(m_entities); ent != end(m_entities); ++ent)
	{
		//the entity with such id found
		if ((*ent)->GetId() == id)
		{
			(*ent)->SetCurrentTransform(transform, currentTime);
			return;
		}
	}

	assert(0 && "No entity with such Id!");
}