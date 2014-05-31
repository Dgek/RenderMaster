#pragma once

class Resource
{
public:
	string m_name;

public:
	__forceinline explicit Resource(const string & name);
};

__forceinline Resource::Resource(const string & name)
	: m_name{ name }
{}

class IResourceExtraData
{
public:
	virtual string VToString() = 0;
};

class ResourceCache;
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

	__forceinline const string & GetName() const;
	__forceinline unsigned int Size() const;
	__forceinline char* Buffer() const;

	__forceinline shared_ptr<IResourceExtraData> GetExtra() const;
	__forceinline void SetExtra(shared_ptr<IResourceExtraData> pExtra);
};

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