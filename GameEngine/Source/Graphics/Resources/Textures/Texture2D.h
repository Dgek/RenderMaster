#pragma once

#include "Texture.h"

class Texture2DDX11 : public Texture
{
public:
	//Constructor
	__forceinline Texture2D();

	virtual bool Create(const D3D11_TEXTURE2D_DESC & pParams);


	virtual bool CreateShaderResourceView(ShaderResourceViewDX11 & view,
		const ShaderResourceViewParamsDX11 & params)	const;

	virtual bool CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
		const ShaderResourceViewParamsDX11 & params)	const;


	virtual bool CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
		const UnorderedAccessViewParamsDX11 & params)	const;

	virtual bool CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
		const UnorderedAccessViewParamsDX11 & params)	const;


	virtual bool CreateRenderTargetView(RenderTargetViewDX11 & view,
		const RenderTargetViewParamsDX11 & params)		const;

	virtual bool CreateRenderTargetView(ID3D11RenderTargetView** ppView,
		const RenderTargetViewParamsDX11 & params)		const;


	virtual bool CreateDepthStencilView(DepthStencilViewDX11 & view,
		const DepthStencilViewParamsDX11 & params)		const;

	virtual bool CreateDepthStencilView(ID3D11DepthStencilView** ppView,
		const DepthStencilViewParamsDX11 & params)		const;


protected:
	ID3D11Texture2D* m_pTexture;
};