#pragma once

struct GraphicsBox : public D3D11_BOX
{
public:

	__forceinline GraphicsBox(unsigned int left, unsigned int top, unsigned int front,
		unsigned int right, unsigned int bottom, unsigned int back);
};

__forceinline GraphicsBox::GraphicsBox(unsigned int left, unsigned int top, unsigned int front,
	unsigned int right, unsigned int bottom, unsigned int back)
{
	this->left = left;
	this->top = top;
	this->front = front;
	this->right = right;
	this->bottom = bottom;
	this->back = back;
}

struct SubresourceData : public D3D11_SUBRESOURCE_DATA
{
	__forceinline void SetData(const void* pData, unsigned int sysMemPitch = 0, unsigned int sysMemSlicePitch = 0);
};

__forceinline void SubresourceData::SetData(const void* pData, unsigned int sysMemPitch, unsigned int sysMemSlicePitch)
{
	pSysMem = pData;
	sysMemPitch = sysMemPitch;
	sysMemSlicePitch = sysMemSlicePitch;
}