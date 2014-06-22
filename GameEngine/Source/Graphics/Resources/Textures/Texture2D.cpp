#include "Core.h"

#include "Texture2D.h"

#include "../Views/ShaderResourceView.h"
#include "../Views/UnorderedAccessView.h"
#include "../Views/DepthStencilView.h"
#include "../Views/RenderTargetView.h"

#include "../../../Help/Strings/Strings.h"

void Texture2D::CreateFromFile(const char * filepath)
{
	ID3D11Resource* pRes = static_cast<ID3D11Resource*>(m_pTexture);
	D3DX11CreateTextureFromFile(DX11API::D3D11Device(), StringHelper::s2ws(filepath).c_str(), nullptr, nullptr, &pRes, nullptr);
	m_pTexture = static_cast<ID3D11Texture2D*>(pRes);
}

bool Texture2D::CreateShaderResourceView(ShaderResourceView & view,
	const ShaderResourceViewParams & params) const
{
	auto hr = DX11API::D3D11Device()->CreateShaderResourceView(m_pTexture, &params, view.GetView(0));

	VALID(hr);
}
bool Texture2D::CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
	const ShaderResourceViewParams & params) const
{
	auto hr = DX11API::D3D11Device()->CreateShaderResourceView(m_pTexture, &params, ppView);

	VALID(hr);
}

bool Texture2D::CreateShaderResourceView(ID3D11ShaderResourceView** ppView) const
{
	auto hr = DX11API::D3D11Device()->CreateShaderResourceView(m_pTexture, nullptr, ppView);

	VALID(hr);
}


bool Texture2D::CreateUnorderedAccessView(UnorderedAccessView & view,
	const UnorderedAccessViewParams & params) const
{
	auto hr = DX11API::D3D11Device()->CreateUnorderedAccessView(m_pTexture, &params, view.GetView(0));

	VALID(hr);
}
bool Texture2D::CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
	const UnorderedAccessViewParams & params) const
{
	auto hr = DX11API::D3D11Device()->CreateUnorderedAccessView(m_pTexture, &params, ppView);

	VALID(hr);
}


bool Texture2D::CreateRenderTargetView(RenderTargetView & view,
	const RenderTargetViewParams & params) const
{
	auto hr = DX11API::D3D11Device()->CreateRenderTargetView(m_pTexture, &params, view.GetView(0));

	VALID(hr);
}

bool Texture2D::CreateRenderTargetView(ID3D11RenderTargetView** ppView,
	const RenderTargetViewParams & params) const
{
	auto hr = DX11API::D3D11Device()->CreateRenderTargetView(m_pTexture, &params, ppView);

	VALID(hr);
}

bool Texture2D::CreateDepthStencilView(DepthStencilView & view,
	const DepthStencilViewParams & params) const
{
	//HRESULT hr = DX11API::D3D11Device()->CreateDepthStencilView(m_pTexture, &params,
	//	const_cast<ID3D11DepthStencilView**>(view.GetView()));

	//VALID(hr);
	return true;
}

bool Texture2D::CreateDepthStencilView(ID3D11DepthStencilView* pView,
	const DepthStencilViewParams & params) const
{
	auto hr = DX11API::D3D11Device()->CreateDepthStencilView(m_pTexture, &params, &pView);

	VALID(hr);
}

bool Texture2D::CreateDepthStencilView(ID3D11DepthStencilView** pView,
	const DepthStencilViewParams & params) const
{
	auto hr = DX11API::D3D11Device()->CreateDepthStencilView(m_pTexture, &params, pView);

	VALID(hr);
}


bool Texture2D::Create(const D3D11_TEXTURE2D_DESC & params)
{
	HRESULT hr = DX11API::D3D11Device()->CreateTexture2D(&params, nullptr, &m_pTexture);

	VALID(hr);
}