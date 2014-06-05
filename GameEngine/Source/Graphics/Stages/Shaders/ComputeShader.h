#pragma once

#include "../../General.h"

class Blob;
class ComputeShader
{
private:
	ID3D11ComputeShader* m_pShader;

public:
	__forceinline ComputeShader();
	__forceinline ~ComputeShader();

	__forceinline void Bind() const;

	bool Create(Blob & shaderbuffer);
	bool CreateAndCompile(const wstring & fileName, const string & entrypoint,
		const string & target, Blob * pErrors = nullptr);

};

__forceinline ComputeShader::ComputeShader()
{}

__forceinline ComputeShader::~ComputeShader()
{
	SAFE_RELEASE(m_pShader);
}

__forceinline void ComputeShader::Bind() const
{
	DX11API::D3D11DeviceContext()->CSSetShader(m_pShader, 0, 0);
}