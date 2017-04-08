/***********************************************************************
SPINLOCK

http://www.gamedev.ru/code/forum/?id=198628
***********************************************************************/

#ifndef SPINLOCK_H
#define SPINLOCK_H

//#include <iostream>
//#include <mutex>
#include <thread>
#include <atomic>

using namespace std;
using namespace std::chrono;

#pragma warning (disable : 4996)

namespace BEngine 
{
	class CSpinLock 
	{
	public:
		CSpinLock() = default;
		CSpinLock(const CSpinLock&) = delete;
		CSpinLock& operator=(const CSpinLock&) = delete;

		void lock()
		{
			const int MAX_SPIN = 1000;
			int count = 0;

			while (true)
			{
				if (*(volatile uint32_t*)&m_locked == 0)
				{
					if (!m_locked.test_and_set(std::memory_order_acquire))
						return;
				}

				if (count < MAX_SPIN)
				{
					count++;
					_Thrd_yield();
				}
				else
				{
					_sleep(0);

				}
			}
		}
		void unlock() {
			m_locked.clear(std::memory_order_release);
		}
	private:
		std::atomic_flag m_locked = ATOMIC_FLAG_INIT;;
	};


	class CSpinLockGuard
	{
	public:
		CSpinLockGuard(CSpinLock & const sl) : m_spinLock(sl)
		{
			m_spinLock.lock();
		}
		~CSpinLockGuard()
		{
			m_spinLock.unlock();
		}
		CSpinLockGuard(const CSpinLockGuard&) = delete;
	private:
		CSpinLock& const m_spinLock;
	};

} //BEngine
#endif //SPINLOCK_H