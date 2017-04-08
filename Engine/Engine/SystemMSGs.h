#ifndef SYSTEM_MSGS_H
#define SYSTEM_MSGS_H

#include "EngineDefinitions.h"

namespace BEngine
{
	enum class SysActionType
	{
		EXIT,
		PAUSE, //Usually called when window has been minimized
		CONTINUE, //Called when restored after window has been minimized
		LOAD_LEVEL, //MSG called to load some level (maybe rename to Map ? )
		RELOAD_LEVEL,
	};


	class CSysLoadLevel
	{
	public:
		uint32 hash;
	};

	class CSystemMSG
	{
	public:
		SysActionType action;
		union
		{
			CSysLoadLevel loadLevel;
		} sysMsg;
	};
}

#endif //SYSTEM_MSGS_H
