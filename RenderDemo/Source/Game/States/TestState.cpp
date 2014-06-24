#include "RenderDemo_pch.h"

#include "TestState.h"

#include "Game\Game.h"
#include "Game\Entities\EntityResource.h"

void TestState::VInitialize(Game * pGame, UINT_PTR uptrData)
{
	auto & projector_resource = EntityResource("SunLight.xml");
	shared_ptr<Entity> pProjector = pGame->VAddEntity(projector_resource.VCreateEntity(pGame));
	projector_resource.VCreateRepresentation(m_pScene.get(), pProjector);
	projector_resource.VCreatePhysicalBody(Global::g_pEngine->GetPhysics(), pProjector);
	
	auto & sponza_resource = EntityResource("Sponza.xml");
	shared_ptr<Entity> pSponza = pGame->VAddEntity(sponza_resource.VCreateEntity(pGame));
	sponza_resource.VCreateRepresentation(m_pScene.get(), pSponza);
	sponza_resource.VCreatePhysicalBody(Global::g_pEngine->GetPhysics(), pSponza);

	auto & point_light_resource = EntityResource("Light.xml");
	
	//for (int i = 0; i < 10; ++i)
	//	for (int j = 0; j < 10; ++j)
		{
			//point_light_resource.m_vOffset = Vec{ 60.0f * i, i*10.0f+j*17.0f, -50.0f * j, 0.0f };
			shared_ptr<Entity> pPointLight = pGame->VAddEntity(point_light_resource.VCreateEntity(pGame));
			point_light_resource.VCreateRepresentation(m_pScene.get(), pPointLight);

			//1250, 720, 450
		//	point_light_resource.m_vOffset = Vec{ 1700, 710, -50.0f, 0.0f };
			//pPointLight = pGame->VAddEntity(point_light_resource.VCreateEntity(pGame));
			//point_light_resource.VCreateRepresentation(m_pScene.get(), pPointLight);
		} 
}

void TestState::VUpdate(Game * pGame, double time, double elapsedTime)
{
	m_pScene->VUpdate(time, elapsedTime);
}

void TestState::VRender(Game * pGame, Renderer * pRenderer, double r64Time, double r64ElapsedTime)
{
	m_pScene->VRender(pRenderer, r64Time);
}