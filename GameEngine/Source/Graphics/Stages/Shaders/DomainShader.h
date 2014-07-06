#pragma once

#include "../../General.h"

class Blob;
class DomainShader
{
protected:
	ID3D11DomainShader* m_pShader;

public:
	DomainShader();
	~DomainShader();

	void Bind() const;

	bool Create(Blob & shaderbuffer);
	bool CreateFromFile(const char * filename);
	bool CreateAndCompile(const wstring & fileName, const string & entrypoint, const string & target, Blob * pErrors = nullptr);
};

__forceinline DomainShader::DomainShader()
{}

__forceinline DomainShader::~DomainShader()
{
	SAFE_RELEASE(m_pShader);
}

__forceinline void DomainShader::Bind() const
{
	DX11API::D3D11DeviceContext()->DSSetShader(m_pShader, 0, 0);
}