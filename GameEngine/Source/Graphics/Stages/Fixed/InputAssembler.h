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

__forceinline InputAssemblerState::InputAssemblerState()
	: m_pLayout{ nullptr }, m_pDesc{ nullptr }, m_num{ 0 }
{}

__forceinline InputAssemblerState::~InputAssemblerState()
{
	SAFE_DELETE_ARRAY(m_pDesc);
	SAFE_RELEASE(m_pLayout);
}

__forceinline void InputAssemblerState::BindInputLayoutAndTopology() const
{
	DX11API::D3D11DeviceContext()->IASetInputLayout(m_pLayout);
	DX11API::D3D11DeviceContext()->IASetPrimitiveTopology(m_topology);
}