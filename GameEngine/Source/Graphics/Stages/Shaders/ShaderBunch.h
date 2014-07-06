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

	bool m_bHullShader;
	bool m_bDomainShader;
	bool m_bGeometryShader;

public:
	ShaderBunch();

	//Shaders
	void SetVertexShader(const string & shaderName, INPUT_LAYOUT* layout,
		unsigned int num, unsigned int topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void SetHullShader(const string & shaderName);
	void SetDomainShader(const string & shaderName);
	void SetGeometryShader(const string & shaderName);
	void SetPixelShader(const string & shaderName);

	void SetVertexShader(shared_ptr<VertexShader> pShader);
	//__forceinline void SetHullShader(shared_ptr<HullShader> pShader);
	//__forceinline void SetDomainShader(shared_ptr<DomainShader> pShader);
	void SetGeometryShader(shared_ptr<GeometryShader> pShader);
	void SetPixelShader(shared_ptr<PixelShader> pShader);

	shared_ptr<VertexShader> GetVertexShader() const;
	shared_ptr<GeometryShader> GetGeometryShader() const;
	shared_ptr<PixelShader> GetPixelShader() const;

	void Bind();
};

__forceinline ShaderBunch::ShaderBunch()
	: m_bHullShader{ false }, m_bDomainShader{ false }, m_bGeometryShader{ false }
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
	m_bHullShader = true;
}

__forceinline void ShaderBunch::SetDomainShader(const string & shaderName)
{
	m_bDomainShader = true;
}

__forceinline void ShaderBunch::SetGeometryShader(const string & shaderName)
{
	Resource shaderResource{ shaderName };
	shared_ptr<ResHandle> pShaderHandle = ResourceCache::SafeGetHandle(&shaderResource);
	shared_ptr<GeometryShaderResourceExtraData> pShaderData = static_pointer_cast<GeometryShaderResourceExtraData>(pShaderHandle->GetExtra());

	m_pGeometryShader = pShaderData->m_pShader;
	m_bGeometryShader = true;
}

__forceinline void ShaderBunch::SetPixelShader(const string & shaderName)
{
	Resource shaderResource{ shaderName };
	shared_ptr<ResHandle> pShaderHandle = ResourceCache::SafeGetHandle(&shaderResource);
	shared_ptr<PixelShaderResourceExtraData> pShaderData = static_pointer_cast<PixelShaderResourceExtraData>(pShaderHandle->GetExtra());

	m_pPixelShader = pShaderData->m_pShader;
}

__forceinline void ShaderBunch::Bind()
{
	m_pVertexShader->Bind();

	//if (m_bHullShader)
	//	m_pHullShader->Bind();

	//if (m_bDomainShader)
	//	m_pDomainShader->Bind();

	if (m_bGeometryShader)
		m_pGeometryShader->Bind();

	m_pPixelShader->Bind();
}

__forceinline void ShaderBunch::SetVertexShader(shared_ptr<VertexShader> pShader)
{
	m_pVertexShader = pShader;
}

__forceinline void ShaderBunch::SetGeometryShader(shared_ptr<GeometryShader> pShader)
{
	m_pGeometryShader = pShader;
	m_bGeometryShader = true;
}

__forceinline void ShaderBunch::SetPixelShader(shared_ptr<PixelShader> pShader)
{
	m_pPixelShader = pShader;
}

__forceinline shared_ptr<VertexShader> ShaderBunch::GetVertexShader() const
{
	return m_pVertexShader;
}

__forceinline shared_ptr<GeometryShader> ShaderBunch::GetGeometryShader() const
{
	return m_pGeometryShader;
}

__forceinline shared_ptr<PixelShader> ShaderBunch::GetPixelShader() const
{
	return m_pPixelShader;
}