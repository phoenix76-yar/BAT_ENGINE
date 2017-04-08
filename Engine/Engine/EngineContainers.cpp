#include "EngineContainers.h"

#ifdef USE_EASTL
void * operator new [](size_t size, const char * pName, int flags, unsigned debugFlags, const char * file, int line)
{
	return operator new[](size);
}
void * operator new [](size_t size, size_t alignment, size_t alignmentOffset, const char * pName, int flags, unsigned debugFlags, const char * file, int line)
{
	// Does not support alignment
	return operator new[](size);
}
void operator delete [](void * ptr, const char * pName, int flags, unsigned debugFlags, const char * file, int line)
{
	operator delete[](ptr);
}
void operator delete [](void * ptr, size_t alignment, size_t alignmentOffset, const char * pName, int flags, unsigned debugFlags, const char * file, int line)
{
	// Does not support alignment
	operator delete[](ptr);
}
#endif