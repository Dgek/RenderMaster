#pragma once

#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

#include "../../../ResourceManager/Resource.h"
#include "../../../ResourceManager/ResourceCache.h"
#include "../../../ResourceManager/Loaders/ShaderLoader.h"

class ShaderBunch
{
private:
	shared_ptr<VertexShader> m_pVertexShader;
	shared_ptr<HullShader> m_pHullShader;
	shared_ptr<DomainShader> m_pDomainShader;
	shared_ptr<GeometryShader> m_pGeometryShader;
	shared_ptr<PixelShader> m_pPixelShader;

public:
	__forceinline ShaderBunch();

	//Shaders
	__forceinline void SetVertexShader(const string & shaderName, INPUT_LAYOUT* layout,
		unsigned int num, unsigned int topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	__forceinline void SetHullShader(const string & shaderName);
	__forceinline void SetDomainShader(const string & shaderName);
	__forceinline void SetGeometryShader(const string & shaderName);
	__forceinline void SetPixelShader(const string & shaderName);

	__forceinline void VBind();
};

__forceinline ShaderBunch::ShaderBunch()
{}

__forceinline void ShaderBunch::SetVertexShader(const string & shaderName, INPUT_LAYOUT* pLayout,
	unsigned int num, unsigned int topology)
{
	Resource shaderResource{ shaderName };
	shared_ptr<ResHandle> pShaderHandle = ResourceCache::SafeGetHandle(&shaderResource);
	shared_ptr<VertexShaderResourceExtraData> pShaderData = static_pointer_cast<VertexShaderResourceExtraData>(pShaderHandle->GetExtra());

	m_pVertexShader = pShaderData->m_pShader;
	m_pVertexShader->SetInputAssemblerState(pLayout, num, static_cast<D3D11_PRIMITIVE_TOPOLOGY>(topology));
}

__forceinline void ShaderBunch::SetHullShader(const string & shaderName)
{

}

__forceinline void ShaderBunch::SetDomainShader(const string & shaderName)
{

}

__forceinline void ShaderBunch::SetGeometryShader(const string & shaderName)
{
	Resource shaderResource{ shaderName };
	shared_ptr<ResHandle> pShaderHandle = ResourceCache::SafeGetHandle(&shaderResource);
	shared_ptr<GeometryShaderResourceExtraData> pShaderData = static_pointer_cast<GeometryShaderResourceExtraData>(pShaderHandle->GetExtra());

	m_pGeometryShader = pShaderData->m_pShader;
}

__forceinline void ShaderBunch::SetPixelShader(const string & shaderName)
{
	Resource shaderResource{ shaderName };
	shared_ptr<ResHandle> pShaderHandle = ResourceCache::SafeGetHandle(&shaderResource);
	shared_ptr<PixelShaderResourceExtraData> pShaderData = static_pointer_cast<PixelShaderResourceExtraData>(pShaderHandle->GetExtra());

	m_pPixelShader = pShaderData->m_pShader;
}

__forceinline void ShaderBunch::VBind()
{
	m_pVertexShader->Bind();
	//m_pHullShader->Bind();
	//m_pDomainShader->Bind();
	m_pGeometryShader->Bind();
	m_pPixelShader->Bind();
}