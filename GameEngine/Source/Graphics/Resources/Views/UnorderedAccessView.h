#pragma once

#include "../../General.h"

struct UnorderedAccessViewParams : public D3D11_UNORDERED_ACCESS_VIEW_DESC
{
public:
	__forceinline UnorderedAccessViewParams() = default;

	__forceinline void InitForTexture2D(DXGI_FORMAT format, int mipslice);

	__forceinline void InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int mipslice);

	__forceinline void InitForStructuredBuffer(DXGI_FORMAT format, int elementOffset, int numElements, D3D11_BUFFER_UAV_FLAG flag);
};

__forceinline void UnorderedAccessViewParams::InitForTexture2D(DXGI_FORMAT format, int mipslice)
{
	Format = format;
	Texture2D.MipSlice = mipslice;

	ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
}

__forceinline void UnorderedAccessViewParams::InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int mipslice)
{
	Format = format;

	ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	Texture2DArray.ArraySize = arraySize;
	Texture2DArray.MipSlice = mipslice;
	Texture2DArray.FirstArraySlice = firstArraySlice;
}

__forceinline void UnorderedAccessViewParams::InitForStructuredBuffer(DXGI_FORMAT format, int elementOffset, int numElements, D3D11_BUFFER_UAV_FLAG flag)
{
	Format = format;
	Buffer.FirstElement = elementOffset;
	Buffer.NumElements = numElements;
	Buffer.Flags = flag;
	ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
}


class UnorderedAccessView
{
private:
	ID3D11UnorderedAccessView** m_ppViews;
	int m_numViews;

public:
	__forceinline explicit UnorderedAccessView(int numViews = 1);

	//__forceinline explicit UnorderedAccessView(ID3D11RenderTargetView * View);

	__forceinline ~UnorderedAccessView();


	/*** Getters ***/
	__forceinline ID3D11UnorderedAccessView** GetView(int index = 0) const;

	/* ==
	Bind Unordered Access Views to the pipeline
	== */
	__forceinline void Bind(int slot, ShaderType type) const;
	__forceinline void BindView(int index, int slot, ShaderType type) const;
};


__forceinline UnorderedAccessView::UnorderedAccessView(int numViews)
	: m_ppViews{ nullptr }, m_numViews{ numViews }
{
	m_ppViews = new ID3D11UnorderedAccessView*[numViews];
}

//__forceinline RenderTargetView::RenderTargetView(ID3D11RenderTargetView * View);

__forceinline UnorderedAccessView::~UnorderedAccessView()
{
	auto i = m_numViews - 1;
	for (; i > 0; i--)
		SAFE_RELEASE(m_ppViews[i]);

	SAFE_DELETE_ARRAY(m_ppViews);
}

__forceinline ID3D11UnorderedAccessView** UnorderedAccessView::GetView(int index) const
{
	return m_ppViews + index;
}

__forceinline void UnorderedAccessView::Bind(int slot, ShaderType type) const
{
	switch (type)
	{
	case ST_Compute:
		DX11API::D3D11DeviceContext()->CSSetUnorderedAccessViews(slot, m_numViews, m_ppViews, nullptr);
		break;
	default:
		break;
	}
}
__forceinline void UnorderedAccessView::BindView(int index, int slot, ShaderType type) const
{
	switch (type)
	{
	case ST_Compute:
		DX11API::D3D11DeviceContext()->CSSetUnorderedAccessViews(slot, 1, m_ppViews + index, nullptr);
		break;
	default:
		break;
	}
}