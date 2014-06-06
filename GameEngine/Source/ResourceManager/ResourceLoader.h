#pragma once

class ResHandle;
class IResourceLoader
{
public:
 
	__forceinline IResourceLoader() =default;

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

__forceinline string DefaultResourceLoader::VGetPattern()
{
	return "*";
}

__forceinline bool DefaultResourceLoader::VUseRawFile()
{
	return true;
}

__forceinline bool DefaultResourceLoader::VNeedFile()
{
	return false;
}

__forceinline int DefaultResourceLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return uRawSize;
}

__forceinline bool DefaultResourceLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, std::shared_ptr<ResHandle> pHandle)
{
	return true;
}
