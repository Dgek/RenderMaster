#pragma once

#include "../../General.h"
#include "../Fixed/InputAssembler.h"

class Blob;
class VertexShader
{
protected:

	ID3D11VertexShader* m_pShader;
	shared_ptr<InputAssemblerState> m_pState;

public:
	__forceinline VertexShader();
	__forceinline ~VertexShader();

	bool Create(Blob & shaderbuffer, INPUT_LAYOUT* layout, unsigned int num, unsigned int topology);
	bool CreateAndCompile(const wstring & fileName, const string & entrypoint, INPUT_LAYOUT* layout,
		unsigned int num, unsigned int topology, const string & target, Blob * pErrors = nullptr);

	__forceinline void Bind() const;
};

__forceinline VertexShader::VertexShader()  
{}

__forceinline VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pShader);
}

__forceinline void VertexShader::Bind() const
{
	DX11API::D3D11DeviceContext()->VSSetShader(m_pShader, 0, 0);

	m_pState->BindInputLayoutAndTopology();
}