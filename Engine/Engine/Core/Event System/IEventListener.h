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

	protected:
		void copyOverMSGs(bvector<CEventMSG>& msgContainer);
		virtual void processInputMSGs(const bvector<CEventMSG>&) {}
		bvector<CEventMSG> m_msgCont;
		CSpinLock m_spinlock;
		bvector<CEventMSG> m_tempMSGCont;
	};




// 	template<class T, EventMSGType MessageType>
// 	class CEventListener : public virtual IEventListener
// 	{
// 	public:
// 		CEventListener() = default;
// 		virtual ~CEventListener() = default;
// 
// 		void copyOverMSGs();
// 	protected:
// 		virtual void processMSGQueue(const std::vector<T>&) = 0;
// 		std::vector<T> m_messageCont;
// 	};
// 
// 	template<class T, EventMSGType MessageType>
// 	inline void CEventListener<T, MessageType>::copyOverMSGs()
// 	{
// 
// 	}

//Example: Graphics Event Listener
//	using CGRCEventListener = CEventListener<CGraphicsMSG, EventMSGTypeV2::GraphicsData>;

} //namespace BEngine
#endif //EVENTSYSTEM_H