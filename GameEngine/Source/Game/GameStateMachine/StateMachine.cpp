#include "Core.h"

#include "StateMachine.h"
#include "State.h"

void StateMachine::Release(Game * pGame)
{
	ReleaseCurrentState(pGame);
}

bool StateMachine::SetState(Game * pGame, State * pState, int stateId)
{
	//some error occured deleting current state
	if (!ReleaseCurrentState(pGame))
		return false;

	//assign new state as current
	m_pCurrentState = pState;

	//nothing else is left to do for null states
	if (!pState)
		return true;

	//assign state id
	m_pCurrentState->m_id = stateId;

	//initialize new game state
	m_pCurrentState->VInitialize(pGame);

	return true;
}

bool StateMachine::ReleaseCurrentState(Game * pGame)
{
	//current state is null
	if (!m_pCurrentState)
		return true;

	//destroy current state and release memory
	delete m_pCurrentState;

	//set current state to NULL
	m_pCurrentState = nullptr;

	return true;
}

//====================
// Running
//====================
void StateMachine::VUpdate(Game * pGame, double time, double elapsedTime)
{
	if (m_pCurrentState)
		m_pCurrentState->VUpdate(pGame, time, elapsedTime);
}
void StateMachine::VRender(Game * pGame, Renderer * pRenderer, double time, double elapsedTime)
{
	if (m_pCurrentState)
		m_pCurrentState->VRender(pGame, pRenderer, time, elapsedTime);
}