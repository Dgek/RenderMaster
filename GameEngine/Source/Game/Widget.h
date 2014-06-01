#pragma once

class Widget
{
public:
	virtual void VUpdate(unsigned int deltaMilliseconds) = 0;
	virtual void VRender(Renderer* pRenderer, double time, double elapsedTime) = 0;
	virtual bool VMsgProc(const SystemMessage & msg) = 0;

	//To order widgets on the screen
	virtual void VSetZ(unsigned char z) = 0;
	virtual unsigned char VGetZ() const = 0;

	virtual void VSetVisible(bool visible) = 0;
	virtual bool VGetVisible() const = 0;

	__forceinline bool operator<(const Widget & widget);
};

__forceinline bool Widget::operator<(const Widget & widget)
{
	return VGetZ() < widget.VGetZ();
}