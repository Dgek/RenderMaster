#pragma once

class SamplerState
{
protected:
	ID3D11SamplerState*	m_pState;

public:
	__forceinline SamplerState();
	__forceinline ~SamplerState();

	__forceinline bool Create(const D3D11_SAMPLER_DESC * pParams);
	__forceinline void Set(unsigned int slot, ShaderType type) const;
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

__forceinline void SamplerState::Set(unsigned int slot, ShaderType type) const
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