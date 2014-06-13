#pragma once

#include "../../General.h"

#include "DepthStencilView.h"

struct RenderTargetViewParams : public D3D11_RENDER_TARGET_VIEW_DESC
{
public:
	__forceinline RenderTargetViewParams() =default;

	__forceinline void InitForTexture2D(DXGI_FORMAT format, int mipslice, bool multiSampled);

	__forceinline void InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int mipslice, bool multiSampled);
};

__forceinline void RenderTargetViewParams::InitForTexture2D(DXGI_FORMAT format, int mipslice, bool multiSampled)
{
	Format = format;
	Texture2D.MipSlice = mipslice;
	
	ViewDimension = multiSampled ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
}

__forceinline void RenderTargetViewParams::InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int mipslice, bool multiSampled)
{
	Format = format;

	if (multiSampled)
	{
		ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
		Texture2DMSArray.ArraySize = arraySize;
		Texture2DMSArray.FirstArraySlice = firstArraySlice;
	}
	else
	{
		ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		Texture2DArray.ArraySize = arraySize;
		Texture2DArray.MipSlice = mipslice;
		Texture2DArray.FirstArraySlice = firstArraySlice;
	}
}

class RenderTargetView
{
private:
	ID3D11RenderTargetView** m_ppViews;
	int m_numViews;

public:
	__forceinline RenderTargetView();

	__forceinline explicit RenderTargetView(int numViews);

	__forceinline explicit RenderTargetView(ID3D11RenderTargetView * View);

	__forceinline ~RenderTargetView();


	/*** Getters ***/
	__forceinline ID3D11RenderTargetView** GetView(int index = 0) const;

	/* ==
	Bind Render Target Views to the pipeline
	== */
	//void Set(DepthStencilViewDX11 * pView) const;
	//__forceinline void BindWithUAV(unsigned int uavStartSlot, unsigned int uavNum)
	__forceinline void Bind(const DepthStencilView & depthview) const;
	__forceinline void Bind() const;
	__forceinline void BindOneView(int index);
	__forceinline void BindWithUAV(unsigned int uavStartSlot, unsigned int uavNum,
		ID3D11UnorderedAccessView * const *, const unsigned int *);

	__forceinline void Clear(float bgColor[4]);
	__forceinline void Clear();
};

__forceinline RenderTargetView::RenderTargetView()
	: m_ppViews{ nullptr }, m_numViews{ 1 }
{}

__forceinline RenderTargetView::RenderTargetView(int numViews)
	: m_ppViews{ nullptr }, m_numViews{ numViews }
{}

__forceinline RenderTargetView::RenderTargetView(ID3D11RenderTargetView * pView)
{
	m_ppViews = &pView;
}

__forceinline RenderTargetView::~RenderTargetView()
{
	auto i = m_numViews - 1;
	for (; i > 0; i--)
		SAFE_RELEASE(m_ppViews[i]);

	SAFE_DELETE_ARRAY(m_ppViews);
}

__forceinline ID3D11RenderTargetView** RenderTargetView::GetView(int index) const
{
	return &m_ppViews[index];
}

//void Set(DepthStencilViewDX11 * pView) const;
__forceinline void RenderTargetView::BindOneView(int index)
{
	DX11API::D3D11DeviceContext()->OMSetRenderTargets(1, m_ppViews + index, nullptr);
}

__forceinline void RenderTargetView::Bind(const DepthStencilView & depthview) const
{
	DX11API::D3D11DeviceContext()->OMSetRenderTargets(1, m_ppViews, *depthview.GetView());
}
__forceinline void RenderTargetView::Bind() const
{
	DX11API::D3D11DeviceContext()->OMSetRenderTargets(1, m_ppViews, nullptr);
}

__forceinline void RenderTargetView::BindWithUAV(unsigned int uavStartSlot, unsigned int uavNum,
	ID3D11UnorderedAccessView * const * ppUnorderedAccessViews, const unsigned int * pUAVInitialCounts)
{
	DX11API::D3D11DeviceContext()->OMSetRenderTargetsAndUnorderedAccessViews(1, m_ppViews, nullptr, uavStartSlot, uavNum, ppUnorderedAccessViews, pUAVInitialCounts);

}

__forceinline void RenderTargetView::Clear(float bgColor[4])
{
	for (auto i = 0; i < m_numViews; i++)
		DX11API::D3D11DeviceContext()->ClearRenderTargetView(m_ppViews[i], bgColor);
}

__forceinline void RenderTargetView::Clear()
{
	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (auto i = 0; i < m_numViews; i++)
		DX11API::D3D11DeviceContext()->ClearRenderTargetView(m_ppViews[i], color);
}