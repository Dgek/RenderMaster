#pragma once

#include "Buffers.h"

class IndexBuffer : public Buffer
{
protected:
	DXGI_FORMAT m_format;
	unsigned int m_indexSize;
	unsigned int m_indexCount;

public:

	__forceinline void Bind(unsigned int offset) const;

	__forceinline bool Create(const D3D11_BUFFER_DESC & desc, const SubresourceData * pData,
		unsigned int numElements, unsigned int elementSize);

	__forceinline unsigned int Count() const;
};

__forceinline IndexBuffer::Bind(unsigned int offset) const
{
	DX11API::D3D11DeviceContext()->IASetIndexBuffer(m_pBuffer, m_format, offset);
}

__forceinline IndexBuffer::Create(const D3D11_BUFFER_DESC & desc, const SubresourceData * pData,
	unsigned int numElements, unsigned int elementSize)
{
	m_indexCount = numElements;
	m_indexSize = elementSize;

	if (elementSize == 4) //4 bytes
	{
		m_format = DXGI_FORMAT_R32_UINT;
	}
	else if (elementSize == 2)
	{
		m_format = DXGI_FORMAT_R16_UINT;
	}
	else
	{
		assert(0 && "Index size should be size of 2 or 4 bytes!");
	}

	return Buffer::Create(desc, pData, numElements);
}

__forceinline unsigned int IndexBuffer::Count() const
{
	return m_indexCount;
}