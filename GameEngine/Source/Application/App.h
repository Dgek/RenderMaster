#pragma once

#include "System.h"
#include "Timer.h"

using namespace std;

class Game;
class IPhysics;
class IMessenger;
class Renderer;
class ResourceCache;
class App
{
public:
	unique_ptr<Game> m_pGame;
	unique_ptr<Timer> m_pTimer;

	unique_ptr<Renderer> m_pRenderer;
	unique_ptr<IPhysics> m_pPhysics;
	unique_ptr<ResourceCache> m_pCache;

private:

	/** 
	Window data
	*/
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCTSTR m_windowClassName;
	bool m_bWindowed;
	int m_width;
	int m_height;

	/* Timing */
	double m_lastUpdate;
	double m_timeStep;

	/** 
	Other data
	*/
	bool m_bQuitting;

	queue<SystemMessage> m_messageQueue;

public:
	App();

	/*============================================
	//Engine Initialization
	=============================================*/
	void InitializeComponents(unique_ptr<Renderer> pRenderer, unique_ptr<IPhysics> pPhysics, unique_ptr<IMessenger> pMessenger, unique_ptr<ResourceCache> pCache);

	void InitializeWindow(HINSTANCE hInstance, int showWnd, bool isWindowed);
	void InitializeGame(unique_ptr<Game> pGame);

	LRESULT MsgProc(const SystemMessage & msg);

	void Update(double totaltime, double elapsedtime);
	void Render(double totaltime, double elapsedtime);

	/*============================================
	//Application main loop
	=============================================*/
	int Run();

	/*============================================
	//Application Shut Down
	=============================================*/
	void Close();

	__forceinline double GetTimeStep() const;
	__forceinline double GetGameTime() const;

	static LRESULT CALLBACK WndProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);
};

__forceinline double App::GetTimeStep() const
{
	return 0.0;
}

__forceinline double App::GetGameTime() const
{
	return 0.0f;
}