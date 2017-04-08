#ifndef RSC_LOAD_TASK_H
#define RSC_LOAD_TASK_H

#include "Task.h"
#include "../../EngineDefinitions.h"
#include "../Resource Manager/RSCefinitions.h"

namespace BEngine
{
	class CRSCDirectory
	{
	public:
		const char* resourceName;
		const char* resourceDirectory;
	};

	class CRSCSimpleLoadTask : public ITask
	{
	public:
		CRSCSimpleLoadTask(CResourceObject* rObject, const CRSCDirectory& cDir);
		~CRSCSimpleLoadTask() = default;

		void excuteTask() override final;
	private:
		void loadRSC(CResourceObject* rscObj, const CRSCDirectory& cDir);

		CResourceObject* m_rscObj;
		CRSCDirectory m_resDir;
	};
}

#endif //RSC_LOAD_TASK_H
