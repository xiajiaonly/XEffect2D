#include "XStdHead.h"
#include "XMouseAndKeyBoardDefine.h"
namespace XE{
	namespace XEE{
XKeyValue charToKeyValue(char p)
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
	case '6':return XKEY_6;
	case '7':return XKEY_7;
	case '8':return XKEY_8;
	case '9':return XKEY_9;
	case 8:return XKEY_BACKSPACE;
	case 9:return XKEY_TAB;
	case 12:return XKEY_CLEAR;
	case 13:return XKEY_RETURN;
	case 19:return XKEY_PAUSE;
	case 27:return XKEY_ESCAPE;
	case 32:return XKEY_SPACE;
	case '!':return XKEY_EXCLAIM;
	case '\"':return XKEY_QUOTEDBL;
	case '#':return XKEY_HASH;
	case '$':return XKEY_DOLLAR;
	case '%':return XKEY_PERSENT;
	case '&':return XKEY_AMPERSAND;
	case '\'':return XKEY_QUOTE;
	case '(':return XKEY_LEFTPAREN;
	case ')':return XKEY_RIGHTPAREN;
	case '*':return XKEY_ASTERISK;
	case '+':return XKEY_PLUS;
	case ',':return XKEY_COMMA;
	case '-':return XKEY_MINUS;
	case '.':return XKEY_PERIOD;
	case '/':return XKEY_SLASH;
	case ':':return XKEY_COLON;
	case ';':return XKEY_SEMICOLON;
	case '<':return XKEY_LESS;
	case '=':return XKEY_EQUALS;
	case '>':return XKEY_GREATER;
	case '?':return XKEY_QUESTION;
	case '@':return XKEY_AT;
	case '[':return XKEY_LEFTBRACKET;
	case '\\':return XKEY_BACKSLASH;
	case ']':return XKEY_RIGHTBRACKET;
	case '^':return XKEY_CARET;
	case '_':return XKEY_UNDERSCORE;
	case '`':return XKEY_BACKQUOTE;
	case 127:return XKEY_DELETE;
	case '{':return XKEY_LEFTBRACES;
	case '|':return XKEY_SEPARATOR;
	case '}':return XKEY_RIGHTBRACES;
	case '~':return XKEY_WAVE;

	//XKEY_N0,	//小键盘按键
	//XKEY_N1,	
	//XKEY_N2,
	//XKEY_N3,
	//XKEY_N4,
	//XKEY_N5,
	//XKEY_N6,
	//XKEY_N7,
	//XKEY_N8,
	//XKEY_N9,
	//XKEY_N_PERIOD,
	//XKEY_N_DIVIDE,
	//XKEY_N_MULTIPLY,
	//XKEY_N_MINUS,
	//XKEY_N_PLUS,
	//XKEY_N_ENTER,
	//XKEY_N_EQUALS,
	//方向按键区
	//XKEY_UP,
	//XKEY_DOWN,
	//XKEY_RIGHT,
	//XKEY_LEFT,
	//XKEY_INSERT,
	//XKEY_HOME,
	//XKEY_END,
	//XKEY_PAGEUP,
	//XKEY_PAGEDOWN,
	//帮助键
	//XKEY_F1,
	//XKEY_F2,
	//XKEY_F3,
	//XKEY_F4,
	//XKEY_F5,
	//XKEY_F6,
	//XKEY_F7,
	//XKEY_F8,
	//XKEY_F9,
	//XKEY_F10,
	//XKEY_F11,
	//XKEY_F12,
	//XKEY_F13,
	//XKEY_F14,
	//XKEY_F15,

	//XKEY_NUMLOCK,
	//XKEY_CAPSLOCK,
	//XKEY_SCROLLOCK,
	//XKEY_RSHIFT,
	//XKEY_LSHIFT,
	//XKEY_RCTRL,
	//XKEY_LCTRL,
	//XKEY_RALT,
	//XKEY_LALT,
	//XKEY_RMETA,
	//XKEY_LMETA,
	//XKEY_LSUPER,		//Left "Windows" key 
	//XKEY_RSUPER,		//Right "Windows" key 
	//XKEY_MODE,			//"Alt Gr" key 
	//XKEY_COMPOSE,	//Multi-key compose key

