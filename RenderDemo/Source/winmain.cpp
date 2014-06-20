#include "RenderDemo_pch.h"

#include "App.h"

#include "Messenger\Messenger.h"
#include "Physics\NullPhysics.h"
#include "ResourceManager\ResourceCache.h"
#include "ResourceManager\ResourceLoader.h"
#include "ResourceManager\Loaders\MeshLoader.h"
#include "ResourceManager\Loaders\MaterialLoader.h"
#include "ResourceManager\Loaders\TextureLoader.h"
#include "ResourceManager\Loaders\ShaderLoader.h"
#include "ResourceManager\Windows\ResourceDirectory.h"
#include "Renderer\TiledRenderer.h"

#include "Game\RenderingDemo.h"
#include "Game\DemoStateFactory.h"

//unique_ptr<App> g_pEngine = make_unique<App>();

////////////////////////////////////
//Entry point for the application
////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	Global::g_pEngine->InitializeWindow(hInstance, nShowCmd, true);

	/* =========================
	Initialize Game Logic
	========================= */
	unique_ptr<RenderingDemo> g_Game = make_unique<RenderingDemo>();

	//Manage states factory and set initial state
	g_Game->SetStateFactory(make_unique<DemoStateFactory>());
	g_Game->SetNextState(GS_BOXTEST);

	/* === Initialize Game === */
	Global::g_pEngine->InitializeGame(move(g_Game));

	/* ==============================
	Initialize Engine Components
	============================== */
	unique_ptr<ResourceDir> g_pFile = make_unique<ResourceDir>(L"Resources");
	shared_ptr<ResourceCache> g_pCache = make_shared<ResourceCache>(300, move(g_pFile));
	//unique_ptr<ResourceCache> g_pCache = make_unique<ResourceCache>(300);
	g_pCache->Init();

	//shared_ptr<IResourceLoader> pMeshLoader = make_shared<MeshLoader>();
	g_pCache->RegisterLoader(make_shared<MaterialLoader>());
	g_pCache->RegisterLoader(make_shared<MeshLoader>());
	g_pCache->RegisterLoader(make_shared<TextureLoader>());
	g_pCache->RegisterLoader(make_shared<VertexShaderLoader>());
	g_pCache->RegisterLoader(make_shared<PixelShaderLoader>());
	g_pCache->RegisterLoader(make_shared<ComputeShaderLoader>());

	/* === Initialize main engine components === */
	Global::g_pEngine->InitializeComponents(make_unique<TiledRenderer>(), make_unique<NullPhysics>(),
		make_unique<Messenger>(), g_pCache);

	// **************************** //
	// ===						=== //
	// ===		Game Loop		=== //
	// ===						=== //
	// **************************** //
	auto g_iExit = Global::g_pEngine->Run();

	return 0;
}