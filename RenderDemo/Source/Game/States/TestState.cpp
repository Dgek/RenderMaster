#include "RenderDemo_pch.h"

#include "TestState.h"

void TestState::VInitialize(Game * pGame, UINT_PTR uptrData)
{
}

void TestState::VUpdate(Game * pGame, double time, double elapsedTime)
{
	m_pScene->VUpdate(time, elapsedTime);
}

void TestState::VRender(Game * pGame, Renderer * pRenderer, double r64Time, double r64ElapsedTime)
{
	m_pScene->VRender(pRenderer, r64Time);
}