#pragma once

#include "../Objects/IMovable.h"

#include "../../Graphics/Resources/Buffers/VertexBuffer.h"
#include "../../Graphics/Resources/Buffers/IndexBuffer.h"
#include "../Materials/Material.h"

#include "../../ResourceManager/Resource.h"
#include "../../ResourceManager/ResourceCache.h"

#include "../../ResourceManager/Loaders/MaterialLoader.h"

class Mesh : public IMovable
{
protected:
	Mat4x4 m_objectTransform;

	shared_ptr<VertexBuffer> m_pVertices;
	shared_ptr<VertexBuffer> m_pTexCoords;
	shared_ptr<VertexBuffer> m_pNormals;
	shared_ptr<VertexBuffer> m_pTangents;

	shared_ptr<Material> m_pMaterial;

public:

	Mesh();
	virtual ~Mesh();

	void SetMaterial(const string & materialName);

	void BindMaterial();
	void BindVertices(unsigned int startSlot, unsigned int offset);
	void BindTexCoords(unsigned int startSlot, unsigned int offset);
	void BindNormals(unsigned int startSlot, unsigned int offset);
	void BindTangents(unsigned int startSlot, unsigned int offset);

	virtual void SetWorldTransform(const Mat4x4 & transform) override;
};

__forceinline Mesh::Mesh()
{}

__forceinline void Mesh::SetMaterial(const string & materialName)
{
	Resource materialResource(materialName);
	shared_ptr<ResHandle> pMaterialHandle = ResourceCache::SafeGetHandle(&materialResource);
	std::shared_ptr<MaterialResourceExtraData> pData = static_pointer_cast<MaterialResourceExtraData>(pMaterialHandle->GetExtra());

	m_pMaterial = pData->m_pMaterial;
}

__forceinline void Mesh::BindMaterial()
{
	m_pMaterial->VBind(0);
}

__forceinline void Mesh::BindVertices(unsigned int startSlot, unsigned int offset)
{
	m_pVertices->Bind(startSlot, offset);
}

__forceinline void Mesh::BindTexCoords(unsigned int startSlot, unsigned int offset)
{
	m_pTexCoords->Bind(startSlot, offset);
}

__forceinline void Mesh::BindNormals(unsigned int startSlot, unsigned int offset)
{
	m_pNormals->Bind(startSlot, offset);
}

__forceinline void Mesh::BindTangents(unsigned int startSlot, unsigned int offset)
{
	m_pTangents->Bind(startSlot, offset);
}