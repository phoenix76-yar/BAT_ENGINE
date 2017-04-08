#include "RSCManager.h"
#include "RSCMSGs.h"

#ifdef _DEBUG
#include <iostream>
#endif

namespace BEngine
{
	void CRSCManager::processInputMSGs(const bvector<CEventMSG>& msgCont)
	{
		while (!m_msgCont.empty())
		{
			switch (m_msgCont.back().m_msgType)
			{
			case EventMSGType::ResourceData:
			{
				auto inMsg = m_msgCont.back();
				this->processMSG(inMsg);
				break;
			}
			default:
				break;
			}
			m_msgCont.pop_back();
		}
	}

	void CRSCManager::processMSG(const CEventMSG& inMsg)
	{
		auto userData = inMsg.userDataBuffer;
		auto inputData = (CRSCMSG*)userData;

		switch (inputData->msgType)
		{
		case RSCMSGType::RSC_LOAD_REQUEST:
		{
#ifdef _DEBUG
			std::cout << "RSC_LOAD_REQUEST\n";
#endif
			//m_QueriesToLoad.push_back(&(inputData->rscMSG.loadReqMSG));
			m_QueriesToLoad.push_back(inputData->rscMSG.loadReqMSG);
			break;
		}
		case RSCMSGType::RSC_DESTROY_REQUEST:
		{

			break;
		}
		default:
			break;
		}



	}
} //namespace BEngine
