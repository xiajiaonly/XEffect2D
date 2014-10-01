#ifndef _JIA_XMOUSEANDKEYBOARDDEFINE_
#define _JIA_XMOUSEANDKEYBOARDDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
//下面是关于鼠标和键盘的一些公共宏的定义
//enum _XMouseState
//{
//	MOUSE_STATE_MOVE,		//鼠标移动
//	MOUSE_STATE_DOWN,		//鼠标按下
//	MOUSE_STATE_UP			//鼠标弹起
//};
//enum _XMouseKeyOrder
//{
//	MOUSE_KEY_ORDER_LEFT,		//鼠标左键
//	MOUSE_KEY_ORDER_MIDDLE,		//鼠标中键
//	MOUSE_KEY_ORDER_RIGHT,		//鼠标右键
//	MOUSE_KEY_ORDER_NULL		//没有按键
//};
enum _XKeyState
{
	KEY_STATE_DOWN,		//按键按下
	KEY_STATE_UP		//按键弹起
};
enum _XMouseState
{
	MOUSE_MOVE,					//鼠标移动
	MOUSE_LEFT_BUTTON_DOWN,		//鼠标左键按下
	MOUSE_LEFT_BUTTON_UP,		//鼠标左键弹起
	MOUSE_RIGHT_BUTTON_DOWN,	//鼠标右键按下
	MOUSE_RIGHT_BUTTON_UP,		//鼠标右键弹起
	MOUSE_MIDDLE_BUTTON_DOWN,	//鼠标中键按下
	MOUSE_MIDDLE_BUTTON_UP,		//鼠标中键弹起
	MOUSE_WHEEL_UP_DOWN,		//鼠标滚轮向上按钮按下
	MOUSE_WHEEL_UP_UP,			//鼠标滚轮向上按钮弹起
	MOUSE_WHEEL_DOWN_DOWN,		//鼠标滚轮向下按钮按下
	MOUSE_WHEEL_DOWN_UP,		//鼠标滚轮向下按钮弹起
	MOUSE_LEFT_BUTTON_DCLICK,	//左键双击
};
#include "SDL.h"
enum _XKeyValue
{
	//普通按键
	XKEY_A,	//字母按键
	XKEY_B,
	XKEY_C,
	XKEY_D,
	XKEY_E,
	XKEY_F,
	XKEY_G,
	XKEY_H,
	XKEY_I,
	XKEY_J,
	XKEY_K,
	XKEY_L,
	XKEY_M,
	XKEY_N,
	XKEY_O,
	XKEY_P,
	XKEY_Q,
	XKEY_R,
	XKEY_S,
	XKEY_T,
	XKEY_U,
	XKEY_V,
	XKEY_W,
	XKEY_X,
	XKEY_Y,
	XKEY_Z,

	XKEY_0,	//数字按键
	XKEY_1,
	XKEY_2,
	XKEY_3,
	XKEY_4,
	XKEY_5,
	XKEY_6,
	XKEY_7,
	XKEY_8,
	XKEY_9,

	XKEY_UNKNOWN,//SDLK_FIRST
	XKEY_BACKSPACE,
	XKEY_TAB,
	XKEY_CLEAR,
	XKEY_RETURN,
	XKEY_PAUSE,
	XKEY_ESCAPE,
	XKEY_SPACE,
	XKEY_EXCLAIM,
	XKEY_QUOTEDBL,
	XKEY_HASH,
	XKEY_DOLLAR,
	XKEY_AMPERSAND,
	XKEY_QUOTE,
	XKEY_LEFTPAREN,
	XKEY_RIGHTPAREN,
	XKEY_ASTERISK,
	XKEY_PLUS,
	XKEY_COMMA,
	XKEY_MINUS,
	XKEY_PERIOD,
	XKEY_SLASH,
	XKEY_COLON,
	XKEY_SEMICOLON,
	XKEY_LESS,
	XKEY_EQUALS,
	XKEY_GREATER,
	XKEY_QUESTION,
	XKEY_AT,
	XKEY_LEFTBRACKET,
	XKEY_BACKSLASH,
	XKEY_RIGHTBRACKET,
	XKEY_CARET,
	XKEY_UNDERSCORE,
	XKEY_BACKQUOTE,
	XKEY_DELETE,

	XKEY_N0,	//小键盘按键
	XKEY_N1,	
	XKEY_N2,
	XKEY_N3,
	XKEY_N4,
	XKEY_N5,
	XKEY_N6,
	XKEY_N7,
	XKEY_N8,
	XKEY_N9,
	XKEY_N_PERIOD,
	XKEY_N_DIVIDE,
	XKEY_N_MULTIPLY,
	XKEY_N_MINUS,
	XKEY_N_PLUS,
	XKEY_N_ENTER,
	XKEY_N_EQUALS,
	//方向按键区
	XKEY_UP,
	XKEY_DOWN,
	XKEY_RIGHT,
	XKEY_LEFT,
	XKEY_INSERT,
	XKEY_HOME,
	XKEY_END,
	XKEY_PAGEUP,
	XKEY_PAGEDOWN,
	//帮助键
	XKEY_F1,
	XKEY_F2,
	XKEY_F3,
	XKEY_F4,
	XKEY_F5,
	XKEY_F6,
	XKEY_F7,
	XKEY_F8,
	XKEY_F9,
	XKEY_F10,
	XKEY_F11,
	XKEY_F12,
	XKEY_F13,
	XKEY_F14,
	XKEY_F15,

