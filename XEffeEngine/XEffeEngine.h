#ifndef _JIA_XEFFEENGINE_
#define _JIA_XEFFEENGINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//++++++++++++++++++++++++++++++++
/*说明:
1、引擎可以通过example09的程序对资源进行自动优化，但是注意粒子系统的资源不能通过这个程序进行优化，
否则将会造成粒子系统的贴图错误，原因在于粒子系统需要较高的贴图处理速度，所以图片资源不能进行优化，
否则将会损失效率，综合考虑之后取消对粒子系统的资源进行优化拼合。
2、需要注意的是，资源优化的资源都是在loading完成之后才能被调用，所以如果在loading之前显示的资源，
或者在loading之时使用的资源最好不要经过资源优化，否则loading将会变得没有什么意义。
3、引擎所有的资源除了music和movie都可以通过XResourcePack程序对资源进行打包和加密，之后引擎可以直
接从资源包中提取资源文件。
4、当然引擎也支持资源不优化或者支援不打包都可以直接使用。
5、目前资源只能一次loading载入，然后在使用，没有测试过动态载入，之后需要测试。
6、
*/
//--------------------------------

#include "glew.h"
#include <stdio.h>
#include <stdlib.h>

#include "XBasicOpenGL.h"

#include "SDL.h"
#include "SDL_image.h"

#include "XSprite.h"
#include "XFrame.h"
#include "XFont.h"
#include "XFontUnicode.h"
#include "XBasicSDL.h"
#include "XOSDefine.h"

#include "XNumber.h"
#include "XNumberEx.h"
#include "XSound.h"
#include "XMusic.h"
#include "XNodeLine.h"
#include "XBezierSpline.h"
#include "XBezierInterpolation.h"
#include "XLogBook.h"
#include "XString.h"
#include "XParticle/XParticles.h"
#include "XControl/XControls.h"
#include "XPhysics/XPhysicsBasic.h"
#include "tinyxml.h"

#include "XMovie.h"

#include "XEffeEngine.h"
#include "XBmp.h"
#include "XDataBasic.h"

#include "XTextureInformation.h"
#include "XFrameEx.h"
#include "X2DPacker.h"

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif

extern SDL_Surface * initWindow(int width,int height,const char *windowTitle,int isFullScreen = 0,int isShowCursor = 1);	//初始化窗口

//下面是引擎的一些功能函数
extern void XSetStop(int temp = -1);	//停止 0,play,1,stop,-1,change state
extern int XGetStopState();
extern void XSetFPS(int temp);		//设置游戏帧率，小于0为不限制帧率(目前这个函数好像没有起到应有的作用)
extern int XEngineIdle();		//引擎的自循环函数用于循环事件处理
#endif
