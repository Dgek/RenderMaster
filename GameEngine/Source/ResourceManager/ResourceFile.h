#pragma once

class Resource;
class IResourceFile
{
public:

	virtual ~IResourceFile() = 0;

	virtual bool VOpen() = 0;
	virtual int VGetRawResourceSize(const Resource & resource) = 0;
	virtual int VGetRawResource(const Resource & resource, char* pBuffer) = 0;
	virtual int VGetNumResources() const = 0;
	virtual string VGetResourceName(int num) const = 0;

	virtual wstring VGetDirectory() const = 0;
};