/***********************************************************************
NONCOPYABLE

Inherit this class to make child class non-copyable.
***********************************************************************/

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace BEngine 
{
	class CNoncopyable
	{
	protected:
		CNoncopyable() = default;
		virtual ~CNoncopyable() = default;

		CNoncopyable(const CNoncopyable&) = delete; // Prevent construction by copying
		CNoncopyable& operator=(const CNoncopyable&) = delete; // Prevent assignment 

		CNoncopyable(const CNoncopyable&&) = delete;
		CNoncopyable& operator=(const CNoncopyable&&) = delete;
	};
} //namespace BEngine
#endif //NONCOPYABLE_H

