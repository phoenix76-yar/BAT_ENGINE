#include "RSCLoadTask.h"

#include "../File Device/FileDevice.h"

#include <iostream>
#include <string.h>

namespace BEngine
{
	CRSCSimpleLoadTask::CRSCSimpleLoadTask(CResourceObject* rObject, const CRSCDirectory& cDir) :
		m_resourceObject(rObject), m_resourceDirectory(cDir)
	{

	}

	void CRSCSimpleLoadTask::excuteTask()
	{
		this->loadRSC(m_resourceObject, m_resourceDirectory);
	}

	// ****** TODO: It will be necessary to implement possibility to allocate memory from
	//different threads (using spin lock)
	void CRSCSimpleLoadTask::loadRSC(CResourceObject* rscObj, const CRSCDirectory& cDir)
	{
		std::string name = cDir.resourceName;
		std::string fullFilePath = (std::string)cDir.resourceDirectory + "\\" + (std::string)cDir.resourceName;
		CBFile inFile(&fullFilePath[0], BFileMode::FILE_RB);
		if (inFile.isOpen())
		{
			//Request from memory allocator to allocate enough memory/blocks
			//for now we will do it manually
			byte* buffer = new byte[inFile.getFileSize()];
			inFile.read(buffer, inFile.getFileSize(), 1);
			rscObj->m_bufferSize = inFile.getFileSize();
			rscObj->m_bufferPtr = buffer;
			inFile.close();
			//rscObj->m_hash = generateHash(&name[0], name.size());
			rscObj->m_loadResult == RSCLoadResult::LOADED_OK;
			//std::cout << "Resource Loaded: " << &cDir.resourceName[0]  << " hash: " << rscObj->m_hash << std::endl;
		}
		else
		{
			rscObj->m_loadResult == RSCLoadResult::LOAD_ERROR;
		}
	}
}




