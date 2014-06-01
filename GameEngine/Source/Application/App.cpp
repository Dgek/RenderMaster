#include "Core.h"

#include "App.h"

#include "System.h"
#include "Timer.h"
#include "../Game/Game.h"
#include "../Physics/PhysicsInterfaces.h"
#include "../Graphics/General.h"

App::App()
: m_bQuitting{ false }, m_pTimer{ make_unique<Timer>() }
{
	//m_pTimer = make_shared<Timer>();
}

void App::InitializeWindow(HINSTANCE hInstance, int showWnd, bool isWindowed)
{
	//assign variables
	m_hInstance = hInstance;
	m_bWindowed = isWindowed;
	m_width = SCREEN_WIDTH;
	m_height = SCREEN_HEIGHT;

	//define struct with window properties
	typedef struct _WNDCLASS {
		unsigned int cbSize;
		unsigned int style;
		WNDPROC lpfnWndProc;
		unsigned int cbClsExtra;
		int cbWndExtra;
		HANDLE hInstance;
		HICON hIcon;
		HCURSOR hCursor;
		HBRUSH hbrBackground;
		LPCTSTR lpszMenuName;
		LPCTSTR lpszClassName;
	} WNDCLASS;

	//fill the WNDCLASS structure
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = App::WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_windowClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//register window
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering window class",
			L"Error", MB_OK | MB_ICONERROR);
	}

	HRESULT hr = HRESULT_FROM_WIN32(GetLastError());

	//create window
	m_hWnd = CreateWindowEx(
		NULL,
		m_windowClassName,
		L"TestGame",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_width, m_height,
		NULL,
		NULL,
		m_hInstance,
		NULL
		);

	//check wether window was created correctly
	if (!m_hWnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
	}

	//show and update window
	ShowWindow(m_hWnd, showWnd);
	UpdateWindow(m_hWnd);

	ShowCursor(false);
}

void App::InitializeGame(unique_ptr<Game> pGame)
{
	m_pGame = move(pGame);
}

bool App::MsgProc(const SystemMessage & msg)
{
	switch (msg.m_type)
	{
		case SMT_KeyDown:
		case SMT_KeyUp:
		case SMT_LMouseDown:
		case SMT_LMouseUp:
		case SMT_RMouseDown:
		case SMT_RMouseUp:
		case SMT_MouseMove:
		{
			return m_pGame->VMsgProc(msg);
		}
		break;
	}
};

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

int App::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/*********************************
			Application received quit message!
			*********************************/

			//Generate system message
			switch (msg.message)
			{
			case WM_QUIT:
				//m_messageQueue.push(QuitMessage());
				Close();
				return msg.wParam;
			case WM_KEYDOWN:
				m_messageQueue.push(KeyDownMessage{ static_cast<char>(msg.wParam) });
				break;
			case WM_KEYUP:
				m_messageQueue.push(KeyUpMessage{ static_cast<char>(msg.wParam) });
				break;
			case WM_RBUTTONDOWN:
				m_messageQueue.push(RMouseDownMessage{ static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)) });
				break;
			case WM_RBUTTONUP:
				m_messageQueue.push(RMouseUpMessage{ static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)) });
				break;
			case WM_LBUTTONDOWN:
				m_messageQueue.push(LMouseDownMessage{ static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)) });
				break;
			case WM_LBUTTONUP:
				m_messageQueue.push(LMouseUpMessage{ static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)) });
				break;
			case WM_MOUSEMOVE:
				m_messageQueue.push(MouseMoveMessage{ static_cast<float>(GET_X_LPARAM(msg.lParam)), static_cast<float>(GET_Y_LPARAM(msg.lParam)) });
				break;
			};

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			while (m_messageQueue.size())
			{
				SystemMessage msg = m_messageQueue.front();
				MsgProc(msg);

				m_messageQueue.pop();
			};
		}
		else
		{
			/*
			Update Timer
			*/
			m_pTimer->Tick();

			/* ==
			Update with fixed framerate
			== */
			double gameTime = m_pTimer->GetGameTimeSeconds();
			//if ( (r64GameTime - m_r64LastUpdate) > 1.0f) m_r64LastUpdate = r64GameTime;
			//while (r64GameTime - m_r64LastUpdate > m_r64TimeStep)
			if ((gameTime - m_lastUpdate) > m_timeStep)
			{
				Update(m_lastUpdate, m_timeStep);
				//m_r64LastUpdate += m_r64TimeStep;
				m_lastUpdate = gameTime;

				m_pPhysics->VSyncData(m_lastUpdate);
			}

			/* ==
			Render
			== */
			Render(gameTime, m_pTimer->GetDeltaTime());
		}
	}

	return msg.wParam;
}

void App::Close()
{
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK App::WndProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE){
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,
		msg,
		wParam,
		lParam);
}