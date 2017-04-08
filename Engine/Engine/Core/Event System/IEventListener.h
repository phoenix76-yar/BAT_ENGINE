/***********************************************************************
IINTERFACE EVENT LISTENER CLASS

This class must be inherited by all systems that want to receive
and submit event messages.

Messages can be posted/submited via EventManager->submitMessage(MSG); method.
This method enables for the system or class that inherits this class,
to post messages back into the system, without additional header file includes.
***********************************************************************/

#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "../../EngineDefinitions.h"
#include "../../EngineContainers.h"
#include "../Multi Threading/Spinlock.h"
#include "EventMSG.h"

namespace BEngine
{
	class IEventListener
	{
		//friend class CMSGChannel;
	public:
		IEventListener() = default;
		virtual ~IEventListener();

		virtual void initialize() = 0;
		virtual void shutDown() = 0;

		// ****** NOTE: Technically those methods should be protected.
		//Called by message channel.
		virtual void processMessage(const CEventMSG& eMSG);
		void submitMessage(const CEventMSG& eMSG);
		void submitMessageQueue(const CEventMSG* msgQueue, uint32 msgCount);
		//void submitMessageQueue(const bvector<CEventMSG>& eMessages);
	protected:
		void registerMSGInputCallBack(void(*msgRCallBack)(CEventMSG eMessage));
		void copyOverMSGs(bvector<CEventMSG>& msgContainer);
		virtual void processInputMSGs(const bvector<CEventMSG>&) {}
		bvector<CEventMSG> m_msgCont;
	private:
		CSpinLock m_spinlock;
		bvector<CEventMSG> m_tempMSGCont;

		void(*m_msgReceiveCallBack)(CEventMSG eMessage);
	};

} //namespace BEngine
#endif //EVENTSYSTEM_H