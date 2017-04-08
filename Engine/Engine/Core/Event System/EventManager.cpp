#include "EventManager.h"
#include "MSGChannel.h"

#include <iostream>

namespace BEngine
{
	CEventManager::~CEventManager()
	{
		shutDown();
	}


	void CEventManager::initialize()
	{

	}

	void CEventManager::shutDown()
	{
		if (!m_channels.empty())
		{
			deleteContainerData(m_channels);
		}		
	}

	void CEventManager::updateBeforeFrame()
	{
		CSpinLockGuard speenlock_guard(m_spinlock);
		//Send out high priority messages
		while (!beforeFrame.m_highPriorityMSGs.empty())
		{
			for (size_t i = 0; i < m_channels.size(); i++)
			{
				if (beforeFrame.m_highPriorityMSGs.back().m_msgType == m_channels[i]->getMSGType())
				{
					// ****** TODO: It would be better to submit a message queue,
					//instead of submitting message by message.
					m_channels[i]->submitMessage(beforeFrame.m_highPriorityMSGs.back());
				}
			}
			beforeFrame.m_highPriorityMSGs.pop_back();
		}

		//Send out medium priority messages
		while (!beforeFrame.m_mediumPriorityMSGs.empty())
		{
			for (size_t i = 0; i < m_channels.size(); i++)
			{
				if (beforeFrame.m_mediumPriorityMSGs.back().m_msgType == m_channels[i]->getMSGType())
				{
					m_channels[i]->submitMessage(beforeFrame.m_mediumPriorityMSGs.back());
				}
			}
			beforeFrame.m_mediumPriorityMSGs.pop_back();
		}

		//Send out low priority messages
		while (!beforeFrame.m_lowPriorityMSGs.empty())
		{
			for (size_t i = 0; i < m_channels.size(); i++)
			{
				if (beforeFrame.m_lowPriorityMSGs.back().m_msgType == m_channels[i]->getMSGType())
				{
					m_channels[i]->submitMessage(beforeFrame.m_lowPriorityMSGs.back());
				}
			}
			beforeFrame.m_lowPriorityMSGs.pop_back();
		}
	}

	void CEventManager::updateAfterFrame()
	{
		CSpinLockGuard speenlock_guard(m_spinlock);
		//Send out high priority messages
		while (!afterFrame.m_highPriorityMSGs.empty())
		{
			for (size_t i = 0; i < m_channels.size(); i++)
			{
				if (afterFrame.m_highPriorityMSGs.back().m_msgType == m_channels[i]->getMSGType())
				{
					m_channels[i]->submitMessage(afterFrame.m_highPriorityMSGs.back());
				}
			}
			afterFrame.m_highPriorityMSGs.pop_back();
		}

		//Send out medium priority messages
		while (!afterFrame.m_mediumPriorityMSGs.empty())
		{
			for (size_t i = 0; i < m_channels.size(); i++)
			{
				if (afterFrame.m_mediumPriorityMSGs.back().m_msgType == m_channels[i]->getMSGType())
				{
					m_channels[i]->submitMessage(afterFrame.m_mediumPriorityMSGs.back());
				}
			}
			afterFrame.m_mediumPriorityMSGs.pop_back();
		}

		//Send out low priority messages
		while (!afterFrame.m_lowPriorityMSGs.empty())
		{
			for (size_t i = 0; i < m_channels.size(); i++)
			{
				if (afterFrame.m_lowPriorityMSGs.back().m_msgType == m_channels[i]->getMSGType())
				{
					m_channels[i]->submitMessage(afterFrame.m_lowPriorityMSGs.back());
				}
			}
			afterFrame.m_lowPriorityMSGs.pop_back();
		}
	}

	void CEventManager::registerChannel(CMSGChannel* channel)
	{
		m_channels.push_back(channel);
	}

	void CEventManager::submitMessage(const CEventMSG& eMessage)
	{
		CSpinLockGuard speenlock_guard(m_spinlock);
		switch (eMessage.m_prLevel)
		{
			case MSGPriorityLevel::HIGH:
			{
				beforeFrame.m_highPriorityMSGs.push_back(eMessage);
				break;
			}
			case MSGPriorityLevel::MEDIUM:
			{
				beforeFrame.m_mediumPriorityMSGs.push_back(eMessage);
				break;
			}
			case MSGPriorityLevel::LOW:
			{
				beforeFrame.m_lowPriorityMSGs.push_back(eMessage);
			}
			default:
				break;
		}
	}

	void CEventManager::submitMessageQueue(const CEventMSG* msgQueue, uint32 msgCount)
	{
		CSpinLockGuard speenlock_guard(m_spinlock);


	}

}


