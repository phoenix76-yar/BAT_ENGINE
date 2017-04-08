/***********************************************************************
IMANAGER

Base class for all system threaded managers.
***********************************************************************/

#ifndef IMANAGER_H
#define IMANAGER_H

#include "EngineDefinitions.h"
#include "EngineContainers.h"
#include "Core/Templates/Noncopyable.h"
#include "Core/Event System/IEventListener.h"

#include <thread>
#include <atomic>

namespace BEngine 
{
	//-----------------------------------------------------------------------
	//ISimple Manager Class
	//
	//To be used for simple non multi threaded engine supportive systems.
	//It can't receive messages/events.
	//-----------------------------------------------------------------------

	class ISimpleManager : public CNoncopyable
	{
	public:
		ISimpleManager()
		{
			m_isInitialized = false;
		}
		virtual ~ISimpleManager() = default;

		virtual void initialize() = 0;
		virtual void shutDown() = 0;
		virtual void update() = 0;
	protected:
		bool m_isInitialized;
	};

	//-----------------------------------------------------------------------
	//IManager Class
	//
	//Multi threaded manager with event management capabilities,
	//to be used as a base class for major sub systems.
	//-----------------------------------------------------------------------

	class IManager : public IEventListener, public CNoncopyable
	{
		public:
			IManager()
			{
				m_isInitialized = false;
				m_managerThread = nullptr;
				m_threadState = false;
			}
			virtual ~IManager() = default;

			virtual void initialize() override;
			virtual void shutDown() override;	
		protected:
			virtual void update() = 0;

			std::atomic<bool> m_isInitialized;
			std::atomic<bool> m_threadState;
			std::thread* m_managerThread;
	};
} //namespace BEngine
#endif //IMANAGER_H
