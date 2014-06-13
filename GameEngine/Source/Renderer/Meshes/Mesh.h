#pragma once

#include "../Objects/IMovable.h"

#include "../../Graphics/Resources/Buffers/VertexBuffer.h"
#include "../../Graphics/Resources/Buffers/IndexBuffer.h"
#include "../Materials/Material.h"

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
	__forceinline void SetPositionBuffer();
	__forceinline void SetTexCoordsBuffer();
	__forceinline void SetNormalsBuffer();

	virtual void SetWorldTransform(const Mat4x4 & transform) override;
};

__forceinline void Mesh::SetMaterial(const string & materialName)
{
	m_pMaterial->VBind(0);
}

__forceinline void Mesh::SetPositionBuffer()
{
	m_pVertices->Bind(0, 0);
}

__forceinline void Mesh::SetTexCoordsBuffer()
{
	m_pTexCoords->Bind(1, 0);
}

__forceinline void Mesh::SetNormalsBuffer()
{
	m_pNormals->Bind(2, 0);
}