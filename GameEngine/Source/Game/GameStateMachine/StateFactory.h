#pragma once

class State;
class StateFactory
{
public:

	virtual State* VCreateState(StateId id) const;
};