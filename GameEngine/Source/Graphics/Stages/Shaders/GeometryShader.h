#pragma once

#include "../../General.h"

class Blob;
class GeometryShader
{
protected:
	ID3D11GeometryShader* m_pShader;

public:
	GeometryShader();
	~GeometryShader();

	void Bind() const;

	bool Create(Blob & shaderbuffer);
	bool CreateFromFile(const char * filename);
	bool CreateAndCompile(const wstring & fileName, const string & entrypoint, const string & target, Blob * pErrors = nullptr);
};

__forceinline GeometryShader::GeometryShader()
{}

__forceinline GeometryShader::~GeometryShader()
{
	SAFE_RELEASE(m_pShader);
}

__forceinline void GeometryShader::Bind() const
{
	DX11API::D3D11DeviceContext()->GSSetShader(m_pShader, 0, 0);
}