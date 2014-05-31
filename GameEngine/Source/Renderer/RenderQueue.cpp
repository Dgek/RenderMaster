#include "Core.h"

#include "RenderQueue.h"

shared_ptr<Mesh> RenderQueue::Next()
{
	if (m_currMesh == m_queue.end())
		return nullptr;

	shared_ptr<Mesh> pMesh = (*m_currMesh);
	m_currMesh++;

	return pMesh;
}

shared_ptr<Mesh> RenderQueue::NextWithDeletion()
{
	//we just rendered all meshes in the queue
	//if (m_cur == (m_numMeshes - 1))	return NULL;
	if (m_queue.empty())
		return nullptr;

	//return m_queue[m_cur++];
	shared_ptr<Mesh> pMesh = m_queue.back();
	m_queue.pop_back();

	return pMesh;
}