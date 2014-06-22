#include "Core.h"

#include "Views.h"

#include "../Application/System.h"

#include "../Game/Widget.h"
#include "../Renderer/Cameras/Camera.h"

#include "../Input/Controllers.h"
#include "../Input/Devices.h"

bool PlayerView::VInit()
{
	bgColor[0] = 1.0f;
	bgColor[1] = 1.0f;
	bgColor[2] = 1.0f;
	bgColor[3] = 1.0f;

	return true;
}

void PlayerView::AddWidget(shared_ptr<Widget> pWidget)
{
	m_widgets.push_back(pWidget);
}

bool PlayerView::RemoveWidget(shared_ptr<Widget> pWidget)
{
	for (auto it = begin(m_widgets); it != end(m_widgets); ++it)
	{
		if ((*it).get() == pWidget.get())
		{
			m_widgets.erase(it);
			return true;
		}
	}

	return false;
}

bool PlayerView::VRestore()
{
	return true;
}

void PlayerView::VUpdate(unsigned int deltaMilliseconds)
{
	//Update widgets
	for (auto it = begin(m_widgets); it != end(m_widgets); ++it)
	{
		(*it)->VUpdate(deltaMilliseconds);
	}

	if (!m_pController)
		return;

	//update controller
	m_pController->VUpdate(deltaMilliseconds);

	//Update only if we have camera
	if (m_bHasCamera)
	{
		if (!m_pCamera->HasTarget())
		{
			//Update camera's position
			auto vel = m_pController->VGetTargetVelocity();
			m_pCamera->SetPosition(m_pCamera->GetPosition() + vel);

			//Update camera's orientation
			auto yaw = m_pController->VGetTargetYaw();
			m_pCamera->SetYaw(yaw);

			auto roll = m_pController->VGetTargetRoll();
			m_pCamera->SetRoll(roll);

			auto pitch = m_pController->VGetTargetPitch();
			m_pCamera->SetPitch(pitch);

			m_pCamera->VUpdate(deltaMilliseconds);

			m_pController->VSetTargetDirection(m_pCamera->GetDir());
		}
		else
		{
			/*Entity* pTarget = m_pCamera->GetTarget();
			Vec offset = Vector(0.0, 3.0f, -6.5f, 0.0f);
			Mat4x4 rot;
			rot.CreateRotationY(m_pController->VGetTargetYaw());
			offset = offset * rot;
			m_pCamera->SetPosition(pTarget->GetCurrentTransform().GetPosition() + offset); */
			m_pCamera->SetYaw(m_pController->VGetTargetYaw());
			m_pCamera->VUpdate(deltaMilliseconds);
		}
	}
}

void PlayerView::VRender(Renderer* pRenderer, double time, double elapsedTime)
{
	float bg[] = { 0.15f, 0.15f, 0.7f, 1.0f };
	DX11API::ClearRenderTargetView(bg, nullptr);
	DX11API::ClearDepthStencilView(true, false, 1.0f, 1, nullptr);

	//render widgets
	for (auto it = begin(m_widgets); it != end(m_widgets); ++it)
	{
		(*it)->VRender(pRenderer, time, elapsedTime);
	}
}

bool PlayerView::VMsgProc(const SystemMessage * msg)
{
	//Use reverse_iterator as widgets process messages
	//from top of screen to bottom
	for (auto it = rbegin(m_widgets); it != rend(m_widgets); ++it)
	{
		if ((*it)->VMsgProc(msg))
			return true;
	}

	//no widget processed a message, so give controllers a try
	auto res = false;
	switch (msg->m_type)
	{

	case SMT_KeyDown:
		if (m_pKeyboardHandler)
		{
			const auto * pMsg = static_cast<const KeyDownMessage *>(msg);
			res = m_pKeyboardHandler->VOnKeyDown(pMsg->m_key);
		}
		break;

	case SMT_KeyUp:
		if (m_pKeyboardHandler)
		{
			const auto * pMsg = static_cast<const KeyUpMessage *>(msg);
			res = m_pKeyboardHandler->VOnKeyUp(pMsg->m_key);
		}
		break;

	case SMT_MouseMove:
		if (m_pMouseHandler)
		{
			const auto * pMsg = static_cast<const MouseMoveMessage *>(msg);
			res = m_pMouseHandler->VOnMouseMove(pMsg->m_posx, pMsg->m_posy);
		}
		break;

	case SMT_LMouseDown:
		if (m_pMouseHandler)
		{
			const auto * pMsg = static_cast<const LMouseDownMessage *>(msg);
			res = m_pMouseHandler->VOnLButtonDown(pMsg->m_posx, pMsg->m_posy);
		}
		break;

	case SMT_LMouseUp:
		if (m_pMouseHandler)
		{
			const auto * pMsg = static_cast<const LMouseUpMessage *>(msg);
			res = m_pMouseHandler->VOnLButtonUp(pMsg->m_posx, pMsg->m_posy);
		}
		break;

	case SMT_RMouseDown:
		if (m_pMouseHandler)
		{
			const auto * pMsg = static_cast<const RMouseDownMessage *>(msg);
			res = m_pMouseHandler->VOnRButtonDown(pMsg->m_posx, pMsg->m_posy);
		}
		break;

	case SMT_RMouseUp:
		if (m_pMouseHandler)
		{
			const auto * pMsg = static_cast<const RMouseUpMessage *>(msg);
			res = m_pMouseHandler->VOnRButtonUp(pMsg->m_posx, pMsg->m_posy);
		}
		break;
	};

	return res;
}