#pragma once

#include "../Graphics/States/BlendState.h"
#include "../Graphics/States/DepthStencilState.h"
#include "../Graphics/States/RasterizerState.h"

class Mesh;
class Light;
class Camera;
class ConstantBuffer;
class SamplerState;
class Texture2D;
class DepthStencilView;
class RenderTargetView;
class ShaderResourceView;
class UnorderedAccessView;
class Renderer
{
protected:
	vector<shared_ptr<Camera>> m_cameras;

	/*** =========================
	Rendering resources
	========================= ***/
	unique_ptr<Texture2D> m_pDepthTexture;
	unique_ptr<DepthStencilView> m_pDepthDSV;

	/*** =========================
	Rendering Parameteres
	========================= ***/
	bool m_bTexturingOn;
	bool m_bLightningOn;

	/** ================================
	Constant Buffers
	================================
	*/
	unique_ptr<ConstantBuffer> m_pcb16Bytes;
	unique_ptr<ConstantBuffer> m_pcb64Bytes;
	unique_ptr<ConstantBuffer> m_pcb128Bytes;
	unique_ptr<ConstantBuffer> m_pcb192Bytes;
	unique_ptr<ConstantBuffer> m_pcb256Bytes;

	static unsigned int m_numSamples;
	static unsigned int m_sampleQuality;
	static bool m_bMSAA;

private:
	/** ================================
	Sampler States
	================================
	*/
	unique_ptr<SamplerState> m_pLinearTiledSampler;
	unique_ptr<SamplerState> m_pAnisotropySampler16;
	unique_ptr<SamplerState> m_pPointClampSampler;
	unique_ptr<SamplerState> m_pLinearLessEqualSampler;

	/** ================================
	Rasterizer States
	================================
	*/
	unique_ptr<RasterizerState> m_pAllEnabledBackCullingRasterizer;
	unique_ptr<RasterizerState> m_pNoCullingStandardRasterizer;
	unique_ptr<RasterizerState> m_pAllDisabledBackCullingRasterizer;

	/** ================================
	Blend States
	================================
	*/
	unique_ptr<BlendState> m_pNoBlending;
	unique_ptr<BlendState> m_pBlendAddStandard;
	unique_ptr<BlendState> m_pBlendLightPass;

	/** ================================
	Depth Stencil States
	================================
	**/
	unique_ptr<DepthStencilState> m_pDepthEnableStencilDisableStandard;
	unique_ptr<DepthStencilState> m_pDepthDisableStencilDisable;

public:
	Renderer();
	virtual ~Renderer();

	/***	Initialize	  ***/
	virtual bool VInitialize(HWND hWnd, unsigned int width, unsigned int height);

	/***	Put something to render	***/
	virtual void VPushMesh(shared_ptr<Mesh> pMesh) = 0;
	virtual void VPushLight(shared_ptr<Light> pLight) = 0;

	/***	Cameras management	***/
	virtual void VAddCamera(shared_ptr<Camera> pCamera);
	virtual void VRemoveCamera(Camera* pCamera);

	virtual void VUpdate(unsigned int deltaMilliseconds) = 0;

	/*** Rendering Core ***/
	virtual void VRender() = 0;

	/*** Accessors ***/
	__forceinline bool LightningOn() const;
	__forceinline bool TexturingOn() const;

	SamplerState* LinearTiledSampler();
	SamplerState* AnisotropySampler16();
	SamplerState* PointClampSampler();
	SamplerState* LinearLessEqualSampler();

	__forceinline RasterizerState* AllEnabledBackCullingRasterizer();
	__forceinline RasterizerState* NoCullingStandardRasterizer();
	__forceinline RasterizerState* AllDisabledBackCullingRasterizer();

	__forceinline BlendState* NoBlending();
	__forceinline BlendState* BlendAddStandard();
	__forceinline BlendState* BlendLightPass();

	__forceinline DepthStencilState* DepthEnableStencilDisableStandard();
	__forceinline DepthStencilState* DepthDisableStencilDisable();
};

