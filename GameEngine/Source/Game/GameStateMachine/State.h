#pragma once

#include "../../Scene/Scene.h"

/** =======================
Represents Game State
======================= **/
class Game;
class State
{
	friend class StateMachine;

protected:

	//Time the state is running in ms
	__int64	m_time;

	//Unique id
	int	m_id;

	//Scene
	shared_ptr<Scene> m_pScene;

public:

	//Constructor and Destructor
	State();

	//=========================================
	//Methods//
	//=========================================

	virtual void VInitialize(Game * pGame, UINT_PTR uptrData = 0) = 0;

	virtual void VUpdate(Game * pGame, double time, double elapsedTime) = 0;

	virtual void VRender(Game * pGame, Renderer * pRenderer, double r64Time, double r64ElapsedTime) = 0;
};

__forceinline State::State()
{
	m_pScene = make_unique<Scene>();
}