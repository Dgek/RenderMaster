#pragma once

class ITexture
{
public:

	/****
	==========================================
	Shader Resource View Creation
	==========================================
	****/

	virtual bool CreateShaderResourceView(ShaderResourceView & view,
		const ShaderResourceViewParams & params) const = 0;

	virtual bool CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
		const ShaderResourceViewParams & params) const = 0;

	/****
	==========================================
	Unordered Access View Creation
	==========================================
	****/

	virtual bool CreateUnorderedAccessView(UnorderedAccessView & view,
		const UnorderedAccessViewParams & params) const = 0;

	virtual bool CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
		const UnorderedAccessViewParams & params) const = 0;

	/****
	==========================================
	Render Target View Creation
	==========================================
	****/

	virtual bool CreateRenderTargetView(RenderTargetView & view,
		const RenderTargetViewParams & params) const = 0;

	virtual bool CreateRenderTargetView(ID3D11RenderTargetView** ppView,
		const RenderTargetViewParams & params) const = 0;

	/****
	==========================================
	Depth Stencil View Creation
	==========================================
	****/

	virtual bool CreateDepthStencilView(DepthStencilView & view,
		const DepthStencilViewParams & params) const = 0;

	virtual bool CreateDepthStencilView(ID3D11DepthStencilView** ppView,
		const DepthStencilViewParams & params) const = 0;

};