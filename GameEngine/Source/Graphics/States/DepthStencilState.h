#pragma once

#include "../General.h"

class DepthStencilState
{
protected:
	ID3D11DepthStencilState*	m_pState;

public:
	__forceinline DepthStencilState();
	__forceinline ~DepthStencilState();

	__forceinline bool Create(const D3D11_DEPTH_STENCIL_DESC * params);

	__forceinline void Bind(unsigned int stencilRef) const;
};

__forceinline DepthStencilState::DepthStencilState() 
{
	m_pState = nullptr; 
}
__forceinline DepthStencilState::~DepthStencilState() 
{ 
	SAFE_RELEASE(m_pState); 
}

__forceinline bool DepthStencilState::Create(const D3D11_DEPTH_STENCIL_DESC * params)
{
	HRESULT hr = DX11API::D3D11Device()->CreateDepthStencilState(params, &m_pState);

	VALID(hr);
}

__forceinline void DepthStencilState::Bind(unsigned int stencilRef) const
{
	DX11API::D3D11DeviceContext()->OMSetDepthStencilState(m_pState, stencilRef);
}