#pragma once

#include "../Materials/Material.h"

class SubMesh
{
public:

	shared_ptr<Material> m_pMaterial;
	int m_firstIndex;
	int m_numIndices;

	__forceinline SubMesh();

	__forceinline void SetMaterial(const string & materialName);

	__forceinline void BindMaterial(unsigned int slot);
	__forceinline void BindMaterialDiffuse(unsigned int slot);
	__forceinline void BindMaterialNormal(unsigned int slot);
	__forceinline void BindMaterialSpecular(unsigned int slot);

	__forceinline void DrawIndexed();

	__forceinline void UnbindMaterial();
	__forceinline void UnBindMaterialDiffuse();
	__forceinline void UnBindMaterialNormal();
	__forceinline void UnBindMaterialSpecular();
};

__forceinline SubMesh::SubMesh()
	: m_firstIndex{ 0 }, m_numIndices{ 0 }
{}

__forceinline void SubMesh::SetMaterial(const string & materialName)
{
	Resource materialResource(materialName);
	shared_ptr<ResHandle> pMaterialHandle = ResourceCache::SafeGetHandle(&materialResource);
	std::shared_ptr<MaterialResourceExtraData> pData = static_pointer_cast<MaterialResourceExtraData>(pMaterialHandle->GetExtra());

	m_pMaterial = pData->m_pMaterial;
}

__forceinline void SubMesh::BindMaterial(unsigned int slot)
{
	m_pMaterial->VBind(slot);
}

__forceinline void SubMesh::BindMaterialDiffuse(unsigned int slot)
{
	m_pMaterial->BindDiffuse(slot);
}

__forceinline void SubMesh::BindMaterialNormal(unsigned int slot)
{
	m_pMaterial->BindNormal(slot);
}

__forceinline void SubMesh::BindMaterialSpecular(unsigned int slot)
{
	m_pMaterial->BindSpecular(slot);
}

__forceinline void SubMesh::DrawIndexed()
{
	DX11API::D3D11DeviceContext()->DrawIndexed(m_numIndices, m_firstIndex, 0);
}

__forceinline void SubMesh::UnbindMaterial()
{
	m_pMaterial->VUnbind();
}

__forceinline void SubMesh::UnBindMaterialDiffuse()
{
	m_pMaterial->UnbindDiffuse();
}

__forceinline void SubMesh::UnBindMaterialNormal()
{
	m_pMaterial->UnbindNormal();
}

__forceinline void SubMesh::UnBindMaterialSpecular()
{
	m_pMaterial->UnbindSpecular();
}