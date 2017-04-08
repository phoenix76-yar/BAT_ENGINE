#include "Level.h"
#include "../PugiXML/pugixml.hpp"

#include "StreamingSection.h"

#include <string>

namespace BEngine
{
	bool ILevel::parseLevelCFG(const char* fileName, bvector<ISection*>& sectionCont)
	{
		BoundBox levelBounds;
		pugi::xml_document levelDoc;
		auto result = levelDoc.load_file(fileName);
		if (result.status == pugi::status_ok)
		{
			auto levelDataNode = levelDoc.child("MapData");
			auto levelBoundsNode = levelDoc.child("Bounds");
			levelBounds.minX = levelBoundsNode.child("min").attribute("x").as_float();
			levelBounds.minY = levelBoundsNode.child("min").attribute("y").as_float();
			levelBounds.minZ = levelBoundsNode.child("min").attribute("z").as_float();

			levelBounds.maxX = levelBoundsNode.child("max").attribute("x").as_float();
			levelBounds.maxY = levelBoundsNode.child("max").attribute("y").as_float();
			levelBounds.maxZ = levelBoundsNode.child("max").attribute("z").as_float();

			this->m_mapBounds = levelBounds;
			auto levelSectionNodes = levelDataNode.child("Sections");

			for (auto sectionNode = levelSectionNodes.child("Section"); 
			sectionNode; sectionNode = sectionNode.next_sibling("Section"))
			{
				std::string entityDefFileName = sectionNode.child("EntityDef").attribute("name").as_string();
				std::string entityPlacementFileName = sectionNode.child("EntityPlacements").attribute("name").as_string();
				ISection* section = new CStreamingSection(&entityDefFileName[0], &entityPlacementFileName[0]);
				sectionCont.push_back(section);
			}
			return true;
		}
		return false;
	}

} //namespace BEngine