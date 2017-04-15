#ifndef SECTION_DEFS_H
#define SECTION_DEFS_H

#include "../../EngineDefinitions.h"

#include "vec3.hpp"
#include "vec4.hpp"
#include "gtc/quaternion.hpp"

#include <string>

using bstring = std::string;

namespace BEngine
{
	enum class EntityType
	{
		UNDEFINED = 0,
		STATIC_DRAWABLE,
		INSTANCED_FOLLIAGE, //Basically instanced drawable (it just has additional buffer)
		STATIC_COLLISION,
		FRAGMENT,
		NAV_MESH,
		SOUND_3D,
		SOUND_ZONE,
		SOUND_EHO_ZONE,
		PARTICLE,
		TRIGGER,
	};

	// ****** NOTE: By this parameter it will be possibly to determine,
	//which objects are more important to pre-load, and which are less important.
	enum class EntityLoadPriority : uchar
	{
		LOAD_HIGH,
		LOAD_MEDIUM,
		LOAD_LOW
	};

	union EntityPlacementFlags
	{
		struct
		{
			bool isDynamic : 1;
			bool hasParentLOD : 1;
			bool hasChildLOD : 1;
		};
		uint32 value;
	};

	union EntityDefFlags
	{
		struct
		{
		};
		uint32 value;
	};

	union SectionFlags
	{

		uint32 value;
	};

	class CEntityPlacement
	{
	public:
		uint32 hash;
		EntityPlacementFlags flags;
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	};

	class CResourceObject;

	class CEntitityDef
	{
	public:
		CEntitityDef() : hash(0), loadDist(0.0f), radius(0.0f), entityType(EntityType::UNDEFINED)
		{
			flags.value = 0;
		}

		uint32 hash;
		EntityDefFlags flags;
		EntityType entityType;
		EntityLoadPriority loadPriority;
		float loadDist; //Draw dist for drawable
		BoundBox entityBoundBox;
		glm::vec3 entityCentre; //Centroid
		float radius; 

		CEntitityDef* parentLOD;
		CEntitityDef* childLOD;

		//Used for additional details, door handles, 
		//additional collision, etc. when ultra high settings are used.
		CEntitityDef* extraLOD;

		//const char* directoryPath;

		bstring entityName;
		bstring textureDictionary;
		bstring shapeDictionary;
		bstring physicsDictionary;

		//const char* entityName;
		//const char* textureDictionary;

		// ****** NOTE: Replace all other type dictionaries with just one common rscDirectoryPath ?
		//const char* shapeDictionary;
		//const char* physicsDictionary;

		CResourceObject* rscObject; //RAM memory buffer object of the entity.
	};
}

#endif //SECTION_DEFS_H
