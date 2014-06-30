#include "Core.h"

#include "ResourceCache.h"

#include "Resource.h"
#include "../Help/Strings/Strings.h"

shared_ptr<ResourceCache> Global::g_pCache = make_shared<ResourceCache>();

ResourceCache::ResourceCache(unsigned int sizeInMb, unique_ptr<IResourceFile> pFile)
{
	m_uCacheSize = sizeInMb * 1024 * 1024;
	m_uAllocated = 0;

	m_pFile = move(pFile);
	Global::g_pCache = shared_ptr<ResourceCache>(this);
}

ResourceCache * const ResourceCache::Get()
{
	return Global::g_pCache.get();
}

std::shared_ptr<ResHandle> ResourceCache::SafeGetHandle(Resource *pResource)
{
	assert(ResourceCache::Get());
		
	return ResourceCache::Get()->GetHandle(pResource);
}

shared_ptr<ResHandle> ResourceCache::GetHandle(Resource *pResource)
{
	shared_ptr<ResHandle> pHandle{ Find(pResource) };
	if (!pHandle)
	{
		pHandle = Load(pResource);
	}
	else
	{
		Update(pHandle);
	}

	return pHandle;
}

shared_ptr<ResHandle> ResourceCache::Load(Resource *pResource)
{
	shared_ptr<IResourceLoader> pLoader;
	shared_ptr<ResHandle> pHandle;

	for (auto it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it)
	{
		shared_ptr<IResourceLoader> pCurLoader = (*it);
		if (StringHelper::WildcardMatch(pCurLoader->VGetPattern().c_str(), pResource->m_name.c_str()))
		{
			pLoader = pCurLoader;
			break;
		}
	}

	if (!pLoader)
	{
		assert(0 && "No loader is found for such a resource!");
		return pHandle;
	}

	unsigned int uRawSize = m_pFile->VGetRawResourceSize(*pResource);
	char* pRawBuffer = pLoader->VUseRawFile() ? Allocate(uRawSize) : new char[uRawSize];

	if (!pRawBuffer)
	{
		//our resource cache ran out all memory
		return shared_ptr<ResHandle>();
	}

	m_pFile->VGetRawResource(*pResource, pRawBuffer);
	char* pBuffer = nullptr;
	unsigned int uSize = 0;

	if (pLoader->VUseRawFile())
	{
		pBuffer = pRawBuffer;
		pHandle = make_shared<ResHandle>(*pResource, pBuffer, uRawSize, this);
	}
	else
	{
		uSize = pLoader->VGetLoadedResourceSize(pRawBuffer, uRawSize);
		pBuffer = Allocate(uSize);
		if (!pRawBuffer || !pBuffer)
		{
			//our resource cache ran out all memory
			return shared_ptr<ResHandle>();
		}

		pHandle = make_shared<ResHandle>(*pResource, pBuffer, uSize, this);
		bool bSuccess;
		if (pLoader->VNeedFile())
		{
			auto dir = m_pFile->VGetDirectory();
			bSuccess = pLoader->VLoadResource(const_cast<char*>((StringHelper::ws2s(dir) + pResource->m_name).c_str()), uRawSize, pHandle);
		}
		else
		{
			bSuccess = pLoader->VLoadResource(pRawBuffer, uRawSize, pHandle);
		}
		SAFE_DELETE_ARRAY(pRawBuffer);

		if (!bSuccess)
		{
			//our resource cache ran out all memory
			return shared_ptr<ResHandle>();
		}
	}

	//put resource handle to cache lists
	if (pHandle)
	{
		m_lru.push_front(pHandle);
		m_resources[pResource->m_name] = pHandle;
	}

	return pHandle;
}

char* ResourceCache::Allocate(unsigned int uSize)
{
	if (!MakeRoom(uSize))
		return nullptr;

	char* pMemory = new char[uSize];
	if (pMemory)
		m_uAllocated += uSize;

	return pMemory;
}

bool ResourceCache::MakeRoom(unsigned int uSize)
{
	if (uSize > m_uCacheSize)
	{
		return false;
	}

	while (uSize > (m_uCacheSize - m_uAllocated))
	{
		//cache is empty and it's still not enough room to allocated the resource
		if (m_lru.empty())
			return false;

		FreeOneResource();
	}

	return true;
}

void ResourceCache::FreeOneResource()
{
	auto freeMe = m_lru.end();
	freeMe--;

	shared_ptr<ResHandle> pHandle = *freeMe;

	m_lru.pop_back();
	m_resources.erase(pHandle->m_resource.m_name);
}

void ResourceCache::MemoryHasBeenFreed(unsigned int uSize)
{
	m_uAllocated -= uSize;
}

int ResourceCache::Preload(const string & pattern, void(*progressCallback)(int, bool &))
{
	if (m_pFile == NULL)
		return 0;

	int iNumFiles = m_pFile->VGetNumResources();
	int iNumLoaded = 0;
	bool bCancel = false;

	for (int i = 0; i < iNumFiles; ++i)
	{
		Resource resource(m_pFile->VGetResourceName(i));

		if (StringHelper::WildcardMatch(pattern.c_str(), resource.m_name.c_str()))
		{
			shared_ptr<ResHandle> pHandle = GetHandle(&resource);
			++iNumLoaded;
		}

		if (progressCallback)
		{
			progressCallback(static_cast<int>(i* 100.0f / static_cast<float>(iNumFiles)), bCancel);
		}
	}

	return iNumLoaded;
}

void ResourceCache::Update(std::shared_ptr<ResHandle> pHandle)
{
	m_lru.remove(pHandle);
	m_lru.push_front(pHandle);
}

std::shared_ptr<ResHandle> ResourceCache::Find(Resource * r)
{
	auto i = m_resources.find(r->m_name);
	if (i == m_resources.end())
		return shared_ptr<ResHandle>();

	return i->second;
}


void ResourceCache::RegisterLoader(shared_ptr<IResourceLoader> pLoader)
{
	m_resourceLoaders.push_front(pLoader);
}