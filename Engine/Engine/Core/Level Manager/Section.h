#ifndef SECTION_H
#define SECTION_H

#include "../../EngineContainers.h"
#include "SectionDefinitions.h"

namespace BEngine
{

	class CEntitityDef;
	class CEntityPlacement;

	//-----------------------------------------------------------------------
	//ISection
	//
	//Base abstract section class.
	//-----------------------------------------------------------------------

	class ISection
	{
		//friend class Map;
	public:
		ISection() = default;
		//ISection(const BoundBox& sBounds) { m_sectionBounds = m_sectionBounds; }
		virtual ~ISection() = default; // ****** TODO: Needs implementation.

		//These methods must be only accessed by class Map. ?
		virtual void update() = 0; 
		//virtual void reload() = 0; // ****** TODO: Could be usefull especially for development
		virtual void preLoadData() = 0; //Sends requests to RSC manager to load data into RAM.
		virtual void unloadData() = 0; //Sends requests to RSC manager to unload data from RAM.

		//void addEntityPlacement(const CEntityPlacement& ePlacement) { m_entityPlacements.push_back(ePlacement); }
		//void addEntityDefinition(const CEntitityDef& eDef) { m_entityDefinitions.push_back(eDef); }

		const BoundBox& getSectionBounds() const { return m_sectionBounds; }
	protected:
		EntityType convertStrToType(const char* type);
		bool loadEntitiyDefXML(const char* entityDefXML);
		bool loadEntityPlacementsXML(const char* entityPlacementsXML);

		BoundBox m_sectionBounds;
		SectionFlags m_flags;
		ISection* m_sectionLODParent;
		// ****** NOTE: Each section could possible have subsection
		//std::vector<ISection*> m_LODChildren;

		//Replace those two by one unordered_map ?
		bvector<CEntityPlacement*> m_entityPlacements;
		bvector<CEntitityDef*> m_entityDefinitions;

		// ****** TODO: Create for each priority level a seperate container, where resource ptr's will be stored!

		// ****** TODO: Needs some testing (performance, usefulness, etc. in a separate project),
		//by the end, placements could point to one - the same object.
		bunordered_map<CEntityPlacement*, CEntitityDef*> m_entityCont;
	};

} //namespace BEngine

#endif //SECTION_H