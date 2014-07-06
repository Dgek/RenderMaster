#pragma once

class ResHandle;
class IResourceLoader
{
public:
 
	IResourceLoader() =default;

	virtual string VGetPattern() = 0;
	virtual bool VUseRawFile() = 0;
	virtual bool VNeedFile() = 0;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) = 0;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) = 0;
};

class DefaultResourceLoader : public IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, std::shared_ptr<ResHandle> pHandle) override;
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
