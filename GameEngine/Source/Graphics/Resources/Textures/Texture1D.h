#pragma once

#include "Texture.h"

class Texture1D : public ITexture
{
public:

	bool Create(const D3D11_TEXTURE1D_DESC & params);


	virtual bool CreateShaderResourceView(ShaderResourceView & view,
		const ShaderResourceViewParams & params) const override;

	virtual bool CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
		const ShaderResourceViewParams & params) const override;


	virtual bool CreateUnorderedAccessView(UnorderedAccessView & view,
		const UnorderedAccessViewParams & params) const override;

	virtual bool CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
		const UnorderedAccessViewParams & params) const override;


	virtual bool CreateRenderTargetView(RenderTargetView & view,
		const RenderTargetViewParams & params) const override;

	virtual bool CreateRenderTargetView(ID3D11RenderTargetView** ppView,
		const RenderTargetViewParams & params) const override;


	virtual bool CreateDepthStencilView(DepthStencilView & view,
		const DepthStencilViewParams & params) const override;

	virtual bool CreateDepthStencilView(ID3D11DepthStencilView* ppView,
		const DepthStencilViewParams & params) const override;

protected:
	ID3D11Texture1D* m_pTexture;
};