__forceinline RasterizerState* Renderer::AllDisabledBackCullingRasterizer()
{
	if (m_pAllDisabledBackCullingRasterizer)
		return m_pAllDisabledBackCullingRasterizer.get();

	//not created yet
	m_pAllDisabledBackCullingRasterizer = make_unique<RasterizerState>();
	RasterizerParams params;
	params.Init(false, false, false, true, 0.0f, 0.0f, 0.0f, false, false, true, true);
	m_pAllDisabledBackCullingRasterizer->Create(&params);

	return m_pAllDisabledBackCullingRasterizer.get();
}

__forceinline RasterizerState* Renderer::NoCullingStandardRasterizer()
{
	if (m_pNoCullingStandardRasterizer)
		return m_pNoCullingStandardRasterizer.get();

	//not created yet
	m_pNoCullingStandardRasterizer = make_unique<RasterizerState>();
	RasterizerParams params;
	params.Init(false, false, false, true, 0.0f, 0.0f, 0.0f, false, false, false, false);
	m_pNoCullingStandardRasterizer->Create(&params);

	return m_pNoCullingStandardRasterizer.get();
}

__forceinline RasterizerState* Renderer::AllEnabledBackCullingRasterizer()
{
	if (m_pAllEnabledBackCullingRasterizer)
		return m_pAllEnabledBackCullingRasterizer.get();

	//not created yet
	m_pAllEnabledBackCullingRasterizer = make_unique<RasterizerState>();
	RasterizerParams params;
	//params.Init(false, false, false, true, 0.0f, 0.0f, 0.0f, true, true, true, true);
	params.Init(false, false, true, true, 0.0f, 0.0f, 0.0f, true, true, true, true);
	m_pAllEnabledBackCullingRasterizer->Create(&params);

	return m_pAllEnabledBackCullingRasterizer.get();
}

__forceinline BlendState* Renderer::NoBlending()
{
	if (m_pNoBlending)
		return m_pNoBlending.get();

	//not created yet
	m_pNoBlending = make_unique<BlendState>();
	RenderTargetBlendParams rtBlendParams;
	rtBlendParams.Init(false, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, 0x0f);
	BlendStateParams params;
	params.Init(false, false, &rtBlendParams, 1);

	m_pNoBlending->Create(&params);

	return m_pNoBlending.get();
}

__forceinline BlendState* Renderer::BlendAddStandard()
{
	if (m_pBlendAddStandard) 
		return m_pBlendAddStandard.get();

	//not created yet
	m_pBlendAddStandard = make_unique<BlendState>();
	RenderTargetBlendParams rtBlendParams;
	rtBlendParams.Init(true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP_ADD, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, 0x0f);
	BlendStateParams params;
	params.Init(false, false, &rtBlendParams, 1);

	m_pBlendAddStandard->Create(&params);

	return m_pBlendAddStandard.get();
}

__forceinline BlendState* Renderer::BlendLightPass()
{
	if (m_pBlendLightPass) 
		return m_pBlendLightPass.get();

	//not created yet
	m_pBlendLightPass = make_unique<BlendState>();
	RenderTargetBlendParams rtBlendParams;
	rtBlendParams.Init(true, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, 0x0f);
	BlendStateParams params;
	params.Init(false, false, &rtBlendParams, 1);

	m_pBlendLightPass->Create(&params);

	return m_pBlendLightPass.get();
}

__forceinline DepthStencilState* Renderer::DepthEnableStencilDisableStandard()
{
	if (m_pDepthEnableStencilDisableStandard)
		return m_pDepthEnableStencilDisableStandard.get();

	m_pDepthEnableStencilDisableStandard = make_unique<DepthStencilState>();

	DepthStencilParams depthStencilParams;
	depthStencilParams.Init(true, D3D11_COMPARISON_LESS, false, 0, 0);

	m_pDepthEnableStencilDisableStandard = make_unique<DepthStencilState>();
	m_pDepthEnableStencilDisableStandard->Create(&depthStencilParams);

	return m_pDepthEnableStencilDisableStandard.get();
}

__forceinline DepthStencilState* Renderer::DepthDisableStencilDisable()
{
	if (m_pDepthDisableStencilDisable)
		return m_pDepthDisableStencilDisable.get();

	DepthStencilParams depthStencilParams;
	depthStencilParams.Init(false, D3D11_COMPARISON_LESS, false, 0, 0);

	m_pDepthDisableStencilDisable = make_unique<DepthStencilState>();
	m_pDepthDisableStencilDisable->Create(&depthStencilParams);

	return m_pDepthDisableStencilDisable.get();
}