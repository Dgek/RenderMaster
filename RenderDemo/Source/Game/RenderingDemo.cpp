#include "RenderDemo_pch.h"

#include "RenderingDemo.h"

//#include "App.h"

#include "../Game/Views.h"

#include "../Math/Geometry/Frustum.h"
#include "../Renderer/Cameras/Camera.h"
#include "../Renderer/Renderer.h"

#include "../Input/Controllers.h"

bool RenderingDemo::VInitialize()
{
	/*** Initialize player view ***/
	auto pView = static_pointer_cast<PlayerView>(VAddGameView(static_pointer_cast<IView>(make_shared<PlayerView>())));
	AddPlayerView(pView);

	Frustum frustum{};
	frustum.Init(0.25f * PI, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 1.0f, VIEW_DISTANCE);

	Viewport viewport{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, VIEW_DISTANCE };

	auto pCamera = make_shared<Camera>(frustum, Vec{ 0.0f, 500.0f, 0.0f, 1.0f },
		Vec{ 0.0f, 0.0f, 1.0f, 0.0f }, Vec{ 0.0f, 1.0f, 0.0f, 0.0f }, viewport);
	pView->VSetCamera(pCamera);

	auto m_pFreeCameraController = make_shared<FreeCameraController>(FreeCameraController());
//	auto m_pPlayerController = make_shared<PlayerWASDController>(PlayerWASDController());

	pView->SetMouseHandler(m_pFreeCameraController);
	pView->SetKeyboardHandler(m_pFreeCameraController);
	pView->SetController(m_pFreeCameraController);

	return Game::VInitialize();
}

LRESULT	RenderingDemo::VMsgProc(const SystemMessage * msg)
{
	if (msg->m_type == SMT_KeyDown)
	{
		const auto * pMsg = static_cast<const KeyDownMessage *>(msg);
		switch (pMsg->m_key)
		{
		case 'I':
			Global::g_pEngine->GetRenderer()->SetOnlyDirect();
			break;
		case 'O':
			Global::g_pEngine->GetRenderer()->SetOnlyGlobal();
			break;
		case 'P':
			Global::g_pEngine->GetRenderer()->SetCombined();
			break;
		}
	}

	return Game::VMsgProc(msg);
}