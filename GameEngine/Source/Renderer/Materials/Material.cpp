#include "Core.h"

#include "Material.h"

bool Material::VInitialize(const string & name)
{
	return true;
}

void Material::VBind(unsigned int slot)
{
	m_pDiffuseSRV->Bind(slot, ST_Pixel);
	m_diffuseSlot = slot++;

	m_pSpecularSRV->Bind(slot, ST_Pixel);
	m_specularSlot = slot++;

	m_pNormalSRV->Bind(slot, ST_Pixel);
	m_normalSlot = slot;
}

void Material::VUnbind()
{
	DX11API::UnbindShaderResourceViews(m_diffuseSlot, 3, ST_Pixel);
	m_diffuseSlot = m_specularSlot = m_normalSlot = -1;
}