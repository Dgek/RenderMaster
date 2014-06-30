#include "Core.h"

#include "EntityResource.h"

#include "Entity.h"

#include "../Game.h"

#include "../../Application/App.h"

#include "../../FileStream/XML/tinystr.h"
#include "../../FileStream/XML/tinyxml.h"

#include "../../Help/Strings/Strings.h"

#include "../../Physics/PhysicsInterfaces.h"

#include "../../Renderer/Lights/DirectionalLight.h"
#include "../../Renderer/Lights/PointLight.h"
#include "../../Renderer/Lights/SpotLight.h"
#include "../../Renderer/Meshes/BoxMesh.h"
#include "../../Renderer/Meshes/PlaneMesh.h"
#include "../../Renderer/Meshes/Sprite.h"
#include "../../Renderer/Meshes/IndexedMesh.h"

#include "../../ResourceManager/Resource.h"
#include "../../ResourceManager/ResourceCache.h"
#include "../../ResourceManager/Loaders/MeshLoader.h"

#include "../../Scene/Scene.h"

EntityResource::EntityResource(const string & filename)
{
	//start loading the document
	TiXmlDocument doc(filename.c_str());
	doc.LoadFile();

	TiXmlElement* pActorRoot = doc.RootElement();

	//Go through each component current actor has
	TiXmlElement* pComponent = pActorRoot->FirstChildElement();
	while (pComponent)
	{
		string componentName = pComponent->Value();

		//Get transform data
		if (componentName == "Transform")			InitTransform(pComponent);
		if (componentName == "PhysicsComponent")	InitPhysicsComponent(pComponent);
		if (componentName == "GraphicsComponent")	InitGraphicsComponent(pComponent);

		pComponent = pComponent->NextSiblingElement();
	}

	m_vOffset = Vec{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_bChangedColor = false;
}

void EntityResource::InitGraphicsComponent(TiXmlElement* pComponent)
{
	TiXmlElement*pTempEl = pComponent->FirstChildElement();
	TiXmlAttribute* pTempAtt;

	while (pTempEl)
	{
		string componentName = pTempEl->Value();

		if (componentName == "BoxComponent")
		{
			auto box = make_shared<GraphicsBoxComponent>();

			pTempAtt = pTempEl->FirstAttribute();
			box->m_fXHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			box->m_fYHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			box->m_fZHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

			TiXmlElement* pLocal = pTempEl->FirstChildElement("Material");
			box->m_material = pLocal->GetText();

			m_graphics.push_back(box);
		}
		else if (componentName == "PlaneComponent")
		{
			auto plane = make_shared<GraphicsPlaneComponent>();

			pTempAtt = pTempEl->FirstAttribute();
			plane->m_fXHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			plane->m_fYHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			plane->m_fZHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

			TiXmlElement* pLocal = pTempEl->FirstChildElement("Material");
			plane->m_material = pLocal->GetText();

			m_graphics.push_back(plane);
		}
		else if (componentName == "SpriteComponent")
		{
			auto sprite = make_shared<GraphicsSpriteComponent>();

			pTempAtt = pTempEl->FirstAttribute();
			sprite->m_fXHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			sprite->m_fYHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

			TiXmlElement* pLocal = pTempEl->FirstChildElement("Material");
			sprite->m_material = pLocal->GetText();

			m_graphics.push_back(sprite);
		}
		else if (componentName == "MeshComponent")
		{
			auto mesh = make_shared<GraphicsMeshComponent>();

			pTempAtt = pTempEl->FirstAttribute();
			mesh->m_filename = pTempAtt->Value();

			m_graphics.push_back(mesh);
		}
		else if (componentName == "SpotLightComponent")
		{
			auto spotLight = make_shared<GraphicsSpotLightComponent>();

			pTempAtt = pTempEl->FirstAttribute();
			spotLight->m_range = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			spotLight->m_innerAngle = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			spotLight->m_outerAngle = static_cast<float>(pTempAtt->DoubleValue());

			//Load color
			TiXmlElement* pLocal = pTempEl->FirstChildElement("Color");
			pTempAtt = pLocal->FirstAttribute();
			float red = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float green = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float blue = static_cast<float>(pTempAtt->DoubleValue());

			spotLight->m_color = Vec{ red, green, blue, 1.0f };

			//Load position
			pLocal = pTempEl->FirstChildElement("Position");
			if (pLocal)
			{
				pTempAtt = pLocal->FirstAttribute();
				float x = static_cast<float>(pTempAtt->DoubleValue());

				pTempAtt = pTempAtt->Next();
				float y = static_cast<float>(pTempAtt->DoubleValue());

				pTempAtt = pTempAtt->Next();
				float z = static_cast<float>(pTempAtt->DoubleValue());

				spotLight->m_position = Vec{ x, y, z, 1.0f };
			}
			else
			{
				spotLight->m_position = Vec{ 0.0f, 0.0f, 0.0f, 1.0f };
			}

			//Load direction
			pLocal = pTempEl->FirstChildElement("Direction");
			pTempAtt = pLocal->FirstAttribute();
			float x = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float y = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float z = static_cast<float>(pTempAtt->DoubleValue());

			spotLight->m_direction = Vec{ x, y, z, 1.0f };

			m_graphics.push_back(spotLight);
		}
		else if (componentName == "DirectionalLightComponent")
		{
			auto dirLight = make_shared<GraphicsDirectionalLightComponent>();

			//Load color
			TiXmlElement* pLocal = pTempEl->FirstChildElement("Color");
			pTempAtt = pLocal->FirstAttribute();
			float red = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float green = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float blue = static_cast<float>(pTempAtt->DoubleValue());

			dirLight->m_color = Vec{ red, green, blue, 1.0f };

			//Load direction
			pLocal = pTempEl->FirstChildElement("Direction");
			pTempAtt = pLocal->FirstAttribute();
			float x = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float y = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float z = static_cast<float>(pTempAtt->DoubleValue());

			dirLight->m_direction = Vec{ x, y, z, 0.0f };

			m_graphics.push_back(dirLight);
		}
		else if (componentName == "PointLightComponent")
		{
			auto pointLight = make_shared<GraphicsPointLightComponent>();

			pTempAtt = pTempEl->FirstAttribute();
			pointLight->m_range = static_cast<float>(pTempAtt->DoubleValue());

			//Load color
			TiXmlElement* pLocal = pTempEl->FirstChildElement("Color");
			pTempAtt = pLocal->FirstAttribute();
			float red = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float green = static_cast<float>(pTempAtt->DoubleValue());

			pTempAtt = pTempAtt->Next();
			float blue = static_cast<float>(pTempAtt->DoubleValue());

			pointLight->m_color = Vec{ red, green, blue, 1.0f };

			pointLight->m_position = m_vPos;

			m_graphics.push_back(pointLight);
		}

		pTempEl = pTempEl->NextSiblingElement();
	}
}

void EntityResource::InitPhysicsComponent(TiXmlElement* pComponent)
{
	//Get physics representation
	TiXmlElement* pTempEl = pComponent->FirstChildElement("BoxComponent");
	TiXmlAttribute* pTempAtt;
	if (pTempEl)
	{
		//Create box representation
		auto box = make_shared<PhysicsBoxComponent>();

		//X width
		pTempAtt = pTempEl->FirstAttribute();
		box->m_fXHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

		//Y width
		pTempAtt = pTempAtt->Next();
		box->m_fYHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

		//Z width
		pTempAtt = pTempAtt->Next();
		box->m_fZHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

		//Mass
		pTempAtt = pTempAtt->Next();
		box->m_mass = static_cast<float>(pTempAtt->DoubleValue());

		//Density
		TiXmlElement* pLocal = pTempEl->FirstChildElement("Density");
		//box->m_density = pLocal->Value();
		box->m_density = pLocal->GetText();

		//Material
		pLocal = pLocal->NextSiblingElement();
		box->m_material = pLocal->GetText();

		//Is object static
		pLocal = pLocal->NextSiblingElement();
		string type = pLocal->GetText();
		box->m_bIsStatic = (type == "Static") ? true : false;

		m_physics.push_back(box);
	}

	pTempEl = pComponent->FirstChildElement("PlaneComponent");
	if (pTempEl)
	{
		//Create plane representation
		auto plane = make_shared<PhysicsPlaneComponent>();

		//X width
		pTempAtt = pTempEl->FirstAttribute();
		plane->m_fXHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

		//Y width
		pTempAtt = pTempAtt->Next();
		plane->m_fYHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

		//Z width
		pTempAtt = pTempAtt->Next();
		plane->m_fZHalfExtent = static_cast<float>(pTempAtt->DoubleValue());

		//Mass
		pTempAtt = pTempAtt->Next();
		plane->m_mass = static_cast<float>(pTempAtt->DoubleValue());

		//Density
		TiXmlElement* pLocal = pTempEl->FirstChildElement("Density");
		plane->m_density = pLocal->GetText();

		//Material
		pLocal = pLocal->NextSiblingElement();
		plane->m_material = pLocal->GetText();

		//Is object static
		pLocal = pLocal->NextSiblingElement();
		string type = pLocal->GetText();
		plane->m_bIsStatic = (type == "Static") ? true : false;

		m_physics.push_back(plane);
	}

	pTempEl = pComponent->FirstChildElement("SphereComponent");
	if (pTempEl)
	{
		//Create plane representation
		auto pSphere = make_shared<PhysicsSphereComponent>();

		//radius
		pTempAtt = pTempEl->FirstAttribute();
		//plane->m_fXHalfExtent = pTempAtt->DoubleValue();
		pSphere->m_fRadius = static_cast<float>(pTempAtt->DoubleValue());

		//Mass
		pTempAtt = pTempAtt->Next();
		pSphere->m_mass = static_cast<float>(pTempAtt->DoubleValue());

		//Density
		TiXmlElement* pLocal = pTempEl->FirstChildElement("Density");
		pSphere->m_density = pLocal->GetText();

		//Material
		pLocal = pLocal->NextSiblingElement();
		pSphere->m_material = pLocal->GetText();

		//Is object static
		pLocal = pLocal->NextSiblingElement();
		string type = pLocal->GetText();
		pSphere->m_bIsStatic = (type == "Static") ? true : false;

		m_physics.push_back(pSphere);
	}

	pTempEl = pComponent->FirstChildElement("MeshComponent");
	if (pTempEl)
	{
		//Create mesh representation
		auto mesh = make_shared<PhysicsMeshComponent>();

		pTempAtt = pTempEl->FirstAttribute();
		mesh->m_filename = pTempAtt->Value();

		//Is object static
		pTempAtt = pTempAtt->Next();
		mesh->m_bIsStatic = (pTempAtt->Value() == "true") ? true : false;

		m_physics.push_back(mesh);
	}

	pTempEl = pComponent->FirstChildElement("CharacterComponent");
	if (pTempEl)
	{
		//Create character representation
		auto pCharacter = make_shared<PhysicsCharacterComponent>();

		//height
		pTempAtt = pTempEl->FirstAttribute();
		pCharacter->m_height = static_cast<float>(pTempAtt->DoubleValue());

		//width
		pTempAtt = pTempAtt->Next();
		pCharacter->m_width = static_cast<float>(pTempAtt->DoubleValue());

		//depth
		pTempAtt = pTempAtt->Next();
		pCharacter->m_depth = static_cast<float>(pTempAtt->DoubleValue());

		//mass
		pTempAtt = pTempAtt->Next();
		pCharacter->m_mass = static_cast<float>(pTempAtt->DoubleValue());

		//force
		pTempEl = pTempEl->FirstChildElement("Force");
		pTempAtt = pTempEl->FirstAttribute();
		pCharacter->m_forceInNewtons = static_cast<float>(pTempAtt->DoubleValue());

		//max slope
		pTempEl = pTempEl->NextSiblingElement("MaxSlope");
		pTempAtt = pTempEl->FirstAttribute();
		pCharacter->m_maxSlopeInRadians = static_cast<float>(DEGREES_TO_RADIANS(pTempAtt->DoubleValue()));

		m_physics.push_back(pCharacter);
	}
}

void EntityResource::InitTransform(TiXmlElement* pComponent)
{
	//Acquire position
	TiXmlElement* pTempEl = pComponent->FirstChildElement("Position");
	TiXmlAttribute* pTempAtt;

	if (pTempEl)
	{
		pTempAtt = pTempEl->FirstAttribute();
		float x = static_cast<float>(pTempAtt->DoubleValue());

		pTempAtt = pTempAtt->Next();
		float y = static_cast<float>(pTempAtt->DoubleValue());

		pTempAtt = pTempAtt->Next();
		float z = static_cast<float>(pTempAtt->DoubleValue());

		m_vPos = Vec{ x, y, z, 1.0f };
	}
	else
	{
		m_vPos = Vec{ 0, 0, 0, 1.0f };
	}

	//Acquire orientation
	pTempEl = pComponent->FirstChildElement("Orientation");

	if (pTempEl)
	{
		pTempAtt = pTempEl->FirstAttribute();
		float x = static_cast<float>(pTempAtt->DoubleValue());

		pTempAtt = pTempAtt->Next();
		float y = static_cast<float>(pTempAtt->DoubleValue());

		pTempAtt = pTempAtt->Next();
		float z = static_cast<float>(pTempAtt->DoubleValue());

		pTempAtt = pTempAtt->Next();
		float w = static_cast<float>(pTempAtt->DoubleValue());

		m_qRot = Quaternion{ x, y, z, w };
	}

	//Acquire scaling
	pTempEl = pComponent->FirstChildElement("Scale");
	m_vScale = Vec{ 1.0f, 1.0f, 1.0f, 1.0f };
	if (pTempEl)
	{
		pTempAtt = pTempEl->FirstAttribute();
		float x = static_cast<float>(pTempAtt->DoubleValue());

		pTempAtt = pTempAtt->Next();
		float y = static_cast<float>(pTempAtt->DoubleValue());

		pTempAtt = pTempAtt->Next();
		float z = static_cast<float>(pTempAtt->DoubleValue());

		m_vScale = Vec{ x, y, z, 1.0f };
	}
}

shared_ptr<Entity> EntityResource::VCreateEntity(Game * pGame)
{
	return pGame->VAddEntity(make_shared<Entity>(pGame->GetNewEntityId()));
}

void EntityResource::VCreateRepresentation(Scene * pScene, shared_ptr<Entity> pEntity)
{
	//Create representation
	auto pRepresentation = make_shared<EntityRepresentation>(pEntity->GetId());
	pEntity->SetRepresentation(pRepresentation);

	//Apply transformations
	Mat4x4 trans, scale;
	trans.CreateTranslation(m_vPos);
	scale.CreateScaling(m_vScale);
	pRepresentation->SetCurrentTransform(scale * trans, Global::g_pEngine->GetGameTimeInSeconds());
	pScene->AddRepresentation(pRepresentation);

	//Now add all graphics components to its representation
	for (auto component = m_graphics.begin(); component != m_graphics.end(); component++)
	{
		string type = (*component)->VGetType();
		if (type == "Box")
		{
			//auto box = static_pointer_cast<GraphicsBoxComponent>(*component);
			shared_ptr<GraphicsBoxComponent> box = static_pointer_cast<GraphicsBoxComponent>(*component);

			//Create box mesh
			auto pBoxMesh = make_shared<BoxMesh>(box->m_fXHalfExtent, box->m_fYHalfExtent, box->m_fZHalfExtent);
			pBoxMesh->SetMaterial(box->m_material);
			pRepresentation->VAddMesh(static_pointer_cast<Mesh>(pBoxMesh));
		}
		else if (type == "Plane")
		{
			auto plane = static_pointer_cast<GraphicsPlaneComponent>(*component);

			//Create plane mesh
			auto pPlaneMesh = make_shared<PlaneMesh>(plane->m_fXHalfExtent, plane->m_fYHalfExtent, plane->m_fZHalfExtent);
			pPlaneMesh->SetMaterial(plane->m_material);
			pRepresentation->VAddMesh(static_pointer_cast<Mesh>(pPlaneMesh));
		}
		else if (type == "Sprite")
		{
			auto sprite = static_pointer_cast<GraphicsSpriteComponent>(*component);

			//Create plane mesh
			auto pSpriteMesh = make_shared<Sprite>(sprite->m_fXHalfExtent, sprite->m_fYHalfExtent);
			pSpriteMesh->SetMaterial(sprite->m_material);
			pRepresentation->VAddMesh(static_pointer_cast<Mesh>(pSpriteMesh));
		}
		else if (type == "Mesh")
		{
			auto mesh = static_pointer_cast<GraphicsMeshComponent>(*component);

			//Create mesh
			Resource meshResource(mesh->m_filename);
			shared_ptr<ResHandle> pMeshes = ResourceCache::SafeGetHandle(&meshResource);
			std::shared_ptr<MeshResourceExtraData> pData = static_pointer_cast<MeshResourceExtraData>(pMeshes->GetExtra());

			//auto uNumMeshes = pData->GetNumMeshes();
			//for (decltype(uNumMeshes) i = 0; i < uNumMeshes; i++)
			//{
				//m_meshes.push_back(pData->m_pMeshes[i]);
				//pRepresentation->VAddMesh(static_pointer_cast<Mesh>(pData->m_pMeshes[i]));
			//}
			//for (auto iter = begin(pData->m_meshes); iter != end(pData->m_meshes); ++iter)
			//{
			//	pRepresentation->VAddMesh(*iter);
			//}
			pRepresentation->VAddMesh(pData->m_pMesh);
		}
		else if (type == "PointLight")
		{
			auto pointLight = static_pointer_cast<GraphicsPointLightComponent>(*component);

			//Create point light
			Vec color;
			if (!m_bChangedColor)
			{
				color = pointLight->m_color;
			}
			else
				color = m_changedColor;
			auto pLight = make_shared<PointLight>(color, pointLight->m_position + m_vOffset, pointLight->m_range);
			//PointLight* pLight = new PointLight(pointLight->m_color, Vector(0, 0, 0, 1), pointLight->m_range);
			pRepresentation->VAddLight(pLight);
		}
		else if (type == "DirectionalLight")
		{
			auto dirLight = static_pointer_cast<GraphicsDirectionalLightComponent>(*component);

			auto pLight = make_shared<DirectionalLight>(dirLight->m_color, Vec{ 0, 0, 0, 1 }, dirLight->m_direction);
			pRepresentation->VAddLight(pLight);
		}
		else if (type == "SpotLight")
		{
			auto spotLight = static_pointer_cast<GraphicsSpotLightComponent>(*component);

			auto pLight = make_shared<SpotLight>(spotLight->m_color, spotLight->m_position + m_vOffset, spotLight->m_direction,
				spotLight->m_range, spotLight->m_innerAngle, spotLight->m_outerAngle);
			pRepresentation->VAddLight(pLight);
		}
	}
}

void EntityResource::VCreatePhysicalBody(IPhysics * pPhysics, shared_ptr<Entity> pEntity)
{
	//pPhysics->VAddBox(pEntity, m_pos, Vector(5, 5, 5, 1.0f), 10.0, string("Normal"), true);
	//Now add all graphics components to its representation
	for (auto component = m_physics.begin(); component != m_physics.end(); ++component)
	{
		string type = (*component)->VGetType();
		if (type == "Box")
		{
			PhysicsBoxComponent * box = static_cast<PhysicsBoxComponent *>((*component).get());
			pPhysics->VAddBox(pEntity.get(), m_vPos + m_vOffset, m_qRot, Vec{ box->m_fXHalfExtent, box->m_fYHalfExtent, box->m_fZHalfExtent, 1.0f },
				box->m_mass, box->m_material, box->m_bIsStatic);
		}
		if (type == "Plane")
		{
			PhysicsPlaneComponent * plane = static_cast<PhysicsPlaneComponent *>((*component).get());
			pPhysics->VAddPlane(pEntity.get(), m_vPos + m_vOffset, m_qRot, Vec{ plane->m_fXHalfExtent, plane->m_fYHalfExtent, plane->m_fZHalfExtent, 1.0f },
				plane->m_mass, plane->m_material, plane->m_bIsStatic);
		}
		if (type == "Sphere")
		{
			PhysicsSphereComponent * sphere = static_cast<PhysicsSphereComponent *>((*component).get());
			pPhysics->VAddSphere(pEntity.get(), m_vPos + m_vOffset, m_qRot, sphere->m_fRadius, sphere->m_mass, sphere->m_material, sphere->m_bIsStatic);
		}
		if (type == "Character")
		{
			PhysicsCharacterComponent * character = static_cast<PhysicsCharacterComponent *>((*component).get());
			pPhysics->VAddCharacter(pEntity.get(), m_vPos + m_vOffset, m_qRot, Vec{ character->m_width, character->m_height, character->m_depth, 0.0f },
				character->m_mass, character->m_forceInNewtons, character->m_maxSlopeInRadians);
		}
	}
}