#pragma once

#include "../../General.h"

class Blob;
class PixelShader
{
protected:
	ID3D11PixelShader*	m_pShader;

public:
	__forceinline PixelShader();
	__forceinline ~PixelShader();

	__forceinline void Bind() const;

	bool Create(Blob & shaderbuffer);
	bool CreateAndCompile(const wstring & fileName, const string & entrypoint, const string & target, Blob * pErrors = nullptr);
};

__forceinline PixelShader::PixelShader()
{}

__forceinline PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pShader);
}

__forceinline void PixelShader::Bind() const
{
	DX11API::D3D11DeviceContext()->PSSetShader(m_pShader, 0, 0);
}