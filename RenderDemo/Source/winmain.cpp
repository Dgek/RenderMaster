#include "RenderDemo_pch.h"

#include "App.h"

#include "Messenger\Messenger.h"
#include "Physics\NullPhysics.h"
#include "ResourceManager\ResourceCache.h"
#include "ResourceManager\ResourceLoader.h"
#include "ResourceManager\Loaders\MeshLoader.h"
#include "ResourceManager\Windows\ResourceDirectory.h"
#include "Renderer\TiledRenderer.h"

#include "Game\RenderingDemo.h"
#include "Game\DemoStateFactory.h"

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
	unique_ptr<RenderingDemo> g_Game = make_unique<RenderingDemo>();

	//Manage states factory and set initial state
	g_Game->SetStateFactory(make_unique<DemoStateFactory>());
	g_Game->SetNextState(GS_BOXTEST);

	/* === Initialize Game === */
	g_pEngine->InitializeGame(move(g_Game));

	/* ==============================
	Initialize Engine Components
	============================== */
	unique_ptr<ResourceDir> g_pFile = make_unique<ResourceDir>(L"Resources");
	unique_ptr<ResourceCache> g_pCache = make_unique<ResourceCache>(300, move(g_pFile));
	//unique_ptr<ResourceCache> g_pCache = make_unique<ResourceCache>(300);
	g_pCache->Init();

	//shared_ptr<IResourceLoader> pMeshLoader = make_shared<MeshLoader>();
	//g_pCache->RegisterLoader(pMeshLoader);

	/* === Initialize main engine components === */
	g_pEngine->InitializeComponents(make_unique<TiledRenderer>(), make_unique<NullPhysics>(),
		make_unique<Messenger>(), move(g_pCache));

	// **************************** //
	// ===						=== //
	// ===		Game Loop		=== //
	// ===						=== //
	// **************************** //
	auto g_iExit = g_pEngine->Run();

	return g_iExit;
}