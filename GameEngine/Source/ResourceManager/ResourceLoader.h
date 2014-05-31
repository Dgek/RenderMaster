#pragma once

class IResourceLoader
{
public:
 
	__forceinline IResourceLoader();

	virtual string VGetPattern() = 0;
	virtual bool VUseRawFile() = 0;
	virtual bool VNeedFile() = 0;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) = 0;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) = 0;
};

class DefaultResourceLoader : public IResourceLoader
{
public:
	__forceinline virtual string VGetPattern() override;
	__forceinline virtual bool VUseRawFile() override;
	__forceinline virtual bool VNeedFile() override;
	__forceinline virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	__forceinline virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, std::shared_ptr<ResHandle> pHandle) override;
};

__forceinline virtual string DefaultResourceLoader::VGetPattern() override
{
	return "*";
}

__forceinline virtual bool DefaultResourceLoader::VUseRawFile() override
{
	return true;
}

__forceinline virtual bool DefaultResourceLoader::VNeedFile() override
{
	return false;
}

__forceinline virtual int DefaultResourceLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override
{
	return uRawSize;
}

__forceinline virtual bool DefaultResourceLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, std::shared_ptr<ResHandle> pHandle) override
{
	return true;
}
