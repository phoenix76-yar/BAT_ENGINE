/***********************************************************************
SINGLETON


***********************************************************************/

#ifndef SINGLETON_H
#define SINGLETON_H

namespace BEngine 
{
	class CSingleton
	{
	public:
		static CSingleton& Instance()
		{
			static CSingleton singleInstance;
			return singleInstance;
		}

	protected:
		CSingleton() = default; // Prevent construction
		~CSingleton() = default; // Prevent unwanted destruction

		CSingleton(const CSingleton&) = delete; // Prevent construction by copying
		CSingleton& operator=(const CSingleton&) = delete; // Prevent assignment 

		CSingleton(const CSingleton&&) = delete;
		CSingleton& operator=(const CSingleton&&) = delete;
	};
} //namespace BEngine
#endif //SINGLETON_H

