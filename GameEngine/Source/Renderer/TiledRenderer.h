#pragma once

#include "Renderer.h"
#include "RenderQueue.h"

class Light;
class TiledRenderer : public Renderer
{
protected:

	RenderQueue m_queue;
	vector<shared_ptr<Light>> m_lights;

	Camera *m_pCurrentCamera;
	Mesh *m_pCurrentMesh;

	//Z prepass


	/*** Tiled Shading Zone ***/
	void PrepareForZPrepass();
	void RenderZPrepass(Mesh* pMesh);
	void FinishZPrepass();

	void LightCulling();
	void FinalShading();
	void NullLightCounter();
	void FinishShadingPass();

	/*** Global Illumination ***/
	void Voxelize();
	void InjectVPLs();
	void PropagateVPLs(unsigned int index);
	void ApplyGlobalIllumination();

public:

	TiledRenderer();

	virtual bool VInitialize(HWND hWnd, unsigned int width, unsigned int height) override;

	virtual void VPushMesh(shared_ptr<Mesh> pMesh) override;
	virtual void VPushLight(shared_ptr<Light> pLight) override;

	virtual void VUpdate(unsigned int deltaMilliseconds) override;
	virtual void VRender() override;

	void VPreRenderMesh(Mesh*);
	void VRenderMesh(Mesh* pMesh);
	void VPostRenderMesh(Mesh* pMesh);

	void PrepareForShadingPass();
	void UpdateLightBuffers();
	void VFinishPass();
};