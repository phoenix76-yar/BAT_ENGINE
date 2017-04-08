#include "PhysicsManager.h"
#include "PhysicsWorld.h"

namespace BEngine
{
	namespace BPhysics
	{
				void CPhysicsManager::processInputMSGs(const bvector<CEventMSG>& msgCont)
				{
	 			while (!m_msgCont.empty())
		 			{
		 				switch (m_msgCont.back().m_msgType)
		 				{
		 				case EventMSGType::PhysicsData:
		 				{
		 					// ****** TODO: Do something here.
		 					break;
						}
		 				default:
		 					break;
		 				}
		 			}
		 		}
	} //namespace BPhysics
} //namespace BEngine