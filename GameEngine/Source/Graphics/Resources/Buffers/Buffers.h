#pragma once

#include "../../General.h"
#include "../GraphicsResource.h"

class Buffer
{
protected:
	ID3D11Buffer* m_pBuffer;

public:

	Buffer();
	virtual ~Buffer();

	ID3D11Resource* GetResourcePointer() const;

	void UpdateSubresource(unsigned int destSubresource, const GraphicsBox * pBox,
		const void * pSrcData, unsigned int srcRowPitch, unsigned int srcDepthPitch);

	bool Create(const D3D11_BUFFER_DESC & desc, const SubresourceData *, unsigned int numElements = 0);
};

__forceinline Buffer::Buffer()
	: m_pBuffer{ nullptr }
{}

__forceinline ID3D11Resource* Buffer::GetResourcePointer() const
{
	return static_cast<ID3D11Resource*>(m_pBuffer);
}

__forceinline void Buffer::UpdateSubresource(unsigned int destSubresource, const GraphicsBox * pBox,
	const void * pSrcData, unsigned int srcRowPitch, unsigned int srcDepthPitch)
{
	DX11API::D3D11DeviceContext()->UpdateSubresource(m_pBuffer, destSubresource, pBox, pSrcData, srcRowPitch, srcDepthPitch);
}

__forceinline bool Buffer::Create(const D3D11_BUFFER_DESC & desc, const SubresourceData * pData, unsigned int numElements)
{
	HRESULT hr = DX11API::D3D11Device()->CreateBuffer(&desc, pData, &m_pBuffer);

	VALID(hr);
}