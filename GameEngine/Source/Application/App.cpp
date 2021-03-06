#include "Core.h"

#include "App.h"

#include "../Game/Game.h"
#include "../Game/Views.h"
#include "../Physics/PhysicsInterfaces.h"
#include "../Graphics/General.h"
#include "../Renderer/Renderer.h"
#include "../ResourceManager/ResourceCache.h"
#include "../Messenger/Messenger.h"

unique_ptr<App> Global::g_pEngine = make_unique<App>();

App::App()
: m_lastUpdate{ 0 }, m_timeStep{ 0.0333 }, m_bQuitting{ false }, m_pTimer{ make_unique<Timer>() }
{
	m_windowClassName = L"window";
}

App::~App()
{}

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
		L"RenderTest",
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

	//try to initialize it
	if (!m_pGame->VInitialize())
	{
		assert(0 && "Error initializing the game!");
	}
}

void App::InitializeComponents(unique_ptr<Renderer> pRenderer, unique_ptr<IPhysics> pPhysics, unique_ptr<IMessenger> pMessenger, shared_ptr<ResourceCache> pCache)
{
	m_pRenderer = move(pRenderer);
	m_pRenderer->VInitialize(m_hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

	for (auto iter = begin(m_pGame->m_views); iter != end(m_pGame->m_views); ++iter)
	{
		if ((*iter)->VHasCamera())
		{
			m_pRenderer->VAddCamera((*iter)->VGetCamera());
		}
	}

	m_pPhysics = move(pPhysics);
	m_pCache = pCache;
}

LRESULT App::MsgProc(const SystemMessage * msg)
{
	switch (msg->m_type)
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

	return 0;
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
	m_pGame->VRender(m_pRenderer.get(), totaltime, elapsedtime);
	m_pRenderer->VRender();

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
				m_messageQueue.push(new KeyDownMessage{ static_cast<char>(msg.wParam) });
				break;
			case WM_KEYUP:
				m_messageQueue.push(new KeyUpMessage{ static_cast<char>(msg.wParam) });
				break;
			case WM_RBUTTONDOWN:
				m_messageQueue.push(new RMouseDownMessage{ static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)) });
				break;
			case WM_RBUTTONUP:
				m_messageQueue.push(new RMouseUpMessage{ static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)) });
				break;
			case WM_LBUTTONDOWN:
				m_messageQueue.push(new LMouseDownMessage{ static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)) });
				break;
			case WM_LBUTTONUP:
				m_messageQueue.push(new LMouseUpMessage{ static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)) });
				break;
			case WM_MOUSEMOVE:
				m_messageQueue.push(new MouseMoveMessage{ static_cast<float>(GET_X_LPARAM(msg.lParam)), static_cast<float>(GET_Y_LPARAM(msg.lParam)) });
				break;
			};

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			while (m_messageQueue.size())
			{
				const SystemMessage * msg = m_messageQueue.front();
				MsgProc(msg);

				m_messageQueue.pop();
				delete msg;
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
	while (!m_messageQueue.empty())
	{
		SystemMessage * pMessage = m_messageQueue.back();
		m_messageQueue.pop();

		delete pMessage;
	}
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