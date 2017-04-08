/***********************************************************************
IWINDOW

Base abstract window class, to be used for implementing different
platform window classes.
***********************************************************************/

#ifndef IWINDOW_H
#define IWINDOW_H

#include "WindowDefinitions.h"

#include <atomic>

namespace BEngine 
{	
	class IWindow
	{
	public:
		IWindow() = default;
		virtual ~IWindow() = default;

		bool isInitialized() const { return m_isInitialized; }

		virtual void initialize() = 0;
		//virtual bool isWindowRunning() const = 0; // ****** TODO: Decide if it is really needed.
 		virtual void update() = 0;
 		virtual void shutDown() = 0;

		void* getWindowHandle() const { return m_windowHandle; }
		void* getHInstance() const { return m_hInstance; }
		void* getHandleDeviceContext() const { return m_hangleDeviceContext; }
		void* getHandleRenderingContext() const { return m_handleRenderingContext; }
	
		uint32 getWidth() const { return m_width; }
		uint32 getHeight() const { return m_height; }

// 		virtual uint32 getWidth() const = 0;
// 		virtual uint32 getHeight() const = 0;
// 		virtual uint32 getClientWidth() const = 0;
// 		virtual uint32 getClientHeight() const = 0;
	protected:
		std::atomic<bool> m_isInitialized;

		uint32 m_width;
		uint32 m_height;

		void* m_hInstance; //HINSTANCE
		void* m_windowHandle; //HWND
		void* m_hangleDeviceContext; //HDC
		void* m_handleRenderingContext; //HGLRC
	};
} //namespace BEngine
#endif //IWINDOW_H


