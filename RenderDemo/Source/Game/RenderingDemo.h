#pragma once

#include "Game\Game.h"

class RenderingDemo : public Game
{
public:
	virtual bool VInitialize() override;
	virtual LRESULT	VMsgProc(const SystemMessage * msg) override;
};