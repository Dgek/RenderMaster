#pragma once

#include "Buffers.h"

class StructuredBuffer : public Buffer
{
public:

	__forceinline bool Create(const D3D11_BUFFER_DESC & desc, const SubresourceData * pData);

	__forceinline bool CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
		const ShaderResourceViewParams & params) const;

	__forceinline bool CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
		const UnorderedAccessViewParams & params) const;
};

__forceinline bool StructuredBuffer::Create(const D3D11_BUFFER_DESC & desc, const SubresourceData * pData)
{
	return Buffer::Create(desc, pData);
}

__forceinline bool StructuredBuffer::CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
	const ShaderResourceViewParams & params) const
{
	HRESULT hr = DX11API::D3D11Device()->CreateShaderResourceView(m_pBuffer, params, ppView);

	VALID(hr);
}

__forceinline bool StructuredBuffer::CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
	const UnorderedAccessViewParams & params) const
{
	HRESULT hr = DX11API::D3D11Device()->CreateUnorderedAccessView(m_pBuffer, params, ppView);

	VALID(hr);
}