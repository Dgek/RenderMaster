#pragma once

#include "../../General.h"
#include "../Fixed/InputAssembler.h"

class Blob;
class VertexShader
{
protected:

	ID3D11VertexShader* m_pShader;
	shared_ptr<InputAssemblerState> m_pState;

	unique_ptr<char[]> m_pBuffer;
	int m_size;

public:
	VertexShader();
	~VertexShader();

	bool Create(Blob & shaderbuffer, INPUT_LAYOUT* layout, unsigned int num, unsigned int topology);
	bool CreateFromFile(const char * filename);
	bool CreateAndCompile(const wstring & fileName, const string & entrypoint, INPUT_LAYOUT* layout,
		unsigned int num, unsigned int topology, const string & target, Blob * pErrors = nullptr);

	bool SetInputAssemblerState(INPUT_LAYOUT* pLayout, unsigned int num, D3D11_PRIMITIVE_TOPOLOGY);

	void ClearBuffer();

	void Bind() const;
};

__forceinline VertexShader::VertexShader()
	: m_size{ 0 }
{
	m_pState = make_shared<InputAssemblerState>();
}

__forceinline VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pShader);
}

__forceinline void VertexShader::ClearBuffer()
{
	m_pBuffer.reset();
	m_size = 0;
}

__forceinline void VertexShader::Bind() const
{
	DX11API::D3D11DeviceContext()->VSSetShader(m_pShader, 0, 0);

	m_pState->BindInputLayoutAndTopology();
}