#pragma once

#include "DepthStencilView.h"

struct RenderTargetViewParams : public D3D11_RENDER_TARGET_VIEW_DESC
{
public:
	RenderTargetViewParams();

	void InitForTexture2D(DXGI_FORMAT format, int mipslice, bool multiSampled);

	void InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int mipslice, bool multiSampled);
};

__forceinline RenderTargetViewParams::RenderTargetViewParams()
{
	ZeroMemory(this, sizeof(RenderTargetViewParams));
}

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
	explicit RenderTargetView(int numViews = 1);

	explicit RenderTargetView(ID3D11RenderTargetView * View);

	~RenderTargetView();


	/*** Getters ***/
	ID3D11RenderTargetView** GetView(int index = 0) const;

	/* ==
	Bind Render Target Views to the pipeline
	== */
	//void Set(DepthStencilViewDX11 * pView) const;
	//__forceinline void BindWithUAV(unsigned int uavStartSlot, unsigned int uavNum)
	void Bind(DepthStencilView * pDepthView) const;
	void Bind(const DepthStencilView & depthview) const;
	void Bind() const;
	void BindOneView(int index);
	void BindWithUAV(unsigned int uavStartSlot, unsigned int uavNum,
		ID3D11UnorderedAccessView * const *, const unsigned int *);

	void Clear(float bgColor[4]);
	void Clear();
};

__forceinline RenderTargetView::RenderTargetView(int numViews)
	: m_ppViews{ nullptr }, m_numViews{ numViews }
{
	m_ppViews = new ID3D11RenderTargetView*[numViews];
}

__forceinline RenderTargetView::RenderTargetView(ID3D11RenderTargetView * pView)
	: m_numViews{ 1 }
{
	m_ppViews = new ID3D11RenderTargetView*[m_numViews];
	m_ppViews[0] = pView;
}

__forceinline ID3D11RenderTargetView** RenderTargetView::GetView(int index) const
{
	return &m_ppViews[index];
}

__forceinline RenderTargetView::~RenderTargetView()
{
	auto i = m_numViews - 1;
	for (; i > 0; i--)
		SAFE_RELEASE(m_ppViews[i]);

	SAFE_DELETE_ARRAY(m_ppViews);
}
