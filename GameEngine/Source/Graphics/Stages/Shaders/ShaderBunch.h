#pragma once

#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

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
	void VSetVertexShader(const string & shaderName, INPUT_LAYOUT* layout,
		unsigned int num, unsigned int topology, const string & target);
	void VSetHullShader(const string & shaderName, const string & target);
	void VSetDomainShader(const string & shaderName, const string & target);
	void VSetGeometryShader(const string & shaderName, const string & target);
	void VSetPixelShader(const string & shaderName, const string & target);

	__forceinline void VBind();
};

__forceinline ShaderBunch::ShaderBunch()
{}

__forceinline void ShaderBunch::VBind()
{
	m_pVertexShader->Bind();
	//m_pHullShader->Bind();
	//m_pDomainShader->Bind();
	m_pGeometryShader->Bind();
	m_pPixelShader->Bind();
}