#include "Core.h"

#include "GeometryShader.h"

#include "../../Other/Blob.h"

bool GeometryShader::Create(Blob & shaderbuffer)
{
	HRESULT hr = DX11API::D3D11Device()->CreateGeometryShader(shaderbuffer.m_pBlob->GetBufferPointer(),
		shaderbuffer.m_pBlob->GetBufferSize(), nullptr, &m_pShader);

	VALID(hr);
}

bool GeometryShader::CreateFromFile(const char * filename)
{
	SHADER_LOAD_FROM_FILE()

	auto hr = DX11API::D3D11Device()->CreateGeometryShader(&compiledShader[0],size, nullptr, &m_pShader);
	VALID(hr);
}

bool GeometryShader::CreateAndCompile(const wstring & fileName, const string & entrypoint, const string & target, Blob * pErrors)
{
	//Compile the shader first
	Blob & compiledShaderBlob = Blob{};
	if (!compiledShaderBlob.CompileShader(fileName, entrypoint, ST_Geometry, target, pErrors))
	{
		assert(0 && "Some error when compiling the geometry shader occured!");
		return false;
	}

	//Create shader
	if (!Create(compiledShaderBlob))
	{
		assert(0 && "Some error when creating the geometry shader occured!");
		return false;
	}

	return true;
}