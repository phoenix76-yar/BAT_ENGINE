/***********************************************************************
MESSAGE CHANNEL

Class where specific systems which are interested in only specfic systems, 
must be registered. Systems can be registered in multiple MSGChannels.
***********************************************************************/

#ifndef MSGCHANNEL_H
#define MSGCHANNEL_H

#include "../../EngineDefinitions.h"
#include "../../EngineContainers.h"
#include "EventMSG.h"

#include <vector>
#include <deque>
#include <list>

namespace BEngine
{
	class IEventListener;

	class CMSGChannel
	{
	public:
		CMSGChannel(EventMSGType eType);
		~CMSGChannel();

		void submitMessage(const CEventMSG& eMessage);
		// ****** TODO: Needs to be implemented.
		//void submitMesssageQueue(const CEventMSG* msgQueue, uint32 msgCount);
		//void submitMessageQueue(const bvector<CEventMSG>& eMessages);


		void registerSystem(IEventListener* eSystem);
		void unregisterSystem(IEventListener* eSystem);

		EventMSGType getMSGType() const { return m_msgType; }

	private:
		EventMSGType m_msgType;
		blist<IEventListener*> m_eventSystems;
		blist<IEventListener*>::iterator m_eventSystemsIt;
	};
} //namespace BEngine
#endif //MSGCHANNEL_H