#include "Worker.h"
#include "Task.h"

#include "../../EngineSpecialTypes.h"

namespace BEngine
{
	//-----------------------------------------------------------------------
	//IWorker class methods
	//-----------------------------------------------------------------------

	IWorker::IWorker(uint32 id) : m_workerID(id) , m_doingWork(false), m_workHasBeenCompleted(false) {}

	void IWorker::addTask(ITask* task)
	{
		if (!m_doingWork.load())
			m_tasks.push_back(task);
	}

	void IWorker::excuteTasks()
	{
		if (!m_tasks.empty())
		{
			m_doingWork = true;
		}

		while (!m_tasks.empty())
		{
			//std::cout << "Worker ID:" << m_workerID << std::endl;
			if (m_tasks.back())
			{
				m_tasks.back()->excuteTask();
				delete m_tasks.back();
			}
			m_tasks.pop_back();
		}
		m_workHasBeenCompleted = true;
	}

	//-----------------------------------------------------------------------
	//CSimpleWorker class methods
	//-----------------------------------------------------------------------

	CSimpleWorker::CSimpleWorker(CStateVariable* state) : 
		IWorker(0),m_threadDeleted(false), m_state(state)
	{
	}

	CSimpleWorker::CSimpleWorker(CStateVariable* state, uint32 id) :
		IWorker(id), m_threadDeleted(false), m_state(state)
	{
	}

	CSimpleWorker::~CSimpleWorker()
	{
		shutDown();
	}

	void CSimpleWorker::shutDown()
	{
		if (!m_threadDeleted.load())
		{
			while (!m_workHasBeenCompleted.load())
			{
				std::this_thread::sleep_for(std::chrono::microseconds(2));
			}

			if (!m_threadDeleted.load())
			{

				if (m_workerThread != nullptr)
				{
					if (m_workerThread->joinable())
					{
						m_workerThread->join();
						delete m_workerThread;
					}
				}
			}
		}
	}

	void CSimpleWorker::doWork()
	{
		if (!m_doingWork.load())
		{
			m_doingWork = true;
			m_workerThread = new std::thread(&CSimpleWorker::excuteTasks, this);
		}
	}

	void CSimpleWorker::excuteTasks()
	{
		if (!m_tasks.empty())
		{
			m_doingWork = true;
		}

		while (!m_tasks.empty())
		{
			if (m_tasks.back())
			{
				m_tasks.back()->excuteTask();
				delete m_tasks.back();
			}
			m_tasks.pop_back();
		}
		m_workHasBeenCompleted = true;
		m_state->SetState(true);
	}
}