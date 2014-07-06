#include "Core.h"

#include "VertexShader.h"

#include "../../General.h"
#include "../../Other/Blob.h"

bool VertexShader::Create(Blob & shaderbuffer, INPUT_LAYOUT* layout, unsigned int num, unsigned int topology)
{
	//create vertex shader
	auto hr = DX11API::D3D11Device()->CreateVertexShader(shaderbuffer.m_pBlob->GetBufferPointer(),
		shaderbuffer.m_pBlob->GetBufferSize(), nullptr, &m_pShader);
	if (hr != S_OK)
	{
		CheckHR(hr);
		return false;
	}

	//create input layout
	hr = DX11API::D3D11Device()->CreateInputLayout(layout, num, shaderbuffer.m_pBlob->GetBufferPointer(),
		shaderbuffer.m_pBlob->GetBufferSize(), &m_pState->m_pLayout);
	if (hr != S_OK)
	{
		CheckHR(hr);
		return false;
	}

	//set primitive topology
	m_pState->m_topology = static_cast<D3D11_PRIMITIVE_TOPOLOGY>(topology);

	return true;
}

bool VertexShader::CreateFromFile(const char * filename)
{
	SHADER_LOAD_FROM_FILE()

	auto hr = DX11API::D3D11Device()->CreateVertexShader(m_pBuffer.get(), m_size, nullptr, &m_pShader);
	VALID(hr);
}

bool VertexShader::CreateAndCompile(const wstring & fileName, const string & entrypoint, INPUT_LAYOUT* layout,
	unsigned int num, unsigned int topology, const string & target, Blob * pErrors)
{
	//Compile the shader first
	shared_ptr<Blob> pCompiledShaderBlob = make_shared<Blob>();
	if (!pCompiledShaderBlob->CompileShader(fileName, entrypoint, ST_Vertex, target, pErrors))
	{
		assert(0 && "Some error when compiling the vertex shader occured!");
		return false;
	}

	//create vertex shader
	HRESULT hr = DX11API::D3D11Device()->CreateVertexShader(pCompiledShaderBlob->m_pBlob->GetBufferPointer(),
		pCompiledShaderBlob->m_pBlob->GetBufferSize(), nullptr, &m_pShader);
	if (hr != S_OK)
	{
		CheckHR(hr);
		return false;
	}

	//create input layout
	hr = DX11API::D3D11Device()->CreateInputLayout(layout, num, pCompiledShaderBlob->m_pBlob->GetBufferPointer(),
		pCompiledShaderBlob->m_pBlob->GetBufferSize(), &m_pState->m_pLayout);
	if (hr != S_OK)
	{
		CheckHR(hr);
		return false;
	}

	//set primitive topology
	m_pState->m_topology = (D3D11_PRIMITIVE_TOPOLOGY)topology;

	return true;
}

bool VertexShader::SetInputAssemblerState(INPUT_LAYOUT* pLayout, unsigned int num, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	//create input layout
	auto hr = DX11API::D3D11Device()->CreateInputLayout(pLayout, num, m_pBuffer.get(),
		m_size, &m_pState->m_pLayout);

	//set primitive topology
	m_pState->m_topology = topology;

	ClearBuffer();

	return true;
}