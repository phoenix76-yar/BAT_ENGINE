#ifndef WINDOW_DEFINITIONS_H
#define WINDOW_DEFINITIONS_H

#include "WindowMSG.h"

namespace BEngine
{
	enum class WinScreenAlignment
	{
		CENTER,
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_TOP,
		ALIGN_BOTTOM
	};

	union WindowScreenPos
	{
		WinScreenAlignment alignment;
		struct
		{
			uint32 x;
			uint32 y;
		} pos;
	};

	struct WindowConstrInfo
	{
		uint32 winSizeX;
		uint32 winSizeY;
		WindowScreenPos screenPos; // ****** NOTE: Currently unused.
	};

	struct SWindowSizeParameters
	{
		uint16 windowWidth;
		uint16 windowHeiht;
		uint16 clientWidth;
		uint16 clientHeigth;
	};
}

#endif //WINDOW_DEFINITIONS_H
