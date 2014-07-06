#pragma once

class Resource
{
public:
	string m_name;

public:
	explicit Resource(const string & name);
};

__forceinline Resource::Resource(const string & name)
	: m_name{ name }
{}

class IResourceExtraData
{
public:
	virtual string VToString() = 0;
};

#include "ResourceCache.h"

class ResHandle
{
	friend class ResourceCache;

protected:
	Resource m_resource;
	char* m_pBuffer;
	unsigned int m_size;
	shared_ptr<IResourceExtraData> m_pExtra;

	ResourceCache* m_pCache;

public:
	ResHandle(const Resource & resource, char* pbuffer, unsigned int uSize, ResourceCache* pCache);
	~ResHandle();

	const string & GetName() const;
	unsigned int Size() const;
	char* Buffer() const;

	shared_ptr<IResourceExtraData> GetExtra() const;
	void SetExtra(shared_ptr<IResourceExtraData> pExtra);
};

__forceinline ResHandle::ResHandle(const Resource & resource, char* pbuffer, unsigned int uSize, ResourceCache* pCache)
	: m_resource{ resource }, m_pBuffer{ pbuffer }, m_size{ uSize }, m_pCache{ pCache }
{}

__forceinline ResHandle::~ResHandle()
{
	SAFE_DELETE_ARRAY(m_pBuffer);
	m_pCache->MemoryHasBeenFreed(m_size);
}

__forceinline const string & ResHandle::GetName() const
{
	return m_resource.m_name;
}

__forceinline unsigned int ResHandle::Size() const
{
	return m_size;
}

__forceinline char* ResHandle::Buffer() const
{
	return m_pBuffer;
}

__forceinline shared_ptr<IResourceExtraData> ResHandle::GetExtra() const
{
	return m_pExtra;
}

__forceinline void ResHandle::SetExtra(shared_ptr<IResourceExtraData> pExtra)
{
	m_pExtra = pExtra;
}