#pragma once


class DemoStateFactory : public StateFactory
{
public:
	// =======	Implement State Factory  ======= //
	virtual State * VCreateState(int i32Id) const;
};