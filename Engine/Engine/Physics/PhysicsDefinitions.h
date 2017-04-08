/***********************************************************************
Physics Definitions

All physics related definitions are placed in this file.
***********************************************************************/

#ifndef PHYSICS_DEFINITIONS_H
#define PHYSICS_DEFINITIONS_H

#include "../EngineDefinitions.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btMaterial.h"

namespace BEngine 
{
	namespace BPhysics
	{
		enum class BCollisionBodyType 
		{
			STATIC_COL_BODY,
			DYNAMIC_COL_BODY
		};

		// ****** NOTE: Cannot be enum class as it might be read from file/data buffer.
		enum BColMatType 
		{
			COL_WOOD,
			COL_CONCRETE,
			COL_ASPHALT,
			COL_GRASS,
			COL_RUBBER,
			COL_PLASTIC,
			COL_ICE,
			COL_CUSTOM
		};

		// ****** NOTE: Don't redefine variables already defined in btMaterial!
		class CBColMaterial : public btMaterial 
		{
		public:
			CBColMaterial() = default;
			CBColMaterial(int a, int b) { flag1 = a; flag2 = b; }
			int				flag1;
			int				flag2;
			BColMatType		m_colMatType;
			int				m_fsColID;
// 			float			friction; //Already defined in btMaterial!
// 			float			restitution; //Already defined in btMaterial!
			//uint32 or BsoundPtr m_collisionSound;
			//uint32 m_impactSoundHash;
		};
	} //Namespace BPhysics	
}
#endif //PHYSICS_DEFINITIONS_H
