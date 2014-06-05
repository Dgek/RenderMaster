#include "RenderDemo_pch.h"

#include "App.h"
#include "ResourceManager\ResourceCache.h"

////////////////////////////////////
//Entry point for the application
////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	auto g_pEngine = make_unique<App>();

	g_pEngine->InitializeWindow(hInstance, nShowCmd, true);

	/* =========================
	Initialize Game Logic
	========================= */
	RenderingDemo g_Game;

	//Manage states factory and set initial state
	g_Game.SetStateFactory(new DemoStateFactory());
	g_Game.SetNextState(GS_BOXTEST);

	/* === Initialize Game === */
	g_pEngine->InitializeGame(&g_Game);

	/* ==============================
	Initialize Engine Components
	============================== */
	unique_ptr<TiledRenderer> g_pRenderer = make_unique<TiledRenderer>();
	unique_ptr<ResourceDir> g_pFile = make_unique<ResourceDir>(L"Resources");
	unique_ptr<ResourceCache> g_pCache = make_unique<ResourceCache>(300, g_pFile);
	g_pCache->Init();

	shared_ptr<IResourceLoader> pMeshLoader = make_shared<MeshLoader>();
	g_pCache->RegisterLoader(pMeshLoader);

	/* === Initialize main engine components === */
	g_pEngine->InitializeComponents();

	// **************************** //
	// ===						=== //
	// ===		Game Loop		=== //
	// ===						=== //
	// **************************** //
	g_iExit = g_pEngine->Run();

	return g_iExit;
}