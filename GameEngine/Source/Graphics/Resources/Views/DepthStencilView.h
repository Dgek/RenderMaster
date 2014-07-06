#pragma once

struct DepthStencilViewParams : public D3D11_DEPTH_STENCIL_VIEW_DESC
{
public:
	DepthStencilViewParams();

	void InitForTexture2D(DXGI_FORMAT format, int mipslice, bool multiSampled);

	void InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int mipslice, bool multiSampled);
};

__forceinline DepthStencilViewParams::DepthStencilViewParams()
{
	ZeroMemory(this, sizeof(DepthStencilViewParams));
}

__forceinline void DepthStencilViewParams::InitForTexture2D(DXGI_FORMAT format, int mipslice, bool multiSampled)
{
	Format = format;
	Texture2D.MipSlice = mipslice;

	ViewDimension = multiSampled ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
}

__forceinline void DepthStencilViewParams::InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int mipslice, bool multiSampled)
{
	Format = format;

	if (multiSampled)
	{
		ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
		Texture2DMSArray.ArraySize = arraySize;
		Texture2DMSArray.FirstArraySlice = firstArraySlice;
	}
	else
	{
		ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		Texture2DArray.ArraySize = arraySize;
		Texture2DArray.MipSlice = mipslice;
		Texture2DArray.FirstArraySlice = firstArraySlice;
	}
}


class DepthStencilView
{
	friend class Renderer;

private:
	ID3D11DepthStencilView* m_pView;

public:

	DepthStencilView();

	~DepthStencilView();


	/*** Getters ***/
	ID3D11DepthStencilView * GetView() const;
};

__forceinline DepthStencilView::DepthStencilView()
	: m_pView{ nullptr }
{}

__forceinline DepthStencilView::~DepthStencilView()
{
	SAFE_RELEASE(m_pView);
}

__forceinline ID3D11DepthStencilView *DepthStencilView::GetView() const
{
	return m_pView;
}