#include "Core.h"

#include "App.h"

#include "Timer.h"
#include "../Game/IGame.h"
#include "../Physics/PhysicsInterfaces.h"
#include "../Graphics/General.h"

App::App()
: m_bQuitting{ false }, m_pTimer{ make_unique<Timer>() }
{
	//m_pTimer = make_shared<Timer>();
}

void App::Update(double totaltime, double elapsedtime)
{
	//quitting state
	if (m_bQuitting)
	{
		PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return;
	}

	if (m_pPhysics)
		m_pPhysics->VUpdate(elapsedtime);

	if (m_pGame)
		m_pGame->VUpdate(totaltime, elapsedtime);
}

void App::Render(double totaltime, double elapsedtime)
{
	DX11API::PresentBackBuffer();
}