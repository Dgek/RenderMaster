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

	Material();

	virtual bool VInitialize(const string & name);

	// ===== Set textures ===== //
	void SetDiffuse(shared_ptr<Texture2D> pDiffuse);
	void SetSpecular(shared_ptr<Texture2D> pSpecular);
	void SetNormal(shared_ptr<Texture2D> pNormal);

	// ===== Bind material to the pipeline ===== //

	void BindDiffuse(unsigned int slot);
	void BindSpecular(unsigned int slot);
	void BindNormal(unsigned int slot);

	virtual void VBind(unsigned int slot);

	// ===== Unbind material form the pipeline ===== //

	void UnbindDiffuse();
	void UnbindSpecular();
	void UnbindNormal();

	virtual void VUnbind(); //always unbind from the pixel shader
};

__forceinline Material::Material()
	: m_diffuseSlot{ -1 }, m_specularSlot{ -1 }, m_normalSlot{ 01 }
{}

__forceinline void Material::SetDiffuse(shared_ptr<Texture2D> pDiffuse)
{
	m_pDiffuse = pDiffuse;

	m_pDiffuseSRV = make_shared<ShaderResourceView>();
	m_pDiffuse->CreateShaderResourceView(m_pDiffuseSRV->GetView());
}

__forceinline void Material::SetSpecular(shared_ptr<Texture2D> pSpecular)
{
	m_pSpecular = pSpecular;

	m_pSpecularSRV = make_shared<ShaderResourceView>();
	m_pSpecular->CreateShaderResourceView(m_pSpecularSRV->GetView());
}

__forceinline void Material::SetNormal(shared_ptr<Texture2D> pNormal)
{
	m_pNormalTex = pNormal;

	m_pNormalSRV = make_shared<ShaderResourceView>();
	m_pNormalTex->CreateShaderResourceView(m_pNormalSRV->GetView());
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
	m_normalSlot = slot;
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