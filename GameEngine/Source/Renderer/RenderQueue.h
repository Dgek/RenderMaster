#pragma once

class Mesh;
class RenderQueue
{
private:

	typedef vector<shared_ptr<Mesh>> MeshList;

	/* ==============================================
	Queue itself:
	//contains pointers to the meshes
	================================================= */
	MeshList m_queue;
	MeshList::const_iterator m_currMesh;

	//********
	//Help variables
	//********
	unsigned int m_cur;
	unsigned int m_numMeshes;

public:

	__forceinline RenderQueue();

	/**	==========================
	Methods
	==========================	**/

	//Add new mesh to the queue
	virtual void VAdd(shared_ptr<Mesh> pMesh);

	//Sort all meshes in effective order
	virtual void VSort();

	//get the next mesh to render
	shared_ptr<Mesh> Next();
	shared_ptr<Mesh> NextWithDeletion();

	void Reset();

	void Clear();
};

__forceinline RenderQueue::RenderQueue()
	: m_numMeshes{ 0 }, m_cur{ 0 }
{}

__forceinline void RenderQueue::VAdd(shared_ptr<Mesh> pMesh)
{
	//push mesh pointer to the queue
	m_queue.push_back(pMesh);

	//increment counter
	m_numMeshes++;
}

__forceinline void RenderQueue::VSort()
{}

__forceinline void RenderQueue::Reset()
{
	m_currMesh = begin(m_queue);
}

__forceinline void RenderQueue::Clear()
{
	while (!m_queue.empty())
	{
		m_queue.pop_back();
	}
}