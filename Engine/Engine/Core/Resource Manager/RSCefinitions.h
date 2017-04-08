#ifndef RSC_DEFS_H
#define RSC_DEFS_H

#include "../../EngineDefinitions.h"
#include <atomic>

namespace BEngine
{
	//-----------------------------------------------------------------------
	//CResourceObject
	//
	//Basic representation of a file inside a RAM, used to create objects.
	//Protected member can be only modified by RSCManager or it's subsystem.
	//This object then could be used to create other objects ?
	//-----------------------------------------------------------------------

	// ****** TODO: Rename to RSCObjectStatus ?
	enum class RSCLoadResult
	{
		EMPTY,
		LOADED_OK,
		NOT_FOUND,
		LOAD_ERROR
	};

	class CResourceObject
	{
		friend class CRSCManager;
		friend class ITask;
		friend class CRSCSimpleLoadTask;
		//friend class FileDevice;
	public:
		CResourceObject()
		{
			m_loadResult = RSCLoadResult::EMPTY;
		}

		~CResourceObject() = default;

		uint64 getBufferSize() const { return m_bufferSize; }
		void* getBufferPtr() const { return m_bufferPtr; }

		RSCLoadResult getRSCResult() const { return m_loadResult; }
		uint32 getInstanceCount() const { return m_instanceCount; }
	protected:
		uint32 m_instanceCount; //How many instances point to this object
		uint64 m_hash;
		uint64 m_bufferSize;
		void* m_bufferPtr;
		RSCLoadResult m_loadResult;
	};

}

#endif //RSC_DEFS_H
