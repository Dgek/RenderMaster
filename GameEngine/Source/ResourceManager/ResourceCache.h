#pragma once

#include "ResourceFile.h"
#include "ResourceLoader.h"

class Resource;
class IResourceLoader;
class ResourceCache
{
	friend class ResHandle;

protected:

	static unique_ptr<ResourceCache> g_pCache;

	unique_ptr<IResourceFile> m_pFile;

	typedef std::list< std::shared_ptr<ResHandle>> ResHandleList;
	typedef std::map<string, std::shared_ptr<ResHandle>> ResHandleMap;
	typedef std::list< std::shared_ptr<IResourceLoader>> ResourceLoaders;

	ResHandleList m_lru; //leat-recetly used
	ResHandleMap m_resources; //fast resource look-up
	ResourceLoaders m_resourceLoaders;

	//////////////////////////////////////////////
	//Chache parameteres
	unsigned int m_uCacheSize; //size of resource cahce
	unsigned int m_uAllocated; //size of allocated resources

	shared_ptr<ResHandle> Find(Resource* pResource);
	void Update(shared_ptr<ResHandle> pHandle);
	shared_ptr<ResHandle> Load(Resource *pResource);
	void Free(shared_ptr<ResHandle>  pFreeMe);

	bool MakeRoom(unsigned int uSize);
	char* Allocate(unsigned int uSize);
	void FreeOneResource();
	void MemoryHasBeenFreed(unsigned int uSize);

public:

	__forceinline static ResourceCache * const Get();

	//Global resource cache methods
	static shared_ptr<ResHandle> SafeGetHandle(Resource *pResource);

	__forceinline ResourceCache(unsigned int sizeInMb, unique_ptr<IResourceFile> pFile);
	__forceinline ~ResourceCache();

	__forceinline bool Init();
	void RegisterLoader(shared_ptr<IResourceLoader> pLoader);

	shared_ptr<ResHandle> GetHandle(Resource *pResource);
	int Preload(const string & pattern, void(*progressCallback)(int, bool &));
	void Flush();
};

__forceinline ResourceCache::ResourceCache(unsigned int sizeInMb, unique_ptr<IResourceFile> pFile)
{
	m_uCacheSize = sizeInMb * 1024 * 1024;
	m_uAllocated = 0;

	m_pFile = move(pFile);
}

__forceinline ResourceCache::~ResourceCache()
{}

__forceinline bool ResourceCache::Init()
{
	bool bResult = false;
	if (m_pFile->VOpen())
	{
		RegisterLoader(make_shared<DefaultResourceLoader>());
		bResult = true;
	}

	return bResult;
}

__forceinline ResourceCache * const ResourceCache::Get()
{
	return ResourceCache::g_pCache.get();
}