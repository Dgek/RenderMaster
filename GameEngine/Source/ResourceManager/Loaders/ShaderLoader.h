#pragma once

#include "../Resource.h"
#include "../ResourceLoader.h"

#include "../../Graphics/Stages/Shaders/VertexShader.h"
#include "../../Graphics/Stages/Shaders/HullShader.h"
#include "../../Graphics/Stages/Shaders/DomainShader.h"
#include "../../Graphics/Stages/Shaders/GeometryShader.h"
#include "../../Graphics/Stages/Shaders/PixelShader.h"
#include "../../Graphics/Stages/Shaders/ComputeShader.h"

class VertexShaderResourceExtraData : public IResourceExtraData
{
public:
	shared_ptr<VertexShader> m_pShader;

	VertexShaderResourceExtraData();
	virtual string VToString() override;
};

__forceinline VertexShaderResourceExtraData::VertexShaderResourceExtraData()
{
	m_pShader = make_shared<VertexShader>();
}

class HullShaderResourceExtraData : public IResourceExtraData
{
public:
	shared_ptr<HullShader> m_pShader;

	HullShaderResourceExtraData();
	virtual string VToString() override;
};

__forceinline HullShaderResourceExtraData::HullShaderResourceExtraData()
{
	m_pShader = make_shared<HullShader>();
}

class DomainShaderResourceExtraData : public IResourceExtraData
{
public:
	shared_ptr<DomainShader> m_pShader;

	DomainShaderResourceExtraData();
	virtual string VToString() override;
};

__forceinline DomainShaderResourceExtraData::DomainShaderResourceExtraData()
{
	m_pShader = make_shared<DomainShader>();
}

class GeometryShaderResourceExtraData : public IResourceExtraData
{
public:
	shared_ptr<GeometryShader> m_pShader;

	GeometryShaderResourceExtraData();
	virtual string VToString() override;
};

__forceinline GeometryShaderResourceExtraData::GeometryShaderResourceExtraData()
{
	m_pShader = make_shared<GeometryShader>();
}

class PixelShaderResourceExtraData : public IResourceExtraData
{
public:
	shared_ptr<PixelShader> m_pShader;

	PixelShaderResourceExtraData();
	virtual string VToString() override;
};

__forceinline PixelShaderResourceExtraData::PixelShaderResourceExtraData()
{
	m_pShader = make_shared<PixelShader>();
}

class ComputeShaderResourceExtraData : public IResourceExtraData
{
public:
	shared_ptr<ComputeShader> m_pShader;

	ComputeShaderResourceExtraData();
	virtual string VToString() override;
};

__forceinline ComputeShaderResourceExtraData::ComputeShaderResourceExtraData()
{
	m_pShader = make_shared<ComputeShader>();
}


class VertexShaderLoader : public IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;
};

__forceinline string VertexShaderLoader::VGetPattern()
{
	return "*.vso";
}

__forceinline bool VertexShaderLoader::VUseRawFile()
{
	return false;
}

__forceinline bool VertexShaderLoader::VNeedFile()
{
	return true;
}

__forceinline int VertexShaderLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}


class HullShaderLoader : public IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;
};

__forceinline string HullShaderLoader::VGetPattern()
{
	return "*.hso";
}

__forceinline bool HullShaderLoader::VUseRawFile()
{
	return false;
}

__forceinline bool HullShaderLoader::VNeedFile()
{
	return true;
}

__forceinline int HullShaderLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}

class DomainShaderLoader : public IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;
};

__forceinline string DomainShaderLoader::VGetPattern()
{
	return "*.dso";
}

__forceinline bool DomainShaderLoader::VUseRawFile()
{
	return false;
}

__forceinline bool DomainShaderLoader::VNeedFile()
{
	return true;
}

__forceinline int DomainShaderLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}

class GeometryShaderLoader : public IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;
};

__forceinline string GeometryShaderLoader::VGetPattern()
{
	return "*.gso";
}

__forceinline bool GeometryShaderLoader::VUseRawFile()
{
	return false;
}

__forceinline bool GeometryShaderLoader::VNeedFile()
{
	return true;
}

__forceinline int GeometryShaderLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}

class ComputeShaderLoader : public IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;
};

__forceinline string ComputeShaderLoader::VGetPattern()
{
	return "*.cso";
}

__forceinline bool ComputeShaderLoader::VUseRawFile()
{
	return false;
}

__forceinline bool ComputeShaderLoader::VNeedFile()
{
	return true;
}

__forceinline int ComputeShaderLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}

class PixelShaderLoader : public IResourceLoader
{
public:
	virtual string VGetPattern() override;
	virtual bool VUseRawFile() override;
	virtual bool VNeedFile() override;
	virtual int VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize) override;
	virtual bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle) override;
};

__forceinline string PixelShaderLoader::VGetPattern()
{
	return "*.pso";
}

__forceinline bool PixelShaderLoader::VUseRawFile()
{
	return false;
}

__forceinline bool PixelShaderLoader::VNeedFile()
{
	return true;
}

__forceinline int PixelShaderLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{
	return 10;
}