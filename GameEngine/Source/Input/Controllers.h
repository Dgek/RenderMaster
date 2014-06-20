#pragma once

#include "Devices.h"

#include "../Math/float2.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"

class MovementController
{
public:
	virtual float VGetTargetYaw() const	{ return 0.0f; }
	virtual float VGetTargetPitch() const { return 0.0f; }
	virtual float VGetTargetRoll() const { return 0.0f; }
	virtual float VGetTargetYawRot() const { return 0.0f; }
	virtual float VGetTargetPitchRot() const { return 0.0f; }
	virtual float VGetTargetRollRot() const	{ return 0.0f; }
	virtual float VGetTargetAngularSpeed() const { return 0.0f; }
	virtual float VGetTargetSpeed() const { return 0.0f; }
	virtual Vec	VGetTargetDirection() const	{ return Vec{ 0.0f, 0.0f, 1.0f, 0.0f }; }
	virtual Vec	VGetTargetVelocity() const { return Vec{ 0.0f, 0.0f, 0.0f, 0.0f }; };

	virtual void VSetTargetDirection(const Vec & vDir) = 0;

	virtual bool VWantToJump() = 0;

	virtual void VUpdate(int deltaMs) {}
};

class PCPlayerController : public IMouseHandler, public IKeyboardHandler
{
protected:
	//key codes
	Key	m_bKey[256];

	//previous mouse position
	float2 m_prevMousePos;
	Point m_screenCenter;

	public:

	__forceinline PCPlayerController();

	__forceinline bool VOnLButtonDown(float x, float y) { return true; }
	__forceinline bool VOnLButtonUp(float x, float y)	{ return true; }

	__forceinline bool VOnRButtonDown(float x, float y) { return true; }
	__forceinline bool VOnRButtonUp(float x, float y) { return true; }

	__forceinline bool VOnMouseMove(float x, float y) { return true; }

	__forceinline bool VOnKeyDown(Key key);
	__forceinline bool VOnKeyUp(Key key);
};

__forceinline PCPlayerController::PCPlayerController()
	: m_screenCenter{ 960, 540 }
{
	for (auto i = 0; i < 256; ++i)
	{
		m_bKey[i] = false;
	}

	//m_screenCenter = Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SetCursorPos(m_screenCenter.x, m_screenCenter.y);
}

__forceinline bool PCPlayerController::VOnKeyDown(Key key)
{
	m_bKey[key] = true;

	return true;
}

__forceinline bool PCPlayerController::VOnKeyUp(Key key)
{
	m_bKey[key] = false;

	return true;
}

class FreeCameraController : public PCPlayerController, public MovementController
{
protected:

	Vec		m_vTargetDirection;
	Vec		m_vTargetVelocity;

	float m_r32TargetYawRot;
	float	m_r32TargetPitchRot;
	float	m_r32TargetRollRot;
	float	m_r32TargetYaw;
	float	m_r32TargetPitch;
	float	m_r32TargetRoll;

	float	m_r32MaxSpeed;
	float	m_r32AngularSpeed;
	float	m_r32CurrentSpeed;
	float	m_r32TargetSpeed;

public:

	//Contructor
	__forceinline FreeCameraController();

	// Mouse Handler Implementation //
	virtual bool VOnMouseMove(float x, float y) override
	{
		/*if (x != m_screenCenter.x() || y != m_screenCenter.y())
		{
		m_r32TargetYawRot = (x - m_screenCenter.x());
		m_r32TargetPitchRot = -(y - m_screenCenter.y());

		m_r32TargetYaw += m_r32TargetYawRot;
		m_r32TargetPitch += m_r32TargetPitchRot;

		SetCursorPos(m_screenCenter.x(), m_screenCenter.y());

		m_prevMousePos.x = x;
		m_prevMousePos.y = y;
		} */

		if (x != m_prevMousePos.x || y != m_prevMousePos.y)
		{
			m_r32TargetYawRot = (x - m_prevMousePos.x) * 0.01f;
			m_r32TargetPitchRot = -(y - m_prevMousePos.y) * 0.01f;

			m_r32TargetYaw += m_r32TargetYawRot;
			m_r32TargetPitch += m_r32TargetPitchRot;
			m_r32TargetPitch = Math::Clamp(static_cast<float>(m_r32TargetPitch),
				static_cast<float>(-0.25f*PI), static_cast<float>(0.25f*PI));

			//SetCursorPos(m_screenCenter.x(), m_screenCenter.y());

			m_prevMousePos.x = x;
			m_prevMousePos.y = y;
		}

		return true;
	}

	//Keyboard handler Implementation //

	// Movement controller implementation //
	virtual float VGetTargetYaw() const	override { return m_r32TargetYaw; }
	virtual float VGetTargetPitch() const override { return m_r32TargetPitch; }
	virtual float VGetTargetRoll() const override { return m_r32TargetRoll; }
	virtual float VGetTargetYawRot() const override { return m_r32TargetYawRot; }
	virtual float VGetTargetPitchRot() const override { return m_r32TargetPitchRot; }
	virtual float VGetTargetRollRot() const override { return m_r32TargetRollRot; }
	virtual float VGetTargetSpeed() const override { return m_r32TargetSpeed; }
	virtual Vec VGetTargetVelocity() const override { return m_vTargetVelocity; }

	virtual void VSetTargetDirection(const Vec & vDir) override { m_vTargetDirection = vDir; }

	virtual bool VWantToJump() override { return false; }

	virtual bool VOnKeyDown(Key key) override 
	{
		m_bKey[key] = true;

		switch (key)
		{
			case 'W':
				m_vTargetVelocity = m_vTargetDirection * m_r32TargetSpeed;
				break;
			case 'S':
				m_vTargetVelocity = -m_vTargetDirection * m_r32TargetSpeed;
				break;
			case 'A':
			{
				Mat4x4 rot;
				rot.CreateRotationY(-PI / 2.0);
				Vec left = m_vTargetDirection * rot;
				m_vTargetVelocity = left * m_r32TargetSpeed;
				break;
			}
			case 'D':
			{
				Mat4x4 rot;
				rot.CreateRotationY(PI / 2.0);
				Vec right = m_vTargetDirection * rot;
				m_vTargetVelocity = right * m_r32TargetSpeed;
				break;
			}
			case 'Q':
			{
				Vec up = Vec(0.0f, 1.0f, 0.0f, 0.0f);
				m_vTargetVelocity = up * m_r32TargetSpeed;
				break;
			}
			case 'E':
			{
				Vec down = Vec(0.0f, -1.0f, 0.0f, 0.0f);
				m_vTargetVelocity = down * m_r32TargetSpeed;
				break;
			}
		}

		return true;
	}

	virtual void VUpdate(int deltaMs) override
	{
		if (!m_bKey['W'] && !m_bKey['S'] && !m_bKey['A'] && !m_bKey['D'] && !m_bKey['Q'] && !m_bKey['E'])
		{
			m_vTargetVelocity = Vec{ 0.0f, 0.0f, 0.0f, 0.0f };
		}
	}
};

__forceinline FreeCameraController::FreeCameraController()
	: m_r32TargetYaw{ 0.0f }, m_r32TargetPitch{ 0.0f }, m_r32TargetRoll{ 0.0f },
	m_r32TargetYawRot{ 0.0f }, m_r32TargetPitchRot{ 0.0f }, m_r32TargetRollRot{ 0.0f },
	m_r32TargetSpeed{ 15.0f },
	m_vTargetDirection{ 0.0f, 0.0f, 1.0f, 0.0f }, m_vTargetVelocity{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}