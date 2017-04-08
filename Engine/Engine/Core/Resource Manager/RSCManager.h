/***********************************************************************
RESOURCE MANAGER


***********************************************************************/

#ifndef RSCMANAGER_H
#define RSCMANAGER_H

#include "../../IManager.h"
#include "RSCefinitions.h"
#include "RSCMSGs.h"

namespace BEngine 
{

	class IWorker;
	class CResourceObject;
	class CMemoryAllocator;

	class CRSCManager : public IManager
	{
	public:
		CRSCManager();
		~CRSCManager();

	private:
		void update();
		void processInputMSGs(const bvector<CEventMSG>&);
		void processMSG(const CEventMSG& inMsg);
		void executeQuery(const CRSCLoadReqMSG& msg);
		void checkWorkers();

		uint64 generateHash(const char* key, size_t length);

	private:
		// ****** TODO: Replace later with bunordered map (needs to update to newer version)
		bunordered_map<uint64, CResourceObject*> m_loadedRSCs; //hash, rscPtr
		blist<IWorker*> m_workers;

		uint64 m_totalRSCLoadRequests;

		CMemoryAllocator* m_pAllocator;
		bvector<CRSCLoadReqMSG> m_QueriesToLoad;
	};
} //namespace BEngine
#endif //RSCMANAGER_H

