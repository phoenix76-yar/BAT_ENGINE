#include "AudioManager.h"

#include "AudioMSGs.h"

namespace BEngine
{
	namespace BAudio
	{
		void CAudioManager::processInputMSGs(const bvector<CEventMSG>& msgCont)
		{
			while (!m_msgCont.empty())
			{
				switch (m_msgCont.back().m_msgType)
				{
				case EventMSGType::AudioData:
				{
					auto inMsg = m_msgCont.back();
					processAudioMSG(inMsg);
					break;
				}
				default:
					break;
				}
				m_msgCont.pop_back();
			}
		}

		void CAudioManager::processAudioMSG(const CEventMSG& inMsg)
		{
			auto userData = inMsg.userDataBuffer;
			auto inputData = *(CAudioMSG*)userData;

			// ****** TODO: Do something here ...
			switch (inputData.msgType)
			{
			case  AudioMSGType::PLAY_SOUND:
			{
				// ****** TODO: Play sound
				break;
			}
			default:
				break;
			}
		}
	} //namespace BAudio
} //namespace BEngine