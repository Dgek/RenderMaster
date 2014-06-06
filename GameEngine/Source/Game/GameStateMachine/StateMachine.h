#pragma once

/** ======================
States Manager
====================== **/
class Game;
class State;
class Renderer;
class StateMachine
{
public:

	//Constructor
	__forceinline StateMachine();

	void Release(Game * pGame);

	//====================
	// State management
	//====================
	bool SetState(Game * pGame, State * pState, int stateId); //set current state
	bool ReleaseCurrentState(Game * pGame); //remove current state

	//====================
	// Running
	//====================
	void VUpdate(Game * pGame, double time, double elapsedTime);
	void VRender(Game * pGame, Renderer * pRenderer, double r64Time, double r64ElapsedTime);

private:

	//Current State
	State *	m_pCurrentState;
};

__forceinline StateMachine::StateMachine()
	: m_pCurrentState{ nullptr }
{}