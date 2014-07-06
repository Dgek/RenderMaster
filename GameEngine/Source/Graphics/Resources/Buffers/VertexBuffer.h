#pragma once

#include "Buffers.h"

class VertexBuffer : public Buffer
{
protected:
	unsigned int m_vertexSize;
	unsigned int m_vertexCount;

public:
	bool Create(const D3D11_BUFFER_DESC & desc, const SubresourceData * pData, unsigned int numElements, unsigned int elementSize);
	void Bind(unsigned int startSlot, unsigned int offset) const;
	void Bind(unsigned int startSlot, unsigned int offset, unsigned int vertexSize) const;
	unsigned int Count() const;
};

__forceinline bool VertexBuffer::Create(const D3D11_BUFFER_DESC & desc, const SubresourceData * pData, unsigned int numElements, unsigned int elementSize)
{
	m_vertexCount = numElements;
	m_vertexSize = elementSize;

	return Buffer::Create(desc, pData, numElements);
}

__forceinline void VertexBuffer::Bind(unsigned int startSlot, unsigned int offset) const
{
	DX11API::D3D11DeviceContext()->IASetVertexBuffers(startSlot, 1, &m_pBuffer, &m_vertexSize, &offset);
}

__forceinline void VertexBuffer::Bind(unsigned int startSlot, unsigned int offset, unsigned int vertexSize) const
{
	DX11API::D3D11DeviceContext()->IASetVertexBuffers(startSlot, 1, &m_pBuffer, &vertexSize, &offset);
}

__forceinline unsigned int VertexBuffer::Count() const 
{
	return m_vertexCount;
}