	//XKEY_HELP,
	//XKEY_PRINT,
	//XKEY_SYSREQ,
	//XKEY_BREAK,
	//XKEY_MENU,
	//XKEY_POWER,		//Power Macintosh power key
	//XKEY_EURO,			//Some european keyboards
	//XKEY_UNDO,			//Atari keyboard has Undo
	default:
		return XKEY_UNKNOWN;
	}
}
char keyValueToChar(XKeyValue v)
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
	case XKEY_BACKSPACE:return 8;
	case XKEY_TAB:return 9;
	case XKEY_CLEAR:return 12;
	case XKEY_RETURN:return 13;
	case XKEY_PAUSE:return 19;
	case XKEY_ESCAPE:return 27;
	case XKEY_SPACE:return ' ';
	case XKEY_EXCLAIM:return '!';
	case XKEY_QUOTEDBL:return '\"';
	case XKEY_HASH:return '#';
	case XKEY_DOLLAR:return '$';
	case XKEY_PERSENT:return '%';
	case XKEY_AMPERSAND:return '&';
	case XKEY_QUOTE:return '\'';
	case XKEY_LEFTPAREN:return '(';
	case XKEY_RIGHTPAREN:return ')';
	case XKEY_ASTERISK:return '*';
	case XKEY_PLUS:return '+';
	case XKEY_COMMA:return ',';
	case XKEY_MINUS:return '-';
	case XKEY_PERIOD:return '.';
	case XKEY_SLASH:return '/';
	case XKEY_COLON:return ':';
	case XKEY_SEMICOLON:return ';';
	case XKEY_LESS:return '<';
	case XKEY_EQUALS:return '=';
	case XKEY_GREATER:return '>';
	case XKEY_QUESTION:return '?';
	case XKEY_AT:return '@';
	case XKEY_LEFTBRACKET:return '[';
	case XKEY_BACKSLASH:return '\\';
	case XKEY_RIGHTBRACKET:return ']';
	case XKEY_CARET:return '^';
	case XKEY_UNDERSCORE:return '_';
	case XKEY_BACKQUOTE:return '`';
	case XKEY_DELETE:return 127;
	case XKEY_N_PERIOD:return '.';
	case XKEY_N_DIVIDE:return '/';
	case XKEY_N_MULTIPLY:return '*';
	case XKEY_N_MINUS:return '-';
	case XKEY_N_PLUS:return '+';
	case XKEY_N_ENTER:return 13;
	case XKEY_N_EQUALS:return '=';
	//case XKEY_UP:return "XKEY_UP";
	//case XKEY_DOWN:return "XKEY_DOWN";
	//case XKEY_RIGHT:return "XKEY_RIGHT";
	//case XKEY_LEFT:return "XKEY_LEFT";
	//case XKEY_INSERT:return "XKEY_INSERT";
	//case XKEY_HOME:return "XKEY_HOME";
	//case XKEY_END:return "XKEY_END";
	//case XKEY_PAGEUP:return "XKEY_PAGEUP";
	//case XKEY_PAGEDOWN:return "XKEY_PAGEDOWN";
	//case XKEY_F1:return "XKEY_F1";
	//case XKEY_F2:return "XKEY_F2";
	//case XKEY_F3:return "XKEY_F3";
	//case XKEY_F4:return "XKEY_F4";
	//case XKEY_F5:return "XKEY_F5";
	//case XKEY_F6:return "XKEY_F6";
	//case XKEY_F7:return "XKEY_F7";
	//case XKEY_F8:return "XKEY_F8";
	//case XKEY_F9:return "XKEY_F9";
	//case XKEY_F10:return "XKEY_F10";
	//case XKEY_F11:return "XKEY_F11";
	//case XKEY_F12:return "XKEY_F12";
	//case XKEY_F13:return "XKEY_F13";
	//case XKEY_F14:return "XKEY_F14";
	//case XKEY_F15:return "XKEY_F15";
	//case XKEY_NUMLOCK:return "XKEY_NUMLOCK";
	//case XKEY_CAPSLOCK:return "XKEY_CAPSLOCK";
	//case XKEY_SCROLLOCK:return "XKEY_SCROLLOCK";
	//case XKEY_RSHIFT:return "XKEY_RSHIFT";
	//case XKEY_LSHIFT:return "XKEY_LSHIFT";
	//case XKEY_RCTRL:return "XKEY_RCTRL";
	//case XKEY_LCTRL:return "XKEY_LCTRL";
	//case XKEY_RALT:return "XKEY_RALT";
	//case XKEY_LALT:return "XKEY_LALT";
	//case XKEY_RMETA:return "XKEY_RMETA";
	//case XKEY_LMETA:return "XKEY_LMETA";
	//case XKEY_LSUPER:return "XKEY_LSUPER";
	//case XKEY_RSUPER:return "XKEY_RSUPER";
	//case XKEY_MODE:return "XKEY_MODE";
	//case XKEY_COMPOSE:return "XKEY_COMPOSE";
	//case XKEY_HELP:return "XKEY_HELP";
	//case XKEY_PRINT:return "XKEY_PRINT";
	//case XKEY_SYSREQ:return "XKEY_SYSREQ";
	//case XKEY_BREAK:return "XKEY_BREAK";
	//case XKEY_MENU:return "XKEY_MENU";
	//case XKEY_POWER:return "XKEY_POWER";
	//case XKEY_EURO:return "XKEY_EURO";
	//case XKEY_UNDO:return "XKEY_UNDO";
	//case XKEY_MAX:return "XKEY_MAX";
	case XKEY_LEFTBRACES:return '{';	//{
	case XKEY_SEPARATOR:return '|';		//|
	case XKEY_RIGHTBRACES:return '}';	//}
	case XKEY_WAVE:return '~';
	default:
		return 0;
	}
}
std::string keyValueToString(XKeyValue v)
{
	switch (v)
	{
	case XKEY_A:return "XKEY_A";
	case XKEY_B:return "XKEY_B";
	case XKEY_C:return "XKEY_C";
	case XKEY_D:return "XKEY_D";
	case XKEY_E:return "XKEY_E";
	case XKEY_F:return "XKEY_F";
	case XKEY_G:return "XKEY_G";
	case XKEY_H:return "XKEY_H";
	case XKEY_I:return "XKEY_I";
	case XKEY_J:return "XKEY_J";
	case XKEY_K:return "XKEY_K";
	case XKEY_L:return "XKEY_L";
	case XKEY_M:return "XKEY_M";
	case XKEY_N:return "XKEY_N";
	case XKEY_O:return "XKEY_O";
	case XKEY_P:return "XKEY_P";
	case XKEY_Q:return "XKEY_Q";
	case XKEY_R:return "XKEY_R";
	case XKEY_S:return "XKEY_S";
	case XKEY_T:return "XKEY_T";
	case XKEY_U:return "XKEY_U";
	case XKEY_V:return "XKEY_V";
	case XKEY_W:return "XKEY_W";
	case XKEY_X:return "XKEY_X";
	case XKEY_Y:return "XKEY_Y";
	case XKEY_Z:return "XKEY_Z";
	case XKEY_0:return "XKEY_0";
	case XKEY_1:return "XKEY_1";
	case XKEY_2:return "XKEY_2";
	case XKEY_3:return "XKEY_3";
	case XKEY_4:return "XKEY_4";
	case XKEY_5:return "XKEY_5";
	case XKEY_6:return "XKEY_6";
	case XKEY_7:return "XKEY_7";
	case XKEY_8:return "XKEY_8";
	case XKEY_9:return "XKEY_9";
	case XKEY_UNKNOWN:return "XKEY_UNKNOWN";
	case XKEY_BACKSPACE:return "XKEY_BACKSPACE";
	case XKEY_TAB:return "XKEY_TAB";
	case XKEY_CLEAR:return "XKEY_CLEAR";
	case XKEY_RETURN:return "XKEY_RETURN";
	case XKEY_PAUSE:return "XKEY_PAUSE";
	case XKEY_ESCAPE:return "XKEY_ESCAPE";
	case XKEY_SPACE:return "XKEY_SPACE";
	case XKEY_EXCLAIM:return "XKEY_EXCLAIM";
	case XKEY_QUOTEDBL:return "XKEY_QUOTEDBL";
	case XKEY_HASH:return "XKEY_HASH";
	case XKEY_DOLLAR:return "XKEY_DOLLAR";
	case XKEY_AMPERSAND:return "XKEY_AMPERSAND";
	case XKEY_QUOTE:return "XKEY_QUOTE";
	case XKEY_LEFTPAREN:return "XKEY_LEFTPAREN";
	case XKEY_RIGHTPAREN:return "XKEY_RIGHTPAREN";
	case XKEY_ASTERISK:return "XKEY_ASTERISK";
	case XKEY_PLUS:return "XKEY_PLUS";
	case XKEY_COMMA:return "XKEY_COMMA";
	case XKEY_MINUS:return "XKEY_MINUS";
	case XKEY_PERIOD:return "XKEY_PERIOD";
	case XKEY_SLASH:return "XKEY_SLASH";
	case XKEY_COLON:return "XKEY_COLON";
	case XKEY_SEMICOLON:return "XKEY_SEMICOLON";
	case XKEY_LESS:return "XKEY_LESS";
	case XKEY_EQUALS:return "XKEY_EQUALS";
	case XKEY_GREATER:return "XKEY_GREATER";
	case XKEY_QUESTION:return "XKEY_QUESTION";
	case XKEY_AT:return "XKEY_AT";
	case XKEY_LEFTBRACKET:return "XKEY_LEFTBRACKET";
	case XKEY_BACKSLASH:return "XKEY_BACKSLASH";
	case XKEY_RIGHTBRACKET:return "XKEY_RIGHTBRACKET";
	case XKEY_CARET:return "XKEY_CARET";
	case XKEY_UNDERSCORE:return "XKEY_UNDERSCORE";
	case XKEY_BACKQUOTE:return "XKEY_BACKQUOTE";
	case XKEY_DELETE:return "XKEY_DELETE";
	case XKEY_N0:return "XKEY_N0";
	case XKEY_N1:return "XKEY_N1";
	case XKEY_N2:return "XKEY_N2";
	case XKEY_N3:return "XKEY_N3";
	case XKEY_N4:return "XKEY_N4";
	case XKEY_N5:return "XKEY_N5";
	case XKEY_N6:return "XKEY_N6";
	case XKEY_N7:return "XKEY_N7";
	case XKEY_N8:return "XKEY_N8";
	case XKEY_N9:return "XKEY_N9";
	case XKEY_N_PERIOD:return "XKEY_N_PERIOD";
	case XKEY_N_DIVIDE:return "XKEY_N_DIVIDE";
	case XKEY_N_MULTIPLY:return "XKEY_N_MULTIPLY";
	case XKEY_N_MINUS:return "XKEY_N_MINUS";
	case XKEY_N_PLUS:return "XKEY_N_PLUS";
	case XKEY_N_ENTER:return "XKEY_N_ENTER";
	case XKEY_N_EQUALS:return "XKEY_N_EQUALS";
	case XKEY_UP:return "XKEY_UP";
	case XKEY_DOWN:return "XKEY_DOWN";
	case XKEY_RIGHT:return "XKEY_RIGHT";
	case XKEY_LEFT:return "XKEY_LEFT";
	case XKEY_INSERT:return "XKEY_INSERT";
	case XKEY_HOME:return "XKEY_HOME";
	case XKEY_END:return "XKEY_END";
	case XKEY_PAGEUP:return "XKEY_PAGEUP";
	case XKEY_PAGEDOWN:return "XKEY_PAGEDOWN";
	case XKEY_F1:return "XKEY_F1";
	case XKEY_F2:return "XKEY_F2";
	case XKEY_F3:return "XKEY_F3";
	case XKEY_F4:return "XKEY_F4";
	case XKEY_F5:return "XKEY_F5";
	case XKEY_F6:return "XKEY_F6";
	case XKEY_F7:return "XKEY_F7";
	case XKEY_F8:return "XKEY_F8";
	case XKEY_F9:return "XKEY_F9";
	case XKEY_F10:return "XKEY_F10";
	case XKEY_F11:return "XKEY_F11";
	case XKEY_F12:return "XKEY_F12";
	case XKEY_F13:return "XKEY_F13";
	case XKEY_F14:return "XKEY_F14";
	case XKEY_F15:return "XKEY_F15";
	case XKEY_NUMLOCK:return "XKEY_NUMLOCK";
	case XKEY_CAPSLOCK:return "XKEY_CAPSLOCK";
	case XKEY_SCROLLOCK:return "XKEY_SCROLLOCK";
	case XKEY_RSHIFT:return "XKEY_RSHIFT";
	case XKEY_LSHIFT:return "XKEY_LSHIFT";
	case XKEY_RCTRL:return "XKEY_RCTRL";
	case XKEY_LCTRL:return "XKEY_LCTRL";
	case XKEY_RALT:return "XKEY_RALT";
	case XKEY_LALT:return "XKEY_LALT";
	case XKEY_RMETA:return "XKEY_RMETA";
	case XKEY_LMETA:return "XKEY_LMETA";
	case XKEY_LSUPER:return "XKEY_LSUPER";
	case XKEY_RSUPER:return "XKEY_RSUPER";
	case XKEY_MODE:return "XKEY_MODE";
	case XKEY_COMPOSE:return "XKEY_COMPOSE";
	case XKEY_HELP:return "XKEY_HELP";
	case XKEY_PRINT:return "XKEY_PRINT";
	case XKEY_SYSREQ:return "XKEY_SYSREQ";
	case XKEY_BREAK:return "XKEY_BREAK";
	case XKEY_MENU:return "XKEY_MENU";
	case XKEY_POWER:return "XKEY_POWER";
	case XKEY_EURO:return "XKEY_EURO";
	case XKEY_UNDO:return "XKEY_UNDO";
	case XKEY_MAX:return "XKEY_MAX";
	case XKEY_LEFTBRACES:return "XKEY_LEFTBRACES";	//{
	case XKEY_SEPARATOR:return "XKEY_SEPARATOR";		//|
	case XKEY_RIGHTBRACES:return "XKEY_RIGHTBRACES";	//}
	case XKEY_WAVE:return "XKEY_WAVE";
	case XKEY_PERSENT:return "XKEY_PERSENT";
	default:return "UNKOWN";
	}
}
XKeyValue vkToKeyValue(unsigned char vk,bool withShift)
{
	switch(vk)
	{
	case VK_BACK:return XKEY_BACKSPACE;
	case VK_TAB:return XKEY_TAB;
	case VK_CLEAR:return XKEY_CLEAR;
	case VK_RETURN:return XKEY_RETURN;
	case VK_SHIFT:return XKEY_LSHIFT;
	case VK_CONTROL:return XKEY_LCTRL;
	case VK_MENU:return XKEY_LALT;
	case VK_PAUSE:return XKEY_PAUSE;
	case VK_CAPITAL:return XKEY_CAPSLOCK;
	//case VK_KANA:return XKEY_CAPSLOCK;
	//case VK_HANGUEL:return XKEY_CAPSLOCK;
	//case VK_HANGUL:return XKEY_CAPSLOCK;
	//case VK_JUNJA:return XKEY_CAPSLOCK;
	//case VK_FINAL:return XKEY_CAPSLOCK;
	//case VK_HANJA:return XKEY_CAPSLOCK;
	//case VK_KANJI:return XKEY_CAPSLOCK;
	case VK_ESCAPE:return XKEY_ESCAPE;
	//case VK_CONVERT:return XKEY_ESCAPE;
	//case VK_NONCONVERT:return XKEY_ESCAPE;
	//case VK_ACCEPT:return XKEY_ESCAPE;
	//case VK_MODECHANGE:return XKEY_ESCAPE;
	case VK_SPACE:return XKEY_SPACE;
	case VK_PRIOR:return XKEY_PAGEUP;
	case VK_NEXT:return XKEY_PAGEDOWN;
	case VK_END:return XKEY_END;
	case VK_HOME:return XKEY_HOME;
	case VK_LEFT:return XKEY_LEFT;
	case VK_UP:return XKEY_UP;
	case VK_RIGHT:return XKEY_RIGHT;
	case VK_DOWN:return XKEY_DOWN;
	//case VK_SELECT:return ;
	case VK_PRINT:return XKEY_PRINT;
	//case VK_EXECUTE:return ;
	//case VK_SNAPSHOT:return XKEY_PRINT;
	case VK_INSERT:return XKEY_INSERT;
	case VK_DELETE:return XKEY_DELETE;
	case VK_HELP:return XKEY_HELP;
	case 'A':return XKEY_A;
	case 'B':return XKEY_B;
	case 'C':return XKEY_C;
	case 'D':return XKEY_D;
	case 'E':return XKEY_E;
	case 'F':return XKEY_F;
	case 'G':return XKEY_G;
	case 'H':return XKEY_H;
	case 'I':return XKEY_I;
	case 'J':return XKEY_J;
	case 'K':return XKEY_K;
	case 'L':return XKEY_L;
	case 'M':return XKEY_M;
	case 'N':return XKEY_N;
	case 'O':return XKEY_O;
	case 'P':return XKEY_P;
	case 'Q':return XKEY_Q;
	case 'R':return XKEY_R;
	case 'S':return XKEY_S;
	case 'T':return XKEY_T;
	case 'U':return XKEY_U;
	case 'V':return XKEY_V;
	case 'W':return XKEY_W;
	case 'X':return XKEY_X;
	case 'Y':return XKEY_Y;
	case 'Z':return XKEY_Z;
	case '0':if(withShift) return XKEY_RIGHTPAREN;else return XKEY_0;
	case '1':if(withShift) return XKEY_EXCLAIM;else return XKEY_1;
	case '2':if(withShift) return XKEY_AT;else return XKEY_2;
	case '3':if(withShift) return XKEY_HASH;else return XKEY_3;
	case '4':if(withShift) return XKEY_DOLLAR;else return XKEY_4;
	case '5':if(withShift) return XKEY_PERSENT;else return XKEY_5;
	case '6':if(withShift) return XKEY_CARET;else return XKEY_6;
	case '7':if(withShift) return XKEY_AMPERSAND;else return XKEY_7;
	case '8':if(withShift) return XKEY_ASTERISK;else return XKEY_8;
	case '9':if(withShift) return XKEY_LEFTPAREN;else return XKEY_9;
	case VK_NUMPAD0:return XKEY_0;
	case VK_NUMPAD1:return XKEY_1;
	case VK_NUMPAD2:return XKEY_2;
	case VK_NUMPAD3:return XKEY_3;
	case VK_NUMPAD4:return XKEY_4;
	case VK_NUMPAD5:return XKEY_5;
	case VK_NUMPAD6:return XKEY_6;
	case VK_NUMPAD7:return XKEY_7;
	case VK_NUMPAD8:return XKEY_8;
	case VK_NUMPAD9:return XKEY_9;
	case VK_LWIN:return XKEY_LSUPER;
	case VK_RWIN:return XKEY_RSUPER;
	//case VK_APPS:
	//case VK_SLEEP:
	case VK_MULTIPLY:return XKEY_ASTERISK;
	case VK_ADD:return XKEY_PLUS;
	//case VK_SEPARATOR:return;
	case VK_SUBTRACT:return XKEY_MINUS;
	case VK_DECIMAL:return XKEY_PERIOD;
	case VK_DIVIDE:return XKEY_SLASH;
	case VK_F1:return XKEY_F1;
	case VK_F2:return XKEY_F2;
	case VK_F3:return XKEY_F3;
	case VK_F4:return XKEY_F4;
	case VK_F5:return XKEY_F5;
	case VK_F6:return XKEY_F6;
	case VK_F7:return XKEY_F7;
	case VK_F8:return XKEY_F8;
	case VK_F9:return XKEY_F9;
	case VK_F10:return XKEY_F10;
	case VK_F11:return XKEY_F11;
	case VK_F12:return XKEY_F12;
	case VK_F13:return XKEY_F13;
	case VK_F14:return XKEY_F14;
	case VK_F15:return XKEY_F15;
	//case VK_F16:return XKEY_F10;
	//case VK_F17:return XKEY_F11;
	//case VK_F18:return XKEY_F12;
	//case VK_F19:return XKEY_F13;
	//case VK_F20:return XKEY_F10;
	//case VK_F21:return XKEY_F11;
	//case VK_F22:return XKEY_F12;
	//case VK_F23:return XKEY_F13;
	//case VK_F24:return XKEY_F14;
	case VK_NUMLOCK:return XKEY_NUMLOCK;
	case VK_SCROLL:return XKEY_SCROLLOCK;
	case VK_LSHIFT:return XKEY_LSHIFT;
	case VK_RSHIFT:return XKEY_RSHIFT;
	case VK_LCONTROL:return XKEY_LCTRL;
	case VK_RCONTROL:return XKEY_RCTRL;
	case VK_LMENU:return XKEY_LALT;
	case VK_RMENU:return XKEY_RALT;
	//case VK_BROWSER_BACK:
	//case VK_BROWSER_FORWARD:
	//case VK_BROWSER_REFRESH:
	//case VK_BROWSER_STOP:
	//case VK_BROWSER_SEARCH:
	//case VK_BROWSER_FAVORITES:
	//case VK_BROWSER_HOME:
	//case VK_VOLUME_MUTE:
	//case VK_VOLUME_DOWN:
	//case VK_VOLUME_UP:
	//case VK_MEDIA_NEXT_TRACK:
	//case VK_MEDIA_PREV_TRACK:
	//case VK_MEDIA_STOP:
	//case VK_MEDIA_PLAY_PAUSE:
	//case VK_LAUNCH_MAIL:
	//case VK_LAUNCH_MEDIA_SELECT:
	//case VK_LAUNCH_APP1:
	//case VK_LAUNCH_APP2:
	case VK_OEM_2:if(withShift) return XKEY_QUESTION;else return XKEY_SLASH;
	case VK_OEM_3:if(withShift) return XKEY_WAVE;/*~*/else return XKEY_BACKQUOTE;
	case VK_OEM_4:if(withShift) return XKEY_LEFTBRACES;/*{*/else return XKEY_LEFTBRACKET;
	case VK_OEM_5:if(withShift) return XKEY_SEPARATOR;/*|*/else return XKEY_BACKSLASH;
	case VK_OEM_6:if(withShift) return XKEY_RIGHTBRACES;/*}*/else return XKEY_RIGHTBRACKET;
	case VK_OEM_7:if(withShift) return XKEY_QUOTEDBL;/*"*/else return XKEY_QUOTE;
	//case VK_OEM_8:if(withShift) return XKEY_QUOTEDBL;/*"*/else return XKEY_QUOTE;
	case VK_OEM_COMMA:if(withShift) return XKEY_LESS;/*<*/else return XKEY_COMMA;
	case VK_OEM_PERIOD:if(withShift) return XKEY_GREATER;/*>*/else return XKEY_PERIOD;
	case VK_OEM_1:if(withShift) return XKEY_COLON;/*:*/else return XKEY_SEMICOLON;
	case VK_OEM_MINUS:if(withShift) return XKEY_UNDERSCORE;/*_*/else return XKEY_MINUS;
	case VK_OEM_PLUS:if(withShift) return XKEY_PLUS;/*+*/else return XKEY_EQUALS;
	default:return XKEY_UNKNOWN;
	}
}
}
}