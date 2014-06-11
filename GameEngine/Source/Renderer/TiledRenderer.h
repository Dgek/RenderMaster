#pragma once

#include "Renderer.h"
#include "RenderQueue.h"

#include "../Graphics/Other/Viewport.h"

class Light;
class Texture2D;
class StructuredBuffer;
class RenderTargetView;
class ShaderResourceView;
class ShaderBunch;
class ComputeShader;
class VertexBuffer;
class TiledRenderer : public Renderer
{
protected:

	RenderQueue m_queue;
	vector<shared_ptr<Light>> m_lights;

	Camera *m_pCurrentCamera;
	Mesh *m_pCurrentMesh;

	//Z prepass
	unique_ptr<Texture2D> m_pDepthNormalTexture;
	unique_ptr<Texture2D> m_pAlbedoGlossTexture;
	unique_ptr<RenderTargetView> m_pPrepassRTVs;
	unique_ptr<ShaderResourceView> m_pPrepassSRVs;
	unique_ptr<ShaderBunch> m_pPrepassShaders;

	//light culling
	unique_ptr<StructuredBuffer> m_psbLightIdx;
	unique_ptr<UnorderedAccessView> m_pLightIdxUAV;
	unique_ptr<ShaderResourceView> m_pLightIdxSRV;

	unique_ptr<StructuredBuffer> m_psbLowerBound;
	unique_ptr<UnorderedAccessView> m_pLowerBoundUAV;
	unique_ptr<ShaderResourceView> m_pLowerBoundSRV;

	unique_ptr<StructuredBuffer> m_psbHigherBound;
	unique_ptr<UnorderedAccessView> m_pHigherBoundUAV;
	unique_ptr<ShaderResourceView> m_pHigherBoundSRV;

	unique_ptr<StructuredBuffer> m_psbLightCounter;
	unique_ptr<UnorderedAccessView> m_pLightCounterUAV;
	unique_ptr<ShaderResourceView> m_pLightCounterSRV;

	unique_ptr<StructuredBuffer> m_psbLightGeometry;
	unique_ptr<ShaderResourceView> m_pLightGeometrySRV;

	unique_ptr<StructuredBuffer> m_psbLights;
	unique_ptr<ShaderResourceView> m_pLightsSRV;

	////////////////////////////////////
	//Voxelized global illumination
	unique_ptr<StructuredBuffer> m_voxelGrid;
	unique_ptr<ShaderResourceView> m_voxelGridSRV;
	unique_ptr<UnorderedAccessView> m_voxelGridUAV;

	unique_ptr<Texture2D> m_voxelTexture;
	unique_ptr<RenderTargetView> m_voxelTextureRTV;

	INPUT_LAYOUT* m_pLightGridLayout;
	unique_ptr<VertexBuffer> m_pVoxelVB;
	unique_ptr<Texture2D> m_redVoxelSH;
	unique_ptr<Texture2D> m_greenVoxelSH;
	unique_ptr<Texture2D> m_blueVoxelSH;
	unique_ptr<RenderTargetView> m_voxelSHRTVs;
	unique_ptr<ShaderResourceView> m_voxelSHSRVs;
	unique_ptr<UnorderedAccessView> m_voxelSHUAVs;
	unique_ptr<ShaderBunch> m_pointLightGridShaders;
	unique_ptr<ShaderBunch> m_dirLightGridShaders;
	Viewport m_gridViewport;

	unique_ptr<Texture2D> m_redVoxelPropagateSH;
	unique_ptr<Texture2D> m_greenVoxelPropagateSH;
	unique_ptr<Texture2D> m_blueVoxelPropagateSH;
	unique_ptr<UnorderedAccessView> m_voxelPropagateSHUAVs;
	unique_ptr<ShaderResourceView> m_voxelPropagateSHSRVs;
	unique_ptr<ComputeShader> m_propagateCS;
	unique_ptr<ComputeShader> m_pPropagateOcclusionCS;

	unique_ptr<ShaderBunch> m_pGlobalIllumShaders;
	unique_ptr<VertexBuffer> m_pGlobalIllumVB;

	INPUT_LAYOUT*	m_pVoxelizationLayout;
	unique_ptr<ShaderBunch> m_voxelizationShaders;
	unique_ptr<ShaderBunch> m_voxelRenderingShaders;
	Viewport m_voxelViewport;

	unique_ptr<BlendState> m_pGlobalIllumBlendState;

	unique_ptr<ComputeShader> m_pClearGridCS;

	/*** Tiled Shading Zone ***/
	void PrepareForZPrepass();
	void RenderZPrepass(Mesh* pMesh);
	void FinishZPrepass();

	void LightCulling();
	void FinalShading();
	void NullLightCounter();
	void FinishShadingPass();

	void UpdateLightBuffers();

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