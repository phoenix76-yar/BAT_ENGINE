#include "PhysicsManager.h"
#include "PhysicsWorld.h"

#include <iostream>

namespace BEngine
{
	namespace BPhysics
	{
		CPhysicsManager::CPhysicsManager()
		{

			m_physicsWorld = nullptr;
		}

		CPhysicsManager::~CPhysicsManager()
		{
			if (m_threadState)
			{
				shutDown();
			}
		}

		void CPhysicsManager::update()
		{
			if (!m_isInitialized) { return; }

			m_physicsWorld = new CPhysicsWorld;
			m_physicsWorld->initialize();

			//This is the place where to initialize physics components

			if (m_physicsWorld == nullptr)
			{
				return;
			}

			while (m_threadState.load())
			{
				copyOverMSGs(m_msgCont);
				processInputMSGs(m_msgCont);

				if (m_msgCont.empty())
				{
					std::this_thread::sleep_for(std::chrono::microseconds(1000));
				}

				m_physicsWorld->update();
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}


			m_physicsWorld->shutDown();
			delete m_physicsWorld;


			m_isInitialized = false;
		}
	}
}


