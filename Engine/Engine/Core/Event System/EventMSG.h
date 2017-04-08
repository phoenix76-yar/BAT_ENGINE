/***********************************************************************
EVENT MESSAGE

http://seanmiddleditch.com/dangers-of-stdshared_ptr/
***********************************************************************/

#ifndef EVENTMSG_H
#define EVENTMSG_H

namespace BEngine
{
	enum class EventMSGType : uint16
	{
		System,
		Game,
		ResourceData,
		UserInput, //Keyboard input, mouse input
		PhysicsData,
		GraphicsData,
		AudioData,
		ScriptData,
		NetworkData,
		CameraData,
		EntityData
	};

	enum class MSGPriorityLevel : uchar
	{
		HIGH,
		MEDIUM,
		LOW
	};

	struct EventMSGConstrInfo
	{
		MSGPriorityLevel pLevel;
		EventMSGType msgType;
		bool processBeforeFrameStart;
	};

	// ****** TODO: Make this class aligned to 64 or 128 bytes !
	class CEventMSG
	{
	public:
		CEventMSG(const EventMSGConstrInfo& cInfo) 
		{
			m_prLevel = cInfo.pLevel;
			m_msgType = cInfo.msgType;
			m_processBeforeFrameStart = cInfo.processBeforeFrameStart;
		}

		CEventMSG(MSGPriorityLevel pLevel, EventMSGType msgType, 
			bool processBeforeFrameStart = true)
		{
			m_prLevel = pLevel;
			m_msgType = msgType;
			m_processBeforeFrameStart = processBeforeFrameStart;
		}

		~CEventMSG() = default;

		EventMSGType m_msgType;
		MSGPriorityLevel m_prLevel;
		bool m_processBeforeFrameStart;
		uchar userDataBuffer[64]; // ****** NOTE: If there isn't enough space for message data increase the size.
	};
}

#endif //EVENTMSG_H