#pragma once

#include "../General.h"

class RasterizerState
{
protected:
	ID3D11RasterizerState*	m_pState;

public:
	__forceinline RasterizerState();
	__forceinline ~RasterizerState();

	__forceinline bool Create(const D3D11_RASTERIZER_DESC * pParams);
	__forceinline void Bind();
};

__forceinline RasterizerState::RasterizerState() 
{ 
	m_pState = nullptr; 
}
__forceinline RasterizerState::~RasterizerState() 
{ 
	SAFE_RELEASE(m_pState); 
}

__forceinline bool RasterizerState::Create(const D3D11_RASTERIZER_DESC * pParams)
{
	HRESULT hr = DX11API::D3D11Device()->CreateRasterizerState(pParams, &m_pState);

	VALID(hr);
}

__forceinline void RasterizerState::Bind()
{
	DX11API::D3D11DeviceContext()->RSSetState(m_pState);
}