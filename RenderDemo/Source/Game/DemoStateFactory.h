#pragma once

#include "Game\GameStateMachine\StateFactory.h"

class State;
class DemoStateFactory : public StateFactory
{
public:
	// =======	Implement State Factory  ======= //
	virtual State* VCreateState(StateId id) const override;
};