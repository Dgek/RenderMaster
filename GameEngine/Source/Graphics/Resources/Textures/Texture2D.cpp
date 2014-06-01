#include "Core.h"

#include "Texture2D.h"

ABOOL Anubis::Texture2D::CreateShaderResourceView(ShaderResourceViewDX11 & view,
	const ShaderResourceViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}
ABOOL Anubis::Texture2D::CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
	const ShaderResourceViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, ppView);

	VALID(hr);
}


ABOOL Texture2D::CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
	const UnorderedAccessViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}
ABOOL Texture2D::CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
	const UnorderedAccessViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, ppView);

	VALID(hr);
}


ABOOL Texture2D::CreateRenderTargetView(RenderTargetViewDX11 & view,
	const RenderTargetViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture2D::CreateRenderTargetView(ID3D11RenderTargetView** ppView,
	const RenderTargetViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture2D::CreateDepthStencilView(DepthStencilViewDX11 & view,
	const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture2D::CreateDepthStencilView(ID3D11DepthStencilView** ppView,
	const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture2D::Create(const D3D11_TEXTURE2D_DESC* pParams)
{
	HRESULT hr = D3D11Device()->CreateTexture2D(pParams, NULL, &m_pTexture);

	VALID(hr);
}