	XKEY_NUMLOCK,
	XKEY_CAPSLOCK,
	XKEY_SCROLLOCK,
	XKEY_RSHIFT,
	XKEY_LSHIFT,
	XKEY_RCTRL,
	XKEY_LCTRL,
	XKEY_RALT,
	XKEY_LALT,
	XKEY_RMETA,
	XKEY_LMETA,
	XKEY_LSUPER,		//Left "Windows" key 
	XKEY_RSUPER,		//Right "Windows" key 
	XKEY_MODE,			//"Alt Gr" key 
	XKEY_COMPOSE,	//Multi-key compose key

	XKEY_HELP,
	XKEY_PRINT,
	XKEY_SYSREQ,
	XKEY_BREAK,
	XKEY_MENU,
	XKEY_POWER,		//Power Macintosh power key
	XKEY_EURO,			//Some european keyboards
	XKEY_UNDO,			//Atari keyboard has Undo
	XKEY_MAX,
};
inline _XKeyValue charToKeyValue(char p)
{
	switch(p)
	{
	case 'A':
	case 'a':return XKEY_A;
	case 'B':
	case 'b':return XKEY_B;
	case 'C':
	case 'c':return XKEY_C;
	case 'D':
	case 'd':return XKEY_D;
	case 'E':
	case 'e':return XKEY_E;
	case 'F':
	case 'f':return XKEY_F;
	case 'G':
	case 'g':return XKEY_G;
	case 'H':
	case 'h':return XKEY_H;
	case 'I':
	case 'i':return XKEY_I;
	case 'J':
	case 'j':return XKEY_J;
	case 'K':
	case 'k':return XKEY_K;
	case 'L':
	case 'l':return XKEY_L;
	case 'M':
	case 'm':return XKEY_M;
	case 'N':
	case 'n':return XKEY_N;
	case 'O':
	case 'o':return XKEY_O;
	case 'P':
	case 'p':return XKEY_P;
	case 'Q':
	case 'q':return XKEY_Q;
	case 'R':
	case 'r':return XKEY_R;
	case 'S':
	case 's':return XKEY_S;
	case 'T':
	case 't':return XKEY_T;
	case 'U':
	case 'u':return XKEY_U;
	case 'v':
	case 'V':return XKEY_V;
	case 'W':
	case 'w':return XKEY_W;
	case 'X':
	case 'x':return XKEY_X;
	case 'Y':
	case 'y':return XKEY_Y;
	case 'Z':
	case 'z':return XKEY_Z;

	case '0':return XKEY_0;
	case '1':return XKEY_1;
	case '2':return XKEY_2;
	case '3':return XKEY_3;
	case '4':return XKEY_4;
	case '5':return XKEY_5;
	case '6':return XKEY_2;
	case '7':return XKEY_3;
	case '8':return XKEY_4;
	case '9':return XKEY_5;
	default:
		return XKEY_UNKNOWN;
	}
}
inline char keyValueToChar(_XKeyValue v)
{
	switch(v)
	{
	case XKEY_A:return 'A';
	case XKEY_B:return 'B';
	case XKEY_C:return 'C';
	case XKEY_D:return 'D';
	case XKEY_E:return 'E';
	case XKEY_F:return 'F';
	case XKEY_G:return 'G';
	case XKEY_H:return 'H';
	case XKEY_I:return 'I';
	case XKEY_J:return 'J';
	case XKEY_K:return 'K';
	case XKEY_L:return 'L';
	case XKEY_M:return 'M';
	case XKEY_N:return 'N';
	case XKEY_O:return 'O';
	case XKEY_P:return 'P';
	case XKEY_Q:return 'Q';
	case XKEY_R:return 'R';
	case XKEY_S:return 'S';
	case XKEY_T:return 'T';
	case XKEY_U:return 'U';
	case XKEY_V:return 'V';
	case XKEY_W:return 'W';
	case XKEY_X:return 'X';
	case XKEY_Y:return 'Y';
	case XKEY_Z:return 'Z';
	case XKEY_N0:
	case XKEY_0:return '0';
	case XKEY_N1:
	case XKEY_1:return '1';
	case XKEY_N2:
	case XKEY_2:return '2';
	case XKEY_N3:
	case XKEY_3:return '3';
	case XKEY_N4:
	case XKEY_4:return '4';
	case XKEY_N5:
	case XKEY_5:return '5';
	case XKEY_N6:
	case XKEY_6:return '6';
	case XKEY_N7:
	case XKEY_7:return '7';
	case XKEY_N8:
	case XKEY_8:return '8';
	case XKEY_N9:
	case XKEY_9:return '9';
	default:
		return 0;
	}
}
#endif