#pragma once

#include "../General.h"

class RenderTargetBlendParams : public D3D11_RENDER_TARGET_BLEND_DESC
{
public:
	RenderTargetBlendParams();
	bool Init(bool blendEnable, unsigned int srcBlend, unsigned int destBlend,
		unsigned int blendOp, unsigned int srcBlendAlpha, unsigned int destBlendAlpha, unsigned int blendOpAlpha,
		unsigned int renderTargetWriteMask);
};

__forceinline RenderTargetBlendParams::RenderTargetBlendParams()
{ 
	ZeroMemory(this, sizeof(RenderTargetBlendParams));
}

__forceinline bool RenderTargetBlendParams::Init(bool blendEnable, unsigned int srcBlend, unsigned int destBlend,
	unsigned int blendOp, unsigned int srcBlendAlpha, unsigned int destBlendAlpha, unsigned int blendOpAlpha,
	unsigned int renderTargetWriteMask)
{
	BlendEnable = blendEnable;
	SrcBlend = (D3D11_BLEND)srcBlend;
	DestBlend = (D3D11_BLEND)destBlend;
	BlendOp = (D3D11_BLEND_OP)blendOp;
	SrcBlendAlpha = (D3D11_BLEND)srcBlendAlpha;
	DestBlendAlpha = (D3D11_BLEND)destBlendAlpha;
	BlendOpAlpha = (D3D11_BLEND_OP)blendOpAlpha;
	RenderTargetWriteMask = renderTargetWriteMask;


	return true;
}

class BlendStateParams : public D3D11_BLEND_DESC
{
public:
	BlendStateParams();
	bool Init(bool alphaToCoverageEnable, bool independantBlendEnable,
		RenderTargetBlendParams* targets, unsigned int numParams);
};

__forceinline BlendStateParams::BlendStateParams()
{ 
	ZeroMemory(this, sizeof(BlendStateParams)); 
}

__forceinline bool BlendStateParams::Init(bool alphaToCoverageEnable, bool independantBlendEnable,
	RenderTargetBlendParams* targets, unsigned int numParams)
{
	AlphaToCoverageEnable = alphaToCoverageEnable;
	IndependentBlendEnable = independantBlendEnable;

	for (int i = 0; i < numParams; i++)
		RenderTarget[i] = targets[i];

	if (numParams < 8 && !independantBlendEnable)
	{
		for (int i = 0; i < 8; i++)
		{
			RenderTarget[i] = *targets;
		}
	}

	return true;
}

class BlendState
{
protected:
	ID3D11BlendState*	m_pState;

public:
	BlendState();
	~BlendState();

	bool Create(const D3D11_BLEND_DESC * pParams);
	void Bind(float blendFactor[4], unsigned int sampleMask = 0xffffffff);
};

__forceinline BlendState::BlendState() 
{ 
	m_pState = nullptr; 
}
__forceinline BlendState::~BlendState() 
{
	SAFE_RELEASE(m_pState); 
}

__forceinline bool BlendState::Create(const D3D11_BLEND_DESC * pParams)
{
	HRESULT hr = DX11API::D3D11Device()->CreateBlendState(pParams, &m_pState);

	VALID(hr);
}

__forceinline void BlendState::Bind(float blendFactor[4], unsigned int sampleMask)
{
	DX11API::D3D11DeviceContext()->OMSetBlendState(m_pState, blendFactor, sampleMask);
}