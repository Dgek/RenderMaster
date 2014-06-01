#include "Core.h"

#include "Texture1D.h"

bool Texture1D::CreateShaderResourceView(ShaderResourceView & view,
	const ShaderResourceViewParams & params) const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

bool Texture1D::CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
	const ShaderResourceViewParams & params) const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, ppView);

	VALID(hr);
}

bool Texture1D::CreateUnorderedAccessView(UnorderedAccessView & view,
	const UnorderedAccessViewParams & params) const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

bool Texture1D::CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
	const UnorderedAccessViewParams & params) const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, ppView);

	VALID(hr);
}

bool Texture1D::CreateRenderTargetView(RenderTargetView & view,
	const RenderTargetViewParams & params) const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

bool Texture1D::CreateRenderTargetView(ID3D11RenderTargetView** ppView,
	const RenderTargetViewParams & params) const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, ppView);

	VALID(hr);
}

bool Texture1D::CreateDepthStencilView(DepthStencilView & view,
	const DepthStencilViewParams & params) const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

bool Texture1D::CreateDepthStencilView(ID3D11DepthStencilView** ppView,
	const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, ppView);

	VALID(hr);
}

bool Texture1D::Create(const D3D11_TEXTURE1D_DESC* pParams)
{
	HRESULT hr = D3D11Device()->CreateTexture1D(pParams, nullptr, &m_pTexture);

	VALID(hr);
}