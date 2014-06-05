#pragma once

#include "Renderer.h"

class TiledRenderer : public Renderer
{
protected:

	/*** Tiled Shading Zone ***/
	void ZPrepass();
	void LightCulling(Camera * pCamera);
	void FinalShading();
	void NullLightCounter();

public:

	TiledRenderer();

	virtual void VRender() override;
};