#pragma once

#include "../Resource.h"
#include "../ResourceLoader.h"

class IndexedMesh;
class MeshResourceExtraData : public IResourceExtraData
{
public:
	unsigned int m_uNumMeshes;
	Vec m_vExtents;

	vector<shared_ptr<IndexedMesh>> m_pMeshes;

	__forceinline MeshResourceExtraData();

	__forceinline unsigned int GetNumMeshes() const;
	__forceinline const Vec & GetExtents() const;

	virtual string VToString() override;
};

__forceinline MeshResourceExtraData::MeshResourceExtraData()
	: m_uNumMeshes{ 0 }
{}

__forceinline unsigned int MeshResourceExtraData::GetNumMeshes() const
{
	return m_uNumMeshes;
}

__forceinline const Vec & MeshResourceExtraData::GetExtents() const
{
	return m_vExtents;
}

class MeshLoader : IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, std::shared_ptr<ResHandle> pHandle) override;
};