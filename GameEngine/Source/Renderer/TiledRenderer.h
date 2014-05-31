#pragma once

#include "Renderer.h"

class TiledRenderer : public Renderer
{
public:

	TiledRenderer();

	virtual void VRender() override;
};