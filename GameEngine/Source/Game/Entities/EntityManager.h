#pragma once
//======================================//
//		Entity Manager Interface        //
//										//
//	Manages, sort and updates all game	//
//	entities.							//
//======================================//
class Entity;
class Mat4x4;
class IEntityManager
{
public:
	//Return entity by its id
	virtual shared_ptr<Entity> VGetEntity(EntityId id) = 0;

	//Add entity to the manager
	virtual void VAddEntity(shared_ptr<Entity> pEntity) = 0;

	//Remove entity from the manager
	virtual bool VRemoveEntity(EntityId id) = 0;

	//Update entities
	virtual void VUpdate(double time, double elapsedTime) = 0;

	//Update entity transform matrix
	virtual void VUpdateEntity(EntityId id, const Mat4x4 & transform, double currentTime) = 0;
};

////////////////////////////////////////
//Implementation of Entity Manager
////////////////////////////////////////
class EntityManager : public IEntityManager
{
private:

	//List of entities 
	typedef vector<shared_ptr<Entity>> Entities;
	Entities m_entities;

	unsigned int m_count; //number of entities in the list

public:

	__forceinline EntityManager();

	///////////////////////////////////////
	/*** IEntityManager Implementation ***/
	//									 //
	///////////////////////////////////////

	virtual shared_ptr<Entity> VGetEntity(EntityId id) override;

	virtual void VAddEntity(shared_ptr<Entity> pEntity) override;

	virtual bool VRemoveEntity(EntityId id) override;

	virtual void VUpdate(double time, double elapsedTime) override;

	virtual void VUpdateEntity(EntityId id, const Mat4x4 & transform, double currentTime) override;
};

__forceinline EntityManager::EntityManager()
	: m_count{ 0 }
{}