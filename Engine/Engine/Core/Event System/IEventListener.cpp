#include "IEventListener.h"
#include "EventManager.h"


namespace BEngine
{
	IEventListener::~IEventListener()
	{
		m_tempMSGCont.clear();
	}

	//This part of code operates on the thread from which the method was called.
	void IEventListener::processMessage(const CEventMSG& eMSG)
	{
		m_spinlock.lock();
		m_tempMSGCont.push_back(eMSG);
		m_spinlock.unlock();
	}

	//This part is a critical section, as the method is called by inherited class (possibly from separate thread).
	void IEventListener::copyOverMSGs(bvector<CEventMSG>& msgContainer)
	{
		m_spinlock.lock();
		// ****** TODO: This is temporary, needs replacement/optimization.
		while (!m_tempMSGCont.empty())
		{
			msgContainer.push_back(m_tempMSGCont.back());
			m_tempMSGCont.pop_back();
		}
		m_spinlock.unlock();

		// ****** TODO: Who cleans up msgContainer ????
	}
}
