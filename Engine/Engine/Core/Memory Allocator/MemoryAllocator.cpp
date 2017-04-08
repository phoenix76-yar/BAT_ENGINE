#include "MemoryAllocator.h"
#include <iostream>

namespace BEngine
{
	CMemoryAllocator::CMemoryAllocator(const MemAllocatConstrInfo& cInfo)
	{
		m_allocationCount = 0;
		m_allocationRequestCount = 0;
		m_deallocationRequestCount = 0;
		m_totalMemAllocatedPerBlock = 0;
		m_totalMemAllocated = 0;

		m_memBlockSize = cInfo.memBlockSize;
		m_allocatedMemBlocksByDefault = cInfo.memBlockCount;
		size_t totalMemToAllocate = m_memBlockSize * m_allocatedMemBlocksByDefault;
		std::cout << "Total Mem. Allocated: " << (totalMemToAllocate / M_1_MEGABYTE) << " mb" << std::endl;
		m_startMemPtr = malloc(totalMemToAllocate);
		m_currentMemPtr = m_startMemPtr;
		m_totalMemAllocatedPerBlock = totalMemToAllocate;
		m_totalMemAllocated += totalMemToAllocate;
	}

	CMemoryAllocator::~CMemoryAllocator()
	{
		m_unusedMemFields.clear();
		free(m_startMemPtr);
		if (m_allocationRequestCount != m_deallocationRequestCount)
		{
			std::cout << "Memory leak detected!\n";
		}
		std::cout << "Memory Manager: Shutting Down\n";
	}

	void* CMemoryAllocator::allocate(size_t size)
	{
		assert( m_totalMemAllocated > size && "There isn't enough space to allocate for this object!");
		m_allocationCount++;
		m_allocationRequestCount++;
		std::cout << "Memory Manager: Allocation Requested:" << size << std::endl;

		//Firstly see if there is already an empty field available that has enough space for required size/lenght
		for (auto it = m_unusedMemFields.begin(); it != m_unusedMemFields.end(); ++it)
		{
			uint32 availableSize = *(uint32*)((uchar*)(*it) - 4);
			if (availableSize >= size)
			{
				std::cout << "Found Some Empty Space\n";
				//We are good we can return this ptr for use
				//But before we do this, remove this from the list

				void* returnPtr = (*it);
				//*(((uint32*)returnPtr - 4)) = size; // ****** NOTE: We can't currently overwrite original allocated space.
				it = m_unusedMemFields.erase(it);

				//So here as we erase one element, we are supposed to construct a new ptr which starts after the mem. location and point after it's size.
				//and add it to the list, kind of deciding address into chunks, there could be another method that combines chunks (fragmentation)

				if (it == m_unusedMemFields.end() && m_unusedMemFields.size() == 0)
					return returnPtr;
				else
					it = m_unusedMemFields.begin();
				//Return
				return returnPtr;
			}
		}

		//If there isn't any empty field at that size

		//Firstly check if there is enough space in this block for another object

		uint64 adress = reinterpret_cast<uint64>(m_currentMemPtr) + size;
		uint64 adress2 = reinterpret_cast<uint64>(m_startMemPtr) + m_totalMemAllocatedPerBlock;
		

		if (adress < adress2)
		{
			//memcpy(m_currentMemPtr, &size, 4); //Instead better assign directly
			*(((uint32*)m_currentMemPtr)) = size;
			void* ptr = (void*)((size_t)m_currentMemPtr + 4);
			m_currentMemPtr = (void*)((size_t)ptr + size);
			return ptr;
		}
		else
		{
			// ****** TODO: Allocate another MemBlock and use it
			assert(adress < adress2 && "There isn't any more space for other object left.");
			return nullptr; // ****** TODO: Needs to be changed !!!
		}
		
	}

	void* CMemoryAllocator::allocate(size_t size, size_t lenght)
	{
		assert(m_totalMemAllocated > size && "There isn't enough space to allocate for this object!");
		m_allocationCount++;
		m_allocationRequestCount++;
		std::cout << "Memory Manager: Allocation Requested:" << size << std::endl;

		//Firstly see if there is already an empty field available that has enough space for required size/lenght
		for (auto it = m_unusedMemFields.begin(); it != m_unusedMemFields.end(); ++it)
		{
			uint32 availableSize = *(uint32*)((uchar*)(*it) - 4);
			if (availableSize >= size)
			{
				std::cout << "Found Some Empty Space\n";
				//We are good we can return this ptr for use
				//But before we do this, remove this from the list
				//*(((uint32*)(*it) - 4)) = size; // ****** NOTE: We can't currently overwrite original allocated space.
				void* returnPtr = (*it);
				it = m_unusedMemFields.erase(it);
				if (it == m_unusedMemFields.end() && m_unusedMemFields.size() == 0)
					return returnPtr;
				else
					it = m_unusedMemFields.begin();
				//Return
				return returnPtr;
			}
		}

		//If there isn't any empty field at that size


		uint64 adress = reinterpret_cast<uint64>(m_currentMemPtr) + size;
		uint64 adress2 = reinterpret_cast<uint64>(m_startMemPtr) + m_totalMemAllocatedPerBlock;
		


		if (adress < adress2)
		{
			//memcpy(m_currentMemPtr, &size, 4);
			*(((uint32*)m_currentMemPtr)) = size;

			void* ptr = (void*)((size_t)m_currentMemPtr + 4);
			m_currentMemPtr = (void*)((size_t)ptr + size);
			return ptr;
		}
		else
		{
			// ****** TODO: Allocate another MemBlock and use it
			assert(adress < adress2 && "There isn't any more space for other object left.");
			return nullptr; // ****** TODO: Needs to be changed !!!
		}
	}

	void CMemoryAllocator::deallocate(void* ptr)
	{
		m_allocationCount--;
		m_deallocationRequestCount++;
		//Technically we just need to mark that the region is empty an zero out all it's values.
		std::cout << "Memory Manager: Deallocation Requested" << std::endl;
		uint32 size = *(uint32*)((uchar*)ptr - 4);
		std::cout << size << " bytes released." << std::endl;
		memset(ptr, 0x00, size);
		m_unusedMemFields.push_back(ptr);
	}
}