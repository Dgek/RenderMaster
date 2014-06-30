#include "RenderDemo_pch.h"

#include "TestState.h"

#include "Game\Game.h"
#include "Game\Entities\EntityResource.h"
#include "Game\Entities\Entity.h"

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
	pSponza->SetCurrentTransform(Mat4x4::CreateScaling(0.5f, 0.5f, 0.5f), 0.0);
	/*
	auto & point_light_resource = EntityResource("Light.xml");
	
	for (int i = 0; i < 1; ++i)
		for (int j = 0; j < 1; ++j)
		{
			point_light_resource.m_vOffset = Vec{ 60.0f * i, 0.0f, 0.0f, 0.0f };
			shared_ptr<Entity> pPointLight = pGame->VAddEntity(point_light_resource.VCreateEntity(pGame));
			point_light_resource.VCreateRepresentation(m_pScene.get(), pPointLight);

			for (int k = 0; k < 12; ++k)
			{
				point_light_resource.m_vOffset += Vec{ 30.0f * (k + 1), 0.0f, 0.0f, 0.0f };
				pPointLight = pGame->VAddEntity(point_light_resource.VCreateEntity(pGame));
				point_light_resource.VCreateRepresentation(m_pScene.get(), pPointLight);
			}

			//1250, 720, 450
			point_light_resource.m_vOffset = Vec{ 1700, 710, -50.0f, 0.0f };
			pPointLight = pGame->VAddEntity(point_light_resource.VCreateEntity(pGame));
			point_light_resource.VCreateRepresentation(m_pScene.get(), pPointLight); 

			for (int k = 0; k < 9; ++k)
			{
				point_light_resource.m_vOffset += Vec{ -50.0f * (k + 1), 0.0f, 0.0f, 0.0f };
				pPointLight = pGame->VAddEntity(point_light_resource.VCreateEntity(pGame));
				point_light_resource.VCreateRepresentation(m_pScene.get(), pPointLight);
			}
	}

	auto & point_light_resource_test = EntityResource{ "TestLight.xml" };
	shared_ptr<Entity> pPointLight = pGame->VAddEntity(point_light_resource_test.VCreateEntity(pGame));
	point_light_resource_test.VCreateRepresentation(m_pScene.get(), pPointLight);

	for (int i = 0; i < 5; ++i)
	{
		point_light_resource_test.m_vOffset += Vec{ 200.0f * (i + 1), 0.0f, 0.0f, 0.0f };
		pPointLight = pGame->VAddEntity(point_light_resource_test.VCreateEntity(pGame));
		point_light_resource_test.VCreateRepresentation(m_pScene.get(), pPointLight);
	}

	point_light_resource_test.m_vOffset = Vec{ 0.0f, 500.0f, 0.0f, 0.0f };
	for (int i = 0; i < 5; ++i)
	{
		point_light_resource_test.m_vOffset += Vec{ 200.0f * (i + 1), 0.0f, 0.0f, 0.0f };
		pPointLight = pGame->VAddEntity(point_light_resource_test.VCreateEntity(pGame));
		point_light_resource_test.VCreateRepresentation(m_pScene.get(), pPointLight);
	}

	point_light_resource_test.m_vOffset = Vec{ 0.0f, -350.0f, 0.0f, 0.0f };
	for (int i = 0; i < 5; ++i)
	{
		point_light_resource_test.m_vOffset += Vec{ 200.0f * (i + 1), 0.0f, 0.0f, 0.0f };
		pPointLight = pGame->VAddEntity(point_light_resource_test.VCreateEntity(pGame));
		point_light_resource_test.VCreateRepresentation(m_pScene.get(), pPointLight);
	}

	auto & point_light_resource_central = EntityResource{ "CentralLight.xml" };
	shared_ptr<Entity> pCentralLight = pGame->VAddEntity(point_light_resource_central.VCreateEntity(pGame));
	point_light_resource_central.VCreateRepresentation(m_pScene.get(), pCentralLight);

	for (int i = 40; i < 256; ++i)
	{
		float xOffset = Math::random(-700, 700, i*110 - 45);
		float yOffset = Math::random(-500, 500, i*44 + 675);
		float zOffset = Math::random(-500, 500, i*2-145);

		float red, green, blue;
		float value = Math::random(0, 500, 111 * i*i + 1);
		red = (value < 250) ? 1.0f : 0.0f;

		value = Math::random(0, 500, 11 *i + 1);
		green = (value < 250) ? 1.0f : 0.0f;

		value = Math::random(0, 500, 98 * i*i*i + 1);
		blue = (value < 250) ? 1.0f : 0.0f;

		if (red == 0.0f && green == 0.0f && blue == 0.0f)
		{
			red = green = blue = 1.0f;
		}

		point_light_resource_central.ChangeColor(red, green, blue);
		point_light_resource_central.m_vOffset = Vec{ xOffset, yOffset, zOffset, 0.0f };
		pCentralLight = pGame->VAddEntity(point_light_resource_central.VCreateEntity(pGame));
		point_light_resource_central.VCreateRepresentation(m_pScene.get(), pCentralLight);
	
		m_lights.push_back(pCentralLight);
	} */
}

void TestState::VUpdate(Game * pGame, double time, double elapsedTime)
{
	for (auto light = begin(m_lights); light != end(m_lights); ++light)
	{
		float x = 7.0f * Math::Sin(time);
		float y = 7.0f * Math::Cos(time);

		Mat4x4 trans = Mat4x4::CreateTranslation(Vec{ x, y, 0.0f, 1.0f });

		(*light)->SetCurrentTransform((*light)->GetCurrentTransform() * trans, time);
	}
	m_pScene->VUpdate(time, elapsedTime);
}

void TestState::VRender(Game * pGame, Renderer * pRenderer, double r64Time, double r64ElapsedTime)
{
	m_pScene->VRender(pRenderer, r64Time);
}