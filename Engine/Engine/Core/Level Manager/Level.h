#ifndef LEVEL_H
#define LEVEL_H

#include "../../EngineDefinitions.h"
#include "../../EngineContainers.h"

#include "vec3.hpp"

namespace BEngine
{
	struct LoadLevelInfo
	{
		//Player starting position/data, 
		//Camera starting position/data
	};

	struct CameraData
	{
		glm::vec3 position;
		glm::vec3 directionVector;
		float viewFarPlane;
		float speed;
	};

	struct LevelUpdateInfo
	{
		float timeMS;
		CameraData camData;
	};

	class ISection;

	//-----------------------------------------------------------------------
	//Class ILevel base abstract class. 
	//
	//Holds a list of sections, is responsible for updating section classes,
	//and determine using prediction table and camera information when to
	//pre-load section data into memory.
	//-----------------------------------------------------------------------

	class ILevel
	{
	public:
		ILevel() = default;
		ILevel(const BoundBox& levelBounds) : m_mapBounds(levelBounds) {}
		virtual ~ILevel() = default;

		virtual void loadLevel(const LevelUpdateInfo& lInfo) = 0;
		virtual void reload() = 0;

		virtual void update(const LevelUpdateInfo& lInfo) = 0;

		const BoundBox& getMapBounds() const { return m_mapBounds; }

		virtual void shutDown() = 0;

	protected:
		// ****** TODO: Needs to be finished!
		virtual bool parseLevelCFG(const char* fileName, bvector<ISection*>& sectionCont);
	//private:
		const char* m_levelName;
		bvector<ISection*> m_levelSections; // ****** TODO: Rename to m_sections;
		BoundBox m_mapBounds;
	};

	//class CStreamingLevel : public ILevel {};

} //namespace BEngine

#endif //LEVEL_H
