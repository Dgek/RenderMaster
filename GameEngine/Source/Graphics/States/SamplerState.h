#pragma once

class SamplerStateParams : public D3D11_SAMPLER_DESC
{
public:
	__forceinline SamplerStateParams();
	__forceinline bool Init(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU,
		D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW,
		float MipLODBias, unsigned int MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc,
		float* BorderColor = nullptr, float MinLOD = 0, float MaxLOD = 0x7FFFFFFF);
};

__forceinline SamplerStateParams::SamplerStateParams() 
{
	ZeroMemory(this, sizeof(SamplerStateParams)); 
}

__forceinline bool SamplerStateParams::Init(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU,
	D3D11_TEXTURE_ADDRESS_MODE addressV, D3D11_TEXTURE_ADDRESS_MODE addressW,
	float mipLODBias, unsigned int maxAnisotropy, D3D11_COMPARISON_FUNC comparisonFunc,
	float* borderColor, float minLOD, float maxLOD)
{
	Filter = filter;
	AddressU = addressU;
	AddressV = addressV;
	AddressW = addressW;
	MipLODBias = mipLODBias;
	MaxAnisotropy = maxAnisotropy;
	ComparisonFunc = comparisonFunc;

	if (borderColor)
	{
		BorderColor[0] = borderColor[0];
		BorderColor[1] = borderColor[1];
		BorderColor[2] = borderColor[2];
		BorderColor[3] = borderColor[3];
	}

	MinLOD = minLOD;
	MaxLOD = maxLOD;

	return true;
}

class SamplerState
{
protected:
	ID3D11SamplerState*	m_pState;

public:
	__forceinline SamplerState();
	__forceinline ~SamplerState();

	__forceinline bool Create(const D3D11_SAMPLER_DESC * pParams);
	__forceinline void Bind(unsigned int slot, ShaderType type) const;
};

__forceinline SamplerState::SamplerState() 
{ 
	m_pState = nullptr; 
}

__forceinline SamplerState::~SamplerState()
{ 
	SAFE_RELEASE(m_pState); 
}

__forceinline bool SamplerState::Create(const D3D11_SAMPLER_DESC * pParams)
{
	HRESULT hr = DX11API::D3D11Device()->CreateSamplerState(pParams, &m_pState);

	VALID(hr);
}

__forceinline void SamplerState::Bind(unsigned int slot, ShaderType type) const
{
	switch (type)
	{
	case ST_Vertex:
		DX11API::D3D11DeviceContext()->VSSetSamplers(slot, 1, &m_pState);
		break;
	case ST_Hull:
		DX11API::D3D11DeviceContext()->HSSetSamplers(slot, 1, &m_pState);
		break;
	case ST_Domain:
		DX11API::D3D11DeviceContext()->DSSetSamplers(slot, 1, &m_pState);
		break;
	case ST_Geometry:
		DX11API::D3D11DeviceContext()->GSSetSamplers(slot, 1, &m_pState);
		break;
	case ST_Pixel:
		DX11API::D3D11DeviceContext()->PSSetSamplers(slot, 1, &m_pState);
		break;
	case ST_Compute:
		DX11API::D3D11DeviceContext()->CSSetSamplers(slot, 1, &m_pState);
		break;
	};
}