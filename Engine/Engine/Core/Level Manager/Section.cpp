#include "Section.h"

#include "../PugiXML/pugixml.hpp"

#include <string>

namespace BEngine
{
	EntityType ISection::convertStrToType(const char* type)
	{
		std::string strType = type;
		if (strType == "STATIC_DRAWABLE"){return EntityType::STATIC_DRAWABLE;}
		else if (strType == "INSTANCED_FOLLIAGE") { return EntityType::INSTANCED_FOLLIAGE; }
		else if (strType == "STATIC_COLLISION") { return EntityType::STATIC_COLLISION; }
		else if (strType == "FRAGMENT") { return EntityType::FRAGMENT; }
		else if (strType == "NAV_MESH") { return EntityType::NAV_MESH; }
		else if (strType == "SOUND_3D") { return EntityType::SOUND_3D; }
		else if (strType == "SOUND_ZONE") { return EntityType::SOUND_ZONE; }
		else if (strType == "SOUND_EHO_ZONE") { return EntityType::SOUND_EHO_ZONE; }
		else if (strType == "PARTICLE") { return EntityType::PARTICLE; }
		else if (strType == "TRIGGER") { return EntityType::TRIGGER; }
		else { return EntityType::UNDEFINED;}	
	}

	bool ISection::loadEntitiyDefXML(const char* entityDefXML)
	{
		pugi::xml_document entityDefDoc;
		auto result = entityDefDoc.load_file(entityDefXML);
		if (result.status == pugi::status_ok)
		{
			auto mapDefNode = entityDefDoc.child("MapTypes");
			auto entityTypesNode = mapDefNode.child("EntityTypes");

			for (auto itemNode = entityTypesNode.child("Item"); 
			itemNode; itemNode = itemNode.next_sibling("Item"))
			{
				BoundBox entityBBox;
				entityBBox.minX = itemNode.child("bbMin").attribute("x").as_float();
				entityBBox.minY = itemNode.child("bbMin").attribute("y").as_float();
				entityBBox.minZ = itemNode.child("bbMin").attribute("z").as_float();

				entityBBox.maxX = itemNode.child("bbMax").attribute("x").as_float();
				entityBBox.maxY = itemNode.child("bbMax").attribute("y").as_float();
				entityBBox.maxZ = itemNode.child("bbMax").attribute("z").as_float();

				glm::vec3 entityCentre;
				entityCentre.x = itemNode.child("bsCentre").attribute("x").as_float();
				entityCentre.y = itemNode.child("bsCentre").attribute("y").as_float();
				entityCentre.z = itemNode.child("bsCentre").attribute("z").as_float();

				CEntitityDef* entity = new CEntitityDef;
				entity->entityType = convertStrToType(itemNode.child("assetType").attribute("type").as_string());
				entity->hash = itemNode.child("hash").attribute("value").as_uint();
				entity->entityBoundBox = entityBBox;
				entity->entityCentre = entityCentre;
				entity->radius = itemNode.child("bsRadius").attribute("value").as_float();
				entity->entityName = itemNode.child("name").attribute("name").as_string();
				entity->textureDictionary = itemNode.child("textureDictionary").attribute("name").as_string();
				entity->shapeDictionary = itemNode.child("shapeDictionary").attribute("name").as_string();
				entity->physicsDictionary = itemNode.child("physicsDictionary").attribute("name").as_string();

				m_entityDefinitions.push_back(entity);
			}
			return true;
		}
		return false;
	}

	bool ISection::loadEntityPlacementsXML(const char* entityPlacementsXML)
	{
		pugi::xml_document mapDataDoc;
		auto result = mapDataDoc.load_file(entityPlacementsXML);
		if (result.status == pugi::status_ok)
		{
			auto mapDataNode = mapDataDoc.child("MapData");
			auto nameNode = mapDataNode.child("name");
			auto flagsNode = mapDataNode.child("flags");
			auto sectionBoundsNode = mapDataNode.child("Bounds");
			auto entPlacementsNode = mapDataNode.child("Entities");

			BoundBox sectionBounds;
			sectionBounds.minX = sectionBoundsNode.child("min").attribute("x").as_float();
			sectionBounds.minY = sectionBoundsNode.child("min").attribute("y").as_float();
			sectionBounds.minZ = sectionBoundsNode.child("min").attribute("z").as_float();

			sectionBounds.maxX = sectionBoundsNode.child("max").attribute("x").as_float();
			sectionBounds.maxY = sectionBoundsNode.child("max").attribute("y").as_float();
			sectionBounds.maxZ = sectionBoundsNode.child("max").attribute("z").as_float();

			m_sectionBounds = sectionBounds;

			for (auto itemNode = entPlacementsNode.child("Item");
			itemNode; itemNode = itemNode.next_sibling("Item"))
			{
				glm::vec3 pos;
				glm::quat rot;
				glm::vec3 scale;

				pos.x = itemNode.child("position").attribute("x").as_float();
				pos.y = itemNode.child("position").attribute("y").as_float();
				pos.z = itemNode.child("position").attribute("z").as_float();

				rot.x = itemNode.child("rotation").attribute("x").as_float();
				rot.y = itemNode.child("rotation").attribute("y").as_float();
				rot.z = itemNode.child("rotation").attribute("z").as_float();
				rot.w = itemNode.child("rotation").attribute("w").as_float();

				scale.x = itemNode.child("scale").attribute("x").as_float();
				scale.y = itemNode.child("scale").attribute("y").as_float();
				scale.z = itemNode.child("scale").attribute("z").as_float();

				CEntityPlacement* entityPlacement = new CEntityPlacement;
				entityPlacement->hash = itemNode.child("hash").attribute("value").as_uint();
				entityPlacement->flags.value = itemNode.child("flags").attribute("value").as_uint();
				entityPlacement->position = pos;
				entityPlacement->rotation = rot;
				entityPlacement->scale = scale;

				m_entityPlacements.push_back(entityPlacement);
			}
			return true;
		}
		return false;
	}
}