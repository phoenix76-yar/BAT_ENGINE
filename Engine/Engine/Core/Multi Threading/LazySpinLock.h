#ifndef LAZY_SPIN_LOCK_H
#define LAZY_SPIN_LOCK_H

#include <atomic>

namespace BEngine
{
	class CLazySpinLock
	{
	public:
		void lock()
		{
			while (!try_lock()) {}
		}

		bool try_lock()
		{
			return !m_locked.test_and_set(std::memory_order_acquire);
		}

		void unlock()
		{
			m_locked.clear(std::memory_order_release);
		}
	private:
		std::atomic_flag m_locked = ATOMIC_FLAG_INIT;
	};
}

#endif //LAZY_SPIN_LOCK_H
