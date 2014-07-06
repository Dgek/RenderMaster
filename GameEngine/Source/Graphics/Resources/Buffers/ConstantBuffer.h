#pragma once

#include "Buffers.h"

class ConstantBuffer : public Buffer
{
public:

	virtual ~ConstantBuffer();

	void Bind(unsigned int slot, ShaderType type);
};

__forceinline void ConstantBuffer::Bind(unsigned int slot, ShaderType type)
{
	switch (type)
	{
	case ST_Vertex:
		DX11API::D3D11DeviceContext()->VSSetConstantBuffers(slot, 1, &m_pBuffer);
		break;
	case ST_Geometry:
		DX11API::D3D11DeviceContext()->GSSetConstantBuffers(slot, 1, &m_pBuffer);
		break;
	case ST_Hull:
		DX11API::D3D11DeviceContext()->HSSetConstantBuffers(slot, 1, &m_pBuffer);
		break;
	case ST_Domain:
		DX11API::D3D11DeviceContext()->DSSetConstantBuffers(slot, 1, &m_pBuffer);
		break;
	case ST_Pixel:
		DX11API::D3D11DeviceContext()->PSSetConstantBuffers(slot, 1, &m_pBuffer);
		break;
	case ST_Compute:
		DX11API::D3D11DeviceContext()->CSSetConstantBuffers(slot, 1, &m_pBuffer);
		break;
	default:
		assert(0);
		break;
	};
}