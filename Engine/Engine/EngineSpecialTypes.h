#ifndef SPECIAL_TYPES_H
#define SPECIAL_TYPES_H

#include "EngineDefinitions.h"
#include "Core/Multi Threading/Spinlock.h"

namespace BEngine
{
	class CStateVariable
	{
	public:
		CStateVariable(){m_state = false;}

		bool GetState() const { return m_state.load(); }
		void SetState(bool state)
		{
			m_spinLock.lock();
			m_state = state;
			m_spinLock.unlock();
		}

	private:
		std::atomic<bool> m_state;
		CSpinLock m_spinLock;
	};
}

#endif //SPECIAL_TYPES_H
