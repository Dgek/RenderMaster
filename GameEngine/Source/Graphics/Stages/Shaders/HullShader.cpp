#include "Core.h"

#include "HullShader.h"

#include "../../General.h"
#include "../../Other/Blob.h"

bool HullShader::Create(Blob & shaderbuffer)
{
	auto hr = DX11API::D3D11Device()->CreateHullShader(shaderbuffer.m_pBlob->GetBufferPointer(),
		shaderbuffer.m_pBlob->GetBufferSize(), nullptr, &m_pShader);

	VALID(hr);
}

bool HullShader::CreateFromFile(const char * filename)
{
	SHADER_LOAD_FROM_FILE()

	auto hr = DX11API::D3D11Device()->CreateHullShader(&compiledShader[0], size, nullptr, &m_pShader);
	VALID(hr);
}

bool HullShader::CreateAndCompile(const wstring & fileName, const string & entrypoint,
	const string & target, Blob * pErrors)
{
	//Compile the shader first
	Blob & compiledShaderBlob = Blob{};
	if (!compiledShaderBlob.CompileShader(fileName, entrypoint, ST_Compute, target, pErrors))
	{
		assert(0 && "Some error when compiling the domain shader occured!");
		return false;
	}

	//Create shader
	if (!Create(compiledShaderBlob))
	{
		assert(0 && "Some error when creating the domain shader occured!");
		return false;
	}

	return true;
}