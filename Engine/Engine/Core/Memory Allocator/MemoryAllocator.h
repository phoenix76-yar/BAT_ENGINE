#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include "../../EngineDefinitions.h"
#include "../../EngineContainers.h"

#include <assert.h>

namespace BEngine
{

 	static constexpr uint32 M_1_BYTE = 1;
 	static constexpr uint32 M_1_KILOBYTE = M_1_BYTE * 1024;
 	static constexpr uint32 M_1_MEGABYTE = M_1_KILOBYTE * M_1_KILOBYTE;
 	static constexpr uint32 M_1_GIBABYTE = M_1_MEGABYTE * M_1_KILOBYTE;

	struct MemAllocatConstrInfo
	{
		size_t memBlockSize;
		size_t memBlockCount;
	};

// 	class IMemoryAllocator
// 	{
// 	public:
// 		IMemoryAllocator() = default;
// 		virtual ~IMemoryAllocator() = default;
// 
// 		virtual void* allocate(size_t size) = 0;
// 		virtual void* allocate(size_t size, size_t lenght) = 0;
// 		virtual void deallocate(void* ptr) = 0;
// 	};

	//-----------------------------------------------------------------------
	//Class Memory Allocator  (Simple memory allocator)
	//
	//The only thing which is not implemented in this allocator is that
	//it has limited memory (pre-allocated), make changes so it would be possible
	//to allocate extra.
	//-----------------------------------------------------------------------

	class CMemoryAllocator
	{
	public:
		CMemoryAllocator(const MemAllocatConstrInfo&);
		virtual ~CMemoryAllocator();

		void* allocate(size_t size);
		void* allocate(size_t size, size_t lenght);
		void deallocate(void* ptr);
	protected:
		uint32 m_allocationCount;
		uint32 m_allocationRequestCount;
		uint32 m_deallocationRequestCount;

		uint64 m_totalMemAllocatedPerBlock;
		uint64 m_totalMemAllocated;

		size_t m_memBlockSize;
		size_t m_allocatedMemBlocksByDefault;

		void* m_startMemPtr;
		void* m_currentMemPtr;

		blist<void*> m_unusedMemFields;
	};

	//-----------------------------------------------------------------------
	//Allocation template methods
	//-----------------------------------------------------------------------

	template <class T>
	T* allocateNew(CMemoryAllocator& allocator)
	{
		return new (allocator.allocate(sizeof(T))) T;
	}

	template <class T>
	T* allocateNew(CMemoryAllocator& allocator, const T& t)
	{
		return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
	}

	template<class T>
	T* allocateArray(CMemoryAllocator& allocator, size_t length)
	{
		assert(length != 0);

		uint32 headerSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0)
			headerSize += 1;

		//Add extra space to store array length in the bytes before the array
		T* p = ((T*)allocator.allocate(sizeof(T) * (length + headerSize), __alignof(T))); // + headerSize;

		//*(((size_t*)p) - 1) = length;

		for (size_t i = 0; i < length; i++)
		{
			new (&p[i]) T;
		}

		return p;
	}

	//-----------------------------------------------------------------------
	//Deallocation template methods
	//-----------------------------------------------------------------------

	template<class T>
	void deallocateDelete(CMemoryAllocator& allocator, T& object)
	{
		object.~T();
		allocator.deallocate(&object); // ****** NOTE: Might cause an error, if input is void* ptr.
	}
}

#endif //MEMORY_ALLOCATOR_H
