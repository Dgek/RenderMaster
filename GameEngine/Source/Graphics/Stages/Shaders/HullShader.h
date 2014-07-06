#pragma once

#include "../../General.h"

class Blob;
class HullShader
{
protected:
	ID3D11HullShader* m_pShader;

public:
	HullShader();
	~HullShader();

	void Bind() const;

	bool Create(Blob & shaderbuffer);
	bool CreateFromFile(const char * filename);
	bool CreateAndCompile(const wstring & fileName, const string & entrypoint, const string & target, Blob * pErrors = nullptr);
};

__forceinline HullShader::HullShader()
{}

__forceinline HullShader::~HullShader()
{
	SAFE_RELEASE(m_pShader);
}

__forceinline void HullShader::Bind() const
{
	DX11API::D3D11DeviceContext()->HSSetShader(m_pShader, 0, 0);
}