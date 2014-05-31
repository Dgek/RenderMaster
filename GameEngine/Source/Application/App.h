#pragma once

#include <memory>
#include <Windows.h>

using namespace std;

class IGame;
class IPhysics;
class IMessenger;
class Renderer;
class ResourceCache;
class Timer;
class App
{
public:
	unique_ptr<IGame> m_pGame;
	unique_ptr<Timer> m_pTimer;

	unique_ptr<IPhysics> m_pPhysics;

private:

	/** 
	Window data
	*/
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	bool m_bWindowed;
	int m_width;
	int m_height;

	/** 
	Other data
	*/
	bool m_bQuitting;

public:
	App();

	/*============================================
	//Engine Initialization
	=============================================*/
	void InitializeComponents(unique_ptr<Renderer> pRenderer, unique_ptr<IPhysics> pPhysics, unique_ptr<IMessenger> pMessenger, unique_ptr<ResourceCache> pCache);

	void InitializeWindow(HINSTANCE hInstance, int showWnd, bool isWindowed);
	void InitializeGame(unique_ptr<IGame> pGame);

	void Update(double totaltime, double elapsedtime);
	void Render(double totaltime, double elapsedtime);

	__forceinline double GetTimeStep() const;
	__forceinline double GetGameTime() const;
};

__forceinline double App::GetTimeStep() const
{
	return 0.0;
}

__forceinline double App::GetGameTime() const
{
	return 0.0f;
}