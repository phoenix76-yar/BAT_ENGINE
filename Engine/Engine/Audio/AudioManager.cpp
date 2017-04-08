#include "AudioManager.h"

namespace BEngine
{
	namespace BAudio
	{
		CAudioManager::~CAudioManager()
		{
			if (m_threadState.load())
			{
				shutDown();
			}
		}

		void CAudioManager::update()
		{
			while (m_threadState.load())
			{
				copyOverMSGs(m_msgCont);
				processInputMSGs(m_msgCont);

				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}

			//Shut down all subsystems in reverse order

			//system->shutDown();	

			//delete all subsystems
			//delete system;

			m_isInitialized = false;
		}
		
	} //namespace BAudio
} //namespace BEngine