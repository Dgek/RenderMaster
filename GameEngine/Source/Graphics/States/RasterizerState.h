#pragma once

class RasterizerState
{
protected:
	ID3D11RasterizerState*	m_pState;

public:
	__forceinline RasterizerState();
	__forceinline ~RasterizerState();

	__forceinline bool Create(const D3D11_RASTERIZER_DESC * pParams);
	__forceinline void Set();
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
	HRESULT hr = DX11API::D3D11Device()->CreateRasterizerState(params, &m_pState);

	VALID(hr);
}

__forceinline void RasterizerState::Set()
{
	DX11API::D3D11DeviceContext()->RSSetState(m_pState);
}