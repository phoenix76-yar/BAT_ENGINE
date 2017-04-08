#include "IManager.h"

namespace BEngine
{

	//-----------------------------------------------------------------------
	//IManager Class Methods
	//-----------------------------------------------------------------------

	void IManager::initialize()
	{
		if (!m_isInitialized)
		{
			m_isInitialized = true;
			m_threadState = true;
			m_managerThread = new std::thread(&IManager::update, this);
		}
	}

	void IManager::shutDown()
	{
		m_threadState = false;

		while (m_isInitialized.load())
		{
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}

		//Stop the thread
		if (m_managerThread != nullptr && m_managerThread->joinable())
		{
			m_managerThread->join();
			delete m_managerThread;
		}
		else
		{
			throw std::logic_error("Exception occurred in RSC Manager.");
		}
	}
} //namespace BEngine