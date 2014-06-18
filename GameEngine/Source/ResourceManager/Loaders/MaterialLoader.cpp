#include "Core.h"

#include "MaterialLoader.h"
#include "TextureLoader.h"

string MaterialResourceExtraData::VToString()
{
	return "MaterialResourceExtraData";
}

bool MaterialLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<MaterialResourceExtraData>();
	pHandle->SetExtra(pExtra);

	ifstream file(pRawBuffer, ios::in);
	if (!file.is_open())
		return false;

	string str, token;
	file >> str;
	while (!file.eof())
	{
		if (str == "Textures")
		{
			LoadTextures(file, pExtra.get());
		}
		else if (str == "Shader")
		{
			if (!LoadShader(file))
			{
				file.close();
				return false;
			}
		}

		file >> str;
	}

	file.close();

	return true;
}

void MaterialLoader::LoadTextures(ifstream & file, MaterialResourceExtraData* pData)
{
	string str, token;
	file >> token;
	while (true)
	{
		file >> str;
		if ((str == "}") || (file.eof()))
			break;
		else if (str == "ColorTexture")
		{
			file >> str;
			Resource textureResource{ str };
			shared_ptr<ResHandle> pTextureHandle = ResourceCache::SafeGetHandle(&textureResource);
			shared_ptr<TextureResourceExtraData> pTextureData = static_pointer_cast<TextureResourceExtraData>(pTextureHandle->GetExtra());
			
			pData->m_pMaterial->SetDiffuse(pTextureData->m_pTexture);
		}
		else if (str == "NormalTexture")
		{
			file >> str;
			Resource textureResource{ str };
			shared_ptr<ResHandle> pTextureHandle = ResourceCache::SafeGetHandle(&textureResource);
			shared_ptr<TextureResourceExtraData> pTextureData = static_pointer_cast<TextureResourceExtraData>(pTextureHandle->GetExtra());

			pData->m_pMaterial->SetNormal(pTextureData->m_pTexture);
		}
		else if (str == "SpecularTexture")
		{
			file >> str;
			Resource textureResource{ str };
			shared_ptr<ResHandle> pTextureHandle = ResourceCache::SafeGetHandle(&textureResource);
			shared_ptr<TextureResourceExtraData> pTextureData = static_pointer_cast<TextureResourceExtraData>(pTextureHandle->GetExtra());

			pData->m_pMaterial->SetSpecular(pTextureData->m_pTexture);
		}
	}
}

bool MaterialLoader::LoadShader(ifstream & file)
{
	return true;
}