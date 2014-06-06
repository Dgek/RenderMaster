#pragma once

#include "Renderer.h"
#include "RenderQueue.h"

class Light;
class TiledRenderer : public Renderer
{
protected:

	RenderQueue m_queue;
	vector<shared_ptr<Light>> m_lights;

	/*** Tiled Shading Zone ***/
	void ZPrepass();
	void LightCulling(Camera * pCamera);
	void FinalShading();
	void NullLightCounter();

public:

	TiledRenderer();

	virtual bool VInitialize(HWND hWnd, unsigned int width, unsigned int height) override;

	virtual void VPushMesh(shared_ptr<Mesh> pMesh) override;
	virtual void VPushLight(shared_ptr<Light> pLight) override;

	virtual void VUpdate(unsigned int deltaMilliseconds) override;
	virtual void VRender() override;
};