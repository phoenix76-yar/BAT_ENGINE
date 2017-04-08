#include "RSCManager.h"
#include <iostream>
#include <string>

#include "../Memory Allocator/MemoryAllocator.h"
#include "../Level Manager/SectionDefinitions.h"
#include "../../EngineSpecialTypes.h"

#include "../Work/RSCLoadTask.h"
#include "../Work/Worker.h"

namespace BEngine
{
	CRSCManager::CRSCManager()
	{
		m_totalRSCLoadRequests = 0;
	}

	CRSCManager::~CRSCManager()
	{
		if (m_threadState.load())
		{
			this->shutDown();
		}
	}

	void CRSCManager::update()
	{
		MemAllocatConstrInfo allocInfo = { 512, 40960 };
		m_pAllocator = new CMemoryAllocator(allocInfo);


		while (m_threadState.load())
		{
			copyOverMSGs(m_msgCont);
			processInputMSGs(m_msgCont);
			checkWorkers();

			if (!m_QueriesToLoad.empty())
			{
				for (auto it = m_QueriesToLoad.begin(); it != m_QueriesToLoad.end();)
				{
					this->executeQuery(*it);
					it = m_QueriesToLoad.erase(it);
				}
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::microseconds(5));
			}
		}

		delete m_pAllocator;
		m_isInitialized = false;
	}

	void CRSCManager::executeQuery(const CRSCLoadReqMSG& msg)
	{
		//Create a new worker
		CSimpleWorker* worker = new CSimpleWorker(msg.notifyObject);

		for (auto it = msg.rscObjs->begin(); it != msg.rscObjs->end(); it++)
		{
			switch ((*it).type)
			{
			case RSCLoadReqMSGType::ENTITY:
			{
				auto entityRes = (CEntitityDef*)it->resourceObj;
				uint64 entityHash = generateHash(&entityRes->entityName[0], entityRes->entityName.size());

				auto search = m_loadedRSCs.find(entityHash);	
				if (search != m_loadedRSCs.end()) //&& search->second->m_loadResult == RSCLoadResult::LOADED_OK
				{
					entityRes->rscObject = search->second;
					entityRes->rscObject->m_instanceCount++;
				}
				else
				{
					entityRes->rscObject = new CResourceObject;
					entityRes->rscObject->m_hash = entityHash;
					entityRes->rscObject->m_instanceCount++;

					//Create directory object
					CRSCDirectory cDir;
					cDir.resourceDirectory = &entityRes->shapeDictionary[0];
					cDir.resourceName = &entityRes->entityName[0];

					//Create a load task
					ITask* loadTask = new CRSCSimpleLoadTask(entityRes->rscObject, cDir);
					m_totalRSCLoadRequests++;
					worker->addTask(loadTask);
					m_loadedRSCs.insert({ entityHash, entityRes->rscObject });
				}
				
				break;
			}
			default:
				break;
			}
		}
		m_workers.push_back(worker);
		worker->doWork();
	}

	void CRSCManager::checkWorkers()
	{
		if (!m_workers.empty())
		{
			for (auto workerIt = m_workers.begin(); workerIt != m_workers.end(); ++workerIt)
			{
				if ((*workerIt)->isComplete())
				{
					delete (*workerIt);
					std::cout << "Worker destroyed!\n";
					std::cout << "Total Requests: " << m_totalRSCLoadRequests << std::endl;
					workerIt = m_workers.erase(workerIt);
					if (workerIt == m_workers.end() && m_workers.size() == 0)
					{
						break;
					}
				}
			}
		}
	}

	uint64 CRSCManager::generateHash(const char* key, size_t length)
	{
		size_t i = 0;
		uint64 hash = 0;
		while (i != length)
		{
			hash += key[i++];
			hash += hash << 10;
			hash ^= hash >> 6;
		}
		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		return hash;
	}
} //namespace BEngine