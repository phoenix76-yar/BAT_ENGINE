#ifndef ENGINE_DEFINITIONS_H
#define ENGINE_DEFINITIONS_H

#include "EngineTypes.h"

namespace BEngine
{
	static constexpr char* ENGINE_NAME = "Base Engine";
	static constexpr int ENGINE_MAJOR_VERSION = 0;
	static constexpr int ENGINE_MINOR_VERSION = 1;

#if defined(__MINGW32__) || defined(__CYGWIN__) || (defined (_MSC_VER) && _MSC_VER < 1300)
#define B_FORCE_INLINE inline
#define B_ATTRIBUTE_ALIGNED16(a) a
#define B_ATTRIBUTE_ALIGNED64(a) a
#define B_ATTRIBUTE_ALIGNED128(a) a
#else
#pragma warning(disable : 4324)
#define B_FORCE_INLINE __forceinline
#define B_ATTRIBUTE_ALIGNED16(a) __declspec(align(16)) a
#define B_ATTRIBUTE_ALIGNED64(a) __declspec(align(64)) a
#define B_ATTRIBUTE_ALIGNED128(a) __declspec (align(128)) a
#endif

	enum class BEngineSystemState
	{
		PAUSED,
		RUNNING,
		INITIALIZING,
		SHUTTING_DOWN
	};

}

#endif //ENGINE_DEFINITIONS_H