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

#include "XSprite.h"
#include "XMaskTex.h"
#include "XFrame.h"
#include "XFont.h"
#include "XFontUnicode.h"
#include "XBasicWindow.h"
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
#include "XXml.h"
#include "XCommonDefine.h"
#include "XConfigManager.h"
#include "XTimer.h"
#include "XErrorReporter.h"
#include "XThread.h"
#include "XPixelCommon.h"
#include "XInputEventCommon.h"
#include "XWindowCommon.h"
#include "XPressDataMode.h"

#include "XMovieFfmpeg.h"

#include "XEffeEngine.h"
#include "XBmp.h"
#include "XDataBasic.h"
#include "XWindowTitle.h"

#include "XTextureInformation.h"
#include "XFrameEx.h"
#include "X2DPacker.h"
#include "XResourcePack.h"

#pragma comment(lib, "../../engine/bin/XEffectEngine_lib.lib")

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif

extern _XBool setSystemFont(const char * filename);
extern _XBool readSystemConfigData(_XWindowData &data);
extern _XBool saveSystemConfigData(const _XWindowData &data);

//注意：3D中1.0f代表1毫米，既1mm
extern _XBool initWindowEx(const _XWindowData& windowData);
inline _XBool initWindowEx(void)
{
	//if(!readSystemConfigData(XEE::windowData)) return XFalse;
	if(!readSystemConfigData(XEE::windowData)) LogStr("读取配置文件失败!");	//2014.4.13修改为读取配置文件失败之后不退出，而是使用默认值建立窗口
	return initWindowEx(XEE::windowData);
}
extern void setProjectAsDaemon();	//设置项目为后台项目

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XControl/XControlManager.h"

//下面是引擎的一些功能函数
namespace XEE
{
	//inline void errProc()
	//{
	//	_XErrorReporter::GetInstance().errorProc();
	//}
	extern int engineIdle();		//引擎的自循环函数用于循环事件处理
	extern void (*drawAfterCtrl)();	//描绘玩控件之后的描绘动作的回调函数
	inline void begin2DDraw()
	{
		if(XEE::windowData.windowType == WINDOW_TYPE_3D &&
			!needReset3DParam)
		{//如果原来为3D窗口才需要这么做
			needReset3DParam = XTrue;
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glDisable(GL_LIGHTING);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_DEPTH);
			glDisable(GL_DEPTH_TEST);
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0,XEE::windowData.w,XEE::windowData.h,0,-1,1);
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
			glDisable(GL_CULL_FACE);
		}
	}
	inline void end2DDraw()
	{
		if(needReset3DParam)
		{//恢复3D描绘的一些参数
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glEnable(GL_CULL_FACE);
			//glEnable(GL_LIGHTING);
			//glEnable(GL_ALPHA_TEST);
			//glEnable(GL_DEPTH);
			//glEnable(GL_DEPTH_TEST);
			glPopAttrib();
			needReset3DParam = XFalse;
		}
	}
	inline void clearScreen()		//清除屏幕的内容
	{
		if(XEE::windowData.windowType == WINDOW_TYPE_3D) 
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//3D
		else glClear(GL_COLOR_BUFFER_BIT);	//only 2D
	}
	inline void clearScreen(const _XFColor & c)
	{
		glClearColor(c.fR,c.fG,c.fB,c.fA);			//清除颜色
		if(XEE::windowData.windowType == WINDOW_TYPE_3D) 
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//3D
		else glClear(GL_COLOR_BUFFER_BIT);	//only 2D
		glClearColor(XEE::defaultBGColor.fR,XEE::defaultBGColor.fG,XEE::defaultBGColor.fB,XEE::defaultBGColor.fA);			//清除颜色
	}
	extern _XBool engineInit();	//窗口建立之前的初始化
	inline void engineSysInit()	//窗口建立之后的初始化
	{
		setSystemFont(XEE::windowData.systemFontFile.c_str());
		if(XEE::windowData.withCustomTitle) XEE::customWinTitle.init();
#if WITH_OBJECT_MANAGER
		_XObjManger.init();
#endif
		_XErrorReporter::GetInstance().init();
	}
	inline void engineDraw()
	{
		if(XEE::haveSystemFont && XEE::windowData.isShowVersion
			&& XEE::showVersionTimer < 60000) XEE::showVersionStr.draw();
		if(XEE::windowData.withCustomTitle) XEE::customWinTitle.draw();
		_XCtrlManger.draw();
#if WITH_OBJECT_MANAGER
		_XObjManger.draw();
#endif
		if(drawAfterCtrl != NULL) drawAfterCtrl();
		//2014.4.14,修改为显示自己的光标
#ifdef WITH_XEE_CURSOR
		if(XEE::windowData.isShowCursor != 0)
		{//下面显示光标
			drawLine(XEE::mousePosition.x - 10.0f,XEE::mousePosition.y,XEE::mousePosition.x + 10.0f,XEE::mousePosition.y,3,1.0f,1.0f,1.0f);
			drawLine(XEE::mousePosition.x,XEE::mousePosition.y - 10.0f,XEE::mousePosition.x,XEE::mousePosition.y + 10.0f,3,1.0f,1.0f,1.0f);
			drawLine(XEE::mousePosition.x - 10.0f,XEE::mousePosition.y,XEE::mousePosition.x + 10.0f,XEE::mousePosition.y,1,0.0f,0.0f,0.0f);
			drawLine(XEE::mousePosition.x,XEE::mousePosition.y - 10.0f,XEE::mousePosition.x,XEE::mousePosition.y + 10.0f,1,0.0f,0.0f,0.0f);
		}
#endif
	}
	inline void updateScreen()				//更新屏幕的内容
	{
		engineDraw();
		end2DDraw();
		_XWindow.update();
#ifdef GET_ALL_PIXEL
		static int maxPixel = 0;
		if(XEE_AllPixel > maxPixel)
		{
			maxPixel = XEE_AllPixel;
			printf("All pixel is %d\n",XEE_AllPixel);
		}
#endif
	}
	inline void simulateFullScreen()
	{
		HWND hWnd = FindWindow(NULL,XEE::windowData.windowTitle.c_str());
		int ScreenX = GetSystemMetrics(SM_CXSCREEN); //获得全屏的宽
		int ScreenY = GetSystemMetrics(SM_CYSCREEN); //获得全屏的高
		int DialogX = GetSystemMetrics(SM_CXDLGFRAME) - 3; //获得你的窗口左边空白边框的宽度	//-3这个值，不同的系统会不同
		int DialogY = GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);  //获得你的窗口
		WINDOWPLACEMENT newment;
		newment.length = sizeof(WINDOWPLACEMENT); 
		newment.flags = WPF_RESTORETOMAXIMIZED; 
		newment.showCmd = SW_SHOWMAXIMIZED; 

		newment.ptMaxPosition.x = 0;
		newment.ptMaxPosition.y = 0;
		newment.ptMinPosition.x = 0;
		newment.ptMinPosition.y = 0;
		newment.rcNormalPosition.left = - DialogX;      //(0,0)点重合
		newment.rcNormalPosition.top = - DialogY;
		newment.rcNormalPosition.bottom = ScreenX - DialogY; //尺寸一致
		newment.rcNormalPosition.right = ScreenY - DialogX;
		SetWindowPlacement(hWnd,&newment); 
	}
}
//这是错误处理机制的前身，现在这个功能已经被_XErrorReporter类替换，详细查看_XErrorReporter的代码和说明
extern void suspend(const char * str);	//中断并显示
#endif
