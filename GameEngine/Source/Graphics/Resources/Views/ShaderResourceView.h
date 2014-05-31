#pragma once

class ShaderResourceViewParams : public D3D11_SHADER_RESOURCE_VIEW_DESC
{
public:
	__forceinline ShaderResourceViewParams() = default;

	__forceinline void InitForTexture2D(DXGI_FORMAT format, int miplevels, int mostdetailedmip, bool multiSampled);

	__forceinline void InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int miplevels, int mostdetailedmip, bool multiSampled);

	__forceinline void InitForCubeTexture(DXGI_FORMAT format, int miplevels, int mostdetailedmip);
	
	__forceinline void InitForStructuredBuffer(DXGI_FORMAT format, int elementOffset, int elementWidth);
};

__forceinline void ShaderResourceViewParams::InitForTexture2D(DXGI_FORMAT format, int miplevels, int mostdetailedmip, bool multiSampled)
{
	Format = format;
	Texture2D.MipLevels = miplevels;
	Texture2D.MostDetailedMip = mostdetailedmip;

	ViewDimension = multiSampled ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
}
__forceinline void ShaderResourceViewParams::InitForTexture2DArray(int arraySize, DXGI_FORMAT format, int firstArraySlice, int miplevels, int mostdetailedmip, bool multiSampled)
{
	Format = format;

	if (multisampled)
	{
		ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
		Texture2DMSArray.ArraySize = arraySize;
		Texture2DMSArray.FirstArraySlice = firstArraySlice;
	}
	else
	{
		ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		Texture2DArray.ArraySize = arraySize;
		Texture2DArray.MipLevels = 1;
		Texture2DArray.FirstArraySlice = firstArraySlice;
	}
}
__forceinline void ShaderResourceViewParams::InitForCubeTexture(DXGI_FORMAT format, int miplevels, int mostdetailedmip)
{
	Format = format;
	TextureCube.MipLevels = miplevels;
	TextureCube.MostDetailedMip = mostdetailedmip;
	ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
}
__forceinline void ShaderResourceViewParams::InitForStructuredBuffer(DXGI_FORMAT format, int elementOffset, int elementWidth)
{
	Format = format
	Buffer.FirstElement = 0;
	Buffer.NumElements = elementWidth;
	ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
}

class ShaderResourceView
{
private:
	ID3D11ShaderResourceView** m_ppViews;
	int m_numViews;

public:

	__forceinline explicit ShaderResourceView(int numViews);

	__forceinline ~ShaderResourceView();


	/*** Getters ***/
	__forceinline ID3D11ShaderResourceView** GetView(int index) const;

	/* ==
	Bind Unordered Access Views to the pipeline
	== */
	__forceinline void Bind(int slot, ShaderType type) const;
	__forceinline void BindView(int index, int slot, ShaderType type) const;
};

__forceinline ShaderResourceView::ShaderResourceView(int numViews)
: m_ppViews{ nullptr }, m_numViews{ numViews }
{}

__forceinline ShaderResourceView::~ShaderResourceView()
{
	auto i = m_numViews - 1;
	for (; i > 0; i--)
		SAFE_RELEASE(m_ppViews[i]);

	SAFE_DELETE_ARRAY(m_ppViews);
}

__forceinline ID3D11ShaderResourceView** ShaderResourceView::GetView(int index) const
{
	return m_ppViews + index;
}

__forceinline void ShaderResourceView::Bind(int slot, ShaderType type) const
{
	switch (type)
	{
	case ST_Vertex:
		D3D11DeviceContext()->VSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Domain:
		D3D11DeviceContext()->DSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Geometry:
		D3D11DeviceContext()->GSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Pixel:
		D3D11DeviceContext()->PSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Compute:
		D3D11DeviceContext()->CSSetShaderResources(slot, m_numViews, m_ppViews);
	default:
		break;
	};
}
__forceinline void ShaderResourceView::BindView(int index, int slot, ShaderType type) const
{
	switch (type)
	{
	case ST_Vertex:
		D3D11DeviceContext()->VSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Domain:
		D3D11DeviceContext()->DSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Geometry:
		D3D11DeviceContext()->GSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Pixel:
		D3D11DeviceContext()->PSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Compute:
		D3D11DeviceContext()->CSSetShaderResources(slot, m_numViews, m_ppViews);
	default:
		break;
	};
}