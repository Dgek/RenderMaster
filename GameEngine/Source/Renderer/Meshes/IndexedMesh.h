#pragma once

#include "Mesh.h"

class IndexedMesh : public Mesh
{
protected:
	shared_ptr<IndexBuffer> m_pIndexBuffer;

public:

	int m_firstIndex;
	int m_numIndices;

public:

	__forceinline IndexedMesh();

	__forceinline void SetIndexedBuffer();
};

__forceinline IndexedMesh::IndexedMesh()
	: m_firstIndex{ 0 }, m_numIndices{ 0 }
{}