#include "Core.h"

#include "ResourceCache.h"

void ResourceCache::RegisterLoader(shared_ptr<IResourceLoader> pLoader)
{
	m_resourceLoaders.push_front(pLoader);
}