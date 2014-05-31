#pragma once

class IMouseHandler
{
public:
	//left mouse button
	virtual bool VOnLButtonDown(float x, float y) = 0;
	virtual bool VOnLButtonUp(float x, float y) = 0;

	//right mouse button
	virtual bool VOnRButtonDown(float x, float y) = 0;
	virtual bool VOnRButtonUp(float x, float y) = 0;

	//mouse move
	virtual bool VOnMouseMove(float x, float y) = 0;
};

// ================= Keyboard ================= //

class IKeyboardHandler
{
public:
	virtual bool VOnKeyDown(Key key) = 0;
	virtual bool VOnKeyUp(Key key) = 0;
};

// ================= Joystick ================= //