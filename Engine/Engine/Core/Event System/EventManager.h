/***********************************************************************
EVENT MANAGER

***********************************************************************/

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "../../EngineDefinitions.h"
#include "../../EngineContainers.h"
#include "../Multi Threading/Spinlock.h"
#include "EventMSG.h"

#include <string>
#include <mutex>

namespace BEngine
{
	class CMSGChannel;

	class CEventManager
	{
	public:
		static CEventManager& getInstance()
		{
			static CEventManager cInstance;
			return cInstance;
		}

		void initialize();
		void shutDown();

		void updateBeforeFrame();
		void updateAfterFrame();

		void registerChannel(CMSGChannel* channel);
		void submitMessage(const CEventMSG& eMessage);
		void submitMessageQueue(const CEventMSG* msgQueue, uint32 msgCount); 
		//void submitMessageQueue(const bvector<CEventMSG>& eMessages); // ****** TODO: Needs to be implemented.
		//It will be probably better than by passing a pointer ...
	private:
		bool m_isInitialized;
		CSpinLock m_spinlock;
		bvector<CMSGChannel*> m_channels;

		struct
		{
			bvector<CEventMSG> m_highPriorityMSGs;
			bvector<CEventMSG> m_mediumPriorityMSGs;
			bvector<CEventMSG> m_lowPriorityMSGs;
		} beforeFrame;

		struct
		{
			bvector<CEventMSG> m_highPriorityMSGs;
			bvector<CEventMSG> m_mediumPriorityMSGs;
			bvector<CEventMSG> m_lowPriorityMSGs;
		} afterFrame;	

	private:
		CEventManager() = default;
		~CEventManager();
		CEventManager(const CEventManager&) = delete;
		CEventManager& operator=(const CEventManager&) = delete;
	};

#define EventManager CEventManager::getInstance()	

} //namespace BEngine
#endif //EVENTMANAGER_H
