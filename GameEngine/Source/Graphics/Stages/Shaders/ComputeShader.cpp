#include "Core.h"

#include "ComputeShader.h"

#include "../../General.h"
#include "../../Other/Blob.h"

bool ComputeShader::Create(Blob & shaderbuffer)
{
	HRESULT hr = DX11API::D3D11Device()->CreateComputeShader(shaderbuffer.m_pBlob->GetBufferPointer(),
		shaderbuffer.m_pBlob->GetBufferSize(), nullptr, &m_pShader);

	VALID(hr);
}

bool ComputeShader::CreateFromFile(const char * filename)
{
	SHADER_LOAD_FROM_FILE()

	auto hr = DX11API::D3D11Device()->CreateComputeShader(&compiledShader[0], size, nullptr, &m_pShader);
	VALID(hr);
}

bool ComputeShader::CreateAndCompile(const wstring & fileName, const string & entrypoint,
	const string & target, Blob * pErrors)
{
	//Compile the shader first
	Blob & compiledShaderBlob = Blob{};
	if (!compiledShaderBlob.CompileShader(fileName, entrypoint, ST_Compute, target, pErrors))
	{
		assert(0 && "Some error when compiling the compute shader occured!");
		return false;
	}

	//Create shader
	if (!Create(compiledShaderBlob))
	{
		assert(0 && "Some error when creating the compute shader occured!");
		return false;
	}

	return true;
}