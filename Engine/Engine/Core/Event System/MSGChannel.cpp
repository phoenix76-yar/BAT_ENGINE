#include "MSGChannel.h"
#include "IEventListener.h"

#include <iostream>

namespace BEngine
{
	CMSGChannel::CMSGChannel(EventMSGType eType) : m_msgType(eType)
	{
	}

	CMSGChannel::~CMSGChannel()
	{
		m_eventSystems.clear();
	}

	void CMSGChannel::submitMessage(const CEventMSG& eMessage)
	{
		for (m_eventSystemsIt = m_eventSystems.begin();
		m_eventSystemsIt != m_eventSystems.end(); ++m_eventSystemsIt)
		{
			(*m_eventSystemsIt)->processMessage(eMessage);
		}
	}

	void CMSGChannel::registerSystem(IEventListener* eSystem)
	{
		m_eventSystems.push_back(eSystem);
	}

	void CMSGChannel::unregisterSystem(IEventListener* eSystem)
	{
		for (m_eventSystemsIt = m_eventSystems.begin();
		m_eventSystemsIt != m_eventSystems.end(); ++m_eventSystemsIt)
		{
			if (eSystem == (*m_eventSystemsIt))
			{
				m_eventSystemsIt = m_eventSystems.erase(m_eventSystemsIt);
				if (m_eventSystemsIt == m_eventSystems.end() && m_eventSystems.size() == 0)
				{
					break;
				}
			}
		}
	}


} //namespace BEngine