#include "XStdHead.h"
#include "XException.h"
#include "X3D\X3DWorld.h"
#include "XCheatModule.h"
#include "XMath\XPressDataMode.h"
#include "XControl\XSoftBoard.h"
#include "XConfigManager.h"
#include "XGameBasic.h"
#include "time.h"
#include "XWindowTitle.h"
#include "XMedia/XSoundCommon.h"
#include "XMedia/XSound.h"
#include "XImm.h"
#include <wincon.h>
#include "XWindowTitle.h"
#include "XSavePNG.h"
#include "XTimer.h"
#include "XFile.h"
#include "XMedia\XMusic.h"
#include "XMedia\XSound.h"
#include "XConfigManager.h"
#include "X3D\X3DWorld.h"
#include "XFont\XFontTTF.h"
#include "XMedia\XSoundCommon.h"
#include "XWindowCommon.h"
#include "XTextureInformation.h"
#include "XResourcePack.h"
#include "XResourceManager.h"
#include "XControl\XControlManager.h"
#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif

#if BACKGROUND_ALPHA_MODE == 0
#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")
#endif
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();
namespace XE{
XFrameWorkData gFrameworkData;
bool XFrameWorkData::create()
{
#if WITH_LOG
	pLog = XMem::createMem<XLogbook>();
	if(pLog == NULL) return false;
#endif
	pFrameWork = XMem::createMem<XFrameWork>();
	if(pFrameWork == NULL) return false;
#if WITH_OBJECT_MANAGER
	pObjManager = XMem::createMem<XObjectManager>();
	if(pObjManager == NULL) return false;
#endif
	pCtrlManager = XMem::createMem<XControlManager>();
	if(pCtrlManager == NULL) return false;
	pResManager = XMem::createMem<XResourceManager>();
	if(pResManager == NULL) return false;
	pResPack = XMem::createMem<XResourcePack>();
	if(pResPack == NULL) return false;
	pTexManager = XMem::createMem<XTextureResourceManager>();
	if(pTexManager == NULL) return false;
	pErrReporter = XMem::createMem<XErrorReporter>();
	if(pErrReporter == NULL) return false;
	pMusicPlayer = XMem::createMem<XMusic>();
	if(pMusicPlayer == NULL) return false;
	pSoundPlayer = XMem::createMem<XSound>();
	if(pSoundPlayer == NULL) return false;
	pCfgManager = XMem::createMem<XConfigManager>();
	if(pCfgManager == NULL) return false;
	p3DWorld = XMem::createMem<X3DWorld>();
	if(p3DWorld == NULL) return false;
	pFontTTF = XMem::createMem<XFontTTF>();
	if(pFontTTF == NULL) return false;
	pOperateManager = XMem::createMem<XOperateManager>();
	if(pOperateManager == NULL) return false;
	pSoundCore = XMem::createMem<XSoundSDLMixer>();
	if(pSoundCore == NULL) return false;
//	pSoundCore = XMem::createMem<XSoundFmod>();
//	if(pSoundCore == NULL) return false;
#ifdef CREATE_WINDOW_WITH_GLUT
	pWindowCore = XMem::createMem<XWindowGlut>();
	if(pWindowCore == NULL) return false;
#endif
#ifdef CREATE_WINDOW_WITH_SDL
	pWindowCore = XMem::createMem<XWindowSDL>();
	if(pWindowCore == NULL) return false;
#endif

	return true;
}
void XFrameWorkData::release()
{
	XMem::XDELETE(pErrReporter);
	XMem::XDELETE(pMusicPlayer);
	XMem::XDELETE(pSoundPlayer);
	XMem::XDELETE(p3DWorld);
	XMem::XDELETE(pTexManager);
	XMem::XDELETE(pFontTTF);
	XMem::XDELETE(pOperateManager);
	XMem::XDELETE(pCfgManager);
	XMem::XDELETE(pFrameWork);
#if WITH_OBJECT_MANAGER
	XMem::XDELETE(pObjManager);
#endif
	XMem::XDELETE(pCtrlManager);
	XMem::XDELETE(pResManager);
	XMem::XDELETE(pResPack);
	XMem::XDELETE(pSoundCore);
	XMem::XDELETE(pWindowCore);
#if WITH_LOG
	XMem::XDELETE(pLog);
#endif
}

XFrameWork::XFrameWork()
	:m_isInited(false)
	,m_rightBtnDownTimer(5000)
	,m_withVirtualMouse(false)

	,m_pGame(NULL)
	,m_needReset3DParam(XFalse)
	,m_drawAfterCtrl(NULL)

	,m_hwnd(0)
	,m_wHDC(0)
	,m_wCurHGLRC(0)
	,m_wCopyHGLRC(0)
	,m_forceToPaint(false)
	,m_isGameExit(false)
	,m_windowWidth(0)		//当前窗口的宽度
	,m_windowHeight(0)	//当前窗口的高度
	,m_scenePosX(0)			//场景的位置	//目前这个定义尚不完整，在很多模式下并没有效果
	,m_scenePosY(0)			//场景的位置
	,m_sceneWidth(0)		//场景的宽度
	,m_sceneHeight(0)		//场景的高度
	,m_sceneScaleX(1.0f)		//窗口的缩放比例
	,m_sceneScaleY(1.0f)		//窗口的缩放比例
	,m_viewAngle3D(45.0f)		//3D观察的角度
	,m_programPropName("xiajia_1981@163.com")
	,m_programPropValue(HANDLE(0x5A))
	,m_isSuspend(XFalse)
	,m_frameTime(0)
	,m_curFPS(0.0f)
	,m_rendDelay(33)
	,m_stopFlag(false)
	,m_manualFPS(-1)
	,m_manualFrameTime(0.0f)
	,m_defResPos(RESOURCE_AUTO)
	,m_defBGColor(0.25f,0.25f,0.25f,1.0f)
	,m_mainThreadHandle(0xffffffff)

	,m_isOpenGL2Supported(XFalse)	//OpenGL是否支持2.0是否支持GLSL
	,m_isLineSmooth(XTrue)		//是否使用划线的平滑处理
	,m_maxTextureSize(1024)		//显卡支持的最大贴图尺寸
	,m_isMultiSampleSupported(XFalse)	//是否支持多重采样
	,m_maxTextureSum(0)	//一次能同时使用的贴图的数量
	,m_isFboSupported(XFalse)	//是否支持FBO

	,m_audioSampleRate(44100)	//音频初始化的频率
	,m_audioFormat(MIX_DEFAULT_FORMAT)		//音频的格式
	,m_audioChannel(2)	//音频的声道

	,m_deamonWithConsole(true)
	,m_isProjectAsDaemon(false)
	,m_customWinProc(NULL)

