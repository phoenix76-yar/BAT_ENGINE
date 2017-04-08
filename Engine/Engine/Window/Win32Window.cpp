#include "Win32Window.h"
#include <windows.h>
#include <thread>
#include <memory>

#include "../Core/Event System/EventManager.h"
#include "../SystemMSGs.h"

namespace BEngine
{
	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	TCHAR windowClassName[] = "RenderWindow";
#ifdef _DEBUG
#ifdef ENGINE_VERSION_X64
	TCHAR rWindowName[] = "Engine Debug x64";
#else
	TCHAR rWindowName[] = "Engine Debug";
#endif
	
#else
#ifdef ENGINE_VERSION_X64
	TCHAR rWindowName[] = "Engine x64";
#else
	TCHAR rWindowName[] = "Engine";
#endif
#endif

	CWin32Window::CWin32Window(const WindowConstrInfo& cInfo)
	{
		m_isInitialized = false;
		m_width = cInfo.winSizeX;
		m_height = cInfo.winSizeY;
	}

	CWin32Window::~CWin32Window()
	{
		UnregisterClass(windowClassName, (HINSTANCE)m_hInstance);
		if (!DestroyWindow((HWND)m_windowHandle))
		{
			MessageBoxA((HWND)m_windowHandle, "Couldn't destroy window", "Error", MB_ICONEXCLAMATION | MB_OK);
		}
		m_isInitialized = false;
	}

	static bool isOnlyInstance(LPCTSTR windowClassName) 
	{
		HANDLE handle = CreateMutex(NULL, TRUE, windowClassName);
		if (GetLastError() != ERROR_SUCCESS) 
		{
			HWND hWnd = FindWindow(windowClassName, NULL);
			if (hWnd) 
			{
				// An instance of your program is already running. 
				int result = MessageBoxA(NULL, "There is already one instance running!", "Information", MB_ICONEXCLAMATION | MB_OK);
				if (result) 
				{
					ShowWindow(hWnd, SW_SHOWNORMAL);
					SetFocus(hWnd);
					SetForegroundWindow(hWnd);
					SetActiveWindow(hWnd);
				}
				return false;
			}
		}
		return true;
	}

	void CWin32Window::initialize()
	{
		//if (!isOnlyInstance(windowClassName)) { exit(0); }

		if (!m_isInitialized)
		{
			m_hInstance = GetModuleHandle(NULL);
			WNDCLASSEX renderWindowClass; // = (*(WNDCLASSEX*)m_renderWindowClass);
			renderWindowClass.cbSize = sizeof(WNDCLASSEX);
			renderWindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW; //0;
			renderWindowClass.lpfnWndProc = MessageHandler; //(WNDPROC)(&Window::WndProc, this);
			renderWindowClass.cbClsExtra = 0;
			renderWindowClass.cbWndExtra = 0;
			renderWindowClass.hInstance = NULL;
			//renderWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			renderWindowClass.hIcon = NULL; //Task bar icon.
			renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW); //IDC_HAND
			renderWindowClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH); //(HBRUSH)(COLOR_WINDOW + 1);
			renderWindowClass.lpszMenuName = NULL;
			renderWindowClass.lpszClassName = windowClassName;
			renderWindowClass.hIconSm = NULL;

			m_renderWindowClass = &renderWindowClass;

