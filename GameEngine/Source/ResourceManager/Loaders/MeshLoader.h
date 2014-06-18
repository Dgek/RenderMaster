#pragma once

#include "../Resource.h"
#include "../ResourceLoader.h"

#include "../../Math/Vector.h"

#include "../../Graphics/Resources/Buffers/VertexBuffer.h"
#include "../../Graphics/Resources/Buffers/IndexBuffer.h"

class IndexedMesh;
class MeshResourceExtraData : public IResourceExtraData
{
public:
	unsigned int m_uNumMeshes;
	Vec m_vExtents;

	vector<shared_ptr<IndexedMesh>> m_pMeshes;
	shared_ptr<VertexBuffer> m_pVertices;
	shared_ptr<VertexBuffer> m_pTexCoords;
	shared_ptr<VertexBuffer> m_pNormals;
	shared_ptr<VertexBuffer> m_pTangents;
	shared_ptr<IndexBuffer> m_pIndexBuffer;

	__forceinline MeshResourceExtraData();

	__forceinline unsigned int GetNumMeshes() const;
	__forceinline const Vec & GetExtents() const;

	virtual string VToString() override;
};

__forceinline MeshResourceExtraData::MeshResourceExtraData()
	: m_uNumMeshes{ 0 }
{
	m_pVertices = make_shared<VertexBuffer>();
	m_pTexCoords = make_shared<VertexBuffer>();
	m_pNormals = make_shared<VertexBuffer>();
	m_pTangents = make_shared<VertexBuffer>();
	m_pIndexBuffer = make_shared<IndexBuffer>();
}

__forceinline unsigned int MeshResourceExtraData::GetNumMeshes() const
{
	return m_uNumMeshes;
}

__forceinline const Vec & MeshResourceExtraData::GetExtents() const
{
	return m_vExtents;
}

class MeshLoader : public IResourceLoader
{
public:
	__forceinline virtual string VGetPattern() override;
	__forceinline virtual bool VUseRawFile() override;
	__forceinline  virtual bool VNeedFile() override;
	__forceinline virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;
};

__forceinline string MeshLoader::VGetPattern()
{
	return "*.mesh";
}

__forceinline bool MeshLoader::VUseRawFile()
{
	return false;
}

__forceinline bool MeshLoader::VNeedFile()
{
	return true;
}

__forceinline int MeshLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}