/***********************************************************************
WIN32WINDOW

Win32 window implementation.
***********************************************************************/

#ifndef WIN32WINDOW_H
#define WIN32WINDOW_H

#include "IWindow.h"

namespace BEngine 
{
	class CWin32Window : public IWindow
	{
		public:
			CWin32Window(const WindowConstrInfo& cInfo);
			~CWin32Window();

			void initialize() override final;
			void update();
			void shutDown() override;
			
		private:
			void* m_renderWindowClass; //WNDCLASSEX	
			void* m_windowMessage; //MSG
	};
} //namespace BEngine
#endif //WIN32WINDOW_H
