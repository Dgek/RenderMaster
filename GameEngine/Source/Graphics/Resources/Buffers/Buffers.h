#pragma once

class Buffer
{
protected:
	ID3D11Buffer* m_pBuffer;

public:

	__forceinline Buffer();
	__forceinline ~Buffer();

	__forceinline ID3D11Resource* GetResourcePointer() const;

	__forceinline void UpdateSubresource(unsigned int destSubresource, const GraphicsBox * pBox,
		const void * pSrcData, unsigned int srcRowPitch, unsigned int srcDepthPitch);

	__forceinline bool Create(const D3D11_BUFFER_DESC & desc, const SubresourceData *, unsigned int numElements);
};

__forceinline Buffer::Buffer()
	: m_pBuffer{ nullptr }
{}

__forceinline Buffer::~Buffer()
{
	SAFE_RELEASE(m_pBuffer);
}

__forceinline ID3D11Resource* Buffer::GetResourcePointer() const
{
	return static_pointer<ID3D11Resource*>(m_pBuffer);
}

__forceinline void Buffer::UpdateSubresource(unsigned int destSubresource, const GraphicsBox * pBox,
	const void * pSrcData, unsigned int srcRowPitch, unsigned int srcDepthPitch)
{
	DX11API::D3D11DeviceContext()->UpdateSubresource(m_pBuffer, destSubresource, pBox, pSrcData, srcRowPitch, srcDepthPitch);
}

__forceinline bool Buffer::Create(const D3D11_BUFFER_DESC & desc, const SubresourceData * pData, unsigned int numElements)
{
	HRESULT hr = DX11API::D3D11Device()->CreateBuffer(desc, pData, &m_pBuffer);

	VALID(hr);
}