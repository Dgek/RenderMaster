#pragma once

#include "GameStateMachine\StateFactory.h"
#include "GameStateMachine\StateMachine.h"

#include "Entities\EntityManager.h"

class Entity;
class IView;
class PlayerView;
struct SystemMessage;
class Mat4x4;
class Game
{
	friend class App;

protected:
	/** -------------------
	Entities
	----------------------- **/
	EntityId m_lastEntityId; //id of last added actor
	unique_ptr<EntityManager> m_pEntityManager; //entity manager

	/** -------------------
	States
	----------------------- **/
	unique_ptr<StateFactory> m_pStateFactory;
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

	Game();
	virtual ~Game();

	virtual bool VInitialize() = 0 { return true; };

	//game states
	void SetStateFactory(unique_ptr<StateFactory> pStateFactory);
	void SetNextState(int i32StateId, UINT_PTR uptrData = 0);

	/*==
	//App stuff
	==*/
	virtual LRESULT	VMsgProc(const SystemMessage * msg);

	/*==
	//Updating methods
	==*/
	virtual void VUpdate(double totaltime, double elapsedtime);

	/*==
	//Rendering methods
	==*/
	virtual void VRender(Renderer* pRenderer, double totaltime, double elapsedtime);
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
	EntityId GetNewEntityId();

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
	m_pStateMachine = new StateMachine{};
	m_pEntityManager = make_unique<EntityManager>();
}

__forceinline EntityId Game::GetNewEntityId() 
{
	return ++m_lastEntityId; 
}