			if (!RegisterClassEx(&renderWindowClass)) //&(*(WNDCLASSEX*)m_renderWindowClass))
			{
				MessageBoxA(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
				//return EResult::E_ERROR;
			}

			m_windowHandle = CreateWindowEx(WS_EX_DLGMODALFRAME, windowClassName, rWindowName,
				WS_OVERLAPPEDWINDOW, 0, 0, m_width, m_height, NULL, NULL, (HINSTANCE)m_hInstance, NULL);

			if (m_windowHandle == NULL)
			{
				//return EResult::E_ERROR;
			}

			RECT winRect;
			GetWindowRect((HWND)m_windowHandle, &winRect);
			int xPos = (GetSystemMetrics(SM_CXSCREEN) - winRect.right) / 2;
			int yPos = (GetSystemMetrics(SM_CYSCREEN) - winRect.bottom) / 2;
			m_width = winRect.right - winRect.left;
			m_height = winRect.bottom - winRect.top;
			SetWindowPos((HWND)m_windowHandle, 0, xPos, yPos, m_width, m_height, NULL);

			ShowWindow((HWND)m_windowHandle, 1);
			UpdateWindow((HWND)m_windowHandle);

			m_isInitialized = true;
			//return EResult::OK;
		}
	}

	// ****** TODO: Not sure if this is a correct way to do it
	void CWin32Window::update()
	{
		MSG m_message;
		while (m_isInitialized.load())
		{
			if (PeekMessage(&m_message, NULL, 0, 0, PM_REMOVE))
			{
				if (m_message.message == WM_QUIT)
				{
					return;
				}
				else
				{
					TranslateMessage(&m_message);
					DispatchMessage(&m_message);
				}
			
			}
 			else //Nothing to do, no other messages (can sleep a bit)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(10));
				/*Render->draw()*/
			}
		}

		if (m_isInitialized == false)
		{
			PostQuitMessage(0); //To terminate the window's update loop
		}
	}

	void CWin32Window::shutDown()
	{
		m_isInitialized = false;
		
		//DestroyWindow((HWND)m_windowHandle);
	}

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CLOSE:
		{
			CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::System);
			CSystemMSG sysMsg;
			sysMsg.action = SysActionType::EXIT;
			memcpy(msg.userDataBuffer, &sysMsg, sizeof(CSystemMSG));
			EventManager.submitMessage(msg);
			//PostQuitMessage(0);
			break;
		}
		case WA_ACTIVE:
		{

			break;
		}		
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{		
				RECT winRect;
				RECT clientRect;
				GetWindowRect(hWnd, &winRect);
				GetClientRect(hWnd, &clientRect);
				int winWidth = winRect.right - winRect.left;
				int winHeight = winRect.bottom - winRect.top;

				int clientWidth = clientRect.right - clientRect.left;
				int clientHeight = clientRect.bottom - clientRect.top;


				CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
				CWindowMSG windowMSG;
				windowMSG.msgType = WindowMSGTypes::WINDOW_RESIZE;
				windowMSG.winMsg.winResize.width = winWidth;
				windowMSG.winMsg.winResize.height = winHeight;
				windowMSG.winMsg.winResize.clientWidth = clientWidth;
				windowMSG.winMsg.winResize.cleintHeight = clientHeight;
				memcpy(msg.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
				EventManager.submitMessage(msg);
			}

			if (wParam == SIZE_MINIMIZED)
			{
				//We need to pause renderer otherwise application will crash
				CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::System);
				CSystemMSG sysMsg;
				sysMsg.action = SysActionType::PAUSE;
				memcpy(msg.userDataBuffer, &sysMsg, sizeof(CSystemMSG));
				EventManager.submitMessage(msg);

				CEventMSG msg2(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
				CWindowMSG windowMSG;
				windowMSG.msgType = WindowMSGTypes::USER_INPUT;
				windowMSG.winMsg.userInput.inDevice = InputDevice::ID_MOUSE;
				windowMSG.winMsg.userInput.inAction = InputAction::IA_WINDOW_MINMIZED;
				memcpy(msg2.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
				EventManager.submitMessage(msg2);
				break;
			}
			else if (wParam == SIZE_RESTORED)
			{
				CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::System);
				CSystemMSG sysMsg;
				sysMsg.action = SysActionType::CONTINUE;
				memcpy(msg.userDataBuffer, &sysMsg, sizeof(CSystemMSG));
				EventManager.submitMessage(msg);

				CEventMSG msg2(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
				CWindowMSG windowMSG;
				windowMSG.msgType = WindowMSGTypes::USER_INPUT;
				windowMSG.winMsg.userInput.inDevice = InputDevice::ID_MOUSE;
				windowMSG.winMsg.userInput.inAction = InputAction::IA_WINDOW_RESTORED;
				memcpy(msg2.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
				EventManager.submitMessage(msg2);
				break;
			}		
			break;
		}
		case WM_KEYDOWN:
		{
			uint32 mappedKeyID = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
			if (mappedKeyID == 27) //Escape button
			{
				CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::System);
				CSystemMSG sysMsg;
				sysMsg.action = SysActionType::EXIT;
				memcpy(msg.userDataBuffer, &sysMsg, sizeof(CSystemMSG));
				EventManager.submitMessage(msg);
				//PostQuitMessage(0);
			}
			CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
			CWindowMSG windowMSG;
			windowMSG.msgType = WindowMSGTypes::USER_INPUT;
			windowMSG.winMsg.userInput.inDevice = InputDevice::ID_KEYBOARD;
			windowMSG.winMsg.userInput.inAction = InputAction::IA_KEY_DOWN;

			if (mappedKeyID == 88) //x
			{
				windowMSG.winMsg.userInput.inKeyID = KeyID::KEY_X;
			}

			memcpy(msg.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
			EventManager.submitMessage(msg);
			break;
		}
		case WM_KEYUP:
		{
			uint32 mappedKeyID = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
			CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
			CWindowMSG windowMSG;
			windowMSG.msgType = WindowMSGTypes::USER_INPUT;
			windowMSG.winMsg.userInput.inDevice = InputDevice::ID_KEYBOARD;
			windowMSG.winMsg.userInput.inAction = InputAction::IA_KEY_UP;

			if (mappedKeyID == 88) //x
			{
				windowMSG.winMsg.userInput.inKeyID = KeyID::KEY_X;
			}

			memcpy(msg.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
			EventManager.submitMessage(msg);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			
			CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
			CWindowMSG windowMSG;
 			windowMSG.msgType = WindowMSGTypes::USER_INPUT;
 			windowMSG.winMsg.userInput.inDevice = InputDevice::ID_MOUSE;
			windowMSG.winMsg.userInput.inAction = InputAction::IA_MOUSE_WHEEL_SCROLL_DOWN;
			if (zDelta > 1)
			{
				windowMSG.winMsg.userInput.inAction = InputAction::IA_MOUSE_WHEEL_SCROLL_UP;
			}	
			memcpy(msg.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
			EventManager.submitMessage(msg);
			break;
		}
		case WM_MOUSEMOVE:
		{
			int iPosX = LOWORD(lParam);
			int iPosY = HIWORD(lParam);
			int fwKeys = GET_KEYSTATE_WPARAM(wParam);
			CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
			CWindowMSG windowMSG;
			windowMSG.msgType = WindowMSGTypes::USER_INPUT;
			windowMSG.winMsg.userInput.inDevice = InputDevice::ID_MOUSE;
			windowMSG.winMsg.userInput.inAction = InputAction::IA_MOUSE_MOVE;
			windowMSG.winMsg.userInput.mousePosX = iPosX;
			windowMSG.winMsg.userInput.mousePosY = iPosY;
			memcpy(msg.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
			EventManager.submitMessage(msg);
			if (fwKeys == MK_MBUTTON)
			{
				
				
			}
			else
			{
				
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			int iPosX = LOWORD(lParam);
			int iPosY = HIWORD(lParam);
			CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
			CWindowMSG windowMSG;
			windowMSG.msgType = WindowMSGTypes::USER_INPUT;
			windowMSG.winMsg.userInput.inDevice = InputDevice::ID_MOUSE;
			windowMSG.winMsg.userInput.inAction = InputAction::IA_KEY_DOWN;
			windowMSG.winMsg.userInput.mousePosX = iPosX;
			windowMSG.winMsg.userInput.mousePosY = iPosY;
			windowMSG.winMsg.userInput.inKeyID = KeyID::MOUSE_RBUTTON;
			memcpy(msg.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
			EventManager.submitMessage(msg);

			break;
		}
		case WM_LBUTTONDOWN:
		{
			int iPosX = LOWORD(lParam);
			int iPosY = HIWORD(lParam);
			CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
			CWindowMSG windowMSG;
			windowMSG.msgType = WindowMSGTypes::USER_INPUT;
			windowMSG.winMsg.userInput.inDevice = InputDevice::ID_MOUSE;
			windowMSG.winMsg.userInput.inAction = InputAction::IA_KEY_DOWN;
			windowMSG.winMsg.userInput.mousePosX = iPosX;
			windowMSG.winMsg.userInput.mousePosY = iPosY;
			windowMSG.winMsg.userInput.inKeyID = KeyID::MOUSE_LBUTTON;
			memcpy(msg.userDataBuffer, &windowMSG, sizeof(CWindowMSG));
			EventManager.submitMessage(msg);

			break;
		}
		case WM_COMMAND:
		{
			//Catch exit press here
			CEventMSG msg(MSGPriorityLevel::HIGH, EventMSGType::UserInput);
			EventManager.submitMessage(msg);
			break;
		}
		case WM_DESTROY:
			//Now unload openGLX stuff
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
} //namespace BEngine