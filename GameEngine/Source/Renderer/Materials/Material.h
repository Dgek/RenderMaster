#pragma once

#include "../../Graphics/Resources/Textures/Texture2D.h"
#include "../../Graphics/Resources/Views/ShaderResourceView.h"

class Material
{
protected:

	MaterialId m_id;

	/*** ======================
			Textures
		====================== ***/
	shared_ptr<Texture2D> m_pDiffuse;
	shared_ptr<Texture2D> m_pSpecular;
	shared_ptr<Texture2D> m_pNormalTex;

	/*** =========================
			Shader Resource Views
		========================= ***/
	shared_ptr<ShaderResourceView> m_pDiffuseSRV;
	shared_ptr<ShaderResourceView> m_pSpecularSRV;
	shared_ptr<ShaderResourceView> m_pNormalSRV;

	// ===== Specific rendering data ===== //

	//remember the slot to which we bound resource view
	int	m_diffuseSlot;
	int	m_specularSlot;
	int	m_normalSlot;

public:

	__forceinline Material();

	virtual bool VInitialize(const string & name);

	// ===== Set textures ===== //
	__forceinline void SetDiffuse(shared_ptr<Texture2D> pDiffuse);
	__forceinline void SetSpecular(shared_ptr<Texture2D> pSpecular);
	__forceinline void SetNormal(shared_ptr<Texture2D> pNormal);

	// ===== Bind material to the pipeline ===== //

	__forceinline void BindDiffuse(unsigned int slot);
	__forceinline void BindSpecular(unsigned int slot);
	__forceinline void BindNormal(unsigned int slot);

	virtual void VBind(unsigned int slot);

	// ===== Unbind material form the pipeline ===== //

	__forceinline void UnbindDiffuse();
	__forceinline void UnbindSpecular();
	__forceinline void UnbindNormal();

	virtual void VUnbind(); //always unbind from the pixel shader
};

__forceinline Material::Material()
	: m_diffuseSlot{ -1 }, m_specularSlot{ -1 }, m_normalSlot{ 01 }
{}

__forceinline void Material::SetDiffuse(shared_ptr<Texture2D> pDiffuse)
{
	m_pDiffuse = pDiffuse;
}

__forceinline void Material::SetSpecular(shared_ptr<Texture2D> pSpecular)
{
	m_pSpecular = pSpecular;
}

__forceinline void Material::SetNormal(shared_ptr<Texture2D> pNormal)
{
	m_pNormalTex = pNormal;
}


__forceinline void Material::BindDiffuse(unsigned int slot)
{
	m_pDiffuseSRV->Bind(slot, ST_Pixel);
	m_diffuseSlot = slot;
}

__forceinline void Material::BindSpecular(unsigned int slot)
{
	m_pSpecularSRV->Bind(slot, ST_Pixel);
	m_specularSlot = slot;
}

__forceinline void Material::BindNormal(unsigned int slot)
{
	m_pNormalSRV->Bind(slot, ST_Pixel);
}

__forceinline void Material::UnbindDiffuse()
{
	DX11API::UnbindShaderResourceViews(m_diffuseSlot, 1, ST_Pixel);
	m_diffuseSlot = -1;
}
__forceinline void Material::UnbindSpecular()
{
	DX11API::UnbindShaderResourceViews(m_specularSlot, 1, ST_Pixel);
	m_specularSlot = -1;
}

__forceinline void Material::UnbindNormal()
{
	DX11API::UnbindShaderResourceViews(m_normalSlot, 1, ST_Pixel);
	m_normalSlot = -1;
}