#pragma once

#include "../General.h"

class BlendState
{
protected:
	ID3D11BlendState*	m_pState;

public:
	__forceinline BlendState();
	__forceinline ~BlendState();

	__forceinline bool Create(const D3D11_BLEND_DESC * pParams);
	__forceinline void Bind(float blendFactor[4], unsigned int sampleMask = 0xffffffff);
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