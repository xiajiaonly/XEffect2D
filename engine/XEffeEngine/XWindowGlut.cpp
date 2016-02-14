#include "XStdHead.h"
#include "XWindowGlut.h"
#include "XMouseAndKeyBoardDefine.h"
namespace XE{
bool XWindowGlut::createWindow(int width,int height,const char *windowTitle,int isFullScreen,int/*withFrame*/)
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);	//GLUT_MULTISAMPLE启用多重采样
    glutInitWindowPosition(100,100);
    glutInitWindowSize(width, height);
    glutCreateWindow(windowTitle);
	if(isFullScreen != 0) glutFullScreen();
	//if(withFrame != 0) ;
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);	//使得STENCIL生效，能产生镜面效果。

	return true;
}
int XWindowGlut::mapKey(int key)
{
	if(key < 512)
	{
		switch(key)
		{
			//字母
		case 'a':
		case 'A':
			return XKEY_A;
		case 'b':
		case 'B':
			return XKEY_B;
		case 'c':
		case 'C':
			return XKEY_C;
		case 'd':
		case 'D':
			return XKEY_D;
		case 'e':
		case 'E':
			return XKEY_E;
		case 'f':
		case 'F':
			return XKEY_F;
		case 'g':
		case 'G':
			return XKEY_G;
		case 'h':
		case 'H':
			return XKEY_H;
		case 'i':
		case 'I':
			return XKEY_I;
		case 'j':
		case 'J':
			return XKEY_J;
		case 'k':
		case 'K':
			return XKEY_K;
		case 'l':
		case 'L':
			return XKEY_L;
		case 'm':
		case 'M':
			return XKEY_M;
		case 'n':
		case 'N':
			return XKEY_N;
		case 'o':
		case 'O':
			return XKEY_O;
		case 'p':
		case 'P':
			return XKEY_P;
		case 'q':
		case 'Q':
			return XKEY_Q;
		case 'r':
		case 'R':
			return XKEY_R;
		case 's':
		case 'S':
			return XKEY_S;
		case 't':
		case 'T':
			return XKEY_T;
		case 'u':
		case 'U':
			return XKEY_U;
		case 'v':
		case 'V':
			return XKEY_V;
		case 'w':
		case 'W':
			return XKEY_W;
		case 'x':
		case 'X':
			return XKEY_X;
		case 'y':
		case 'Y':
			return XKEY_Y;
		case 'z':
		case 'Z':
			return XKEY_Z;
			//数字
		case '0': return XKEY_0;
		case '1': return XKEY_1;
		case '2': return XKEY_2;
		case '3': return XKEY_3;
		case '4': return XKEY_4;
		case '5': return XKEY_5;
		case '6': return XKEY_6;
		case '7': return XKEY_7;
		case '8': return XKEY_8;
		case '9': return XKEY_9;
			//功能按键
		case 32: return XKEY_SPACE;
		case 27: return XKEY_ESCAPE;
		case 127: return XKEY_DELETE;
		case 8: return XKEY_BACKSPACE;
		case 13: return XKEY_RETURN;
		case 9: return XKEY_TAB;
			//符号
		case '+': return XKEY_PLUS;
	//	case '-': return ;
	//	case '*': return ;
	//	case '/': return ;
		default:
			return XKEY_UNKNOWN;
		}
	}else
	{
		switch(key - 512)
		{
		case GLUT_KEY_F1: return XKEY_F1; 
		case GLUT_KEY_F2: return XKEY_F2; 
		case GLUT_KEY_F3: return XKEY_F3; 
		case GLUT_KEY_F4: return XKEY_F4; 
		case GLUT_KEY_F5: return XKEY_F5; 
		case GLUT_KEY_F6: return XKEY_F6; 
		case GLUT_KEY_F7: return XKEY_F7; 
		case GLUT_KEY_F8: return XKEY_F8; 
		case GLUT_KEY_F9: return XKEY_F9; 
		case GLUT_KEY_F10: return XKEY_F10; 
		case GLUT_KEY_F11: return XKEY_F11; 
		case GLUT_KEY_F12: return XKEY_F12; 
		case GLUT_KEY_LEFT: return XKEY_LEFT; 
		case GLUT_KEY_RIGHT: return XKEY_RIGHT; 
		case GLUT_KEY_UP: return XKEY_UP; 
		case GLUT_KEY_DOWN: return XKEY_DOWN; 
		case GLUT_KEY_PAGE_UP: return XKEY_PAGEUP; 
		case GLUT_KEY_PAGE_DOWN: return XKEY_PAGEDOWN; 
		case GLUT_KEY_HOME: return XKEY_HOME; 
		case GLUT_KEY_END: return XKEY_END; 
		case GLUT_KEY_INSERT: return XKEY_INSERT; 
		default:
			return XKEY_UNKNOWN;
		}
	}
}
}