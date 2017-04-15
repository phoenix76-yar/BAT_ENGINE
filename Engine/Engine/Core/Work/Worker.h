#ifndef WORKER_H
#define WORKER_H

#include "../../EngineDefinitions.h"
#include "../../EngineContainers.h"
#include "../Multi Threading/Spinlock.h"

#include <atomic>

namespace BEngine
{
	class ITask;


	//-----------------------------------------------------------------------
	//IWorker
	//
	//Abstract base class for other work types
	//-----------------------------------------------------------------------

	class IWorker
	{
	public:
		IWorker(uint32 id);
		virtual ~IWorker() = default;

		virtual void shutDown() = 0;
		virtual void doWork() = 0;
		virtual void addTask(ITask* task);

		bool isComplete() const { return m_workHasBeenCompleted.load(); }
	protected:
		virtual void excuteTasks();

		uint32 m_workerID;
		bvector<ITask*> m_tasks;
		std::atomic<bool> m_doingWork;
		std::atomic<bool> m_workHasBeenCompleted;
	};


	class CStateVariable;

	//-----------------------------------------------------------------------
	//CSimpleWorker
	//
	//Simple implementation of the interface worker, it accepts all kinds of
	//tasks and executes on a separate thread doWork method only once, and then
	//shuts down.
	//
	// ****** NOTE: Not tested completely yet!
	//-----------------------------------------------------------------------

	class CSimpleWorker : public IWorker
	{
	public:
		CSimpleWorker(CStateVariable* state);
		CSimpleWorker(CStateVariable* state, uint32 id);
		~CSimpleWorker();

		void shutDown() override final;
		void doWork() override final;
	private:
		void excuteTasks() override final;
		std::thread* m_workerThread;
		std::atomic<bool> m_threadDeleted;
		CStateVariable* m_state;
	};
}

#endif //WORKER_H