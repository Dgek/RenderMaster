#include "Core.h"

#include "PixelShader.h"

#include "../../Other/Blob.h"

bool PixelShader::Create(Blob & shaderbuffer)
{
	HRESULT hr = DX11API::D3D11Device()->CreatePixelShader(shaderbuffer.m_pBlob->GetBufferPointer(),
		shaderbuffer.m_pBlob->GetBufferSize(), NULL, &m_pShader);

	VALID(hr);
}

bool PixelShader::CreateFromFile(const char * filename)
{
	SHADER_LOAD_FROM_FILE()

	auto hr = DX11API::D3D11Device()->CreatePixelShader(&compiledShader[0], size, nullptr, &m_pShader);
	VALID(hr);
}

bool PixelShader::CreateAndCompile(const wstring & fileName, const string & entrypoint, const string & target, Blob * pErrors)
{
	//Compile the shader first
	Blob compiledShaderBlob{};
	if (!compiledShaderBlob.CompileShader(fileName, entrypoint, ST_Pixel, target, pErrors))
	{
		assert(0 && "Some error when compiling the pixel shader occured!");
		return false;
	}

	//Create shader
	if (!Create(compiledShaderBlob))
	{
		assert(0 && "Some error when creating the pixel shader occured!");
		return false;
	}

	return true;
}