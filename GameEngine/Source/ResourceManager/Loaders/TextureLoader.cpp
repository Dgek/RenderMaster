#include "Core.h"

#include "TextureLoader.h"

string TextureResourceExtraData::VToString()
{
	return "TextureResourceExtraData";
}

bool TextureLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<TextureResourceExtraData>();
	pHandle->SetExtra(pExtra);

	pExtra->m_pTexture->CreateFromFile(pRawBuffer);

	return true;
}
