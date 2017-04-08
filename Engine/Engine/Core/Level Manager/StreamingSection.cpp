#include "StreamingSection.h"

#include "../../Core/Event System/EventMSG.h"
#include "../../Core/Event System/EventManager.h"
#include "../../Core/Resource Manager/RSCMSGs.h"
#include "../../EngineSpecialTypes.h" //For CStateVariable

#include <iostream>
#include <string>

namespace BEngine
{
	CStreamingSection::CStreamingSection(const char* entityDefXML, const char* entityPlacementsXML)
	{
		m_notifyObject = new CStateVariable;

		loadEntitiyDefXML(entityDefXML);
		loadEntityPlacementsXML(entityPlacementsXML);
	}

	CStreamingSection::~CStreamingSection()
	{

	}

	void CStreamingSection::update()
	{
		if (m_notifyObject->GetState())
		{
			std::cout << "Stream: Portion of RSC's has been loaded.\n";
			
			// ****** TODO: Add method here to process the loaded RSC's !!
			processLoadedRSCs();

			//And only when loaded RSC's have been processed, it is time to set state to false
			m_notifyObject->SetState(false);
		}
	}

	void CStreamingSection::preLoadData()
	{
		//Construct resource load request message
		CRSCLoadReqMSG rscLoadReqMSG;
		rscLoadReqMSG.notifyObject = m_notifyObject;

		// ****** NOTE: Maybe use here std::shared_ptr ?
		//You allocate this memory, then when CStateVariable state changes
		//technically you can acquire the members of the vector, use them and delete the vector
		//or refill vector with a new data, and resend it.
		rscLoadReqMSG.rscObjs = new bvector<CRSCMSGObject>;

		for (auto item : m_entityDefinitions)
		{
			CRSCMSGObject obj;
			obj.type = RSCLoadReqMSGType::ENTITY;
			obj.resourceObj = item;
			//obj.entityDef = item;
			rscLoadReqMSG.rscObjs->push_back(obj);
		}

		CRSCMSG baseResourceMSG;
		baseResourceMSG.msgType = RSCMSGType::RSC_LOAD_REQUEST;
		baseResourceMSG.rscMSG.loadReqMSG = rscLoadReqMSG;

		CEventMSG eventMSG(MSGPriorityLevel::HIGH, EventMSGType::ResourceData);
		memcpy(&eventMSG.userDataBuffer, &baseResourceMSG, sizeof(CRSCMSG));

		EventManager.submitMessage(eventMSG);
	}

	void CStreamingSection::unloadData()
	{
		// ****** TODO: TEMPORIARY JUST TO MINIMIZE MEMORY LEAK !!!
		// ****** TODO: THIS PROCEDURE MUST BE DONE IN RSC MANAGER NOT HERE, NEEDS TO BE FIXED !!!
// 		while (!m_entityDefinitions.empty())
// 		{
// 			if (m_entityDefinitions.back())
// 			{
// 				if (m_entityDefinitions.back()->rscObject)
// 				{
// 					delete m_entityDefinitions.back()->rscObject;
// 				}
// 				delete  m_entityDefinitions.back();
// 			}
// 			m_entityDefinitions.pop_back();
// 		}
	}

	void CStreamingSection::processLoadedRSCs()
	{
		std::cout << "Stream Manager:\n";
		//Now we can use RSC's as long as they are loaded into memory.
		for (auto item : m_entityDefinitions)
		{
			std::cout << "Item: " << item->entityName 
				<< " Size: " << item->rscObject->getBufferSize() 
				<< " Instance Count: " << item->rscObject->getInstanceCount() << std::endl;
		}
// 		auto rsc=  m_entityDefinitions.back()->rscObject;
// 		auto t = (uchar*)rsc->getBufferPtr();	
// 		std::cout << "------------------------------------------\n" <<  t << std::endl;
	}
} //namespace BEngine