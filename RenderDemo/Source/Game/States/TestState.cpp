#include "RenderDemo_pch.h"

#include "TestState.h"

#include "Game\Game.h"
#include "Game\Entities\EntityResource.h"

void TestState::VInitialize(Game * pGame, UINT_PTR uptrData)
{
	EntityResource & projector_resource = EntityResource("SunLight.xml");
	shared_ptr<Entity> pProjector = pGame->VAddEntity(projector_resource.VCreateEntity(pGame));
	projector_resource.VCreateRepresentation(m_pScene.get(), pProjector);
	projector_resource.VCreatePhysicalBody(Global::g_pEngine->GetPhysics(), pProjector);

	EntityResource & sponza_resource = EntityResource("Sponza.xml");
	shared_ptr<Entity> pSponza = pGame->VAddEntity(sponza_resource.VCreateEntity(pGame));
	sponza_resource.VCreateRepresentation(m_pScene.get(), pSponza);
	sponza_resource.VCreatePhysicalBody(Global::g_pEngine->GetPhysics(), pSponza);
}

void TestState::VUpdate(Game * pGame, double time, double elapsedTime)
{
	m_pScene->VUpdate(time, elapsedTime);
}

void TestState::VRender(Game * pGame, Renderer * pRenderer, double r64Time, double r64ElapsedTime)
{
	m_pScene->VRender(pRenderer, r64Time);
}