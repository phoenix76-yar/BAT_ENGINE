/***********************************************************************
PHYSICS MANAGER


***********************************************************************/

#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "../IManager.h"
#include "PhysicsDefinitions.h"

namespace BEngine 
{
	namespace BPhysics
	{
		class CPhysicsWorld;

		class CPhysicsManager : public IManager
		{
		public:
			CPhysicsManager();
			~CPhysicsManager();

		private:
			void processInputMSGs(const bvector<CEventMSG>&);
			void update();

		private:
			CPhysicsWorld* m_physicsWorld;
		};
	}
	
} //namespace BEngine
#endif //PHYSICSMANAGER_H


//#include "PhysicsManager.h"
