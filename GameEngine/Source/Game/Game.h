#pragma once

#include "GameStateMachine\StateFactory.h"
#include "GameStateMachine\StateMachine.h"

class Entity;
class IView;
class View;
class PlayerView;
class EntityManager;
class SystemMessage;
class Mat4x4;
class Game
{
protected:
	/** -------------------
	Entities
	----------------------- **/
	EntityId m_lastEntityId; //id of last added actor
	unique_ptr<EntityManager> m_pEntityManager; //entity manager

	/** -------------------
	States
	----------------------- **/
	StateFactory* m_pStateFactory;
	StateMachine* m_pStateMachine;

	//keep data about the next game state
	struct NextGameState
	{
		int m_i32Id;	//next state id
		UINT_PTR m_uptrData; //next state data
		bool m_bSetState;//true if state change is necessary
	};
	NextGameState	m_ngsState;

	/** -------------------
	Views
	----------------------- **/
	vector<shared_ptr<IView>> m_views;
	vector<shared_ptr<PlayerView>> m_playerViews;

public:

	//============================
	// Initializing
	//============================

	__forceinline Game();
	virtual ~Game();

	//game states
	void SetStateFactory(StateFactory * pStateFactory);
	void SetNextState(int i32StateId, UINT_PTR uptrData = 0);

	/*==
	//App stuff
	==*/
	virtual LRESULT	VMsgProc(const SystemMessage & msg);

	/*==
	//Updating methods
	==*/
	virtual void VUpdate(double totaltime, double elapsedtime) = 0;

	/*==
	//Rendering methods
	==*/
	virtual void VRender(double totaltime, double elapsedtime) = 0;
	virtual void VRenderDebug();

	///////////////////////////////////////////////////////////////////////////////////////////

	/*==
	//Entities management
	==*/
	virtual shared_ptr<Entity> VAddEntity(shared_ptr<Entity> pEntity);
	virtual shared_ptr<Entity> VGetEntity(EntityId id);
	virtual bool VRemoveEntity(EntityId id);

	//Update entity transform matrix
	virtual void VUpdateEntity(EntityId id, const Mat4x4 & transform, double currentTime);
	__forceinline EntityId GetNewEntityId();

	/*==
	//States management
	==*/
	bool SetState(int i32StateId);

	/*==
	//Views management
	==*/
	virtual shared_ptr<IView> VAddGameView(shared_ptr<IView> pView);
	virtual void VRemoveGameView(shared_ptr<IView> pView);
	//const Views * GetViews() const { return &m_views; }

	void AddPlayerView(shared_ptr<PlayerView> pView);
	shared_ptr<PlayerView> GetPlayerView(int player);
};

__forceinline Game::Game()
	: m_lastEntityId{ 0 }
{
	m_pStateMachine = new StateMachine();
	m_pEntityManager = new EntityManager();
}

__forceinline EntityId Game::GetNewEntityId() 
{
	return ++m_lastEntityId; 
}