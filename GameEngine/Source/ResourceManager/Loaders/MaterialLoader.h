#pragma once

#include "../Resource.h"
#include "../ResourceLoader.h"

#include "../../Renderer/Materials/Material.h"

class MaterialResourceExtraData : public IResourceExtraData
{
public:
	shared_ptr<Material> m_pMaterial;

	__forceinline MaterialResourceExtraData();
	virtual string VToString() override;
};

__forceinline MaterialResourceExtraData::MaterialResourceExtraData()
{
	m_pMaterial = make_shared<Material>();
}


class MaterialLoader : public IResourceLoader
{
public:
	__forceinline virtual string VGetPattern() override;
	__forceinline virtual bool VUseRawFile() override;
	__forceinline  virtual bool VNeedFile() override;
	__forceinline virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;

private:

	void LoadTextures(ifstream & file, MaterialResourceExtraData* pData);
	bool LoadShader(ifstream & file);
};

__forceinline string MaterialLoader::VGetPattern()
{
	return "*.mtl";
}

__forceinline bool MaterialLoader::VUseRawFile()
{
	return false;
}

__forceinline bool MaterialLoader::VNeedFile()
{
	return true;
}

__forceinline int MaterialLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}