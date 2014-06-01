#pragma once

struct GraphicsBox : public D3D11_BOX
{
public:

	__forceinline GraphicsBox(unsigned int left, unsigned int top, unsigned int front,
		unsigned int right, unsigned int bottom, unsigned int back);
};

__forceinline GraphicsBox::GraphicsBox(unsigned int left, unsigned int top, unsigned int front,
	unsigned int right, unsigned int bottom, unsigned int back)
	: D3D11_BOX{ left, top, front, right, bottom, back }
{}