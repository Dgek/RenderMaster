#include "Core.h"

#include "Blob.h"

bool Blob::CompileShader(const wstring & fileName, const string & entrypoint,
	ShaderType type, const string & target, Blob * pErrors) const
{
	//Compilation
	HRESULT hr;

	//hr = D3DX11CompileFromFile(fileName.c_str(), 0, 0, entrypoint.c_str(), target.c_str(), D3DCOMPILE_DEBUG, 0, 0,
	//	(ID3DBlob**)&m_pBlob, &pErrors->m_pBlob, 0);

	//TODO::IMPORTANT

	//If any errors, draw them to debug output
	if (hr != S_OK)
	{
		OutputDebugStringA((char*)pErrors->m_pBlob->GetBufferPointer());
		CheckHR(hr);
		return false;
	}

	return true;
}