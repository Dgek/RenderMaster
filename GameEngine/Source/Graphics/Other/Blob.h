#pragma once

class Blob
{
public:
	ID3D10Blob* m_pBlob;

public:

	Blob();
	~Blob();

	bool CompileShader(const wstring & fileName, const string & entrypoint,
		ShaderType type, const string & target, Blob * pErrors) const;

};

__forceinline Blob::Blob()
	: m_pBlob{ nullptr }
{}

__forceinline Blob::~Blob()
{
	SAFE_RELEASE(m_pBlob);
}