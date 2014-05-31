#include "../ResourceFile.h"
#include "../../Help/Strings/Strings.h"

class ResourceDir : public IResourceFile
{
protected:

	vector<WIN32_FIND_DATA> m_resourcesData;
	ContentMap m_directoryContent;
	wstring m_directory;

protected:
	void ReadResourceDirectory(const wstring & pattern);

public:

	explicit ResourceDir(const wstring & dir);

	virtual bool VOpen() override;
	virtual int VGetRawResourceSize(const Resource & resource) override;
	virtual int VGetRawResource(const Resource & resource, char* pBuffer) override;
	__forceinline virtual int VGetNumResources() const override;
	__forceinline virtual string VGetResourceName(int num) const override;

	__forceinline virtual wstring VGetDirectory() const override;

	int Find(const string & name);
};

__forceinline int ResourceDir::VGetNumResources() const
{
	return m_resourcesData.size();
}

__forceinline string ResourceDir::VGetResourceName(int num) const
{
	wstring resName = m_resourcesData[num].cFileName;
	return StringHelper::ws2s(resName);
}

__forceinline wstring ResourceDir::VGetDirectory() const
{
	return m_directory;
}