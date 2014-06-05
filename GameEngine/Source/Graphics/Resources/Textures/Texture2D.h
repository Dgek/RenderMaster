#pragma once

#include "Texture.h"

class ShaderResourceViewParams;
class UnorderedAccessViewParams;
class DepthStencilViewParams;
class RenderTargetViewParams;
class ShaderResourceView;
class UnorderedAccessView;
class DepthStencilView;
class RenderTargetView;

class Texture2D : public ITexture
{
public:
	//Constructor
	__forceinline Texture2D();

	virtual bool Create(const D3D11_TEXTURE2D_DESC & pParams);


	virtual bool CreateShaderResourceView(ShaderResourceView & view,
		const ShaderResourceViewParams & params)	const;

	virtual bool CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
		const ShaderResourceViewParams & params)	const;


	virtual bool CreateUnorderedAccessView(UnorderedAccessView & view,
		const UnorderedAccessViewParams & params)	const;

	virtual bool CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
		const UnorderedAccessViewParams & params)	const;


	virtual bool CreateRenderTargetView(RenderTargetView & view,
		const RenderTargetViewParams & params)		const;

	virtual bool CreateRenderTargetView(ID3D11RenderTargetView** ppView,
		const RenderTargetViewParams & params)		const;


	virtual bool CreateDepthStencilView(DepthStencilView & view,
		const DepthStencilViewParams & params)		const;

	virtual bool CreateDepthStencilView(ID3D11DepthStencilView** ppView,
		const DepthStencilViewParams & params)		const;


protected:
	ID3D11Texture2D* m_pTexture;
};