	,m_autoShutDownTimer(0)
	,m_autoShutDownState(-1)
	,m_showVersionTimer(0)
	,m_isSetToFullScreen(false)
	,m_haveSystemFont(XFalse)
	,m_curframeCounter(0)
#if WITH_ENGINE_STATISTICS
	,m_withEngineStatistics(false)	//是否开启引擎级别的统计
	,m_totalTextureSum(0)			//总的贴图数量
	,m_totalTextureArea(0)		//总的贴图面积
	,m_totalVideoMemUsage(0)		//总的显存使用量
	,m_totalFrameTextureArea(0)	//每帧刷新贴图的面积(尚未实现)
	,m_curVideoMemBandwidthUsage(0.0f)	//当前内存带宽的使用(尚未实现)
#endif
#if BACKGROUND_ALPHA_MODE == 1
	,m_isBGAlphaInited(false)
	,m_pBits(NULL)
	,m_hMemDc(NULL)
	,m_oldGDIObj(NULL)
	,m_bgAlphaPbo(NULL)
	,m_hBitmap(NULL)
#endif
{
#if BACKGROUND_ALPHA_MODE == 1
	m_ptSrc.x = 0;m_ptSrc.y = 0;
#endif
	m_softBoard = XMem::createMem<XSoftBoard>();
	if(m_softBoard == NULL) LogStr("Mem Error!");
	m_moveX = XMem::createMem<XPressDataDouble>();
	if(m_moveX == NULL) LogStr("Mem Error!");
	m_moveY = XMem::createMem<XPressDataDouble>();
	if(m_moveY == NULL) LogStr("Mem Error!");
	m_cheatModule = XMem::createMem<XCheatModule>();
	if(m_cheatModule == NULL) LogStr("Mem Error!");
//	m_pConfigMng = XMem::createMem<XConfigManager>();
//	if(m_pConfigMng == NULL) LogStr("Mem Error!");
	m_customWinTitle = XMem::createMem<XWindowTitle>();
	if(m_customWinTitle == NULL) LogStr("Mem Error!");
	m_showVersionFont = XMem::createMem<XFontUnicode>();
	if(m_showVersionFont == NULL) LogStr("Mem Error!");
	m_systemFont = XMem::createMem<XFontUnicode>();
	if(m_systemFont == NULL) LogStr("Mem Error!");
	m_periodicLogTimer = XMem::createMem<XSimpleTimer>();
	if(m_periodicLogTimer == NULL) LogStr("Mem Error!");
}
XFrameWork::~XFrameWork()
{
	XMem::XDELETE(m_softBoard);
	XMem::XDELETE(m_moveX);
	XMem::XDELETE(m_moveY);
	XMem::XDELETE(m_cheatModule);
//	XMem::XDELETE(m_pConfigMng);
	XMem::XDELETE(m_customWinTitle);
	XMem::XDELETE(m_showVersionFont);
	XMem::XDELETE(m_systemFont);
	XMem::XDELETE(m_periodicLogTimer);
}
void XFrameWork::engineRelease()
{
	gFrameworkData.pSoundPlayer->release();
	XCurSndCore.close();
	XWindow.release();
#if BACKGROUND_ALPHA_MODE == 1
	releaseBhAlphaData();
#endif
}
void XFrameWork::engineKeyProc(int key,XKeyState keyState)
{
	XCtrlManager.keyProc(key,keyState);
#if WITH_OBJECT_MANAGER
	XObjManager.keyProc(key,keyState);
#endif
}
XVector2 XFrameWork::getMousePosFromWindow(int mouseX,int mouseY)	//系统坐标空间向窗口坐标空间进行转换
{
	float x = 0,y = 0;
	switch(m_windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
		switch(m_windowData.turnOverMode)
		{
		case WINDOW_TURNOVER_MODE_NULL:
			x = mouseX * m_sceneScaleX + m_scenePosX;
			y = mouseY * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR:
			x = (m_windowData.w - mouseX) * m_sceneScaleX + m_scenePosX;
			y = mouseY * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_TTD:
			x = mouseX * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseY) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR_TTD:
			x = (m_windowData.w - mouseX) * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseY) * m_sceneScaleY + m_scenePosY;
			break;
		}
		break;
	case WINDOW_ROTATE_MODE_90:
		switch(m_windowData.turnOverMode)
		{
		case WINDOW_TURNOVER_MODE_NULL:
			x = (m_windowData.w - mouseY) * m_sceneScaleX + m_scenePosX;
			y = mouseX * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR:
			x = mouseY * m_sceneScaleX + m_scenePosX;
			y = mouseX * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_TTD:
			x = (m_windowData.w - mouseY) * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseX) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR_TTD:
			x = mouseY * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseX) * m_sceneScaleY + m_scenePosY;
			break;
		}
		break;
	case WINDOW_ROTATE_MODE_180:
		switch(m_windowData.turnOverMode)
		{
		case WINDOW_TURNOVER_MODE_NULL:
			x = (m_windowData.w - mouseX) * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseY) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR:
			x = mouseX * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseY) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_TTD:
			x = (m_windowData.w - mouseX) * m_sceneScaleX + m_scenePosX;
			y = mouseY * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR_TTD:
			x = mouseX * m_sceneScaleX + m_scenePosX;
			y = mouseY * m_sceneScaleY + m_scenePosY;
			break;
		}
		break;
	case WINDOW_ROTATE_MODE_270:
		switch(m_windowData.turnOverMode)
		{
		case WINDOW_TURNOVER_MODE_NULL:
			x = mouseY * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseX) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR:
			x = (m_windowData.w - mouseY) * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseX) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_TTD:
			x = mouseY * m_sceneScaleX + m_scenePosX;
			y = mouseX * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR_TTD:
			x = (m_windowData.w - mouseY) * m_sceneScaleX + m_scenePosX;
			y = mouseX * m_sceneScaleY + m_scenePosY;
			break;
		}
		break;
	}
	return XVector2(x,y);
}
void XFrameWork::engineMouseProc(int x,int y,XMouseState mouseState)
{
/*	XVector2 tempPos(x,y);
	switch(mouseState)
	{
	case MOUSE_MOVE:
		m_mousePosition = tempPos = getMousePosFromWindow(x,y);
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_RIGHT_BUTTON_DOWN:
		if(m_windowData.isFullScreen != 0 && m_windowData.isTouch != 0)
			tempPos = getMousePosFromWindow(XEE_SDL_getMouseX(),XEE_SDL_getMouseY());
		else
			tempPos = getMousePosFromWindow(x,y);
		break;
	case MOUSE_LEFT_BUTTON_UP:
	case MOUSE_RIGHT_BUTTON_UP:
		tempPos = getMousePosFromWindow(x,y);
		break;
	case MOUSE_MIDDLE_BUTTON_DOWN:
	case MOUSE_MIDDLE_BUTTON_UP:
		break;
	}
	XCtrlManager.mouseProc(tempPos.x,tempPos.y,mouseState);
#if WITH_OBJECT_MANAGER
	XObjManager.mouseProc(tempPos.x,tempPos.y,mouseState);
#endif
	*/
	//方案2,会影响整体设计，但是速度快
	XVector2 tempPos(x,y);
	switch(mouseState)
	{
	case MOUSE_MOVE:
		m_mousePosition = tempPos = getMousePosFromWindow(x,y);
		XCtrlManager.mouseProc(tempPos.x,tempPos.y,mouseState);
#if WITH_OBJECT_MANAGER
		XObjManager.mouseProc(tempPos.x,tempPos.y,mouseState);
#endif
		if(m_windowData.withCustomTitle) m_customWinTitle->mouseProc(tempPos.x,tempPos.y,mouseState);
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		if(m_windowData.isFullScreen != 0 && m_windowData.isTouch != 0)
			tempPos = getMousePosFromWindow(XEE_SDL_getMouseX(),XEE_SDL_getMouseY());
		else
			tempPos = getMousePosFromWindow(x,y);
		XCtrlManager.mouseProc(tempPos.x,tempPos.y,mouseState);
#if WITH_OBJECT_MANAGER
		XObjManager.mouseProc(tempPos.x,tempPos.y,mouseState);
#endif
		if(m_windowData.withCustomTitle) m_customWinTitle->mouseProc(tempPos.x,tempPos.y,mouseState);
		break;
	case MOUSE_RIGHT_BUTTON_DOWN:
		if(m_windowData.isFullScreen != 0 && m_windowData.isTouch != 0)
			tempPos = getMousePosFromWindow(XEE_SDL_getMouseX(),XEE_SDL_getMouseY());
		else
			tempPos = getMousePosFromWindow(x,y);
		XCtrlManager.mouseProc(tempPos.x,tempPos.y,mouseState);
		//if(m_windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
		break;
	case MOUSE_LEFT_BUTTON_UP:
		tempPos = getMousePosFromWindow(x,y);
		XCtrlManager.mouseProc(tempPos.x,tempPos.y,mouseState);
#if WITH_OBJECT_MANAGER
		XObjManager.mouseProc(tempPos.x,tempPos.y,mouseState);
#endif
		if(m_windowData.withCustomTitle) m_customWinTitle->mouseProc(tempPos.x,tempPos.y,mouseState);
		break;
	case MOUSE_RIGHT_BUTTON_UP:
		tempPos = getMousePosFromWindow(x,y);
		XCtrlManager.mouseProc(tempPos.x,tempPos.y,mouseState);
		//if(m_windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
		break;
	case MOUSE_MIDDLE_BUTTON_DOWN:
	case MOUSE_MIDDLE_BUTTON_UP:
		break;
	case MOUSE_WHEEL_UP_DOWN:
	case MOUSE_WHEEL_UP_UP:
	case MOUSE_WHEEL_DOWN_DOWN:
	case MOUSE_WHEEL_DOWN_UP:
		tempPos = getMousePosFromWindow(x,y);
		XCtrlManager.mouseProc(tempPos.x,tempPos.y,mouseState);
		//if(m_windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
		break;
	}
}
void XFrameWork::engineInputEvent(const XInputEvent& inputEvent)
{
	static bool isImmState = false;
	switch(inputEvent.type)
	{
	case EVENT_KEYBOARD:
		if(XImm::getCompositionState()) isImmState = true;
		switch(inputEvent.keyState)
		{
		case KEY_STATE_DOWN:	//键盘按下
			if(!isImmState)
			{
				//printf("DOWN\n");
				engineKeyProc(inputEvent.keyValue,KEY_STATE_DOWN);
			}
			break;
		case KEY_STATE_UP:	//键盘弹起
			if(!isImmState)
			{
				//printf("UP\n");
				engineKeyProc(inputEvent.keyValue,KEY_STATE_UP);
			}
			if(!XImm::getCompositionState()) isImmState = false;
			break;
		}
		break;
	case EVENT_MOUSE:
		engineMouseProc(inputEvent.mouseX,inputEvent.mouseY,inputEvent.mouseState);
		break;
	}
}
XBool XFrameWork::mouseProcCommon(float x,float y,XMouseState mouseState)//这里控制如何开启软键盘
{//右键双击来控制切换软键盘
	if(mouseState != MOUSE_RIGHT_BUTTON_DOWN) return XFalse;
	if(m_rightBtnDownTimer < 300)
	{//有效
		if(m_softBoard->getVisible()) m_softBoard->disVisible();
		else m_softBoard->setVisible();
	}else
	{
		m_rightBtnDownTimer = 0;
	}
	return XFalse;
}
bool XFrameWork::cmdProc(void *pClass,const char *cmd,std::string &ret)
{
	XFrameWork &pPar = *(XFrameWork *)pClass;
	if(strcmp(cmd,"-?") == 0 || strcmp(cmd,"-help") == 0)
	{//下面显示允许的命令条目和说明
		ret = "-windowWidth 获取窗口宽度\n";
		ret += "-windowHeight 获取窗口高度\n";
		ret += "-windowSize 获取窗口尺寸\n";
		ret += "-fps 当前帧率\n";
		ret += "-switchFullScreen 全屏与窗口进行切换\n";
		ret += "-virtualMouse 虚拟鼠标开关\n";
		ret += "-setRegRun 设置开机运行\n";
		ret += "-disRegRun 取消开机运行\n";
		ret += "-softBoard 软键盘开关\n";
		ret += "-exit 程序退出\n";
		ret += "-gameInfo 获取程序信息\n";
		return true;
	}
	if(strcmp(cmd,"-windowWidth") == 0)
	{
		ret = XString::toString(pPar.m_windowWidth);
		return true;
	}
	if(strcmp(cmd,"-exit") == 0)
	{
		ret = "程序退出";
		if (gFrameworkData.pFrameWork != NULL)
			gFrameworkData.pFrameWork->setGameExit();
		return true;
	}
	if (strcmp(cmd, "-gameInfo") == 0)
	{
		ret = "程序信息:\n";
		ret += "执行文件：" + XFile::getCurrentExeFileFullPath() + "\n";
		ret += "编译时间：" + std::string(__DATE__) + " " + std::string(__TIME__) + "\n";
		if (gFrameworkData.pFrameWork != NULL)
		{
			ret += "项目名称：" + gFrameworkData.pFrameWork->m_windowData.windowTitle + "\n";
			ret += "版本号：" + gFrameworkData.pFrameWork->m_windowData.versionString + "\n";
		}
		return true;
	}
	if(strcmp(cmd,"-windowHeight") == 0)
	{
		ret = XString::toString(pPar.m_windowHeight);
		return true;
	}
	if(strcmp(cmd,"-windowSize") == 0)
	{
		ret = XString::toString(pPar.m_windowWidth) + "," + XString::toString(pPar.m_windowHeight);
		return true;
	}
	if(strcmp(cmd,"-fps") == 0)
	{
		ret = XString::toString(pPar.m_curFPS);
		return true;
	}
	if(strcmp(cmd,"-switchFullScreen") == 0)
	{
		pPar.toggleFullScreen();
		if(pPar.m_isSetToFullScreen)
			ret = "设置为全屏";
		else
			ret = "设置为窗口";
		return true;
	}
	if(strcmp(cmd,"-virtualMouse") == 0)
	{
		pPar.m_withVirtualMouse = !pPar.m_withVirtualMouse;
		if(pPar.m_withVirtualMouse) 
		{
			ret = "开启虚拟鼠标";
		//	pPar.m_moveX.setCurData(m_mousePosition.x);
		//	pPar.m_moveY.move(m_mousePosition.y);
		}else ret = "关闭虚拟鼠标";
		return true;
	}
	if(strcmp(cmd,"-setRegRun") == 0)
	{
		if(XEE::setRegRun(true)) ret = "设置开机启动成功";
		else ret = "设置开机启动失败,需要管理员权限.";
		return true;
	}
	if(strcmp(cmd,"-disRegRun") == 0)
	{
		if(XEE::setRegRun(false)) ret = "取消开机启动成功";
		else ret = "取消开机启动失败,需要管理员权限.";
		return true;
	}
	if(strcmp(cmd,"-softBoard") == 0)
	{
		if(pPar.m_softBoard->getVisible())
		{
			ret = "隐藏软键盘";
			pPar.m_softBoard->disVisible();
		}else
		{
			ret = "显示软键盘";
			pPar.m_softBoard->setVisible();
		}
		return true;
	}
	return false;
}
XBool XFrameWork::keyboardProcCommon(int keyOrder,XKeyState keyState)	//这里响应一些特殊的按键信息
{
	m_cheatModule->keyboardProc(keyOrder,keyState);
	if(m_withVirtualMouse)
	{//开启虚拟鼠标
		switch(keyOrder)
		{
		case XKEY_A:
			if(keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			else mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			break;
		case XKEY_S:
			if(keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0);
			else mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0);
			break;
		case XKEY_D:
			if(keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
			else mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
			break;
		}
	}
	return XFalse;
}
bool XFrameWork::initCommon()
{
	if(m_isInited) return false;
	if(!m_cheatModule->init()) return false;
	if(!m_softBoard->initWithoutSkin()) return false;
	m_softBoard->disVisible();
	m_cheatModule->addCmdProc(cmdProc,this);

	m_moveX->init(0.0f,m_windowWidth,0.0f,500,MODE_NO_RECOVER);
	m_moveX->setKeyValue(XKEY_LEFT,XKEY_RIGHT);
	m_moveX->setParam(1.0f,0.1f,1.0f);
	m_moveY->init(0.0f,m_windowHeight,0.0f,500,MODE_NO_RECOVER);
	m_moveY->setKeyValue(XKEY_UP,XKEY_DOWN);
	m_moveY->setParam(1.0f,0.1f,1.0f);

	m_isInited = true;
	return true;
}
void XFrameWork::updateCommon(float stepTime)
{
	if(m_rightBtnDownTimer < 1000) m_rightBtnDownTimer += stepTime;
	m_moveX->move(stepTime);
	m_moveY->move(stepTime);
	if(m_withVirtualMouse)
	{//这里设置鼠标的位置
		//printf("%f,%f\n",m_moveX.getCurData(),m_moveY.getCurData());
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,m_moveX->getCurData() * 65536.0f / m_windowWidth,
			m_moveY->getCurData() * 65536.0f / m_windowHeight,0,0);
	}
}
void XFrameWork::inputCommon(const XInputEvent &event)
{
	switch(event.type)
	{
	case EVENT_KEYBOARD:
		keyboardProcCommon(event.keyValue,event.keyState);
		break;
	case EVENT_MOUSE:
		mouseProcCommon(event.mouseX,event.mouseY,event.mouseState);
		break;
	}
	if(m_withVirtualMouse)
	{
		m_moveX->updateState(event);
		m_moveY->updateState(event);
	}
}
#ifdef CREATE_WINDOW_WITH_SDL
int XFrameWork::inputEvent()
{//下面是事件处理函数，例如：键盘事件或者鼠标事件
	XInputEvent tmpEvent;		//SDL事件句柄
	while (getInputEventSDL(tmpEvent))
	{
		switch (tmpEvent.type)
		{
		case EVENT_EXIT:return 1;
		case EVENT_RESIZE:	//窗口大小发生变化
			m_windowData.w = tmpEvent.mouseX;
			m_windowData.h = tmpEvent.mouseY;
			initWindowMatrix();
			break;
		case EVENT_KEYBOARD:
			if (tmpEvent.keyState == KEY_STATE_DOWN)
			{
				switch (tmpEvent.keyValue)
				{
				case XKEY_ESCAPE:return 1;
				}
			}
			break;
		}
		if (m_pGame != NULL)
			((XGameBasic *)m_pGame)->input(tmpEvent);
		if(XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL) 
			gFrameworkData.pCfgManager->inputProc(tmpEvent);
		inputCommon(tmpEvent);
		engineInputEvent(tmpEvent);
		if (m_windowData.windowType == WINDOW_TYPE_3D && gFrameworkData.p3DWorld != NULL)
			gFrameworkData.p3DWorld->keyEvent(tmpEvent);
	}
	if(m_isGameExit) return 1;
	return 0;
}
bool XFrameWork::singletonProc()
{
	if(m_windowData.isSingletonMode && isProgramExist(XFile::getCurrentExeFileFullPath())) return true;
	return false;
}
//void XFrameWork::startGame(void *game,const XWindowInfo* windowInfo)
//{
//	if (game == NULL) return;
//	if (windowInfo == NULL)
//	{
//		if(!readSystemConfigData(m_windowData)) LogStr("读取配置文件失败!");
//	}else
//	{
//		m_windowData = * windowInfo;
//	}
//	if(m_windowData.withException)
//	{
//		__try{
//			XException::initException();
//			if(!initWindow()) XEE_Error("建立窗体失败");
//			if(!singletonProc())
//				gameProc(game);
//			else
//				LogStr("程序已经启动，不能重复启动!");
//			engineRelease();
//		}
//		__except (XException::unhandledExceptionFilter(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
//		{
//			//XException::unhandledExceptionFilter(GetExceptionInformation());
//		}
//	}else
//	{
//		if(!initWindow()) XEE_Error("建立窗体失败");
//		if(!singletonProc())
//			gameProc(game);
//		else
//			LogStr("程序已经启动，不能重复启动!");
//		engineRelease();
//	}
//}
void XFrameWork::gameProc(void *game)
{
	m_pGame = game;
	float stepTime;
	if(m_windowData.windowType == WINDOW_TYPE_3D)
	{
		initCommon();
		XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
		if(gFrameworkData.p3DWorld == NULL || !gFrameworkData.p3DWorld->init(NULL)) XEE_Error("建立窗体失败");
		if(!tmpGame.init()) XEE_Error("初始化失败");
		XCtrlManager.setGame(&tmpGame);
		XSimpleTimer drawTimer;	//显示的计时器，显示太快实际上没有意义，逻辑线程可以跑很快，但是显示线程没有必要很快
		drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//程序主循环
			if(m_windowData.withLowConsumption) XEE::sleep(1);	//如果需要较高的反应效率，这里可以屏蔽
			stepTime = engineIdle();
			if(gFrameworkData.p3DWorld != NULL)
				gFrameworkData.p3DWorld->move(stepTime);
			if(XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL) 
				gFrameworkData.pCfgManager->update();
			tmpGame.move(stepTime);
			updateCommon(stepTime);
			if (!getWindowNeedDraw() ||
				!drawTimer.update(stepTime)) continue;
			if(m_windowData.withClearBG) clearScreen();			//清除屏幕，优化这里会有一点效果
			//TODO:下面描绘需要描绘的3D内容
			if(gFrameworkData.p3DWorld != NULL)
				gFrameworkData.p3DWorld->draw();
			begin2DDraw();

			tmpGame.draw();
			if(XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL) 
				gFrameworkData.pCfgManager->draw();
			updateScreen();			//更新屏幕的内容
		}
		setGameExit();
		tmpGame.release();
	}else
	{
		initCommon();
		XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
		if (!tmpGame.init()) XEE_Error("初始化失败");
		XCtrlManager.setGame(&tmpGame);
		XSimpleTimer drawTimer;	//显示的计时器，显示太快实际上没有意义，逻辑线程可以跑很快，但是显示线程没有必要很快
		drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//程序主循环
			if(m_windowData.withLowConsumption) XEE::sleep(1);	//如果需要较高的反应效率，这里可以屏蔽
			stepTime = engineIdle();
			if(XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL) 
				gFrameworkData.pCfgManager->update();
			tmpGame.move(stepTime);
			updateCommon(stepTime);
			if (!getWindowNeedDraw() ||
				!drawTimer.update(stepTime)) continue;
			if(m_windowData.withClearBG) clearScreen();			//清除屏幕,如果每次都全屏刷新的话，优化这里会有一点效果
			tmpGame.draw();
			if(XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL) 
				gFrameworkData.pCfgManager->draw();
			updateScreen();			//更新屏幕的内容
		}
		setGameExit();
		tmpGame.release();
	}
}
#endif
#ifdef CREATE_WINDOW_WITH_GLUT
void XFrameWork::startGame(XGameBasic *game)
{
	if (game == NULL) return;
	pGame = game;
	__try{
		glutInit(&argc, argv);
		if (!initWindowEx()) XEE_Error("建立窗体失败");
		if (!pGame->init()) XEE_Error("初始化失败");

		glutDisplayFunc(draw);
		glutIdleFunc(idle);
		//glutReshapeFunc(changeSize);
		glutKeyboardFunc(input);
		glutSpecialFunc(sInput);
		glutMouseFunc(mouseProc);
		glutMotionFunc(mouseMove);
		glutPassiveMotionFunc(mouseMove);
		glutMainLoop();

		pGame->release();
		release();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		XEE_Error("运行异常!");
	}
	return 0;
}
#endif
XBool XFrameWork::readSystemConfigData(XWindowInfo &data)
{//读取配置数据:策略修改：如果配置项不存在，则使用默认值
	TiXmlDocument doc(XEE_SYSTEM_CONFIG);
	if (!doc.LoadFile()) return XFalse;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if (rootNode == NULL) return XFalse;
	int temp;
	XXml::getXmlAsInt(rootNode, "windowType", (int &)data.windowType);
	XXml::getXmlAsInt(rootNode, "w", data.w);
	XXml::getXmlAsInt(rootNode, "h", data.h);
	XXml::getXmlAsInt(rootNode, "sceneW", data.sceneW);
	XXml::getXmlAsInt(rootNode, "sceneH", data.sceneH);
	XXml::getXmlAsInt(rootNode, "positionX", data.positionX);
	XXml::getXmlAsInt(rootNode, "positionY", data.positionY);
	XXml::getXmlAsInt(rootNode, "isFullScreen", data.isFullScreen);
	XXml::getXmlAsInt(rootNode, "isTouch", data.isTouch);
	XXml::getXmlAsInt(rootNode, "withLog", data.withLog);
	XXml::getXmlAsInt(rootNode, "isShowCursor", data.isShowCursor);
	XXml::getXmlAsInt(rootNode, "withFrame", data.withFrame);
	XXml::getXmlAsInt(rootNode, "windowMode", (int &)data.mode);
	XXml::getXmlAsString(rootNode, "windowTitle", data.windowTitle);
	XXml::getXmlAsInt(rootNode, "rotateMode", (int &)data.rotateMode);
	XXml::getXmlAsInt(rootNode, "turnOverMode", (int &)data.turnOverMode);
	XXml::getXmlAsInt(rootNode, "CFGMode", (int &)data.CFGMode);
	XXml::getXmlAsBool(rootNode, "isStartDelay", data.isStartDelay);
	XXml::getXmlAsInt(rootNode, "startDelayTimer", data.startDelayTimer);
	XXml::getXmlAsBool(rootNode, "isShowVersion", data.isShowVersion);
	XXml::getXmlAsString(rootNode, "versionString", data.versionString);
	XXml::getXmlAsBool(rootNode, "isAutoShutDown", data.isAutoShutDown);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_year", data.autoShutDownYear);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_month", data.autoShutDownMonth);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_day", data.autoShutDownDay);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_hour", data.autoShutDownHour);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_minute", data.autoShutDownMinute);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_second", data.autoShutDownSecond);
	XXml::getXmlAsBool(rootNode, "isShowConsole", data.isShowConsole);
	XXml::getXmlAsBool(rootNode, "isProjectAsDaemon", data.isProjectAsDaemon);
	XXml::getXmlAsString(rootNode, "fontFilename", data.systemFontFile);
	XXml::getXmlAsString(rootNode, "commonResource", data.commonResourcePos);
	if (XXml::getXmlAsInt(rootNode, "defaultResourcePosition", temp) != NULL)
	{
		switch (temp)
		{
		case RESOURCE_LOCAL_FOLDER:
		case RESOURCE_LOCAL_PACK:
		case RESOURCE_WEB:
		case RESOURCE_AUTO:
			data.defResourcePos = (XResourcePosition)temp;
			break;
		}//其他情况不改变资源位置的设置
	}
	XXml::getXmlAsInt(rootNode, "backgroundValume", data.BGVolume);
	XXml::getXmlAsInt(rootNode, "formwordValume", data.FWVolume);
	XXml::getXmlAsBool(rootNode, "isInitWindowPos", data.isInitWindowPos);
	XXml::getXmlAsInt(rootNode, "windowPosX", data.windowPosX);
	XXml::getXmlAsInt(rootNode, "windowPosY", data.windowPosY);
	XXml::getXmlAsBool(rootNode, "isAlwaysTop", data.isAlwaysTop);
	XXml::getXmlAsBool(rootNode, "withCustomTitle", data.withCustomTitle);
	XXml::getXmlAsBool(rootNode, "withAlphaBG", data.withAlphaBG);
	if(data.withAlphaBG) data.withFrame = 0;	//如果使用透明窗体的话，则不能有边框
	XXml::getXmlAsInt(rootNode, "logicFps", data.logicFps);
	XXml::getXmlAsInt(rootNode, "drawFps", data.drawFps);
	XXml::getXmlAsBool(rootNode, "withException", data.withException);
	XXml::getXmlAsBool(rootNode, "withLowConsumption", data.withLowConsumption);
	XXml::getXmlAsBool(rootNode, "withClearBG", data.withClearBG);
	XXml::getXmlAsBool(rootNode, "isRestartWhenCrash", data.isRestartWhenCrash);
	XXml::getXmlAsBool(rootNode, "isSingletonMode", data.isSingletonMode);

	if (data.withFrame != 0 && data.withCustomTitle) data.withCustomTitle = false;
	if (data.withCustomTitle && data.positionY > -MIN_FONT_CTRL_SIZE)
	{
		int tmpH = data.positionY + MIN_FONT_CTRL_SIZE;
		data.positionY = -MIN_FONT_CTRL_SIZE;
		data.sceneH += tmpH;
		data.h += tmpH;
	}

	return XTrue;
}
XBool XFrameWork::saveSystemConfigData(const XWindowInfo &data)
{//将配置信息保存到配置文件
	TiXmlDocument doc(XEE_SYSTEM_CONFIG);
	TiXmlDeclaration declaration("1.0", "gb2312", "yes");
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");
	if (!XXml::addComment(elmRoot, "窗口类型：0、普通窗口，1、可自适应、旋转和翻转的窗口，2、支持3D渲染的窗口")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowType", XString::toString(data.windowType))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口宽度")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "w", XString::toString(data.w))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口高度")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "h", XString::toString(data.h))) return XFalse;
	if (!XXml::addComment(elmRoot, "场景宽度")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "sceneW", XString::toString(data.sceneW))) return XFalse;
	if (!XXml::addComment(elmRoot, "场景高度")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "sceneH", XString::toString(data.sceneH))) return XFalse;
	if (!XXml::addComment(elmRoot, "场景的x偏移")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "positionX", XString::toString(data.positionX))) return XFalse;
	if (!XXml::addComment(elmRoot, "场景的y偏移")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "positionY", XString::toString(data.positionY))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否全屏：0、不全屏，1、全屏")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isFullScreen", XString::toString(data.isFullScreen))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否支持触摸屏：0、不支持，1、支持")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isTouch", XString::toString(data.isTouch))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否输出日志：0、不输出，1、后台输出，2、文件输出，3、都输出")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withLog", XString::toString(data.withLog))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否显示鼠标：0、不显示，1、显示")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isShowCursor", XString::toString(data.isShowCursor))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口是否有边框：0、没有边框，1、有边框")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withFrame", XString::toString(data.withFrame))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口模式：0、左上角对齐并裁剪，1、重点对齐并裁剪，2、短边拉伸长边裁剪，3、长边拉伸短边填空白，4、自动拉伸全屏")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowMode", XString::toString(data.mode))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口标题")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowTitle", data.windowTitle)) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口的旋转模式：0、不旋转，1、90旋转，2、180度旋转，3、270旋转")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "rotateMode", XString::toString(data.rotateMode))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口的反转模式：0、不翻转，1、左右翻转，2、上下翻转，3、左右上下翻转")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "turnOverMode", XString::toString(data.turnOverMode))) return XFalse;
	if (!XXml::addComment(elmRoot, "配置的工作模式：0、普通模式，1、服务器模式，2、客户端模式")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "CFGMode", XString::toString(data.CFGMode))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否开启启动延迟")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isStartDelay", data.isStartDelay)) return XFalse;
	if (!XXml::addComment(elmRoot, "启动延迟的时间：单位ms")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "startDelayTimer", XString::toString(data.startDelayTimer))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否显示版本号")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isShowVersion", data.isShowVersion)) return XFalse;
	if (!XXml::addComment(elmRoot, "版本号")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "versionString", data.versionString)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否启动定时关机")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isAutoShutDown", data.isAutoShutDown)) return XFalse;
	if (!XXml::addComment(elmRoot, "定时关机的时间.小于0表示这项无效")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_year", XString::toString(data.autoShutDownYear))) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_month", XString::toString(data.autoShutDownMonth))) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_day", XString::toString(data.autoShutDownDay))) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_hour", XString::toString(data.autoShutDownHour))) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_minut", XString::toString(data.autoShutDownMinute))) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_second", XString::toString(data.autoShutDownSecond))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否显示后台")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isShowConsole", data.isShowConsole)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否作为后台程序")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isProjectAsDaemon", data.isProjectAsDaemon)) return XFalse;
	if (!XXml::addComment(elmRoot, "默认资源的位置：0、文件夹，1、资源包，2、网络资源，3、系统默认")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "defaultResourcePosition", XString::toString(data.defResourcePos))) return XFalse;
	if (!XXml::addComment(elmRoot, "默认的字体文件")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "fontFilename", data.systemFontFile)) return XFalse;
	if (!XXml::addComment(elmRoot, "通用资源的位置")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "commonResource", data.commonResourcePos)) return XFalse;
	if (!XXml::addComment(elmRoot, "背景音乐的音量")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "backgroundValume", XString::toString(data.BGVolume))) return XFalse;
	if (!XXml::addComment(elmRoot, "前景音乐的音量")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "formwordValume", XString::toString(data.FWVolume))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否设置窗口位置")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isInitWindowPos", data.isInitWindowPos)) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口位置X")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowPosX", XString::toString(data.windowPosX))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口位置Y")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowPosY", XString::toString(data.windowPosY))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否置顶窗口")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isAlwaysTop", data.isAlwaysTop)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否使用自己的标题栏")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withCustomTitle", data.withCustomTitle)) return XFalse;
	if (!XXml::addComment(elmRoot, "背景是否透明")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withAlphaBG", data.withAlphaBG)) return XFalse;
	if (!XXml::addComment(elmRoot, "系统字体的大小")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "systemFontSize", XString::toString(data.systemFontSize))) return XFalse;
	if (!XXml::addComment(elmRoot, "逻辑帧率")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "logicFps", XString::toString(data.logicFps))) return XFalse;
	if (!XXml::addComment(elmRoot, "显示帧率")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "drawFps", XString::toString(data.drawFps))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否处理异常")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withException", data.withException)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否启动低耗模式")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withLowConsumption", data.withLowConsumption)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否清除背景")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withClearBG", data.withClearBG)) return XFalse;
	if (!XXml::addComment(elmRoot, "程序发生crash之后是否自动重新启动")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isRestartWhenCrash", data.isRestartWhenCrash)) return XFalse;
	if (!XXml::addComment(elmRoot, "程序运行是否只有一个实例")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isSingletonMode", data.isSingletonMode)) return XFalse;

	if (doc.InsertEndChild(elmRoot) == NULL) return XFalse;
	doc.SaveFile();
	return XTrue;
}
XBool XFrameWork::engineInit()
{
	m_defResPos = m_windowData.defResourcePos;
#if WITH_LOG
	gFrameworkData.pLog->setLogLevel((XLogBookLevel)m_windowData.withLog);
#endif
	if (m_windowData.isStartDelay
		&& m_windowData.startDelayTimer > 0) Sleep(m_windowData.startDelayTimer);	//程序启动前延迟等待一段时间
	if (!m_windowData.isShowConsole)
	{
		//FreeConsole();	//如果需要则隐藏控制后台 在内win732bit中会造成crash
		ShowWindow(GetConsoleWindow(),SW_HIDE);
	}
	if (m_windowData.isProjectAsDaemon) setProjectAsDaemon();
	//初始化一些变量的值
	m_windowWidth = m_windowData.w;
	m_windowHeight = m_windowData.h;
	m_scenePosX = m_windowData.positionX;
	m_scenePosY = m_windowData.positionY;
	m_sceneWidth = m_windowData.sceneW;
	m_sceneHeight = m_windowData.sceneH;
	m_sceneScaleX = 1.0f;		//窗口的缩放比例
	m_sceneScaleY = 1.0f;		//窗口的缩放比例
	srand((unsigned)time(0)); //引入随机因子
	m_periodicLogTimer->setStepTime(10000);	//10s一个周期
	m_defWorkPath = XFile::getWorkPath();	//记录工作路径
	return XTrue;
}
int XFrameWork::engineIdle()
{
	//		static XTimer tempTimer;
	//		static Uint32 current,last = 0,five = 0,nframes = 0;
	//		current = tempTimer.getCurrentTime();
	//		++nframes;
	//		if(current - five > 5000) 
	//		{//将帧率显示出来
	//			curFPS = nframes * 0.2f;
	////#ifdef DEBUG_MODE
	//			if(manualFPS > 0) printf("%.1f FPS@ %d\n",curFPS,manualFPS);
	//			else printf("%.1f FPS\n",curFPS);
	////#endif
	//			nframes = 0;
	//			five = current;
	//		}
	//		frameTime = current - last;
	//		last = current;
	//上下两种方式在低帧率时差距较大，不知道为什么
	static Uint32 current,last = 0;
	if(last <= 0)
	{
		last = XTime::getCurrentTicks();	//防止第一次调用的时候由于程序启动的时间消耗而造成第一帧特别长
		XEE::getCpuUsage();
	}
	current = XTime::getCurrentTicks();
	m_frameTime = current - last;
	last = current;

	++ m_curframeCounter;
	if(m_periodicLogTimer->update(m_frameTime))
	{
		m_curFPS = m_curframeCounter * 1000.0f / m_periodicLogTimer->getStepTime();	//计算当前帧率
		if (m_manualFPS > 0) LogNull("%.1f FPS@ %d", m_curFPS, m_manualFPS);
		else LogNull("%.1f FPS", m_curFPS);
		m_curframeCounter = 0;
		//输出内存的使用率
		m_curMemInfo = XEE::getMemUsageInfo();
		LogNull("Top:%d Bytes,Cur:%d Bytes,CPU:%.2f", m_curMemInfo.peakWorkSet, m_curMemInfo.curWorkingSet, XEE::getCpuUsage());
	}
	//注意：这里需要2000才能保证控制的准确，不知道为什么
	if (m_manualFPS > 0 && m_frameTime < m_manualFrameTime)
	{
		//SDL_Delay(XEE_setFrameTime - XEE_frameTime);
		Sleep(m_manualFrameTime - m_frameTime);
	}
	if (XErrReporter.isErrored()
		&& !XErrReporter.getIsMainProc())
	{
		XErrReporter.setMainProc();
		XErrReporter.errorProc();
		exit(1);
	}
	engineMove(m_frameTime);

	return m_frameTime;	//返回两次调用之间的时间差
}
void XFrameWork::engineMove(int delay)
{
	if (m_showVersionTimer < 61000)	//版本号显示一分钟
		m_showVersionTimer += delay;
	//下面是自动关机的控制
	if (m_windowData.isAutoShutDown)
	{//下面控制自动关机
		m_autoShutDownTimer += delay;
		if (m_autoShutDownTimer > 1000)
		{//一秒钟判断一次
			m_autoShutDownTimer = 0;
			XSystemTime tempT;
			XTime::getTimeMs(tempT);
			if ((tempT.year >= m_windowData.autoShutDownYear || m_windowData.autoShutDownYear < 0)
				&& (tempT.month >= m_windowData.autoShutDownMonth || m_windowData.autoShutDownMonth < 0)
				&& (tempT.day >= m_windowData.autoShutDownDay || m_windowData.autoShutDownDay < 0)
				&& (tempT.hour >= m_windowData.autoShutDownHour || m_windowData.autoShutDownHour < 0)
				&& (tempT.minute >= m_windowData.autoShutDownMinute || m_windowData.autoShutDownMinute < 0)
				&& (tempT.second >= m_windowData.autoShutDownSecond || m_windowData.autoShutDownSecond < 0))
			{//符合条件触发自动关机
				if (m_autoShutDownState == 0) XEE::shutDownSystem();
			}else
			{
				m_autoShutDownState = 0;
			}
		}
	}
	XCurSndCore.update(delay);
	XCtrlManager.update(delay);
#if WITH_OBJECT_MANAGER
	XObjManager.move(delay);
#endif
	if (m_windowData.withCustomTitle) m_customWinTitle->move(delay);
#ifdef GET_ALL_PIXEL
	XEE_AllPixel = 0;
#endif
}
void XFrameWork::engineSysInit()	//窗口建立之后的初始化
{
	setSystemFont(m_windowData.systemFontFile.c_str());
	if(m_windowData.withCustomTitle) m_customWinTitle->init();
	if(m_windowData.logicFps > 0) setFPS(m_windowData.logicFps);
	if(m_windowData.drawFps <= 0) m_rendDelay = -1;
	else m_rendDelay = 1000.0f / m_windowData.drawFps;
#if WITH_OBJECT_MANAGER
	XObjManager.init();
#endif
	XErrReporter.init();
	if(m_windowData.withAlphaBG)
	{//设置窗口背景为透明
#if BACKGROUND_ALPHA_MODE == 0
		DWORD style = GetWindowLong(m_hwnd, GWL_EXSTYLE|WS_EX_TRANSPARENT);
		style &= ~WS_OVERLAPPEDWINDOW;
		SetWindowLong(m_hwnd, GWL_EXSTYLE, style);
		DWM_BLURBEHIND bb = {0};
		HRGN hRgn = CreateRectRgn(0, 0, -1, -1);
		bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
		bb.hRgnBlur = hRgn;
		bb.fEnable = TRUE;
		DwmEnableBlurBehindWindow(m_hwnd, &bb);
#endif
#if BACKGROUND_ALPHA_MODE == 1
		DWORD style = GetWindowLong(m_hwnd, GWL_EXSTYLE|WS_EX_TRANSPARENT);
		style |= WS_EX_LAYERED;
		SetWindowLong(m_hwnd, GWL_EXSTYLE, style);
		initBgAlphaData();
#endif
	}
}
void XFrameWork::engineDraw()
{
	if (m_haveSystemFont && m_windowData.isShowVersion
		&& m_showVersionTimer < 60000) m_showVersionFont->draw();
	if (m_windowData.withCustomTitle) m_customWinTitle->draw();
	XCtrlManager.draw();
#if WITH_OBJECT_MANAGER
	XObjManager.draw();
#endif
	if (m_drawAfterCtrl != NULL) m_drawAfterCtrl();
	//2014.4.14,修改为显示自己的光标
#ifdef WITHXEE_CURSOR
	if (m_windowData.isShowCursor != 0)
	{//下面显示光标
		XRender::drawLine(mousePosition.x - 10.0f, mousePosition.y, mousePosition.x + 10.0f, mousePosition.y, 3, 1.0f, 1.0f, 1.0f);
		XRender::drawLine(mousePosition.x, mousePosition.y - 10.0f, mousePosition.x, mousePosition.y + 10.0f, 3, 1.0f, 1.0f, 1.0f);
		XRender::drawLine(mousePosition.x - 10.0f, mousePosition.y, mousePosition.x + 10.0f, mousePosition.y, 1, 0.0f, 0.0f, 0.0f);
		XRender::drawLine(mousePosition.x, mousePosition.y - 10.0f, mousePosition.x, mousePosition.y + 10.0f, 1, 0.0f, 0.0f, 0.0f);
	}
#endif
}
void XFrameWork::begin2DDraw()
{
	if (m_windowData.windowType == WINDOW_TYPE_3D &&
		!m_needReset3DParam)
	{//如果原来为3D窗口才需要这么做
		m_needReset3DParam = XTrue;
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_DEPTH);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, m_windowData.w, m_windowData.h, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_CULL_FACE);
	}
}
void XFrameWork::end2DDraw()
{
	if (!m_needReset3DParam) return;//恢复3D描绘的一些参数
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
	m_needReset3DParam = XFalse;
}
#if BACKGROUND_ALPHA_MODE == 1
void XFrameWork::initBgAlphaData()				//更新屏幕的内容
{
	if(m_isBGAlphaInited) return;
	m_isBGAlphaInited = true;
	GetWindowRect(m_hwnd,&m_rc);
	m_ptDest.x = m_rc.left;
	m_ptDest.y = m_rc.top;
	m_sizeDest.cx = m_windowWidth;
	m_sizeDest.cy = m_windowHeight;
	m_hMemDc = CreateCompatibleDC(m_wHDC);
	m_bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmpinfo.bmiHeader.biWidth = m_windowWidth;
	m_bmpinfo.bmiHeader.biHeight = m_windowHeight;
	m_bmpinfo.bmiHeader.biPlanes = 1;
	m_bmpinfo.bmiHeader.biBitCount = 32;//32bpp
	m_bmpinfo.bmiHeader.biCompression = BI_RGB;
	m_hBitmap = CreateDIBSection(m_wHDC,&m_bmpinfo,DIB_RGB_COLORS,&m_pBits,NULL,0);
	assert(m_hBitmap != NULL);
	m_blf.BlendOp = AC_SRC_OVER;
	m_blf.BlendFlags = 0;
	m_blf.SourceConstantAlpha = 255;
	m_blf.AlphaFormat = AC_SRC_ALPHA;//1;
	m_bgAlphaPbo = XMem::createMem<XPBO>();
	assert(m_bgAlphaPbo != NULL);
	m_bgAlphaPbo->init(m_windowWidth,m_windowHeight,0,0,m_windowWidth,m_windowHeight,COLOR_BGRA);
	//wglMakeCurrent(m_hMemDc,wCurrentHGLRC);
	m_oldGDIObj = SelectObject(m_hMemDc,m_hBitmap);
//	bgAlphaFbo.init(windowWidth,windowHeight,COLOR_RGBA);
}
void XFrameWork::releaseBhAlphaData()				//更新屏幕的内容
{
	if(!m_isBGAlphaInited) return;
	if(m_hMemDc != NULL)
	{
		SelectObject(m_hMemDc,m_oldGDIObj);
		DeleteDC(m_hMemDc);
		m_hMemDc = NULL;
	}
	if(m_hBitmap != NULL) 
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	XMem::XDELETE(m_bgAlphaPbo);
	m_isBGAlphaInited = false;
}
#endif
void XFrameWork::updateScreen()				//更新屏幕的内容
{
	engineDraw();
	end2DDraw();
//		//XWindow.update();		//上下两句是等效的
	SwapBuffers(m_wHDC);
#ifdef GET_ALL_PIXEL
	static int maxPixel = 0;
	if (XEE_AllPixel > maxPixel)
	{
		maxPixel = XEE_AllPixel;
		printf("All pixel is %d\n", XEE_AllPixel);
	}
#endif
#if BACKGROUND_ALPHA_MODE == 1
#ifdef ULW_ALPHA
	if(m_windowData.withAlphaBG)
	{
//		m_bgAlphaFbo->removeFBO();
		//这里R和B要反过来颜色才正确
		//glReadPixels(0,0,windowWidth,windowHeight,GL_BGRA,GL_UNSIGNED_BYTE,pBits);	//这一句性能消耗很大，需要优化掉
//		XGL::EnableTexture2D();
//		XGL::BindTexture2D(bgAlphaFbo.getTexture(0),0);
//		glGetTexImage(GL_TEXTURE_2D,0,GL_BGRA,GL_UNSIGNED_BYTE,pBits);
		if(m_bgAlphaPbo->getPixel((unsigned char *)m_pBits))
			UpdateLayeredWindow(m_hwnd,NULL,NULL,&m_sizeDest,m_hMemDc,&m_ptSrc,0,&m_blf,ULW_ALPHA);	//这里设置黑色作为透明色 ULW_ALPHA = 2
//		bgAlphaFbo.useFBO();
		//unsigned char *tmp = XWindow.getWindowBuff();
	//	if(!BitBlt(hMemDc,0,0,windowWidth,windowHeight,wHDC,0,0,SRCCOPY))
	//	{
	//		printf("error!\n");
	//	}
	//	SelectObject(hMemDc,hOldBitmap);
	}
#endif
#endif
}
XBool XFrameWork::initWindow()
{
	if (!engineInit()) return XFalse;
	//if(m_windowData.w <= 0 || m_windowData.h <= 0) return XFalse;//检查输入参数是否正确
	//srand((unsigned)time(0)); //引入随机因子
	bool ret = false;
	switch (m_windowData.windowType)
	{
	case WINDOW_TYPE_EX:
		if (m_windowData.rotateMode == WINDOW_ROTATE_MODE_90
			|| m_windowData.rotateMode == WINDOW_ROTATE_MODE_270)
			ret = createWindow(m_windowData.h, m_windowData.w,
			m_windowData.windowTitle.c_str(), m_windowData.isFullScreen, m_windowData.withFrame);
		else ret = createWindow(m_windowData.w, m_windowData.h,
			m_windowData.windowTitle.c_str(), m_windowData.isFullScreen, m_windowData.withFrame);
		break;
	case WINDOW_TYPE_NORMAL:
		ret = createWindow(m_windowData.w, m_windowData.h,
			m_windowData.windowTitle.c_str(), m_windowData.isFullScreen, m_windowData.withFrame);
		break;
	case WINDOW_TYPE_3D:
		ret = createWindow(m_windowData.w, m_windowData.h,
			m_windowData.windowTitle.c_str(), m_windowData.isFullScreen, m_windowData.withFrame);
		break;
	}
	if (!ret) return XFalse;
	XCurSndCore.setMusicVolume(m_windowData.BGVolume * 0.01f * 128.0f);
	XCurSndCore.setVolume(-1, m_windowData.FWVolume * 0.01f * 128.0f);	//好像没有生效
	initGL();
	//初始化第二个上下文
	wglMakeCurrent(m_wHDC,m_wCopyHGLRC);
	initGL();
	wglMakeCurrent(m_wHDC,m_wCurHGLRC);	//恢复上下文
	//2014.4.14这里修改为显示自己的光标
#ifdef WITHXEE_CURSOR
	XWindow.setCurcor(false);
#else
	if (m_windowData.isShowCursor == 0) XWindow.setCurcor(false);	//隐藏光标
#endif
	//if(screen != NULL)
	//{
	//	XEngineLogo EngineLogo;
	//	XResPack.setOutFileName("XEngine.log");
	//	EngineLogo.init(XVector2(m_windowData.w * 0.5 - 256 + 160,
	//		m_windowData.h * 0.5 - 256 + 128 + 96),RESOURCE_LOCAL_PACK);
	//	XResPack.setOutFileName(NULL);
	//	EngineLogo.reset();
	//	EngineLogo.setBackSize(m_windowData.w/8.0f,m_windowData.h/8.0f);
	//	int interval = 0;
	//	while(true) 
	//	{
	//		interval = engineIdle();
	//		EngineLogo.move(interval);
	//		EngineLogo.draw();
	//		if(EngineLogo.getIsEnd() != 0) break;
	//	}
	//	EngineLogo.release();
	//}
	//	setSystemFont(XEE_SYSTEM_FONT_PATH);
	//#if WITH_OBJECT_MANAGER
	//	XObjManager.init();
	//#endif
	engineSysInit();
	//atexit(release);
	return XTrue;	
}
XBool XFrameWork::initGL()
{
	switch (m_windowData.windowType)
	{
	case WINDOW_TYPE_EX:
	case WINDOW_TYPE_NORMAL:
		if (!initOpenGL2D()) return XFalse;
		break;
	case WINDOW_TYPE_3D:
		if (!initOpenGL3D()) return XFalse;
		break;
	}
	XGL::initOpenGLState();
	return XTrue;
}
void XFrameWork::initWindowMatrix()
{
	switch(m_windowData.windowType)
	{
	case WINDOW_TYPE_EX:
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glLoadIdentity();								//变换坐标系函数
		glViewport(0, 0, m_windowData.w, m_windowData.h);
		switch(m_windowData.mode)
		{
		case WINDOW_SIZE_MODE_CLIP_LP:
			m_scenePosX = m_windowData.positionX;
			m_scenePosY = m_windowData.positionY;
			m_sceneWidth = m_windowData.w;
			m_sceneHeight = m_windowData.h;
			m_sceneScaleX = 1.0f;
			m_sceneScaleY = 1.0f;
			break;
		case WINDOW_SIZE_MODE_CLIP_MID:
			m_scenePosX = ((m_windowData.sceneW - m_windowData.w) >> 1) + m_windowData.positionX;
			m_scenePosY = ((m_windowData.sceneH - m_windowData.h) >> 1) + m_windowData.positionY;
			m_sceneWidth = m_windowData.w;
			m_sceneHeight = m_windowData.h;
			m_sceneScaleX = 1.0f;
			m_sceneScaleY = 1.0f;
			break;
		case WINDOW_SIZE_MODE_CLIP_RESIZE:
			m_sceneScaleX = (float)(m_windowData.sceneW) / (float)(m_windowData.w);
			m_sceneScaleY = (float)(m_windowData.sceneH) / (float)(m_windowData.h);
			if(m_sceneScaleX <= m_sceneScaleY)
			{
				m_sceneWidth = m_windowData.sceneW;
				m_sceneHeight = m_windowData.h * m_sceneScaleX;
				m_scenePosX = m_windowData.positionX;
				m_scenePosY = ((m_windowData.sceneH - m_sceneHeight) >> 1) + m_windowData.positionY;
			}else
			{
				m_sceneWidth = m_windowData.w * m_sceneScaleY;
				m_sceneHeight = m_windowData.sceneH;
				m_scenePosX = ((m_windowData.sceneW - m_sceneWidth) >> 1) + m_windowData.positionX;
				m_scenePosY = m_windowData.positionY;
			}
			break;
		case WINDOW_SIZE_MODE_RESIZE_CLIP:
			m_sceneScaleX = (float)(m_windowData.sceneW) / (float)(m_windowData.w);
			m_sceneScaleY = (float)(m_windowData.sceneH) / (float)(m_windowData.h);
			if(m_sceneScaleX >= m_sceneScaleY)
			{
				m_sceneWidth = m_windowData.sceneW;
				m_sceneHeight = m_windowData.h * m_sceneScaleX;
				m_scenePosX = m_windowData.positionX;
				m_scenePosY = ((m_windowData.sceneH - m_sceneHeight) >> 1) + m_windowData.positionY;
			}else
			{
				m_sceneWidth = m_windowData.w * m_sceneScaleY;
				m_sceneHeight = m_windowData.sceneH;
				m_scenePosX = ((m_windowData.sceneW - m_sceneWidth) >> 1) + m_windowData.positionX;
				m_scenePosY = m_windowData.positionY;
			}
			break;
		case WINDOW_SIZE_MODE_RESIZE:
			m_scenePosX = m_windowData.positionX;
			m_scenePosY = m_windowData.positionY;
			m_sceneWidth = m_windowData.sceneW;
			m_sceneHeight = m_windowData.sceneH;
			m_sceneScaleX = (float)(m_windowData.sceneW) / (float)(m_windowData.w);
			m_sceneScaleY = (float)(m_windowData.sceneH) / (float)(m_windowData.h);
			break;
		}
		switch(m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			switch(m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_scenePosX,m_sceneWidth + m_scenePosX,
					m_sceneHeight + m_scenePosY,m_scenePosY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_sceneWidth + m_scenePosX,m_scenePosX,
					m_sceneHeight + m_scenePosY,m_scenePosY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_scenePosX,m_sceneWidth + m_scenePosX,
					m_scenePosY,m_sceneHeight + m_scenePosY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_sceneWidth + m_scenePosX,m_scenePosX,
					m_scenePosY,m_sceneHeight + m_scenePosY,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_90:
			switch(m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_sceneHeight + m_scenePosY,m_scenePosY,
					m_scenePosX,m_sceneWidth + m_scenePosX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_sceneHeight + m_scenePosY,m_scenePosY,
					m_sceneWidth + m_scenePosX,m_scenePosX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_scenePosY,m_sceneHeight + m_scenePosY,
					m_scenePosX,m_sceneWidth + m_scenePosX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_scenePosY,m_sceneHeight + m_scenePosY,
					m_sceneWidth + m_scenePosX,m_scenePosX,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180:
			switch(m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_sceneWidth + m_scenePosX,m_scenePosX,
					m_scenePosY,m_sceneHeight + m_scenePosY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_scenePosX,m_sceneWidth + m_scenePosX,
					m_scenePosY,m_sceneHeight + m_scenePosY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_sceneWidth + m_scenePosX,m_scenePosX,
					m_sceneHeight + m_scenePosY,m_scenePosY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_scenePosX,m_sceneWidth + m_scenePosX,
					m_sceneHeight + m_scenePosY,m_scenePosY,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_270:
			switch(m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_scenePosY,m_sceneHeight + m_scenePosY,
					m_sceneWidth + m_scenePosX,m_scenePosX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_scenePosY,m_sceneHeight + m_scenePosY,
					m_scenePosX,m_sceneWidth + m_scenePosX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_sceneHeight + m_scenePosY,m_scenePosY,
					m_sceneWidth + m_scenePosX,m_scenePosX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_sceneHeight + m_scenePosY,m_scenePosY,
					m_scenePosX,m_sceneWidth + m_scenePosX,-1,1);
				break;
			}
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		switch(m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			switch(m_windowData.mode)
			{
			case WINDOW_SIZE_MODE_CLIP_LP:
				glTranslatef(m_windowData.h,0,0);
				glRotatef(90,0,0,1);
				break;
			case WINDOW_SIZE_MODE_CLIP_MID:
			case WINDOW_SIZE_MODE_CLIP_RESIZE:
			case WINDOW_SIZE_MODE_RESIZE_CLIP:
			case WINDOW_SIZE_MODE_RESIZE:
				glTranslatef(m_windowData.sceneH,0,0);
				glRotatef(90,0,0,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180: break;	//do nothing
		case WINDOW_ROTATE_MODE_270: 
			switch(m_windowData.mode)
			{
			case WINDOW_SIZE_MODE_CLIP_LP:
				glTranslatef(m_windowData.h,0,0);
				glRotatef(90,0,0,1);
				break;
			case WINDOW_SIZE_MODE_CLIP_MID:
			case WINDOW_SIZE_MODE_CLIP_RESIZE:
			case WINDOW_SIZE_MODE_RESIZE_CLIP:
			case WINDOW_SIZE_MODE_RESIZE:
				glTranslatef(m_windowData.sceneH,0,0);
				glRotatef(90,0,0,1);
				break;
			}
			break;
		}
		break;
	case WINDOW_TYPE_NORMAL:
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glLoadIdentity();								//变换坐标系函数
		glViewport(0, 0, m_windowData.w, m_windowData.h);
		glOrtho(0,m_windowData.w,m_windowData.h,0,-1,1);					//only for 2D的原始数据
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_scenePosX = 0;
		m_scenePosY = 0;
		m_sceneWidth = m_windowData.w;
		m_sceneHeight = m_windowData.h;
		m_sceneScaleX = 1.0f;
		m_sceneScaleY = 1.0f;
		break;
	case WINDOW_TYPE_3D:
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glLoadIdentity();								//变换坐标系函数
		glViewport(0, 0, m_windowData.w, m_windowData.h);
		gluPerspective(45.0f,(float)(m_windowData.w) / (float)(m_windowData.h),1.0f,10000.0f);		//最后两个数值需要与之前的吻合，不然会出现模型破损的问题
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_scenePosX = 0;
		m_scenePosY = 0;
		m_sceneWidth = m_windowData.w;
		m_sceneHeight = m_windowData.h;
		m_sceneScaleX = 1.0f;
		m_sceneScaleY = 1.0f;
		break;
	}
}
XBool XFrameWork::initOpenGL3D()
{
	initWindowMatrix();	//初始化矩阵
	initGlew();			//初始化glew
	glShadeModel(GL_SMOOTH);		//启动反走样
	if(m_windowData.withAlphaBG) m_defBGColor.setColor(0.0f,0.0f,0.0f,0.0f);	//如果是透明背景的话，这列设置为透明
	glClearColor(m_defBGColor.fR,m_defBGColor.fG,m_defBGColor.fB,m_defBGColor.fA);				//清除颜色
	glHint(GL_POLYGON_SMOOTH,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);//建议：优化掉这一步，不用每次都调用
	if(m_isMultiSampleSupported)
	{
		glEnable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
		glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
		if(m_isLineSmooth) glEnable(GL_LINE_SMOOTH);
	}
	//3D
	glClearDepth(1.0f);									//Depth Buffer Setup
	glClearStencil(0);
//	glEnable(GL_DEPTH);	
	glEnable(GL_DEPTH_TEST);							//Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								//The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	//Really Nice Perspective Calculations
	XGL::EnableTexture2D();							// Enable 2D Texture Mapping
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping

	glAlphaFunc(GL_GREATER,0.1f);						//允许进行alpha测试
	glEnable(GL_ALPHA_TEST); 
	//下面的代码是为了解决背光面的问题
	//glEnable(GL_CULL_FACE);	//只允许渲染正面

	glEnable(GL_NORMALIZE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//+++++++++++++++++++++++++++++
	//注意这里，颜色与法线贴图有冲突
	glEnable(GL_COLOR_MATERIAL);	//开启这里会造成法线贴图颜色太暗的问题,不开启这个会造成颜色不生效
	//-----------------------------
	glMaterialfv(GL_FRONT,GL_SPECULAR,XVector4(1.0f,1.0f,1.0f,1.0f));
	glMaterialf(GL_FRONT,GL_SHININESS,128.0f);

	//glDisable(GL_LINE_SMOOTH);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	return XTrue;
}
#pragma comment(lib,"glu32.lib")
void XFrameWork::initGlew()
{
	if(glewInit() != 0)
	{//初始化失败
		LogStr("Glew init error!");
		m_isOpenGL2Supported = XFalse;
	}else
	{
		//记录显卡的最大贴图尺寸
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&m_maxTextureSize);	
		//记录最大同时贴图四处
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxTextureSum);
		//检查是否支持FBO
		m_isFboSupported = glewIsSupported("GL_EXT_framebuffer_object");
		if(m_isFboSupported) LogStr("FBO is supported.");
		else LogStr("FBO is not supported.");
		//检查硬件是否支持OpenGL2.0
		if(glewIsSupported("GL_VERSION_2_0"))
		{
			LogStr("Ready for OpenGL 2.0");
			m_isOpenGL2Supported = XTrue;
		}else
		{
			LogStr("OpenGL 2.0 not supported");
			m_isOpenGL2Supported = XFalse;
		}
		//开启多重采样抗锯齿
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_MULTISAMPLE);//，初始化的时候开启；2）
		GLint bufSum = 0;
		GLint samplesSum = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufSum);
		glGetIntegerv(GL_SAMPLES, &samplesSum);//可以检查多重采样是否可用，前者返回1，后者返回大于1表示可用; 3）
		if(bufSum == 1 && samplesSum > 1)
		{
			m_isMultiSampleSupported = XTrue;
			glEnable(GL_MULTISAMPLE);	//这里生效需要在显卡的控制面板中设置为最佳图像质量
		}else
			m_isMultiSampleSupported = XFalse;
	}
}
XBool XFrameWork::initOpenGL2D()
{
	initWindowMatrix();	//初始化矩阵
	initGlew();			//初始化glew
	glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	if(m_windowData.withAlphaBG) m_defBGColor.setColor(0.0f,0.0f,0.0f,0.0f);	//如果是透明背景的话，这列设置为透明
	glClearColor(m_defBGColor.fR,m_defBGColor.fG,m_defBGColor.fB,m_defBGColor.fA);			//清除颜色
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);//建议：优化掉这一步，不用每次都调用
	glHint(GL_POLYGON_SMOOTH,GL_NICEST);
	if(m_isMultiSampleSupported)
	{
		glEnable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
		glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
		if(m_isLineSmooth) glEnable(GL_LINE_SMOOTH);
	}
	//初始化GL的相关数据
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glDisable(GL_DEPTH);							//2D项目不需要深度测试
	glDisable(GL_DEPTH_TEST);						//2D项目不需要深度测试
	//glEnalbe(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	return XTrue;
}
int XFrameWork::suspendKeyProc()
{
	int ret = 0;
	XInputEvent tmpEvent;
	while (getInputEvent(tmpEvent))
	{
		switch (tmpEvent.type)
		{
		case EVENT_KEYBOARD:
			if (tmpEvent.keyState == KEY_STATE_DOWN)
			{
				switch (tmpEvent.keyValue)
				{
				case XKEY_ESCAPE: ret = 1; break;
				}
			}
			break;
		}
	}
	return ret;
}
void XFrameWork::suspend(const char * str)
{
	m_isSuspend = XTrue;
	if (m_haveSystemFont)
	{//有自己的字体
		if (str == NULL)
		{
			while (true)
			{
				if (suspendKeyProc() != 0) exit(1);
				clearScreen();
				updateScreen();
				Sleep(1);
			}
		}
		else
		{
			//	string tempStr = str;
			//	tempStr += "\n";
			//	LogStr(tempStr.c_str());
			LogStr(str);
			m_systemFont->setString(str);
			while (true)
			{
				if (suspendKeyProc() != 0)
				{
					LogRelease();
					exit(1);
				}
				clearScreen();
				m_systemFont->draw();
				updateScreen();
				Sleep(1);
			}
		}
	}
	else
	{//没有自己的字体
		if (str == NULL)
		{//啥也不干就是死锁
			while (true)
			{
				if (suspendKeyProc() != 0) exit(1);
				Sleep(1);
			}
		}
		else
		{//显示错误信息并进入死锁
			//	string tempStr = str;
			//	tempStr += "\n";
			//	LogStr(tempStr.c_str());
			LogStr(str);
			while (true)
			{
				if (suspendKeyProc() != 0)
				{
					LogRelease();
					exit(1);
				}
				Sleep(1);
			}
		}
	}
}
#ifdef XEE_OS_WINDOWS
BOOL CALLBACK EnumWndProc(HWND hwnd,LPARAM lParam)     
{
	if (gFrameworkData.pFrameWork == NULL) return false;
	if (GetProp(hwnd, gFrameworkData.pFrameWork->m_programPropName.c_str()) == 
		gFrameworkData.pFrameWork->m_programPropValue)
	{     
		*(HWND*)lParam = hwnd;     
		return false;     
	}     
	return true;     
} 
bool XFrameWork::isProgramExist(const std::string &name)
{
	m_programPropName = name;
	HWND oldHWnd = NULL;     
	EnumWindows(EnumWndProc,(LPARAM)&oldHWnd);	//枚举所有运行的窗口     
	if(oldHWnd != NULL)     
	{   
		ShowWindow(oldHWnd,SW_SHOWNORMAL);		//激活找到的前一个程序     
		SetForegroundWindow(oldHWnd);			//把它设为前景窗口     
		return true;							//退出本次运行     
	} 
	SetProp(m_hwnd,m_programPropName.c_str(),m_programPropValue); 
	return false;
}
#endif
//定义自己的消息处理函数
//将消息处理函数下传
WNDPROC currentWndProc = NULL;
static LRESULT CALLBACK winMsgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) 
{
	if (gFrameworkData.pFrameWork == NULL)
		return CallWindowProc(currentWndProc, hwnd, Msg, wParam, lParam);
	bool end = false;
	//++++++++++++++++++++++++++++++++++++
	//窗口和全屏切换的一个测试
	switch(Msg)
	{
	case WM_ACTIVATE:
		if (!gFrameworkData.pFrameWork->m_isSetToFullScreen) break;
		if (wParam)
		{//回复窗口，设置分辨率
			DEVMODE dm;
			dm.dmSize = sizeof(DEVMODE);
			dm.dmPelsWidth = getWindowWidth();
			dm.dmPelsHeight = getWindowHeight();
			dm.dmBitsPerPel = 32;
			dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
			ChangeDisplaySettings(&dm,CDS_FULLSCREEN);
			WINDOWPLACEMENT state;
			state.length = sizeof(WINDOWPLACEMENT);
			state.flags = 0;
			state.showCmd = SW_SHOWNORMAL;
			state.rcNormalPosition.left = 0;
			state.rcNormalPosition.top = 0;
			state.rcNormalPosition.right = getWindowWidth();
			state.rcNormalPosition.bottom = getWindowHeight();
			SetWindowPlacement(hwnd,&state);
			printf("激活\n");
		}else
		{//最小化窗口，回复分辨率
			ChangeDisplaySettings(0,CDS_RESET);

			WINDOWPLACEMENT state;
			state.length = sizeof(WINDOWPLACEMENT);
			state.showCmd = SW_MINIMIZE;
			SetWindowPlacement(hwnd,&state);
			printf("非激活\n");
		}
		break;
	}
	//-----------------------------------------------------
	LRESULT ret;
	if (gFrameworkData.pFrameWork->getIsProjectAsDaemon())
	{
		ret = gFrameworkData.pFrameWork->daemonProc(hwnd, Msg, wParam, lParam, end);
		if(end) return ret;
	}
	if (gFrameworkData.pFrameWork->m_customWinProc != NULL)
	{
		ret = gFrameworkData.pFrameWork->m_customWinProc(hwnd, Msg, wParam, lParam, end);
		if(end) return ret;
	}
	//下面处理输入法事件
	ret = XImm::inputMsgProc(hwnd,Msg,wParam,lParam,end);
	if(end) return ret;
	return CallWindowProc(currentWndProc,hwnd,Msg,wParam,lParam);
}
//--------------------------------------
static void registerMyMsgProc()
{
	if (gFrameworkData.pFrameWork == NULL) return;
	//DragAcceptFiles (wHandle, TRUE);	//是否允许向窗口拖拽文件
	//方案1
	//currentWndProc = (WNDPROC)SetWindowLongPtr(wHandle, GWL_WNDPROC, (long)winMsgProc);
	//方案2
#ifdef _WIN64
	currentWndProc = (WNDPROC)GetWindowLongPtr(gFrameworkData.pFrameWork->getHWND(), GWLP_WNDPROC);
	SetWindowLongPtr(gFrameworkData.pFrameWork->getHWND(), GWLP_WNDPROC, (long)winMsgProc);
#else
	currentWndProc = (WNDPROC)GetWindowLongPtr(gFrameworkData.pFrameWork->getHWND(), GWL_WNDPROC);
	SetWindowLongPtr(gFrameworkData.pFrameWork->getHWND(), GWL_WNDPROC, (long)winMsgProc);
#endif
}
bool XFrameWork::createWindow(int width,int height,const char *windowTitle,int isFullScreen,int withFrame)
{
	if(!XWindow.createWindow(width,height,windowTitle,isFullScreen,withFrame)) return false;
	m_windowWidth = width;
	m_windowHeight = height;
	//初始化一些变量
	m_wHDC = wglGetCurrentDC();
	m_wCurHGLRC = wglGetCurrentContext();
	m_wCopyHGLRC = wglCreateContext(m_wHDC);
	//wglCopyContext(m_wCurHGLRC,m_wCopyHGLRC,GL_ALL_ATTRIB_BITS);
	wglShareLists(m_wCurHGLRC,m_wCopyHGLRC);
//	m_hwnd = GetActiveWindow();
	m_hwnd = WindowFromDC(m_wHDC);
	m_mainThreadHandle = GetCurrentThreadId();

	if(windowTitle != NULL) XWindow.setWindowTitle(windowTitle);	//设置窗口标题
	if(m_windowData.isInitWindowPos
		|| m_windowData.isAlwaysTop) setWindowPos(m_windowData.windowPosX,m_windowData.windowPosY,m_windowData.isAlwaysTop);

	registerMyMsgProc();

	XCurSndCore.init();
	return true;
}
void XFrameWork::setStyle(XColorStyle style)
{
	switch (style)
	{
	case COLOR_STYLE_NORMAL:
		XCCS::lightBGColor.setColor(0.9f, 0.9f, 0.9f, 1.0f);
		XCCS::onColor.setColor(0.85f, 0.85f, 0.85f, 1.0f);
		XCCS::blackOnColor.setColor(0.8f, 0.8f, 0.8f, 1.0f);
		XCCS::normalColor.setColor(0.75f, 0.75f, 0.75f, 1.0f);
		XCCS::blackNormalColor.setColor(0.7f, 0.7f, 0.7f, 1.0f);
		XCCS::lightSpecialColor.setColor(0.65f, 0.65f, 0.65f, 1.0f);
		XCCS::specialColor.setColor(0.6f, 0.6f, 0.6f, 1.0f);
		XCCS::downColor.setColor(0.5f, 0.5f, 0.5f, 1.0f);
		XCCS::lightMouseColor.setColor(0.45f, 0.45f, 0.45f, 1.0f);
		XCCS::mouseColor.setColor(0.4f, 0.4f, 0.4f, 1.0f);
		XCCS::disableColor.setColor(0.35f, 0.35f, 0.35f, 1.0f);
		XCCS::blackDownColor.setColor(0.25f, 0.25f, 0.25f, 1.0f);
		XCCS::lightBlackColor.setColor(0.2f, 0.2f, 0.2f, 1.0f);
		XCCS::blackColor.setColor(0.1f, 0.1f, 0.1f, 1.0f);
		XCCS::darkColor.setColor(0.0f, 0.0f, 0.0f, 1.0f);
		setBGColor(XFColor(0.25f, 0.25f, 0.25f, 1.0f));//默认的背景颜色
		m_systemFont->setColor(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	case COLOR_STYLE_LIGHT:
		XCCS::lightBGColor.setColor(0.9f, 0.9f, 0.9f, 1.0f);
		XCCS::onColor.setColor(0.875f, 0.875f, 0.875f, 1.0f);
		XCCS::blackOnColor.setColor(0.85f, 0.85f, 0.85f, 1.0f);
		XCCS::normalColor.setColor(0.825f, 0.825f, 0.825f, 1.0f);
		XCCS::blackNormalColor.setColor(0.8f, 0.8f, 0.8f, 1.0f);
		XCCS::lightSpecialColor.setColor(0.775f, 0.775f, 0.775f, 1.0f);
		XCCS::specialColor.setColor(0.75f, 0.75f, 0.75f, 1.0f);
		XCCS::downColor.setColor(0.725f, 0.725f, 0.725f, 1.0f);
		XCCS::lightMouseColor.setColor(0.7f, 0.7f, 0.7f, 1.0f);
		XCCS::mouseColor.setColor(0.675f, 0.675f, 0.675f, 1.0f);
		XCCS::disableColor.setColor(0.65f, 0.65f, 0.65f, 1.0f);
		XCCS::blackDownColor.setColor(0.625f, 0.625f, 0.625f, 1.0f);
		XCCS::lightBlackColor.setColor(0.60f, 0.60f, 0.60f, 1.0f);
		XCCS::blackColor.setColor(0.575f, 0.575f, 0.575f, 1.0f);
		XCCS::darkColor.setColor(0.55f, 0.55f, 0.55f, 1.0f);
		setBGColor(XFColor(0.95f, 0.95f, 0.9f, 1.0f));//默认的背景颜色
		m_systemFont->setColor(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	}
}
#define IDR_HIDE         (16)
#define IDR_HIDECONSOLE  (18)
#define IDR_QUIT         (20)
#define IDI_ICON1		 (101)
void XFrameWork::releaseIcon(){Shell_NotifyIcon(NIM_DELETE,&m_nid);}
//将消息处理函数下传
LRESULT XFrameWork::daemonProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam,bool &end) 
{
	end = false;
	static UINT WM_TASKBARCREATED = 0;  
	static bool isHide = false;
	static bool isHideConsole = false;
	static bool isHideWindows = false;
	static HMENU hmenu; 
	static bool isInited = false;
	if(!isInited)
	{//初始化所有数据
		isInited = true;
		WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
		m_nid.cbSize = sizeof(m_nid);
		m_nid.hWnd = hwnd;
		m_nid.uID = 0;
		m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_nid.uCallbackMessage = WM_USER;
		m_nid.hIcon = LoadIcon(GetModuleHandle(NULL),(LPCTSTR)IDI_ICON1);   //IDI_APPLICATION
		lstrcpy(m_nid.szTip, TEXT(m_windowData.windowTitle.c_str()));
		Shell_NotifyIcon(NIM_ADD, &m_nid);
		hmenu = CreatePopupMenu();
		AppendMenu(hmenu,MF_STRING,IDR_HIDE,TEXT("隐藏窗口"));
		CheckMenuItem(hmenu,IDR_HIDE,MF_BYCOMMAND|MF_CHECKED);
		if (m_deamonWithConsole)
		{
			AppendMenu(hmenu,MF_STRING,IDR_HIDECONSOLE,TEXT("隐藏控制台"));
			CheckMenuItem(hmenu,IDR_HIDECONSOLE,MF_BYCOMMAND|MF_CHECKED);
		}
		AppendMenu(hmenu,MF_STRING,IDR_QUIT,TEXT("退出"));
		//这里设置为第一次调用的时候隐藏窗口
		ShowWindow(hwnd,SW_HIDE);
		ShowWindow(GetConsoleWindow(),SW_HIDE);
		isHide = true;
	}
	switch(Msg)
	{
	//case WM_SETTEXT:	//这里设置为第一次调用的时候隐藏窗口
	//	if(!isHide)
	//	{
	//		ShowWindow(hwnd,SW_HIDE);
	//		ShowWindow(GetConsoleWindow(),SW_HIDE);
	//		isHide = true;
	//	}
	//	break;
	case WM_USER:
		if(lParam == WM_RBUTTONDOWN)
		{
			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hwnd);
			int menuState = TrackPopupMenu(hmenu,TPM_RETURNCMD,pt.x,pt.y,NULL,hwnd,NULL);
			switch(menuState)
			{
			case IDR_HIDE:
				isHideWindows = !isHideWindows;
				if(!isHideWindows)
				{
					ShowWindow(hwnd,SW_HIDE);
					CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND |  MF_CHECKED);
				}else
				{
					ShowWindow(hwnd,SW_NORMAL);
					CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND |  MF_UNCHECKED);
				}
				break;
			case IDR_HIDECONSOLE:
				isHideConsole = !isHideConsole;
				if (!isHideConsole)
				{
					ShowWindow(GetConsoleWindow(),SW_HIDE);
					CheckMenuItem(hmenu, IDR_HIDECONSOLE, MF_BYCOMMAND |  MF_CHECKED);
				}else
				{
					ShowWindow(GetConsoleWindow(),SW_NORMAL);
					CheckMenuItem(hmenu, IDR_HIDECONSOLE, MF_BYCOMMAND |  MF_UNCHECKED);
				}
				break;
			case IDR_QUIT:
				SendMessage(hwnd,WM_CLOSE,wParam,lParam);
				releaseIcon();
				break;
			case 0: 
				PostMessage(hwnd,Msg,NULL,NULL);
				break;
			default:
				PostMessage(hwnd,Msg,wParam,lParam);
				break;
			}
		}
		if(lParam == WM_LBUTTONDBLCLK)
		{//双击处理
			SetForegroundWindow(hwnd);
			isHideWindows = !isHideWindows;
			if(!isHideWindows)
			{
				ShowWindow(hwnd,SW_HIDE);
				CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND |  MF_CHECKED);
			}else
			{
				ShowWindow(hwnd,SW_NORMAL);
				CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND |  MF_UNCHECKED);
			}
		}
		break;
	case WM_DESTROY: 
	case WM_CLOSE: 
	case WM_QUIT: 
		releaseIcon();
		break;
	default:
		if(Msg == WM_TASKBARCREATED)
		{
			isHide = false;
			SendMessage(hwnd,WM_PAINT,wParam,lParam);
		}
		break;
	}
	return 0;
}
XBool XFrameWork::setSystemFont(const char * filename)
{
	if(m_haveSystemFont) return XFalse;
	if(!m_systemFont->initFromTTF(filename,m_windowData.systemFontSize, RESOURCE_LOCAL_FOLDER, false))
	{
		LogStr("系统字体加载失败!");
		return XFalse;
	}
	m_systemFont->setPosition(getWinW() >> 1, getWinH() >> 1);
	m_systemFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_systemFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_systemFont->setColor(0.75f, 0.75f, 0.75f, 1.0f);
	m_haveSystemFont = XTrue;
	if (m_windowData.isShowVersion)
	{
		m_showVersionFont->setACopy(*m_systemFont);
		m_showVersionFont->setPosition(0.0f, 0.0f);
		m_showVersionFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_showVersionFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		m_showVersionFont->setColor(0.75f, 0.75f, 0.75f, 1.0f);
		m_showVersionFont->setString(m_windowData.versionString.c_str());
	}

	return XTrue;
}
void XFrameWork::toggleFullScreen()
{
	if (m_windowData.isFullScreen != 0) return;
	//printf("进行窗口和全屏的切换\n");
	static RECT rect;
	static LONG state = 0;
	if (!m_isSetToFullScreen)
	{//设置为全屏
		GetWindowRect(getHWND(), &rect);
		state = GetWindowLongPtr(getHWND(), GWL_STYLE);

		DEVMODE dm;
		dm.dmSize = sizeof(DEVMODE);
		dm.dmPelsWidth = getWinW();
		dm.dmPelsHeight = getWinH();
		dm.dmBitsPerPel = 32;
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
		{
			SetWindowLongPtr(getHWND(), GWL_STYLE,
				WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
			MoveWindow(getHWND(), 0, 0, getWinW(), getWinH(), TRUE);
			//setWindowPos(0,0);
			m_isSetToFullScreen = true;
		}
	}else
	{//设置为窗口
		if (ChangeDisplaySettings(0, CDS_RESET) == DISP_CHANGE_SUCCESSFUL)
		{
			if (state == 0)
			{//没有初始数据
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);	//获取边框的数据
				SetWindowLongPtr(getHWND(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
				MoveWindow(getHWND(), 0, 0, getWinW() + rect.right - rect.left, getWinH() + rect.bottom - rect.top, TRUE);
			}else
			{
				SetWindowLongPtr(getHWND(), GWL_STYLE, state);
				MoveWindow(getHWND(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
			}
			m_isSetToFullScreen = false;
		}
	}
}
#if WITH_ENGINE_STATISTICS
void XFrameWork::logEngineStatistics()			//将引擎统计数据输出到日志
{
	if(!m_withEngineStatistics) return;
	LogNull("贴图数:%d",m_totalTextureSum);
	LogNull("贴图面积:%d",m_totalTextureArea);
	LogNull("显存使用量:%d",m_totalVideoMemUsage);
	LogNull("平均帧贴图面积:%d",m_totalFrameTextureArea);
	LogNull("平均帧显存带宽使用量:%f",m_curVideoMemBandwidthUsage);
}
void XFrameWork::addStaticsticsTexInfo(unsigned int id,int level,int type)
{
	if(!m_withEngineStatistics) return;
	int w = 0,h = 0,format;
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_WIDTH,&w);
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_HEIGHT,&h);
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_INTERNAL_FORMAT,&format);
	++ m_totalTextureSum;
	m_totalTextureArea += w * h;
	switch(format)
	{
	case GL_RGB:
	case GL_BGR:
		m_totalVideoMemUsage += w * h * 3;
		break;
	case GL_RGBA:
	case GL_BGRA:
		m_totalVideoMemUsage += (w * h) << 2;
		break;
	case GL_RGBA32F:
		m_totalVideoMemUsage += (w * h) << 3;
		break;
	case GL_RG:
		m_totalVideoMemUsage += (w * h) << 1;
		break;
	case GL_RED:
	case GL_LUMINANCE:
		m_totalVideoMemUsage += w * h;
		break;
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_STENCIL:
		break;
	}
}
void XFrameWork::decStaticsticsTexInfo(unsigned int id,int level,int type)
{//未经过验证
	if(!m_withEngineStatistics) return;
	switch(type)
	{
	case GL_TEXTURE_CUBE_MAP:
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP,id);
		break;
	case GL_TEXTURE_2D:
		XGL::BindTexture2D(id);
		break;
	}
	int w = 0,h = 0,format;
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_WIDTH,&w);
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_HEIGHT,&h);
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_INTERNAL_FORMAT,&format);
	-- m_totalTextureSum;
	m_totalTextureArea -= w * h;
	switch(format)
	{
	case GL_RGB:
	case GL_BGR:
		m_totalVideoMemUsage -= w * h * 3;
		break;
	case GL_RGBA:
	case GL_BGRA:
		m_totalVideoMemUsage -= (w * h) << 2;
		break;
	case GL_RGBA32F:
		m_totalVideoMemUsage -= (w * h) << 3;
		break;
	case GL_RG:
		m_totalVideoMemUsage -= (w * h) << 1;
		break;
	case GL_RED:
	case GL_LUMINANCE:
		m_totalVideoMemUsage -= w * h;
		break;
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_STENCIL:
		break;
	}	
	printf("%d\n",m_totalVideoMemUsage);
	switch(type)
	{
	case GL_TEXTURE_CUBE_MAP:
		glBindTexture(GL_TEXTURE_CUBE_MAP,0);
		glDisable(GL_TEXTURE_CUBE_MAP);
		break;
	case GL_TEXTURE_2D:
		break;
	}
}
#endif
struct XScreenShotData
{
	char fileName[MAX_FILE_NAME_LENGTH];
	int w;
	int h;
	unsigned int *data;
	XScreenShotData()
		:data(NULL)
	{}
};
XScreenShotData *screenShotData = NULL;
//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入
#ifdef XEE_OS_WINDOWS
DWORD WINAPI saveDataToPNG(void *)
#endif
#ifdef XEE_OS_LINUX
static void *saveDataToPNG(void * pParam)
#endif
{
	if(screenShotData == NULL || screenShotData->data == NULL) 
	{
#ifdef XEE_OS_WINDOWS
		return 1;
#endif
#ifdef XEE_OS_LINUX
		return;
#endif
	}
	//将数据保存成图片
	unsigned char *pData = XMem::createArrayMem<unsigned char>(screenShotData->w * screenShotData->h * 4);
	int w = screenShotData->w << 2;
	unsigned char *pd = pData + screenShotData->w * screenShotData->h * 4 - w;
	unsigned char *ps = (unsigned char *)screenShotData->data;
	for(int i = 0;i < screenShotData->h;++ i)	//上下翻转
	{
		memcpy(pd,ps,w);
		ps += w;
		pd -= w;
	}
//	printf("%d\n",getCurrentTicks());
	XPng::savePNG(screenShotData->fileName,pData,screenShotData->w,screenShotData->h,COLOR_RGBA,Z_BEST_COMPRESSION);
//	printf("%d\n",getCurrentTicks());
	//释放资源
	XMem::XDELETE_ARRAY(pData);
	XMem::XDELETE_ARRAY(screenShotData->data);
	XMem::XDELETE(screenShotData);
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入
XBool XFrameWork::gameShot(const char *fileName,int w,int h,bool withThread,float x,float y)
{
	if(fileName == NULL) return XFalse;
	if(x < 0.0f || y < 0.0f) return XFalse;
	if(w <= 0 || h <= 0) return XFalse;
	if(screenShotData != NULL) return XFalse;
	screenShotData = XMem::createMem<XScreenShotData>();
	if(screenShotData == NULL) return XFalse;

	screenShotData->h = h;
	screenShotData->w = w;
	strcpy(screenShotData->fileName,fileName);
	screenShotData->data = XMem::createArrayMem<unsigned int>(w * h);
	if(screenShotData->data == NULL)
	{
		XMem::XDELETE(screenShotData);
		return XFalse;
	}

	//读取数据
	//glReadPixels(0,0,w,h,GL_RGBA,GL_BYTE,screenShotData->data);
	glReadPixels(x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,screenShotData->data);
	if(withThread)
	{//优化之后直接开启一个线程来保存图片
#ifdef XEE_OS_WINDOWS
		DWORD tempThreadP;
		if(CreateThread(0,0,saveDataToPNG,NULL,0,&tempThreadP) == 0)
		{
			return XFalse;
		}
#endif
#ifdef XEE_OS_LINUX
		pthread_t tempThreadP;
			if(pthread_create(&tempThreadP, NULL, &(saveDataToPNG), NULL) != 0)
			{
			return XFalse;
		}
#endif
	}else
	{
		saveDataToPNG(NULL);
	}
	return XTrue;
}
const XFontUnicode &getDefaultFont()
{
	if(gFrameworkData.pFrameWork != NULL && gFrameworkData.pFrameWork->m_systemFont != NULL)
		return *gFrameworkData.pFrameWork->m_systemFont;
	else
	{
		LogStr("在使用默认字体时请先初始化框架!");
		return XFontUnicode(0);
	}
}
#if !WITH_INLINE_FILE
#include "XFrameWork.inl"
#endif
}