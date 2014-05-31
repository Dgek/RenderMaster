#include "Core.h"

#include "Scene.h"

void Scene::VRender(Renderer* pRenderer, double currentTime)
{
	//TODOfor_each(auto object : m_staticObjects)
	for (auto iter = begin(m_staticObjects); iter != end(m_staticObjects); ++iter)
	{
		(*iter)->VRender(this, pRenderer, currentTime);
	}

	m_pRoot->VRender(this, pRenderer, currentTime);
}