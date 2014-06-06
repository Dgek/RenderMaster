#pragma once

#include "Game\GameStateMachine\State.h"

class TestState : public State
{
protected:

public:

	__forceinline TestState();

	virtual void VInitialize(Game * pGame, UINT_PTR uptrData = 0) override;

	virtual void VUpdate(Game * pGame, double time, double elapsedTime) override;

	virtual void VRender(Game * pGame, Renderer * pRenderer, double r64Time, double r64ElapsedTime) override;

};

__forceinline TestState::TestState()
	: State{}
{}