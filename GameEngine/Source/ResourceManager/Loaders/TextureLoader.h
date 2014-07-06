#pragma once

#include "../Resource.h"
#include "../ResourceLoader.h"

#include "../../Graphics/Resources/Textures/Texture2D.h"

class TextureResourceExtraData : public IResourceExtraData
{
public:
	shared_ptr<Texture2D> m_pTexture;

	TextureResourceExtraData();
	virtual string VToString() override;
};

__forceinline TextureResourceExtraData::TextureResourceExtraData()
{
	m_pTexture = make_shared<Texture2D>();
}


class TextureLoader : public IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;
};

__forceinline string TextureLoader::VGetPattern()
{
	return "*.dds";
}

__forceinline bool TextureLoader::VUseRawFile()
{
	return false;
}

__forceinline bool TextureLoader::VNeedFile()
{
	return true;
}

__forceinline int TextureLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}