#pragma once

#include "Mesh.h"

class IndexedMesh : public Mesh
{
protected:
	shared_ptr<IndexBuffer> m_pIndexBuffer;

public:

	__forceinline IndexedMesh();
	virtual ~IndexedMesh();

	__forceinline void SetIndexedBuffer();
};