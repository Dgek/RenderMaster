#pragma once

class InputAssemblerState
{
	friend class VertexShader;

protected:
	ID3D11InputLayout* m_pLayout;
	INPUT_LAYOUT* m_pDesc;
	D3D11_PRIMITIVE_TOPOLOGY m_topology;
	unsigned char m_num;

public:

	__forceinline InputAssemblerState();
	__forceinline ~InputAssemblerState();

	__forceinline void BindInputLayoutAndTopology() const;
};