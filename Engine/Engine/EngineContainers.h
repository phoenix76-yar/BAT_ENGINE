#ifndef ENGINE_CONTAINERS_H
#define ENGINE_CONTAINERS_H

#define USE_EASTL

#ifdef USE_EASTL
#include <EASTL/vector.h>
#include <EASTL/sort.h>
#include <EASTL/algorithm.h>
#include <EASTL/list.h>
#include <EASTL/map.h>
#include <EASTL/unordered_map.h>
#include <EASTL/set.h>

#include <EASTL/functional.h>
#include <EASTL/utility.h>
#else
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#endif

#ifdef USE_EASTL
void * operator new[](size_t size, const char * pName, int flags, unsigned debugFlags, const char * file, int line);
void * operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char * pName, int flags, unsigned debugFlags, const char * file, int line);
void operator delete[](void * ptr, const char * pName, int flags, unsigned debugFlags, const char * file, int line);
void operator delete[](void * ptr, size_t alignment, size_t alignmentOffset, const char * pName, int flags, unsigned debugFlags, const char * file, int line);
#endif

namespace BEngine
{
	template<class T>
#ifdef USE_EASTL
	using bvector = eastl::vector<T>;
#else
	using bvector = std::vector<T>;
#endif

	template<class T>
#ifdef USE_EASTL
	using blist = eastl::list<T>;
#else
	using blist = std::list<T>;
#endif //USE_EASTL

	template<class Key, class ValType>
#ifdef USE_EASTL
	using bmap = eastl::map<Key, ValType>;
#else
	using bmap = std::map<Key, ValType>;
#endif //USE_EASTL

	template<class Key, class ValType>
#ifdef USE_EASTL
	using bunordered_map = eastl::unordered_map<Key, ValType>;
#else
	using bunordered_map = std::unordered_map<Key, ValType>;
#endif //USE_EASTL

	template<class Type1, class Type2>
#ifdef USE_EASTL
	using bpair = eastl::pair<Type1, Type2>;
#else
	using bpair = std::pair<Type1, Type2>;
#endif //USE_EASTL

	template<class Key, class ValType>
#ifdef USE_EASTL
	using bset = eastl::set<Key, ValType>;
#else
	using bset = std::set<Key, ValType>;
#endif //USE_EASTL

	// ****** NOTE: Use with care, do not use for multi threaded code (not thread safe).
	template<class T>
	void deleteContainerData(T& cont)
	{
		while (!cont.empty()) 
		{
			if (cont.back()) 
			{
				delete cont.back();
			}
			cont.pop_back();
		}
	}

}

#endif //ENGINE_CONTAINERS_H