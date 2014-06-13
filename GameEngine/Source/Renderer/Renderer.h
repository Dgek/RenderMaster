#pragma once

class Mesh;
class Light;
class Camera;
class ConstantBuffer;
class SamplerState;
class RasterizerState;
class BlendState;
class DepthStencilState;
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
};