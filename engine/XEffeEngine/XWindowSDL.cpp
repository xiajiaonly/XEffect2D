#include "XWindowSDL.h"
#include "XLogBook.h"

//bool _XWindowSDL::createWindow(int width,int height,const char *windowTitle,int isFullScreen,int withFrame)
bool _XWindowSDL::createWindow(int width,int height,const char *,int isFullScreen,int withFrame)
{
	const SDL_VideoInfo* info = NULL;	//显示设备信息
	int bpp = 0;			//窗口色深
	int flags = 0;			//某个标记

	// ----- SDL init --------------- 
//	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) 
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) 
	{//初始化SDL
		fprintf(stderr,"Video initialization failed: %s\n",SDL_GetError());
		LogNull("Video initialization failed: %s\n",SDL_GetError());
		return false;
	}
	atexit(SDL_Quit);				//退出时退出SDL
	
	info = SDL_GetVideoInfo();		//获得窗口设备信息
	bpp = info->vfmt->BitsPerPixel;	//获得显示色深
	
	// ----- OpenGL attribute setting via SDL --------------- 
	//SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);		//设置蓝色位宽
	//bpp = 24;
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);	//使得STENCIL生效，能产生镜面效果。
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//设置双缓存
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);	//设置平滑处理,没有生效，多重采样要生效的话必须要SDL_GL_DOUBLEBUFFER为1
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);

	//SDL_NOFRAME没有边框的 |SDL_RESIZABLE
	if(isFullScreen != 0) flags = SDL_RESIZABLE | SDL_OPENGL | SDL_FULLSCREEN;			//设置为全屏
	else flags = SDL_RESIZABLE | SDL_OPENGL | SDL_DOUBLEBUF;
	if(withFrame == 0) flags |= SDL_NOFRAME;
	//窗口标志符
	if((m_screen = SDL_SetVideoMode(width, height, bpp, flags)) == NULL) 
	{//初始化窗口信息
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		LogNull("Video mode set failed: %s\n", SDL_GetError());
        return false;
	}
	//initMultisample(wglGetCurrentDC());	//这里使用wglext的方式设置多重采样，不过好像没有生效，原因尚不明确,需要进一步的测试
	return true;
}
//int SDLKeyToXEEKeyMap[] =
//{
//	//普通按键
//	/*XKEY_A = */SDLK_a,	//字母按键
//	/*XKEY_B = */SDLK_b,
//	/*XKEY_C = */SDLK_c,
//	/*XKEY_D = */SDLK_d,
//	/*XKEY_E = */SDLK_e,
//	/*XKEY_F = */SDLK_f,
//	/*XKEY_G = */SDLK_g,
//	/*XKEY_H = */SDLK_h,
//	/*XKEY_I = */SDLK_i,
//	/*XKEY_J = */SDLK_j,
//	/*XKEY_K = */SDLK_k,
//	/*XKEY_L = */SDLK_l,
//	/*XKEY_M = */SDLK_m,
//	/*XKEY_N = */SDLK_n,
//	/*XKEY_O = */SDLK_o,
//	/*XKEY_P = */SDLK_p,
//	/*XKEY_Q = */SDLK_q,
//	/*XKEY_R = */SDLK_r,
//	/*XKEY_S = */SDLK_s,
//	/*XKEY_T = */SDLK_t,
//	/*XKEY_U = */SDLK_u,
//	/*XKEY_V = */SDLK_v,
//	/*XKEY_W = */SDLK_w,
//	/*XKEY_X = */SDLK_x,
//	/*XKEY_Y = */SDLK_y,
//	/*XKEY_Z = */SDLK_z,
//
//	/*XKEY_0 = */SDLK_0,	//数字按键
//	/*XKEY_1 = */SDLK_1,
//	/*XKEY_2 = */SDLK_2,
//	/*XKEY_3 = */SDLK_3,
//	/*XKEY_4 = */SDLK_4,
//	/*XKEY_5 = */SDLK_5,
//	/*XKEY_6 = */SDLK_6,
//	/*XKEY_7 = */SDLK_7,
//	/*XKEY_8 = */SDLK_8,
//	/*XKEY_9 = */SDLK_9,
//
//	/*XKEY_UNKNOWN = */SDLK_UNKNOWN,//SDLK_FIRST
//	/*XKEY_BACKSPACE = */SDLK_BACKSPACE,
//	/*XKEY_TAB = */SDLK_TAB,
//	/*XKEY_CLEAR = */SDLK_CLEAR,
//	/*XKEY_RETURN = */SDLK_RETURN,
//	/*XKEY_PAUSE = */SDLK_PAUSE,
//	/*XKEY_ESCAPE = */SDLK_ESCAPE,
//	/*XKEY_SPACE = */SDLK_SPACE,
//	/*XKEY_EXCLAIM = */SDLK_EXCLAIM,
//	/*XKEY_QUOTEDBL = */SDLK_QUOTEDBL,
//	/*XKEY_HASH = */SDLK_HASH,
//	/*XKEY_DOLLAR = */SDLK_DOLLAR,
//	/*XKEY_AMPERSAND = */SDLK_AMPERSAND,
//	/*XKEY_QUOTE = */SDLK_QUOTE,
//	/*XKEY_LEFTPAREN = */SDLK_LEFTPAREN,
//	/*XKEY_RIGHTPAREN = */SDLK_RIGHTPAREN,
//	/*XKEY_ASTERISK = */SDLK_ASTERISK,
//	/*XKEY_PLUS = */SDLK_PLUS,
//	/*XKEY_COMMA = */SDLK_COMMA,
//	/*XKEY_MINUS = */SDLK_MINUS,
//	/*XKEY_PERIOD = */SDLK_PERIOD,
//	/*XKEY_SLASH = */SDLK_SLASH,
//	/*XKEY_COLON = */SDLK_COLON,
//	/*XKEY_SEMICOLON = */SDLK_SEMICOLON,
//	/*XKEY_LESS = */SDLK_LESS,
//	/*XKEY_EQUALS = */SDLK_EQUALS,
//	/*XKEY_GREATER = */SDLK_GREATER,
//	/*XKEY_QUESTION = */SDLK_QUESTION,
//	/*XKEY_AT = */SDLK_AT,
//	/*XKEY_LEFTBRACKET = */SDLK_LEFTBRACKET,
//	/*XKEY_BACKSLASH = */SDLK_BACKSLASH,
//	/*XKEY_RIGHTBRACKET = */SDLK_RIGHTBRACKET,
//	/*XKEY_CARET = */SDLK_CARET,
//	/*XKEY_UNDERSCORE = */SDLK_UNDERSCORE,
//	/*XKEY_BACKQUOTE = */SDLK_BACKQUOTE,
//	/*XKEY_DELETE = */SDLK_DELETE,
//
//	/*XKEY_N0 = */SDLK_KP0,	//小键盘按键
//	/*XKEY_N1 = */SDLK_KP1,	
//	/*XKEY_N2 = */SDLK_KP2,
//	/*XKEY_N3 = */SDLK_KP3,
//	/*XKEY_N4 = */SDLK_KP4,
//	/*XKEY_N5 = */SDLK_KP5,
//	/*XKEY_N6 = */SDLK_KP6,
//	/*XKEY_N7 = */SDLK_KP7,
//	/*XKEY_N8 = */SDLK_KP8,
//	/*XKEY_N9 = */SDLK_KP9,
//	/*XKEY_N_PERIOD = */SDLK_KP_PERIOD,
//	/*XKEY_N_DIVIDE = */SDLK_KP_DIVIDE,
//	/*XKEY_N_MULTIPLY = */SDLK_KP_MULTIPLY,
//	/*XKEY_N_MINUS = */SDLK_KP_MINUS,
//	/*XKEY_N_PLUS = */SDLK_KP_PLUS,
//	/*XKEY_N_ENTER = */SDLK_KP_ENTER,
//	/*XKEY_N_EQUALS = */SDLK_KP_EQUALS,
//	//方向按键区
//	/*XKEY_UP = */SDLK_UP,
//	/*XKEY_DOWN = */SDLK_DOWN,
//	/*XKEY_RIGHT = */SDLK_RIGHT,
//	/*XKEY_LEFT = */SDLK_LEFT,
//	/*XKEY_INSERT = */SDLK_INSERT,
//	/*XKEY_HOME = */SDLK_HOME,
//	/*XKEY_END = */SDLK_END,
//	/*XKEY_PAGEUP = */SDLK_PAGEUP,
//	/*XKEY_PAGEDOWN = */SDLK_PAGEDOWN,
//	//帮助键
//	/*XKEY_F1 = */SDLK_F1,
//	/*XKEY_F2 = */SDLK_F2,
//	/*XKEY_F3 = */SDLK_F3,
//	/*XKEY_F4 = */SDLK_F4,
//	/*XKEY_F5 = */SDLK_F5,
//	/*XKEY_F6 = */SDLK_F6,
//	/*XKEY_F7 = */SDLK_F7,
//	/*XKEY_F8 = */SDLK_F8,
//	/*XKEY_F9 = */SDLK_F9,
//	/*XKEY_F10 = */SDLK_F10,
//	/*XKEY_F11 = */SDLK_F11,
//	/*XKEY_F12 = */SDLK_F12,
//	/*XKEY_F13 = */SDLK_F13,
//	/*XKEY_F14 = */SDLK_F14,
//	/*XKEY_F15 = */SDLK_F15,
//
//	/*XKEY_NUMLOCK = */SDLK_NUMLOCK,
//	/*XKEY_CAPSLOCK = */SDLK_CAPSLOCK,
//	/*XKEY_SCROLLOCK = */SDLK_SCROLLOCK,
//	/*XKEY_RSHIFT = */SDLK_RSHIFT,
//	/*XKEY_LSHIFT = */SDLK_LSHIFT,
//	/*XKEY_RCTRL = */SDLK_RCTRL,
//	/*XKEY_LCTRL = */SDLK_LCTRL,
//	/*XKEY_RALT = */SDLK_RALT,
//	/*XKEY_LALT = */SDLK_LALT,
//	/*XKEY_RMETA = */SDLK_RMETA,
//	/*XKEY_LMETA = */SDLK_LMETA,
//	/*XKEY_LSUPER = */SDLK_LSUPER,		//Left "Windows" key 
//	/*XKEY_RSUPER = */SDLK_RSUPER,		//Right "Windows" key 
//	/*XKEY_MODE = */SDLK_MODE,			//"Alt Gr" key 
//	/*XKEY_COMPOSE = */SDLK_COMPOSE,	//Multi-key compose key
//
//	/*XKEY_HELP = */SDLK_HELP,
//	/*XKEY_PRINT = */SDLK_PRINT,
//	/*XKEY_SYSREQ = */SDLK_SYSREQ,
//	/*XKEY_BREAK = */SDLK_BREAK,
//	/*XKEY_MENU = */SDLK_MENU,
//	/*XKEY_POWER = */SDLK_POWER,		//Power Macintosh power key
//	/*XKEY_EURO = */SDLK_EURO,			//Some european keyboards
//	/*XKEY_UNDO = */SDLK_UNDO,			//Atari keyboard has Undo
//};
#include "XMouseAndKeyBoardDefine.h"
int _XWindowSDL::mapKey(int key)
{
	//方案1:遍历
	//for(int i = 0;i < XKEY_MAX;++ i)
	//{
	//	if(SDLKeyToXEEKeyMap[i] == key) return i;
	//}
	//return XKEY_UNKNOWN;
	//方案2
	switch(key)
	{
	case SDLK_a: return XKEY_A;	//字母按键
	case SDLK_b: return XKEY_B;
	case SDLK_c: return XKEY_C;
	case SDLK_d: return XKEY_D;
	case SDLK_e: return XKEY_E;
	case SDLK_f: return XKEY_F;
	case SDLK_g: return XKEY_G;
	case SDLK_h: return XKEY_H;
	case SDLK_i: return XKEY_I;
	case SDLK_j: return XKEY_J;
	case SDLK_k: return XKEY_K;
	case SDLK_l: return XKEY_L;
	case SDLK_m: return XKEY_M;
	case SDLK_n: return XKEY_N;
	case SDLK_o: return XKEY_O;
	case SDLK_p: return XKEY_P;
	case SDLK_q: return XKEY_Q;
	case SDLK_r: return XKEY_R;
	case SDLK_s: return XKEY_S;
	case SDLK_t: return XKEY_T;
	case SDLK_u: return XKEY_U;
	case SDLK_v: return XKEY_V;
	case SDLK_w: return XKEY_W;
	case SDLK_x: return XKEY_X;
	case SDLK_y: return XKEY_Y;
	case SDLK_z: return XKEY_Z;

	case SDLK_0: return XKEY_0;	//数字按键
	case SDLK_1: return XKEY_1;
	case SDLK_2: return XKEY_2;
	case SDLK_3: return XKEY_3;
	case SDLK_4: return XKEY_4;
	case SDLK_5: return XKEY_5;
	case SDLK_6: return XKEY_6;
	case SDLK_7: return XKEY_7;
	case SDLK_8: return XKEY_8;
	case SDLK_9: return XKEY_9;

	case SDLK_UNKNOWN: return XKEY_UNKNOWN;//SDLK_FIRST
	case SDLK_BACKSPACE: return XKEY_BACKSPACE;
	case SDLK_TAB: return XKEY_TAB;
	case SDLK_CLEAR: return XKEY_CLEAR;
	case SDLK_RETURN: return XKEY_RETURN;
	case SDLK_PAUSE: return XKEY_PAUSE;
	case SDLK_ESCAPE: return XKEY_ESCAPE;
	case SDLK_SPACE: return XKEY_SPACE;
	case SDLK_EXCLAIM: return XKEY_EXCLAIM;
	case SDLK_QUOTEDBL: return XKEY_QUOTEDBL;
	case SDLK_HASH: return XKEY_HASH;
	case SDLK_DOLLAR: return XKEY_DOLLAR;
	case SDLK_AMPERSAND: return XKEY_AMPERSAND;
	case SDLK_QUOTE: return XKEY_QUOTE;
	case SDLK_LEFTPAREN: return XKEY_LEFTPAREN;
	case SDLK_RIGHTPAREN: return XKEY_RIGHTPAREN;
	case SDLK_ASTERISK: return XKEY_ASTERISK;
	case SDLK_PLUS: return XKEY_PLUS;
	case SDLK_COMMA: return XKEY_COMMA;
	case SDLK_MINUS: return XKEY_MINUS;
	case SDLK_PERIOD: return XKEY_PERIOD;
	case SDLK_SLASH: return XKEY_SLASH;
	case SDLK_COLON: return XKEY_COLON;
	case SDLK_SEMICOLON: return XKEY_SEMICOLON;
	case SDLK_LESS: return XKEY_LESS;
	case SDLK_EQUALS: return XKEY_EQUALS;
	case SDLK_GREATER: return XKEY_GREATER;
	case SDLK_QUESTION: return XKEY_QUESTION;
	case SDLK_AT: return XKEY_AT;
	case SDLK_LEFTBRACKET: return XKEY_LEFTBRACKET;
	case SDLK_BACKSLASH: return XKEY_BACKSLASH;
	case SDLK_RIGHTBRACKET: return XKEY_RIGHTBRACKET;
	case SDLK_CARET: return XKEY_CARET;
	case SDLK_UNDERSCORE: return XKEY_UNDERSCORE;
	case SDLK_BACKQUOTE: return XKEY_BACKQUOTE;
	case SDLK_DELETE: return XKEY_DELETE;

	case SDLK_KP0: return XKEY_N0;	//小键盘按键
	case SDLK_KP1: return XKEY_N1;	
	case SDLK_KP2: return XKEY_N2;
	case SDLK_KP3: return XKEY_N3;
	case SDLK_KP4: return XKEY_N4;
	case SDLK_KP5: return XKEY_N5;
	case SDLK_KP6: return XKEY_N6;
	case SDLK_KP7: return XKEY_N7;
	case SDLK_KP8: return XKEY_N8;
	case SDLK_KP9: return XKEY_N9;
	case SDLK_KP_PERIOD: return XKEY_N_PERIOD;
	case SDLK_KP_DIVIDE: return XKEY_N_DIVIDE;
	case SDLK_KP_MULTIPLY: return XKEY_N_MULTIPLY;
	case SDLK_KP_MINUS: return XKEY_N_MINUS;
	case SDLK_KP_PLUS: return XKEY_N_PLUS;
	case SDLK_KP_ENTER: return XKEY_N_ENTER;
	case SDLK_KP_EQUALS: return XKEY_N_EQUALS;
	//方向按键区
	case SDLK_UP: return XKEY_UP;
	case SDLK_DOWN: return XKEY_DOWN;
	case SDLK_RIGHT: return XKEY_RIGHT;
	case SDLK_LEFT: return XKEY_LEFT;
	case SDLK_INSERT: return XKEY_INSERT;
	case SDLK_HOME: return XKEY_HOME;
	case SDLK_END: return XKEY_END;
	case SDLK_PAGEUP: return XKEY_PAGEUP;
	case SDLK_PAGEDOWN: return XKEY_PAGEDOWN;
	//帮助键
	case SDLK_F1: return XKEY_F1;
	case SDLK_F2: return XKEY_F2;
	case SDLK_F3: return XKEY_F3;
	case SDLK_F4: return XKEY_F4;
	case SDLK_F5: return XKEY_F5;
	case SDLK_F6: return XKEY_F6;
	case SDLK_F7: return XKEY_F7;
	case SDLK_F8: return XKEY_F8;
	case SDLK_F9: return XKEY_F9;
	case SDLK_F10: return XKEY_F10;
	case SDLK_F11: return XKEY_F11;
	case SDLK_F12: return XKEY_F12;
	case SDLK_F13: return XKEY_F13;
	case SDLK_F14: return XKEY_F14;
	case SDLK_F15: return XKEY_F15;

	case SDLK_NUMLOCK: return XKEY_NUMLOCK;
	case SDLK_CAPSLOCK: return XKEY_CAPSLOCK;
	case SDLK_SCROLLOCK: return XKEY_SCROLLOCK;
	case SDLK_RSHIFT: return XKEY_RSHIFT;
	case SDLK_LSHIFT: return XKEY_LSHIFT;
	case SDLK_RCTRL: return XKEY_RCTRL;
	case SDLK_LCTRL: return XKEY_LCTRL;
	case SDLK_RALT: return XKEY_RALT;
	case SDLK_LALT: return XKEY_LALT;
	case SDLK_RMETA: return XKEY_RMETA;
	case SDLK_LMETA: return XKEY_LMETA;
	case SDLK_LSUPER: return XKEY_LSUPER;		//Left "Windows" key 
	case SDLK_RSUPER: return XKEY_RSUPER;		//Right "Windows" key 
	case SDLK_MODE: return XKEY_MODE;			//"Alt Gr" key 
	case SDLK_COMPOSE: return XKEY_COMPOSE;	//Multi-key compose key

	case SDLK_HELP: return XKEY_HELP;
	case SDLK_PRINT: return XKEY_PRINT;
	case SDLK_SYSREQ: return XKEY_SYSREQ;
	case SDLK_BREAK: return XKEY_BREAK;
	case SDLK_MENU: return XKEY_MENU;
	case SDLK_POWER: return XKEY_POWER;		//Power Macintosh power key
	case SDLK_EURO: return XKEY_EURO;			//Some european keyboards
	case SDLK_UNDO: return XKEY_UNDO;			//Atari keyboard has Undo
	default:
		return XKEY_UNKNOWN;
	}
}