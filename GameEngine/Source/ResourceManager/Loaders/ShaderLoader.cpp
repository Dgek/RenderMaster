#include "Core.h"

#include "ShaderLoader.h"

#include "../../Graphics/Other/Blob.h"

string VertexShaderResourceExtraData::VToString()
{
	return "VertexShaderResourceExtraData";
}

string HullShaderResourceExtraData::VToString()
{
	return "HullShaderResourceExtraData";
}

string DomainShaderResourceExtraData::VToString()
{
	return "DomainShaderResourceExtraData";
}

string GeometryShaderResourceExtraData::VToString()
{
	return "GeometryShaderResourceExtraData";
}

string PixelShaderResourceExtraData::VToString()
{
	return "PixelShaderResourceExtraData";
}

string ComputeShaderResourceExtraData::VToString()
{
	return "ComputeShaderResourceExtraData";
}

bool VertexShaderLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<VertexShaderResourceExtraData>();
	pHandle->SetExtra(pExtra);

	pExtra->m_pShader->CreateFromFile(pRawBuffer);

	return true;
}

bool HullShaderLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<HullShaderResourceExtraData>();
	pHandle->SetExtra(pExtra);

	//pExtra->m_pShader->CreateFromFile(pRawBuffer);

	return true;
}

bool DomainShaderLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<DomainShaderResourceExtraData>();
	pHandle->SetExtra(pExtra);

	//pExtra->m_pShader->CreateFromFile(pRawBuffer);

	return true;
}

bool GeometryShaderLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<GeometryShaderResourceExtraData>();
	pHandle->SetExtra(pExtra);

	pExtra->m_pShader->CreateFromFile(pRawBuffer);

	return true;
}

bool PixelShaderLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<PixelShaderResourceExtraData>();
	pHandle->SetExtra(pExtra);

	pExtra->m_pShader->CreateFromFile(pRawBuffer);

	return true;
}

bool ComputeShaderLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<ComputeShaderResourceExtraData>();
	pHandle->SetExtra(pExtra);

	pExtra->m_pShader->CreateFromFile(pRawBuffer);

	return true;
}