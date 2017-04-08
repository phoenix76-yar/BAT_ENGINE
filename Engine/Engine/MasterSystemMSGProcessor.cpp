#include "MasterSystem.h"

//Event system related includes
#include "Core/Event System/EventManager.h"
#include "Core/Event System/IEventListener.h"
#include "Core/Event System/MSGChannel.h"
#include "SystemMSGs.h"

namespace BEngine
{
	void CMasterSystem::processInputMSGs(const bvector<CEventMSG>& msgCont)
	{
		while (!m_msgCont.empty())
		{
			auto inMSG = m_msgCont.back();
			switch (inMSG.m_msgType)
			{
			case EventMSGType::System:
			{
				std::cout << "EventMSGType::System\n";
				auto sysMSG = *(CSystemMSG*)inMSG.userDataBuffer;
				switch (sysMSG.action)
				{
				case SysActionType::EXIT:
				{
					this->shutDown();
					break;
				}
				case SysActionType::LOAD_LEVEL:
				{
					// ****** TODO: Force Level to load a level
					break;
				}
				case SysActionType::RELOAD_LEVEL:
				{
					// ****** TODO: Force Level to reload itself.
					break;
				}
				default:
					break;
				}
				break;
			}
			default:
				break;
			}
			m_msgCont.pop_back();
		}
	}
} //namespace BEngine