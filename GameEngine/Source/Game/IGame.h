#pragma once

class IGame
{
public:

	virtual void VUpdate(double totaltime, double elapsedtime) = 0;
	virtual void VRender(double totaltime, double elapsedtime) = 0;
};