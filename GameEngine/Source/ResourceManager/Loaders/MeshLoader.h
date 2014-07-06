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

	shared_ptr<IndexedMesh> m_pMesh;
	//vector<shared_ptr<IndexedMesh>> m_meshes;
	//shared_ptr<VertexBuffer> m_pVertices;
	//shared_ptr<VertexBuffer> m_pTexCoords;
	//shared_ptr<VertexBuffer> m_pNormals;
	//shared_ptr<VertexBuffer> m_pTangents;
	//shared_ptr<IndexBuffer> m_pIndexBuffer;

	MeshResourceExtraData();

	unsigned int GetNumMeshes() const;
	const Vec & GetExtents() const;

	virtual string VToString() override;
};

__forceinline MeshResourceExtraData::MeshResourceExtraData()
	: m_uNumMeshes{ 0 }
{
	//m_pVertices = make_shared<VertexBuffer>();
	//m_pTexCoords = make_shared<VertexBuffer>();
	///m_pNormals = make_shared<VertexBuffer>();
	//m_pTangents = make_shared<VertexBuffer>();
	//m_pIndexBuffer = make_shared<IndexBuffer>();
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
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
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