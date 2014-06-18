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

	shared_ptr<Material> m_pMaterial;

public:

	__forceinline Mesh();
	virtual ~Mesh();

	__forceinline void SetMaterial(const string & materialName);

	__forceinline void BindMaterial();
	__forceinline void BindPositionBuffer();
	__forceinline void BindTexCoordsBuffer();
	__forceinline void BindNormalsBuffer();

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

__forceinline void Mesh::BindPositionBuffer()
{
	m_pVertices->Bind(0, 0);
}

__forceinline void Mesh::BindTexCoordsBuffer()
{
	m_pTexCoords->Bind(1, 0);
}

__forceinline void Mesh::BindNormalsBuffer()
{
	m_pNormals->Bind(2, 0);
}