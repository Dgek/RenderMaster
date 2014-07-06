#pragma once

#include "Mesh.h"
#include "SubMesh.h"

class IndexedMesh : public Mesh
{
	friend class MeshLoader;

public:
	shared_ptr<IndexBuffer> m_pIndexBuffer;
	vector<shared_ptr<SubMesh>> m_subMeshes;

public:

	void SetIndexedBuffer();

	void AddSubMesh(shared_ptr<SubMesh> pSubMesh);

	void BindIndexBuffer(unsigned int offset);
};


__forceinline void IndexedMesh::AddSubMesh(shared_ptr<SubMesh> pSubMesh)
{
	m_subMeshes.push_back(pSubMesh);
}

__forceinline void IndexedMesh::BindIndexBuffer(unsigned int offset)
{
	m_pIndexBuffer->Bind(offset);
}