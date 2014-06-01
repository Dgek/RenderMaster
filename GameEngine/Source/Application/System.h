#pragma once

struct SystemMessage
{
	SystemMessageType m_type;

	__forceinline explicit SystemMessage(SystemMessageType type);
};

struct QuitMessage : public SystemMessage
{
	__forceinline QuitMessage();
};

struct KeyboardMessage : public SystemMessage
{
	char m_key;

	__forceinline KeyboardMessage(SystemMessageType type, char key);
};

struct KeyDownMessage : public KeyboardMessage
{
	__forceinline explicit KeyDownMessage(char key);
};

struct KeyUpMessage : public KeyboardMessage
{
	__forceinline explicit KeyUpMessage(char key);
};

struct MouseMessage : public SystemMessage
{
	float	m_posx;
	float	m_posy;

	__forceinline MouseMessage(SystemMessageType type, float x, float y);
};

struct LMouseDownMessage : public MouseMessage
{
	__forceinline LMouseDownMessage(float x, float y);
};

struct LMouseUpMessage : public MouseMessage
{
	__forceinline LMouseUpMessage(float x, float y);
};

struct RMouseDownMessage : public MouseMessage
{
	__forceinline RMouseDownMessage(float x, float y);
};

struct RMouseUpMessage : public MouseMessage
{
	__forceinline RMouseUpMessage(float x, float y);
};

struct MouseMoveMessage : public MouseMessage
{
	__forceinline MouseMoveMessage(float x, float y);
};

__forceinline SystemMessage::SystemMessage(SystemMessageType type)
	: m_type{ type }
{}

__forceinline QuitMessage::QuitMessage()
	: SystemMessage{ SMT_Quit }
{}


__forceinline KeyboardMessage::KeyboardMessage(SystemMessageType type, char key)
	: SystemMessage{ type }, m_key{ key }
{}

__forceinline KeyDownMessage::KeyDownMessage(char key) 
	: KeyboardMessage(SMT_KeyDown, key)
{}

__forceinline KeyUpMessage::KeyUpMessage(char key) 
	: KeyboardMessage(SMT_KeyUp, key)
{}


__forceinline MouseMessage::MouseMessage(SystemMessageType type, float x, float y)
	: SystemMessage{ type }, m_posx{ x }, m_posy{ y }
{}

__forceinline LMouseDownMessage::LMouseDownMessage(float x, float y) 
	: MouseMessage{ SMT_LMouseDown, x, y }
{}

__forceinline LMouseUpMessage::LMouseUpMessage(float x, float y) 
	: MouseMessage{ SMT_LMouseUp, x, y }
{}

__forceinline RMouseDownMessage::RMouseDownMessage(float x, float y) 
	: MouseMessage{ SMT_RMouseDown, x, y }
{}

__forceinline RMouseUpMessage::RMouseUpMessage(float x, float y) 
	: MouseMessage{ SMT_RMouseUp, x, y }
{}

__forceinline MouseMoveMessage::MouseMoveMessage(float x, float y)
	: MouseMessage{ SMT_MouseMove, x, y }
{}