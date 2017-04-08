#ifndef TASK_H
#define TASK_H

#include "../../EngineDefinitions.h"

namespace BEngine
{
	//-----------------------------------------------------------------------
	//ITask
	//
	//Each task can contain only one thing to do (keep it simple to maintain)
	//-----------------------------------------------------------------------

	class ITask
	{
	public:
		ITask() = default;
		virtual ~ITask() = default;

		virtual void excuteTask() = 0;
	};
}

#endif //TASK_H