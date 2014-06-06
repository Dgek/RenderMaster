#include "Core.h"

#include "Renderer.h"

#include "../Graphics/General.h"

Renderer::Renderer()
{
}

bool Renderer::VInitialize(HWND hWnd, unsigned int width, unsigned int height)
{
	DX11API::InitializeGraphics(hWnd);

	return true;
}

void Renderer::VAddCamera(shared_ptr<Camera> pCamera)
{
	m_cameras.push_back(pCamera);
}

void Renderer::VRemoveCamera(Camera* pCamera)
{
	auto camera = begin(m_cameras);
	for (; camera != end(m_cameras); ++camera)
		if ((*camera).get() == pCamera)
			break;

	if (camera != end(m_cameras))
		m_cameras.erase(camera);

	assert(0 && "No such camera was found!");
}