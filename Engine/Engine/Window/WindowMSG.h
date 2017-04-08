#ifndef WINDOW_MSG_H
#define WINDOW_MSG_H

#include "../EngineDefinitions.h"

namespace BEngine
{
	enum class KeyID : uint16
	{
		KEY_Escape = 1,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_0,
		KEY_Minus,
		KEY_Equals,
		KEY_Backspace,
		KEY_Tab,
		KEY_Q,
		KEY_W,
		KEY_E,
		KEY_R,
		KEY_T,
		KEY_Y,
		KEY_U,
		KEY_I,
		KEY_O,
		KEY_P,
		KEY_LBracket,
		KEY_RBracket,
		KEY_Enter,
		KEY_LCtrl,
		KEY_A,
		KEY_S,
		KEY_D,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_Semicolon,
		KEY_Apostrophe,
		KEY_Tilde,
		KEY_LShift,
		KEY_Backslash,
		KEY_Z,
		KEY_X,
		KEY_C,
		KEY_V,
		KEY_B,
		KEY_N,
		KEY_M,
		KEY_Comma,
		KEY_Period,
		KEY_Slash,
		KEY_RShift,
		KEY_NP_Multiply,
		KEY_LAlt,
		KEY_Space,
		KEY_CapsLock,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_NumLock,
		KEY_ScrollLock,
		KEY_NP_7,
		KEY_NP_8,
		KEY_NP_9,
		KEY_NP_Substract,
		KEY_NP_4,
		KEY_NP_5,
		KEY_NP_6,
		KEY_NP_Add,
		KEY_NP_1,
		KEY_NP_2,
		KEY_NP_3,
		KEY_NP_0,
		KEY_F11,
		KEY_F12,
		KEY_F13,
		KEY_F14,
		KEY_F15,
		KEY_Colon,
		KEY_Underline,
		KEY_NP_Enter,
		KEY_RCtrl,
		KEY_NP_Period,
		KEY_NP_Divide,
		KEY_Print,
		KEY_RAlt,
		KEY_Pause,
		KEY_Home,
		KEY_Up,
		KEY_PgUp,
		KEY_Left,
		KEY_Right,
		KEY_End,
		KEY_Down,
		KEY_PgDn,
		KEY_Insert,
		KEY_Delete,
		KEY_LWin,
		KEY_RWin,
		KEY_Apps,
		KEY_OEM_102,
		MOUSE_RBUTTON,
		MOUSE_LBUTTON,
		MOUSE_SCROLL,
	};

	enum class InputDevice : uchar
	{
		ID_KEYBOARD,
		ID_MOUSE,
		ID_JOYSTIC,
		ID_CONTROLLER
	};

	enum class InputAction : uchar
	{
		IA_KEY_DOWN,
		IA_KEY_UP,
		IA_MOUSE_MOVE,
		IA_MOUSE_WHEEL_SCROLL_UP,
		IA_MOUSE_WHEEL_SCROLL_DOWN,
		IA_WINDOW_MINMIZED,
		IA_WINDOW_RESTORED //Unminimized
	};

	// ****** NOTE: Somehow it will be necessary also to sort out controller inputs.
	class CUserInputMSG
	{
	public:
		KeyID inKeyID; // ****** TODO: uint16 to be changed with KeyID enum
		InputDevice inDevice;
		InputAction inAction;
		uint16 mousePosX;
		uint16 mousePosY;
	};

	class CWindowResizeMSG
	{
	public:
		uint16 width;
		uint16 height;
		uint16 clientWidth;
		uint16 cleintHeight;
	};

	enum class WindowMSGTypes : uchar
	{
		USER_INPUT,
		WINDOW_RESIZE,
		WINDOW_MOVE
	};

#ifdef _DEBUG
#include <iostream>
#endif

	class CWindowMSG
	{
	public:
		union WindowMsg
		{
			CUserInputMSG userInput;
			CWindowResizeMSG winResize;
		};

		CWindowMSG() = default;
#ifdef _DEBUG
		~CWindowMSG()
		{
			//std::cout << "Win MSG Destruct\n";
		}
#else
		~CWindowMSG() = default;
#endif

		WindowMSGTypes msgType;
		WindowMsg winMsg;
	};

}

#endif // WINDOW_MSG_H