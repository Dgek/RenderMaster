#include "Core.h"

#include "Game.h"
#include "Views.h"
#include "Entities\EntityManager.h"

Game::~Game()
{
	m_pStateMachine->Release(this);
	m_pStateMachine = nullptr;
}

LRESULT	Game::VMsgProc(const SystemMessage & msg)
{
	for (auto iter = begin(m_views); iter != end(m_views); ++iter)
	{
		if ((*iter)->VMsgProc(msg))
		{
			return 1;
		}
	}

	return 0;
}

void Game::VUpdate(double time, double elapsedTime)
{
	//change game state if needed
	if (m_ngsState.m_bSetState)
	{
		m_ngsState.m_bSetState = false;
		SetState(m_ngsState.m_i32Id);
	}

	//update a game state
	m_pStateMachine->VUpdate(this, time, elapsedTime);

	const unsigned int deltaMilliseconds = static_cast<unsigned int>(elapsedTime * 1000.0f);
	for (auto iter = begin(m_views); iter != end(m_views); ++iter)
		(*iter)->VUpdate(deltaMilliseconds);

	//update entities
	m_pEntityManager->VUpdate(time, elapsedTime);
}

/********************************
** Entities management
*********************************/
shared_ptr<Entity> Game::VAddEntity(shared_ptr<Entity> pEntity)
{
	m_pEntityManager->VAddEntity(pEntity);
	return pEntity;
}

shared_ptr<Entity> Game::VGetEntity(EntityId id)
{
	return m_pEntityManager->VGetEntity(id);
}

bool Game::VRemoveEntity(EntityId id)
{
	return m_pEntityManager->VRemoveEntity(id);
}

void Game::VUpdateEntity(EntityId id, Mat4x4 const & transform, double r64CurrentTime)
{
	m_pEntityManager->VUpdateEntity(id, transform, r64CurrentTime);
}

/********************************
** States management
*********************************/
void Game::SetStateFactory(unique_ptr<StateFactory> pStateFactory)
{
	m_pStateFactory = move(pStateFactory);
}

void Game::SetNextState(int i32StateId, UINT_PTR uptrData)
{
	m_ngsState.m_i32Id = i32StateId;
	m_ngsState.m_uptrData = uptrData;
	m_ngsState.m_bSetState = true;
}

bool Game::SetState(int i32StateId)
{
	//create state
	State * pState = m_pStateFactory->VCreateState(i32StateId);

	//bind state
	return m_pStateMachine->SetState(this, pState, i32StateId);
}

/********************************
** Views management
*********************************/
shared_ptr<IView> Game::VAddGameView(shared_ptr<IView> pView)
{
	m_views.push_back(pView);
	return pView;
}

void Game::VRemoveGameView(shared_ptr<IView> pView)
{
}

void Game::AddPlayerView(shared_ptr<PlayerView> pView)
{
	m_playerViews.push_back(pView);
}

shared_ptr<PlayerView> Game::GetPlayerView(int player)
{
	return m_playerViews[player];
}

/**********************************
** Rendering methods
**********************************/
void Game::VRenderDebug()
{
}

void Game::VRender(Renderer * pRenderer, double time, double elapsedTime)
{
	//render current game state
	for (auto iter = begin(m_views); iter != end(m_views); ++iter)
		(*iter)->VRender(pRenderer, time, elapsedTime);

	//render a game state
	m_pStateMachine->VRender(this, pRenderer, time, elapsedTime);
}