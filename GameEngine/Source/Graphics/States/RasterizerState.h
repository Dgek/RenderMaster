#pragma once

#include "../General.h"

class RasterizerParams : public D3D11_RASTERIZER_DESC
{
public:
	RasterizerParams();
	bool Init(bool wireframe, bool cullFront, bool cullBack, bool frontCounterClockwise,
		int depthBias, float depthBiasClamp, float slopeScaledDepthBias, 
		bool depthClipEnable, bool scissorEnable, bool multiSampleEnable,
		bool antialiasedLineEnable);
};

__forceinline RasterizerParams::RasterizerParams() 
{
	ZeroMemory(this, sizeof(RasterizerParams)); 
}

__forceinline bool RasterizerParams::Init(bool wireframe, bool cullFront, bool cullBack, bool frontCounterClockwise,
	int depthBias, float depthBiasClamp, float slopeScaledDepthBias,
	bool depthClipEnable, bool scissorEnable, bool multiSampleEnable,
	bool antialiasedLineEnable)
{
	if (wireframe)
		FillMode = D3D11_FILL_WIREFRAME;
	else
		FillMode = D3D11_FILL_SOLID;

	if (!cullFront && !cullBack)
		CullMode = D3D11_CULL_NONE;
	else if (cullFront && !cullBack)
		CullMode = D3D11_CULL_FRONT;
	else if (!cullFront && cullBack)
		CullMode = D3D11_CULL_BACK;
	else if (cullFront && cullBack)
		assert(0 && "Rasterizer cannot cull front and back faces concurrently!");

	FrontCounterClockwise = frontCounterClockwise;
	DepthBias = depthBias;
	DepthBiasClamp = depthBiasClamp;
	SlopeScaledDepthBias = slopeScaledDepthBias;
	DepthClipEnable = depthClipEnable;
	ScissorEnable = scissorEnable;
	MultisampleEnable = multiSampleEnable;
	AntialiasedLineEnable = antialiasedLineEnable;

	return true;
}

class RasterizerState
{
protected:
	ID3D11RasterizerState*	m_pState;

public:
	RasterizerState();
	~RasterizerState();

	bool Create(const D3D11_RASTERIZER_DESC * pParams);
	void Bind();
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