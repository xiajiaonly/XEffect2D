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
#include "XMedia\XProjectionCorrection.h"
#include "XMedia\XMaskCorrection.h"

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XCaregiver.h"

#if BACKGROUND_ALPHA_MODE == 0
#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")
#endif
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();
namespace XE{
XFrameWorkData gFrameworkData;
#if WITH_LOG
bool XFrameWorkData::create(bool isIniLog,XLogBookMode mode,const std::string& serverFilename)
#else
bool XFrameWorkData::create()
#endif
{
#if WITH_LOG
	pLog = XMem::createMem<XLogbook>();
	if (pLog == NULL) return false;
	if (isIniLog)
	{
		pLog->setWorkMode(mode);
		pLog->setServerFilename(serverFilename);
	}
#endif
	pFrameWork = XMem::createMem<XFrameWork>();
//	printf("%d\n", sizeof(XFrameWork));
	if (pFrameWork == NULL) return false;
#if WITH_OBJECT_MANAGER
	pObjManager = XMem::createMem<XObjectManager>();
	if (pObjManager == NULL) return false;
#endif
	pCtrlManager = XMem::createMem<XControlManager>();
	if (pCtrlManager == NULL) return false;
	pResManager = XMem::createMem<XResourceManager>();
	if (pResManager == NULL) return false;
	pResPack = XMem::createMem<XResourcePack>();
	if (pResPack == NULL) return false;
	pTexManager = XMem::createMem<XTextureResourceManager>();
	if (pTexManager == NULL) return false;
	pErrReporter = XMem::createMem<XErrorReporter>();
	if (pErrReporter == NULL) return false;
	pMusicPlayer = XMem::createMem<XMusic>();
	if (pMusicPlayer == NULL) return false;
	pSoundPlayer = XMem::createMem<XSound>();
	if (pSoundPlayer == NULL) return false;
	pCfgManager = XMem::createMem<XConfigManager>();
	if (pCfgManager == NULL) return false;
	p3DWorld = XMem::createMem<X3DWorld>();
	if (p3DWorld == NULL) return false;
	pFontTTF = XMem::createMem<XFontTTF>();
	if (pFontTTF == NULL) return false;
	pOperateManager = XMem::createMem<XOperateManager>();
	if (pOperateManager == NULL) return false;
#if AUDIO_MATHOD == 0
	pSoundCore = XMem::createMem<XSoundSDLMixer>();
	if (pSoundCore == NULL) return false;
#endif
#if AUDIO_MATHOD == 1
	pSoundCore = XMem::createMem<XSoundFmod>();
	if (pSoundCore == NULL) return false;
#endif
#if CREATE_WINDOW_METHOD == 0
	pWindowCore = XMem::createMem<XWindowSDL>();
	if (pWindowCore == NULL) return false;
#endif
#if CREATE_WINDOW_METHOD == 1
	pWindowCore = XMem::createMem<XWindowGlut>();
	if (pWindowCore == NULL) return false;
#endif
#if CREATE_WINDOW_METHOD == 2
	pWindowCore = XMem::createMem<XWindowGlfw>();
	if (pWindowCore == NULL) return false;
#endif
	if (pLicense != NULL && !pLicense->init()) return false;

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
	, m_rightBtnDownTimer(5000.0f)
	, m_withVirtualMouse(false)
	, m_isPassKeyLicense(true)

	, m_pGame(NULL)
	, m_needReset3DParam(XFalse)
	, m_drawAfterCtrl(NULL)
	, m_hwnd(0)
	, m_wHDC(0)
	, m_wCurHGLRC(0)
	, m_wCopyHGLRC(0)
	, m_forceToPaint(false)
	, m_isGameExit(false)
	, m_windowWidth(0)		//当前窗口的宽度
	, m_windowHeight(0)	//当前窗口的高度
	, m_scenePosX(0)			//场景的位置	//目前这个定义尚不完整，在很多模式下并没有效果
	, m_scenePosY(0)			//场景的位置
	, m_sceneWidth(0)		//场景的宽度
	, m_sceneHeight(0)		//场景的高度
	, m_sceneScaleX(1.0f)		//窗口的缩放比例
	, m_sceneScaleY(1.0f)		//窗口的缩放比例
	, m_viewAngle3D(45.0f)		//3D观察的角度
	, m_programPropName("xiajia_1981@163.com")
	, m_programPropValue(HANDLE(0x5A))
	, m_isSuspend(XFalse)
	, m_isFrameTimerEscape(true)
	, m_frameTime(0.0f)
	, m_curFPS(0.0f)
	, m_maxFrameStep(-1.0f)
	, m_rendDelay(33)
	, m_stopFlag(false)
	, m_manualFPS(-1)
	, m_manualFrameTime(0.0f)
	, m_defResPos(RES_LOCAL_FOLDER)
	, m_defBGColor(0.25f, 1.0f)
	, m_mainThreadHandle(0xffffffff)

	, m_isOpenGL2Supported(XFalse)	//OpenGL是否支持2.0是否支持GLSL
	, m_isLineSmooth(XTrue)		//是否使用划线的平滑处理
	, m_maxTextureSize(1024)		//显卡支持的最大贴图尺寸
	, m_isMultiSampleSupported(XFalse)	//是否支持多重采样
	, m_maxTextureSum(0)	//一次能同时使用的贴图的数量
	, m_isFboSupported(XFalse)	//是否支持FBO
	, m_multiSampleSum(0)

	, m_audioSampleRate(44100)	//音频初始化的频率
#if AUDIO_MATHOD == 0
	, m_audioFormat(MIX_DEFAULT_FORMAT)		//音频的格式
#endif
#if AUDIO_MATHOD == 1
	, m_audioFormat(0)		//音频的格式
#endif
	, m_audioChannel(2)	//音频的声道

	, m_deamonWithConsole(true)
	, m_isProjectAsDaemon(false)
	, m_customWinProc(NULL)

	, m_autoShutDownState(-1)
	, m_showVersionTimer(0.0f)
	, m_isSetToFullScreen(false)
	, m_haveSystemFont(XFalse)
	, m_curframeCounter(0)
#if WITH_ENGINE_STATISTICS
	, m_withEngineStatistics(false)	//是否开启引擎级别的统计
	, m_totalTextureSum(0)			//总的贴图数量
	, m_totalTextureArea(0)		//总的贴图面积
	, m_totalVideoMemUsage(0)		//总的显存使用量
	, m_totalFrameTextureArea(0)	//每帧刷新贴图的面积(尚未实现)
	, m_curVideoMemBandwidthUsage(0.0f)	//当前内存带宽的使用(尚未实现)
#endif
#if BACKGROUND_ALPHA_MODE == 1
	, m_isBGAlphaInited(false)
	, m_pBits(NULL)
	, m_hMemDc(NULL)
	, m_oldGDIObj(NULL)
	, m_bgAlphaPbo(NULL)
	, m_hBitmap(NULL)
#endif
	, m_isMouseLBDown(false)
	, m_isMouseRBDown(false)

	, m_textureNeedFit2N(true)
	, m_errorExitTimer(0.0f)
	, m_softBoard(NULL)
	, m_isImmState(false)
	, m_projCorrection(nullptr)
	, m_maskCorrection(nullptr)
	, m_corStage(CSTAGE_NULL)
{
#if BACKGROUND_ALPHA_MODE == 1
	m_ptSrc.x = 0; m_ptSrc.y = 0;
#endif
	m_softBoard = XMem::createMem<XSoftBoard>();
	if (m_softBoard == NULL) LogStr("Mem Error!");
	m_moveX = XMem::createMem<XPressDataDouble>();
	if (m_moveX == NULL) LogStr("Mem Error!");
	m_moveY = XMem::createMem<XPressDataDouble>();
	if (m_moveY == NULL) LogStr("Mem Error!");
	m_cheatModule = XMem::createMem<XCheatModule>();
	if (m_cheatModule == NULL) LogStr("Mem Error!");
//	m_pConfigMng = XMem::createMem<XConfigManager>();
//	if(m_pConfigMng == NULL) LogStr("Mem Error!");
	m_customWinTitle = XMem::createMem<XWindowTitle>();
	if (m_customWinTitle == NULL) LogStr("Mem Error!");
	m_showVersionFont = XMem::createMem<XFontUnicode>();
	if (m_showVersionFont == NULL) LogStr("Mem Error!");
	m_systemFont = XMem::createMem<XFontUnicode>();
	if (m_systemFont == NULL) LogStr("Mem Error!");
	m_periodicLogTimer = XMem::createMem<XSimpleTimer>();
	if (m_periodicLogTimer == NULL) LogStr("Mem Error!");
	m_pCaregiver = XMem::createMem<XCaregiver>();
	if (m_pCaregiver == NULL) LogStr("Mem Error!");
#if LIMITED_VERSION == 1
	m_limitedTimer = XRand::randomf(7200.0f, 4800.0f);
#endif
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
	XMem::XDELETE(m_pCaregiver);
}
void XFrameWork::engineRelease()
{
	XSoundPlayer.release();
	XCurSndCore.close();
#if _WIN32_WINNT >= 0x0501 && WITH_TOUCH_SUPPLY == 1
	UnregisterTouchWindow(m_hwnd);
#endif
	XWindow.release();
#if BACKGROUND_ALPHA_MODE == 1
	releaseBhAlphaData();
#endif
}
void XFrameWork::engineKeyProc(int key, XKeyState keyState)
{
	if(m_windowData.withCheatModule)
		m_cheatModule->keyboardProc(key, keyState);
	if (m_withVirtualMouse)
	{//开启虚拟鼠标
		switch (key)
		{
		case XKEY_A:
			if (keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			else mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			break;
		case XKEY_S:
			if (keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
			else mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
			break;
		case XKEY_D:
			if (keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
			else mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			break;
		}
	}

	XCtrlManager.keyProc(key, keyState);
#if WITH_OBJECT_MANAGER
	XObjManager.keyProc(key, keyState);
#endif
}
XVec2 XFrameWork::getMousePosFromWindow(int mouseX, int mouseY)	//系统坐标空间向窗口坐标空间进行转换
{
	float x = 0, y = 0;
	switch (m_windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
		switch (m_windowData.turnOverMode)
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
		switch (m_windowData.turnOverMode)
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
		switch (m_windowData.turnOverMode)
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
		switch (m_windowData.turnOverMode)
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
	return XVec2(x, y);
}
void XFrameWork::engineMouseProc(int x, int y, XMouseState mouseState)
{
	if (m_windowData.withSoftBoard && mouseState == MOUSE_RIGHT_BUTTON_DOWN)
	{
		if (m_rightBtnDownTimer < 300.0f)
		{//有效
			if (m_softBoard->getVisible()) m_softBoard->disVisible();
			else m_softBoard->setVisible();
		}
		else
		{
			m_rightBtnDownTimer = 0.0f;
		}
	}
	m_upMousePosition = m_mousePosition;
	if (m_windowData.isFullScreen && m_windowData.isTouch)
		m_mousePosition = getMousePosFromWindow(XEE_SDL_getMouseX(), XEE_SDL_getMouseY());
	else
		m_mousePosition = getMousePosFromWindow(x, y);
	XCtrlManager.mouseProc(getMousePos(), mouseState);
#if WITH_OBJECT_MANAGER
	XObjManager.mouseProc(getMousePos(), mouseState);
#endif
	if (m_windowData.withCustomTitle) m_customWinTitle->mouseProc(getMousePos(), mouseState);
}
bool XFrameWork::cmdProc(void *pClass, const char *cmd, std::string &ret)
{
	XFrameWork &pPar = *(XFrameWork *)pClass;
	if (strcmp(cmd, "-?") == 0 || strcmp(cmd, "-help") == 0)
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
		ret += "-getCode 获取认证码\n";
		ret += "-setKey 设置激活码\n";
		ret += "-check 获取激活状态\n";
		return true;
	}
	if (strcmp(cmd, "-getCode") == 0)
	{
		if (gFrameworkData.pLicense == NULL)
			ret = "不支持该功能";
		else
		{
			gFrameworkData.pLicense->createStr();
			ret = gFrameworkData.pLicense->getCodeStr();
			gFrameworkData.pLicense->saveCode();
			//	printf("%s\n",gFrameworkData.pLicense->createKey().c_str());
			//	if(gFrameworkData.pLicense->certification()) printf("认证成功\n");
			//	else printf("认证成功\n");
		}
		return true;
	}
	if (strcmp(cmd, "-check") == 0)
	{
		if (gFrameworkData.pLicense == NULL)
			ret = "不支持该功能";
		else
		{
			if (gFrameworkData.pLicense->certification()) ret = "认证成功";
			else ret = "认证失败";
		}
		return true;
	}
	if (cmd[0] == '-' && cmd[1] == 's' && cmd[2] == 'e' && cmd[3] == 't' &&
		cmd[4] == 'K' && cmd[5] == 'e' && cmd[6] == 'y' && cmd[7] == ' ')
	{
		if (gFrameworkData.pLicense == NULL)
			ret = "不支持该功能";
		else
		{
			gFrameworkData.pLicense->setKey(cmd + 8);
			//	printf("%s\n",gFrameworkData.pLicense->getKeyStr().c_str());
			gFrameworkData.pLicense->saveKey();
		}
		return true;
	}
	if (strcmp(cmd, "-windowWidth") == 0)
	{
		ret = XString::toString(pPar.m_windowWidth);
		return true;
	}
	if (strcmp(cmd, "-exit") == 0)
	{
		ret = "程序退出";
		if (gFrameworkData.pFrameWork != NULL)
			gFrameworkData.pFrameWork->setGameExit();
		return true;
	}
	if (strcmp(cmd, "-gameInfo") == 0)
	{
		ret = "程序信息:\n";
		ret += "运行时间：" + XTime::sysTime2String(XTime::getTimeMs());
		ret += "执行文件：" + XFile::getCurrentExeFileFullPath() + "\n";
		ret += "编译时间：" + std::string(__DATE__) + " " + std::string(__TIME__) + "\n";
		if (gFrameworkData.pFrameWork != NULL)
		{
			ret += "项目名称：" + gFrameworkData.pFrameWork->m_windowData.windowTitle + "\n";
			ret += "版本号：" + gFrameworkData.pFrameWork->m_windowData.versionString + "\n";
		}
		return true;
	}
	if (strcmp(cmd, "-windowHeight") == 0)
	{
		ret = XString::toString(pPar.m_windowHeight);
		return true;
	}
	if (strcmp(cmd, "-windowSize") == 0)
	{
		ret = XString::toString(pPar.m_windowWidth) + "," + XString::toString(pPar.m_windowHeight);
		return true;
	}
	if (strcmp(cmd, "-fps") == 0)
	{
		ret = XString::toString(pPar.m_curFPS);
		return true;
	}
	if (strcmp(cmd, "-switchFullScreen") == 0)
	{
		pPar.toggleFullScreen();
		if (pPar.m_isSetToFullScreen)
			ret = "设置为全屏";
		else
			ret = "设置为窗口";
		return true;
	}
	if (strcmp(cmd, "-virtualMouse") == 0)
	{
		pPar.m_withVirtualMouse = !pPar.m_withVirtualMouse;
		if (pPar.m_withVirtualMouse)
		{
			ret = "开启虚拟鼠标";
			//	pPar.m_moveX.setCurData(getMousePos().x);
			//	pPar.m_moveY.move(getMousePos().y);
		}
		else ret = "关闭虚拟鼠标";
		return true;
	}
	if (strcmp(cmd, "-setRegRun") == 0)
	{
		if (XEE::setRegRun(true)) ret = "设置开机启动成功";
		else ret = "设置开机启动失败,需要管理员权限.";
		return true;
	}
	if (strcmp(cmd, "-disRegRun") == 0)
	{
		if (XEE::setRegRun(false)) ret = "取消开机启动成功";
		else ret = "取消开机启动失败,需要管理员权限.";
		return true;
	}
	if (strcmp(cmd, "-softBoard") == 0 && pPar.m_windowData.withSoftBoard)
	{
		if (pPar.m_softBoard->getVisible())
		{
			ret = "隐藏软键盘";
			pPar.m_softBoard->disVisible();
		}
		else
		{
			ret = "显示软键盘";
			pPar.m_softBoard->setVisible();
		}
		return true;
	}
	return false;
}
//bool XFrameWork::drawFont(float x,float y,const char *str)
//{
//	if(m_systemFont == NULL) return false;
//	m_systemFont->setString(str);
//	m_systemFont->setPosition(x,y);
//	m_systemFont->draw();
//	return true;
//}
bool XFrameWork::drawFont(const XVec2& pos, const char *str, const XFColor&c)
{
	if (m_systemFont == NULL) return false;
	XFColor tmp = m_systemFont->getColor();
	m_systemFont->setColor(c);
	m_systemFont->setString(str);
	m_systemFont->setPosition(pos);
	m_systemFont->draw();
	m_systemFont->setColor(tmp);
	return true;
}
bool XFrameWork::drawFont(const XVec2& pos, const char *str)
{
	if (m_systemFont == NULL) return false;
	m_systemFont->setString(str);
	m_systemFont->setPosition(pos);
	m_systemFont->draw();
	return true;
}
bool XFrameWork::initCommon()
{
	if (m_isInited) return false;
	if (m_windowData.withCheatModule)
	{
		if (!m_cheatModule->init()) return false;
		m_cheatModule->addCmdProc(cmdProc, this);
	}
	if (m_windowData.withSoftBoard)
	{
		if (!m_softBoard->initWithoutSkin()) return false;
		m_softBoard->disVisible();
	}

	m_moveX->init(0.0f, m_windowWidth, 0.0f, 500, MODE_NO_RECOVER);
	m_moveX->setKeyValue(XKEY_LEFT, XKEY_RIGHT);
	m_moveX->setParam(1.0f, 0.1f, 1.0f);
	m_moveY->init(0.0f, m_windowHeight, 0.0f, 500, MODE_NO_RECOVER);
	m_moveY->setKeyValue(XKEY_UP, XKEY_DOWN);
	m_moveY->setParam(1.0f, 0.1f, 1.0f);

	if (gFrameworkData.pLicense != NULL && !gFrameworkData.pLicense->certification())
	{
		m_isPassKeyLicense = false;
		LogStr("认证失败");
	}

	m_isInited = true;
	return true;
}
bool XFrameWork::initCommonAfterSet()
{
	if (m_windowData.withProjectCorrection)
	{
		m_projCorrection = XMem::createMem<XProjectionCorrection>();
		if (m_projCorrection == nullptr)
		{
			LogStr("投影矫正初始化失败!");
			m_windowData.withProjectCorrection = false;
			return false;
		}
		m_projCorrection->setConfigFilename("projectCFG.xml");
		m_projCorrection->init();
		m_projCorrection->setEnableInput(false);
	}
	if (m_windowData.withMaskCorrection)
	{
		m_maskCorrection = XMem::createMem<XMaskCorrection>();
		if (m_maskCorrection == nullptr)
		{
			LogStr("投影矫正初始化失败!");
			m_windowData.withMaskCorrection = false;
			return false;
		}
		m_maskCorrection->init(m_windowData.maskFilename.c_str(), "maskCFG.xml");
		m_maskCorrection->setEnableInput(false);
	}
	return true;
}
void XFrameWork::updateCommon(float stepTime)
{
	if (m_rightBtnDownTimer < 1000.0f) m_rightBtnDownTimer += stepTime;
	if (m_withVirtualMouse)
	{//这里设置鼠标的位置
		m_moveX->move(stepTime);
		m_moveY->move(stepTime);
		//printf("%f,%f\n",m_moveX.getCurData(),m_moveY.getCurData());
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, m_moveX->getCurData() * 65536.0f / m_windowWidth,
			m_moveY->getCurData() * 65536.0f / m_windowHeight, 0, 0);
	}
}
bool XFrameWork::getIsLBDrag(const XInputEvent &event)const
{
	return (event.type == EVENT_MOUSE && event.mouseState == MOUSE_MOVE && 
		getIsMouseLBDown());
}
bool XFrameWork::getIsRBDrag(const XInputEvent &event)const
{
	return (event.type == EVENT_MOUSE && event.mouseState == MOUSE_MOVE &&
		getIsMouseRBDown());
}
bool XFrameWork::singletonProc()
{
	return m_windowData.isSingletonMode &&
		isProgramExist(XFile::getCurrentExeFileFullPath());
}
void XFrameWork::errorLoop()
{
	if (m_windowData.withLowConsumption) XEE::sleep(1);	//如果需要较高的反应效率，这里可以屏蔽
	float stepTime = engineIdle();
	//TODO:Update
	m_errorExitTimer += stepTime * 0.001f;
	if (m_systemFont != NULL && m_systemFont->getIsInited())
	{
		char tmpStr[512];
		m_systemFont->setPosition(getSceneCenter());
		m_systemFont->setColor(XFColor::red);
		m_systemFont->setScale(1.5f);
		if (!XEG.getIsGameExit() && XEG.m_windowData.isRestartWhenCrash)
		{
			sprintf_s(tmpStr, 512, "程序发生异常\n将于%.0f秒后[重新启动]", 10.0f - m_errorExitTimer);
		}
		else
		{
			sprintf_s(tmpStr, 512, "程序发生异常\n将于%.0f秒后[退出]", 10.0f - m_errorExitTimer);
		}
		m_systemFont->setString(tmpStr);
	}
	if (m_errorExitTimer > 10.0f)
	{
#if WITH_LOG
		LogRelease();	//保持日志文件的完整性
#endif		
		if (!XEG.getIsGameExit() && XEG.m_windowData.isRestartWhenCrash) XEG.restartGame();	//重新启动程序
		XEG.setGameExit();				//防止由于退出时再次产生异常
		exit(1);
	}

	updateCommon(stepTime);
	if (!getWindowNeedDraw() ||
		!m_drawTimer.update(stepTime)) return;
	if (m_windowData.withClearBG) clearScreen();			//清除屏幕,如果每次都全屏刷新的话，优化这里会有一点效果
	//TODO:Draw
	XRender::drawFillRectExA(getSceneCenter() - XVec2(250, 80), XVec2(500, 160), XFColor::yellow, true, true);
	if (m_systemFont != NULL && m_systemFont->getIsInited())
		m_systemFont->draw();
	updateScreen(false);			//更新屏幕的内容
}
void XFrameWork::mainLoop2D()
{
	XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
	if (m_windowData.withLowConsumption) XEE::sleep(1);	//如果需要较高的反应效率，这里可以屏蔽
	float stepTime = engineIdle();
	if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
		gFrameworkData.pCfgManager->update();
	if (m_isPassKeyLicense)
	{
#if LIMITED_VERSION == 1
		if (m_limitedTimer >= 0.0f)
		{//限时版本只能运行1小时
			m_limitedTimer -= stepTime * 0.001f;
			tmpGame.move(stepTime);
		}
#else
		tmpGame.move(stepTime);
#endif
	}
	updateCommon(stepTime);
	if (!getWindowNeedDraw() ||
		!m_drawTimer.update(stepTime)) return;
	if (m_windowData.withProjectCorrection || m_windowData.withMaskCorrection)
	{
		if (m_windowData.withProjectCorrection && m_windowData.withMaskCorrection)
		{
			m_projCorrection->begin();
			if (m_windowData.withClearBG) clearScreen();			//清除屏幕,如果每次都全屏刷新的话，优化这里会有一点效果
			tmpGame.draw();
			if (m_projCorrection->getIsShowCtrl())
			{//如果是调试投影矫正的话，这些显示网格
				int size = XMath::getGCM(XEG.getSceneW(), XEG.getSceneH()) >> 1;
				int w = XEG.getSceneW() / size;
				int h = XEG.getSceneH() / size;
				for (int i = 0, index = 0; i <= w; ++i, index += size)
				{
					XRender::drawLine(XVec2(index, 0), XVec2(index, XEG.getSceneH()), 2.0f, XFColor::blue);
				}
				for (int i = 0, index = 0; i <= h; ++i, index += size)
				{
					XRender::drawLine(XVec2(0, index), XVec2(XEG.getSceneW(), index), 2.0f, XFColor::blue);
				}
			}
			m_projCorrection->end();

			m_maskCorrection->begin();
			m_projCorrection->draw();
			m_maskCorrection->end();
			m_maskCorrection->draw();
		}else
		if (m_windowData.withProjectCorrection)
		{
			m_projCorrection->begin();
			if (m_windowData.withClearBG) clearScreen();			//清除屏幕,如果每次都全屏刷新的话，优化这里会有一点效果
			tmpGame.draw();
			if (m_projCorrection->getIsShowCtrl())
			{//如果是调试投影矫正的话，这些显示网格
				int size = XMath::getGCM(XEG.getSceneW(), XEG.getSceneH()) >> 1;
				int w = XEG.getSceneW() / size;
				int h = XEG.getSceneH() / size;
				for (int i = 0, index = 0; i <= w; ++i, index += size)
				{
					XRender::drawLine(XVec2(index, 0), XVec2(index, XEG.getSceneH()), 2.0f, XFColor::blue);
				}
				for (int i = 0, index = 0; i <= h; ++i, index += size)
				{
					XRender::drawLine(XVec2(0, index), XVec2(XEG.getSceneW(), index), 2.0f, XFColor::blue);
				}
			}
			m_projCorrection->end();
			m_projCorrection->draw();
		}
		else
		{
			m_maskCorrection->begin();
			if (m_windowData.withClearBG) clearScreen();			//清除屏幕,如果每次都全屏刷新的话，优化这里会有一点效果
			tmpGame.draw();
			m_maskCorrection->end();
			m_maskCorrection->draw();
		}
	}
	else
	{
		if (m_windowData.withClearBG) clearScreen();			//清除屏幕,如果每次都全屏刷新的话，优化这里会有一点效果
		tmpGame.draw();
	}
	if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
		gFrameworkData.pCfgManager->draw();
	updateScreen();			//更新屏幕的内容
}
void XFrameWork::mainLoop3D()
{
	XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
	if (m_windowData.withLowConsumption) XEE::sleep(1);	//如果需要较高的反应效率，这里可以屏蔽
	float stepTime = engineIdle();
	if (gFrameworkData.p3DWorld != NULL)
		gFrameworkData.p3DWorld->move(stepTime);
	if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
		gFrameworkData.pCfgManager->update();
	if (m_isPassKeyLicense)
	{
#if LIMITED_VERSION == 1
		if (m_limitedTimer >= 0.0f)
		{//限时版本只能运行1小时
			m_limitedTimer -= stepTime * 0.001f;
			tmpGame.move(stepTime);
		}
#else
		tmpGame.move(stepTime);
#endif
	}
	updateCommon(stepTime);
	if (!getWindowNeedDraw() ||
		!m_drawTimer.update(stepTime)) return;
	if (m_windowData.withClearBG) clearScreen();			//清除屏幕，优化这里会有一点效果
	//TODO:下面描绘需要描绘的3D内容
	if (gFrameworkData.p3DWorld != NULL)
		gFrameworkData.p3DWorld->draw();
	begin2DDraw();

	tmpGame.draw();
	if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
		gFrameworkData.pCfgManager->draw();
	updateScreen();			//更新屏幕的内容
}
void XFrameWork::miniInputEvent(const XInputEvent &tmpEvent)
{
	switch (tmpEvent.type)
	{
	case EVENT_KEYBOARD:
		if (XImm::getCompositionState()) m_isImmState = true;
		if (!m_isImmState) engineKeyProc(tmpEvent.keyValue, tmpEvent.keyState);
		if (tmpEvent.keyState == KEY_STATE_UP && !XImm::getCompositionState())
			m_isImmState = false;
		break;
	case EVENT_MOUSE:
		engineMouseProc(tmpEvent.mouseX, tmpEvent.mouseY, tmpEvent.mouseState);
		break;
	}
}
//返回是否退出
bool XFrameWork::inputEvent(const XInputEvent &tmpEvent, bool send2Game)
{
	switch (tmpEvent.type)
	{
	case EVENT_EXIT:return true;
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
			case XKEY_ESCAPE: return true;
			case XKEY_F1:
				if (saveSystemConfigData(m_windowData)) XEG.setTopMsgStr("完整配置文件已经保存完成!");
				else XEG.setTopMsgStr("配置文件保存失败!");
				break;
			}
		}
		if (XImm::getCompositionState()) m_isImmState = true;
		if (!m_isImmState) engineKeyProc(tmpEvent.keyValue, tmpEvent.keyState);
		if (tmpEvent.keyState == KEY_STATE_UP && !XImm::getCompositionState())
			m_isImmState = false;
		break;
	case EVENT_MOUSE:
		switch (tmpEvent.mouseState)
		{
		case MOUSE_LEFT_BUTTON_DOWN:m_isMouseLBDown = true; break;
		case MOUSE_LEFT_BUTTON_UP:m_isMouseLBDown = false; break;
		case MOUSE_RIGHT_BUTTON_DOWN:m_isMouseRBDown = true; break;
		case MOUSE_RIGHT_BUTTON_UP:m_isMouseRBDown = false; break;
		}
		engineMouseProc(tmpEvent.mouseX, tmpEvent.mouseY, tmpEvent.mouseState);
		break;
	}
	//for test
	//if (tmpEvent.isMouseLeftDown())
	//	printf("哈哈!\n");
	if (m_withVirtualMouse)
	{
		m_moveX->updateState(tmpEvent);
		m_moveY->updateState(tmpEvent);
	}

	if (send2Game)
	{
		m_inputEventBuff.doProc(tmpEvent);
		if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
			gFrameworkData.pCfgManager->inputProc(tmpEvent);
		if (m_windowData.windowType == WINDOW_TYPE_3D && gFrameworkData.p3DWorld != NULL)
			gFrameworkData.p3DWorld->keyEvent(tmpEvent);
		if (m_windowData.withProjectCorrection)
		{
			if (tmpEvent.isKeyBoardDown() && tmpEvent.keyValue == XKEY_F2)
			{
				if (m_corStage != CSTAGE_PROJECTION)
				{
					m_corStage = CSTAGE_PROJECTION;
					m_projCorrection->setIsShowCtrl(true);
					m_projCorrection->setEnableInput(true);
					if (m_maskCorrection != nullptr)
					{
						m_maskCorrection->setIsShowCtrl(false);
						m_maskCorrection->setEnableInput(false);
					}
					setTopMsgStr("投影矫正");
				}
				else
				{
					m_corStage = CSTAGE_NULL;
					m_projCorrection->setIsShowCtrl(false);
					m_projCorrection->setEnableInput(false);
					setTopMsgStr("普通模式");
				}
			}
			if (m_projCorrection->inputProc(tmpEvent)) return false;
		}
		if (m_windowData.withMaskCorrection)
		{
			if (tmpEvent.isKeyBoardDown() && tmpEvent.keyValue == XKEY_F3)
			{
				if (m_corStage != CSTAGE_MASK)
				{
					m_corStage = CSTAGE_MASK;
					m_maskCorrection->setIsShowCtrl(true);
					m_maskCorrection->setEnableInput(true);
					if (m_projCorrection != nullptr)
					{
						m_projCorrection->setIsShowCtrl(false);
						m_projCorrection->setEnableInput(false);
					}
					setTopMsgStr("Mask矫正");
				}
				else
				{
					m_corStage = CSTAGE_NULL;
					m_maskCorrection->setIsShowCtrl(false);
					m_maskCorrection->setEnableInput(false);
					setTopMsgStr("普通模式");
				}
			}
			if (m_maskCorrection->inputProc(tmpEvent)) return false;
		}
		if (m_pGame != NULL)
			((XGameBasic *)m_pGame)->input(tmpEvent);
	}
	return false;
}
#if CREATE_WINDOW_METHOD == 0
bool XFrameWork::inputEvent(bool send2Game)
{//下面是事件处理函数，例如：键盘事件或者鼠标事件
	XInputEvent tmpEvent;		//SDL事件句柄
	try
	{//注意：由于SDL中对消息的处理存在bug，此处可能会造成crash，需要进一步的调试，这里进行临时修补
		while (getInputEvent(tmpEvent))
		{
			if (inputEvent(tmpEvent, send2Game)) return true;
		}
	}
	catch (...)
	{
		return false;
	}
	return m_isGameExit;
}
//void XFrameWork::startGame(void *game,const XWindowInfo* windowInfo)
//{
//	if (game == NULL) return;
//	if (windowInfo == NULL)
//	{
//		if(!readSystemConfigData(m_windowData,true)) LogStr("读取配置文件失败!");
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
//	if(m_manualFPS > 0)
//		m_fpsTimer.setStepTime(1000.0f / m_manualFPS);
	initCommon();
	if (m_windowData.windowType == WINDOW_TYPE_3D)
	{
		XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
		if (gFrameworkData.p3DWorld == NULL || !gFrameworkData.p3DWorld->init(NULL)) XEE_Error("建立窗体失败");

		if (!tmpGame.init()) XEE_Error("初始化失败");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//程序主循环
//			if(m_manualFPS <= 0 || m_fpsTimer.update())
				mainLoop3D();
		}
		setGameExit();
		tmpGame.release();
	}
	else
	{
		XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
		if (!tmpGame.init()) XEE_Error("初始化失败");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//程序主循环
//			if(m_manualFPS <= 0 || m_fpsTimer.update())
			mainLoop2D();
		}
		setGameExit();
		tmpGame.release();
	}
	m_pGame = nullptr;
	XCtrlManager.setGame(nullptr);
}
#endif
#if CREATE_WINDOW_METHOD == 1
/*
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
}*/
void XFrameWork::glutIdle()
{
	if(XEG.m_windowData.windowType == WINDOW_TYPE_3D)
		XEG.mainLoop3D();
	else 
		XEG.mainLoop2D();
}
void XFrameWork::glutDraw(){}
//void draw(){}
//void idle()
//{
//	pGame->move(engineIdle());
//	clearScreen();			//清除屏幕
//	pGame->draw();
//	updateScreen();			//更新屏幕的内容
//	//mySleep(1);
//}
void XFrameWork::inputProc(const XInputEvent &input)
{
	//if(XEG.m_windowData.windowType == WINDOW_TYPE_3D)
	//	((XGameBasic3D *)m_pGame)->input(input);
	//else 
	//	((XGameBasic *)m_pGame)->input(input);
	if(inputEvent(input)) exit(1);
}
void XFrameWork::glutInput(unsigned char c, int x, int y)
{
	XInputEvent tmpEvent;
	tmpEvent.type = EVENT_KEYBOARD;
	tmpEvent.keyState = KEY_STATE_DOWN;
	tmpEvent.keyValue = (XKeyValue)XWindow.mapKey(c);
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	XEG.inputProc(tmpEvent);
	if (tmpEvent.keyValue == XKEY_ESCAPE) exit(1);
}
void XFrameWork::glutsInput(int k, int x, int y)
{
	XInputEvent tmpEvent;
	tmpEvent.type = EVENT_KEYBOARD;
	tmpEvent.keyState = KEY_STATE_DOWN;
	tmpEvent.keyValue = (XKeyValue)XWindow.mapKey(k + 512);
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	XEG.inputProc(tmpEvent);
	if (tmpEvent.keyValue == XKEY_ESCAPE) exit(1);
}
void XFrameWork::glutMouseMove(int x, int y)
{
	XInputEvent tmpEvent;
	tmpEvent.type = EVENT_MOUSE;
	tmpEvent.mouseState = MOUSE_MOVE;
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	XEG.inputProc(tmpEvent);
}
void XFrameWork::glutMouseProc(int b, int s, int x, int y)
{
	XInputEvent tmpEvent;
	tmpEvent.type = EVENT_MOUSE;
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	if (s == GLUT_DOWN)
	{
		switch (b)
		{
		case GLUT_LEFT_BUTTON: tmpEvent.mouseState = MOUSE_LEFT_BUTTON_DOWN; break;
		case GLUT_RIGHT_BUTTON: tmpEvent.mouseState = MOUSE_RIGHT_BUTTON_DOWN; break;
		case GLUT_MIDDLE_BUTTON: tmpEvent.mouseState = MOUSE_MIDDLE_BUTTON_DOWN; break;
		}
	}
	else
	{
		switch (b)
		{
		case GLUT_LEFT_BUTTON: tmpEvent.mouseState = MOUSE_LEFT_BUTTON_UP; break;
		case GLUT_RIGHT_BUTTON: tmpEvent.mouseState = MOUSE_RIGHT_BUTTON_UP; break;
		case GLUT_MIDDLE_BUTTON: tmpEvent.mouseState = MOUSE_MIDDLE_BUTTON_UP; break;
		}
	}
	XEG.inputProc(tmpEvent);
}
void XFrameWork::gameProc(void *game)
{
	m_pGame = game;
	//初始化glut,这初始化的参数也太蛋疼了吧
	//方案1：简单
	int ac = 0;
	char *av[1] = {NULL};
	//方案2：防止意外
	//int ac = 1;
	//char filename[MAX_FILE_NAME_LENGTH];
	//char *av[1] = {filename};
	//strcpy(filename,XFile::getCurrentExeFileFullPath().c_str());
	glutInit(&ac, av);

	initCommon();
	if(m_windowData.windowType == WINDOW_TYPE_3D)
	{
		XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
		if(gFrameworkData.p3DWorld == NULL || !gFrameworkData.p3DWorld->init(NULL)) XEE_Error("建立窗体失败");
		if(!tmpGame.init()) XEE_Error("初始化失败");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		
		glutDisplayFunc(glutDraw);
		glutIdleFunc(glutIdle);
		//glutReshapeFunc(changeSize);
		glutKeyboardFunc(glutInput);
		glutSpecialFunc(glutsInput);
		glutMouseFunc(glutMouseProc);
		glutMotionFunc(glutMouseMove);
		glutPassiveMotionFunc(glutMouseMove);
		glutMainLoop();

		setGameExit();
		tmpGame.release();
	}else
	{
		XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
		if (!tmpGame.init()) XEE_Error("初始化失败");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		XSimpleTimer drawTimer;	//显示的计时器，显示太快实际上没有意义，逻辑线程可以跑很快，但是显示线程没有必要很快
		m_drawTimer.setStepTime(m_rendDelay);

		glutDisplayFunc(glutDraw);
		glutIdleFunc(glutIdle);
		//glutReshapeFunc(changeSize);
		glutKeyboardFunc(glutInput);
		glutSpecialFunc(glutsInput);
		glutMouseFunc(glutMouseProc);
		glutMotionFunc(glutMouseMove);
		glutPassiveMotionFunc(glutMouseMove);
		glutMainLoop();

		setGameExit();
		tmpGame.release();
	}
	m_pGame = nullptr;
	XCtrlManager.setGame(nullptr);
}
#endif
#if CREATE_WINDOW_METHOD == 2
bool XFrameWork::inputEvent(bool send2Game)
{//下面是事件处理函数，例如：键盘事件或者鼠标事件
	XInputEvent tmpEvent;		//SDL事件句柄
	try
	{//注意：由于SDL中对消息的处理存在bug，此处可能会造成crash，需要进一步的调试，这里进行临时修补
		while (getInputEvent(tmpEvent))
		{
			if (inputEvent(tmpEvent, send2Game)) return true;
		}
	}
	catch (...)
	{
		return 0;
	}
	return m_isGameExit;
}
void XFrameWork::gameProc(void *game)
{
	m_pGame = game;
	//	if(m_manualFPS > 0)
	//		m_fpsTimer.setStepTime(1000.0f / m_manualFPS);
	initCommon();
	if (m_windowData.windowType == WINDOW_TYPE_3D)
	{
		XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
		if (gFrameworkData.p3DWorld == NULL || !gFrameworkData.p3DWorld->init(NULL)) XEE_Error("建立窗体失败");

		if (!tmpGame.init()) XEE_Error("初始化失败");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//程序主循环
		 //			if(m_manualFPS <= 0 || m_fpsTimer.update())
			mainLoop3D();
		}
		setGameExit();
		tmpGame.release();
	}
	else
	{
		XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
		if (!tmpGame.init()) XEE_Error("初始化失败");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//程序主循环
		 //			if(m_manualFPS <= 0 || m_fpsTimer.update())
			mainLoop2D();
		}
		setGameExit();
		tmpGame.release();
	}
	m_pGame = nullptr;
	XCtrlManager.setGame(nullptr);
}
#endif
XBool XFrameWork::readSystemConfigData(XWindowInfo &data, bool forceTitle)
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
	XXml::getXmlAsBool(rootNode, "isFullScreen", data.isFullScreen);
	XXml::getXmlAsBool(rootNode, "isTouch", data.isTouch);
	XXml::getXmlAsInt(rootNode, "withLog", data.withLog);
	XXml::getXmlAsBool(rootNode, "isShowCursor", data.isShowCursor);
	XXml::getXmlAsBool(rootNode, "withFrame", data.withFrame);
	XXml::getXmlAsInt(rootNode, "windowMode", (int &)data.mode);
	if(!forceTitle || data.windowTitle.size() == 0)
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
		case RES_LOCAL_FOLDER:
		case RES_LOCAL_PACK:
		case RES_WEB:
		case RES_AUTO:
			data.defResourcePos = (XResPos)temp;
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
	if(data.withAlphaBG) data.withFrame = false;	//如果使用透明窗体的话，则不能有边框
	XXml::getXmlAsInt(rootNode, "logicFps", data.logicFps);
	XXml::getXmlAsInt(rootNode, "drawFps", data.drawFps);
	XXml::getXmlAsBool(rootNode, "withException", data.withException);
	XXml::getXmlAsBool(rootNode, "withLowConsumption", data.withLowConsumption);
	XXml::getXmlAsBool(rootNode, "withClearBG", data.withClearBG);
	XXml::getXmlAsBool(rootNode, "isRestartWhenCrash", data.isRestartWhenCrash);
	XXml::getXmlAsBool(rootNode, "isSingletonMode", data.isSingletonMode);
	XXml::getXmlAsBool(rootNode, "withProjectCorrection", data.withProjectCorrection);
	XXml::getXmlAsBool(rootNode, "withMaskCorrection", data.withMaskCorrection);
	XXml::getXmlAsString(rootNode, "maskFilename", data.maskFilename);
	XXml::getXmlAsBool(rootNode, "withCheatModule", data.withCheatModule);
	XXml::getXmlAsBool(rootNode, "withSoftBoard", data.withSoftBoard);
	XXml::getXmlAsBool(rootNode, "withCaregiver", data.withCaregiver);

	if (data.withFrame && data.withCustomTitle) data.withCustomTitle = false;
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
	if (!XXml::addLeafNode(elmRoot, "w", data.w)) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口高度")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "h", data.h)) return XFalse;
	if (!XXml::addComment(elmRoot, "场景宽度")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "sceneW", data.sceneW)) return XFalse;
	if (!XXml::addComment(elmRoot, "场景高度")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "sceneH", data.sceneH)) return XFalse;
	if (!XXml::addComment(elmRoot, "场景的x偏移")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "positionX", data.positionX)) return XFalse;
	if (!XXml::addComment(elmRoot, "场景的y偏移")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "positionY", data.positionY)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否全屏：0、不全屏，1、全屏")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isFullScreen", data.isFullScreen)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否支持触摸屏：0、不支持，1、支持")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isTouch", data.isTouch)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否输出日志：0、不输出，1、后台输出，2、文件输出，3、都输出")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withLog", XString::toString(data.withLog))) return XFalse;
	if (!XXml::addComment(elmRoot, "是否显示鼠标：0、不显示，1、显示")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isShowCursor", data.isShowCursor)) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口是否有边框：0、没有边框，1、有边框")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withFrame", data.withFrame)) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口模式：0、左上角对齐并裁剪，1、重点对齐并裁剪，2、短边拉伸长边裁剪，3、长边拉伸短边填空白，4、自动拉伸全屏")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowMode", XString::toString(data.mode))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口标题")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowTitle", data.windowTitle)) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口的旋转模式：0、不旋转，1、90旋转，2、180度旋转，3、270旋转")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "rotateMode", XString::toString(data.rotateMode))) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口的反转模式：0、不翻转，1、左右翻转，2、上下翻转，3、左右上下翻转")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "turnOverMode", XString::toString(data.turnOverMode))) return XFalse;
	if (!XXml::addComment(elmRoot, "配置的工作模式：0、普通模式，1、服务器模式，2、客户端模式")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "CFGMode", data.CFGMode)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否开启启动延迟")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isStartDelay", data.isStartDelay)) return XFalse;
	if (!XXml::addComment(elmRoot, "启动延迟的时间：单位ms")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "startDelayTimer", data.startDelayTimer)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否显示版本号")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isShowVersion", data.isShowVersion)) return XFalse;
	if (!XXml::addComment(elmRoot, "版本号")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "versionString", data.versionString)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否启动定时关机")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isAutoShutDown", data.isAutoShutDown)) return XFalse;
	if (!XXml::addComment(elmRoot, "定时关机的时间.小于0表示这项无效")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_year", data.autoShutDownYear)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_month", data.autoShutDownMonth)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_day", data.autoShutDownDay)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_hour", data.autoShutDownHour)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_minut", data.autoShutDownMinute)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_second", data.autoShutDownSecond)) return XFalse;
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
	if (!XXml::addLeafNode(elmRoot, "backgroundValume", data.BGVolume)) return XFalse;
	if (!XXml::addComment(elmRoot, "前景音乐的音量")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "formwordValume", data.FWVolume)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否设置窗口位置")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isInitWindowPos", data.isInitWindowPos)) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口位置X")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowPosX", data.windowPosX)) return XFalse;
	if (!XXml::addComment(elmRoot, "窗口位置Y")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowPosY", data.windowPosY)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否置顶窗口")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isAlwaysTop", data.isAlwaysTop)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否使用自己的标题栏")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withCustomTitle", data.withCustomTitle)) return XFalse;
	if (!XXml::addComment(elmRoot, "背景是否透明")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withAlphaBG", data.withAlphaBG)) return XFalse;
	if (!XXml::addComment(elmRoot, "系统字体的大小")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "systemFontSize",data.systemFontSize)) return XFalse;
	if (!XXml::addComment(elmRoot, "逻辑帧率")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "logicFps", data.logicFps)) return XFalse;
	if (!XXml::addComment(elmRoot, "显示帧率")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "drawFps", data.drawFps)) return XFalse;
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
	if (!XXml::addComment(elmRoot, "使能投影矫正功能")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withProjectCorrection", data.withProjectCorrection)) return XFalse;
	if (!XXml::addComment(elmRoot, "使能矩形遮黑功能")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withMaskCorrection", data.withMaskCorrection)) return XFalse;
	if (!XXml::addComment(elmRoot, "遮黑的文件名，需要的地方为黑，不需要的地方为白色")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "maskFilename", data.maskFilename)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否使用命令输入功能")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withCheatModule", data.withCheatModule)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否提供虚拟键盘的支持")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withSoftBoard", data.withSoftBoard)) return XFalse;
	if (!XXml::addComment(elmRoot, "是否使用监护功能")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withCaregiver", data.withCaregiver)) return XFalse;

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
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}
	if (m_windowData.isProjectAsDaemon) setProjectAsDaemon();
	//初始化一些变量的值
	m_windowWidth = m_windowData.w;
	m_windowHeight = m_windowData.h;
	m_scenePosX = m_windowData.positionX;
	m_scenePosY = m_windowData.positionY;
	m_sceneWidth = m_windowData.sceneW;
	m_sceneHeight = m_windowData.sceneH;
	m_sceneScaleX = m_sceneScaleY = 1.0f;		//窗口的缩放比例
	srand((unsigned)time(0)); //引入随机因子
	m_periodicLogTimer->setStepTime(10000);	//10s一个周期
	m_defWorkPath = XFile::getWorkPath();	//记录工作路径
	m_autoShutDownTimer.setStepTime(1000.0f);
	XEE::getCpuUsage();
	return XTrue;
}
float XFrameWork::engineIdle()
{
	//		static XTimer tempTimer;
	//		static Uint32 current,last = 0,five = 0,nframes = 0;
	//		current = tempTimer.getCurrentTime();
	//		++nframes;
	//		if(current - five > 5000) 
	//		{//将帧率显示出来
	//			curFPS = nframes * 0.2f;
	////#ifdef DEBUG_MODE
	//			if(manualFPS > 0) printf("%.1f FPS@%d\n",curFPS,manualFPS);
	//			else printf("%.1f FPS\n",curFPS);
	////#endif
	//			nframes = 0;
	//			five = current;
	//		}
	//		frameTime = current - last;
	//		last = current;
	//上下两种方式在低帧率时差距较大，不知道为什么
	//方案1：
	//static Uint32 current,last = 0;
	//if(last <= 0)
	//{
	//	last = XTime::getCurrentTicks();	//防止第一次调用的时候由于程序启动的时间消耗而造成第一帧特别长
	//	XEE::getCpuUsage();
	//}
	//current = XTime::getCurrentTicks();
	//m_frameTime = current - last;
	//last = current;
	//方案2：
	static XStepTimer steptimer;
	float curFrameTime = steptimer.update();
	if (m_isFrameTimerEscape)
	{
		m_frameTime = 1.0f;
		m_isFrameTimerEscape = false;
	}
	else
	{
		m_frameTime = curFrameTime;
	}
	//LogNull("%f",m_frameTime); //for test

	++m_curframeCounter;
	if (curFrameTime > m_maxFrameStep) m_maxFrameStep = curFrameTime;
	if (m_periodicLogTimer->update(curFrameTime))
	{
		m_curFPS = m_curframeCounter * 1000.0f / m_periodicLogTimer->getStepTime();	//计算当前帧率

		XEE::XMemUsageInfo tmpInfo = XEE::getMemUsageInfo();
		if (m_curMemInfo.curWorkingSet == 0)
		{//第一次进入的时候显示总量
			m_curMemInfo = tmpInfo;
			if (m_manualFPS > 0) LogNull("FPS:%.1f@%d Max:%.0fms Top:%.1fMB Cur:%dB CPU:%.1f%%",
				m_curFPS, m_manualFPS, m_maxFrameStep, m_curMemInfo.peakWorkSet * 9.5367431640625e-7,
				m_curMemInfo.curWorkingSet, XEE::getCpuUsage());
			else LogNull("FPS:%.1f Max:%.0fms Top:%.1fMB Cur:%dB CPU:%.1f%%", m_curFPS, m_maxFrameStep,
				m_curMemInfo.peakWorkSet * 9.5367431640625e-7, m_curMemInfo.curWorkingSet, XEE::getCpuUsage());
		}
		else
		{//否则显示增量
			LogNull("f:%.1f m:%.0f t:%.1f c:%d CPU:%.1f%%", m_curFPS, m_maxFrameStep,
				(tmpInfo.peakWorkSet - m_curMemInfo.peakWorkSet) * 9.5367431640625e-7,
				tmpInfo.curWorkingSet - m_curMemInfo.curWorkingSet, XEE::getCpuUsage());
			m_curMemInfo = tmpInfo;
		}
		m_curframeCounter = 0;
		m_maxFrameStep = -1.0f;
	}
	//这种控制方式，帧率控制不精确，但是CPU消耗较小
	if (m_manualFPS > 0 && curFrameTime < m_manualFrameTime)
	{
		//SDL_Delay(XEE_setFrameTime - XEE_frameTime);
		Sleep(m_manualFrameTime - curFrameTime);
	}
	if (XErrReporter.isErrored()
		&& !XErrReporter.getIsMainProc())
	{
		XErrReporter.setMainProc();
		XErrReporter.errorProc();
		exit(1);
	}
	engineMove(m_frameTime);

	if (m_windowData.withCaregiver &&m_pCaregiver != NULL)
		m_pCaregiver->update(m_frameTime);

	return m_frameTime;	//返回两次调用之间的时间差
}
void XFrameWork::engineMove(float steptime)
{
	if (m_windowData.isShowVersion && m_showVersionTimer < 60000.0f)	//版本号显示一分钟
		m_showVersionTimer += steptime;
	//下面是自动关机的控制
	if (m_windowData.isAutoShutDown && m_autoShutDownTimer.update(steptime))
	{//下面控制自动关机
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
		}
		else
		{
			m_autoShutDownState = 0;
		}
	}
	XCurSndCore.update(steptime);
	XCtrlManager.update(steptime);
#if WITH_OBJECT_MANAGER
	XObjManager.move(steptime);
#endif
	if (m_windowData.withCustomTitle) m_customWinTitle->move(steptime);
#ifdef GET_ALL_PIXEL
	XEE_AllPixel = 0;
#endif
	if (!m_fontMD.getIsEnd())
		m_fontMD.move(steptime);
}
void XFrameWork::engineSysInit()	//窗口建立之后的初始化
{
	setSystemFont(m_windowData.systemFontFile.c_str());
	if (m_windowData.withCustomTitle) m_customWinTitle->init();
	if (m_windowData.logicFps > 0) setFPS(m_windowData.logicFps);
	if (m_windowData.drawFps <= 0) m_rendDelay = -1;
	else m_rendDelay = 1000.0f / m_windowData.drawFps;
#if WITH_OBJECT_MANAGER
	XObjManager.init();
#endif
	XErrReporter.init();
	if (m_windowData.withAlphaBG)
	{//设置窗口背景为透明
#if BACKGROUND_ALPHA_MODE == 0
		DWORD style = GetWindowLong(m_hwnd, GWL_EXSTYLE | WS_EX_TRANSPARENT);
		style &= ~WS_OVERLAPPEDWINDOW;
		SetWindowLong(m_hwnd, GWL_EXSTYLE, style);
		DWM_BLURBEHIND bb = { 0 };
		HRGN hRgn = CreateRectRgn(0, 0, -1, -1);
		bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
		bb.hRgnBlur = hRgn;
		bb.fEnable = TRUE;
		DwmEnableBlurBehindWindow(m_hwnd, &bb);
#endif
#if BACKGROUND_ALPHA_MODE == 1
		DWORD style = GetWindowLong(m_hwnd, GWL_EXSTYLE | WS_EX_TRANSPARENT);
		style |= WS_EX_LAYERED;
		SetWindowLong(m_hwnd, GWL_EXSTYLE, style);
		initBgAlphaData();
#endif
	}
}
void XFrameWork::engineDraw()
{
	if (m_haveSystemFont && m_windowData.isShowVersion
		&& m_showVersionTimer < 60000.0) m_showVersionFont->draw();
	if (m_windowData.withCustomTitle) m_customWinTitle->draw();
	XCtrlManager.draw();
	XCtrlManager.drawUp();
#if WITH_OBJECT_MANAGER
	XObjManager.draw();
#endif
	if (m_drawAfterCtrl != NULL) m_drawAfterCtrl();
	//2014.4.14,修改为显示自己的光标
#ifdef WITHXEE_CURSOR
	if (m_windowData.isShowCursor)
	{//下面显示光标
		XRender::drawCross(getMousePos(), 10.0f, 3, XFColor::white);
		XRender::drawCross(getMousePos(), 8.0f, 2, XFColor::black);
	}
#endif
	if (!m_fontMD.getIsEnd() && m_systemFont != NULL)
		drawFont(getSceneCenter(), m_topMsgStr.c_str(), XFColor(1.0f, m_fontMD.getCurData()));
#if LIMITED_VERSION == 1
	if (m_systemFont == NULL) XEE::makeCrash();
	drawFont(getSceneCenter(), "内测版本");
#endif
}
void XFrameWork::setTopMsgStr(const std::string &str, bool withLog)
{
	if(withLog) LogStr(str.c_str());
	m_fontMD.setEx(0.0f, 1.0f, 1.0f, MD_MODE_SIN_MULT, 1);
	m_topMsgStr = str;
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
	if (m_isBGAlphaInited) return;
	m_isBGAlphaInited = true;
	GetWindowRect(m_hwnd, &m_rc);
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
	m_hBitmap = CreateDIBSection(m_wHDC, &m_bmpinfo, DIB_RGB_COLORS, &m_pBits, NULL, 0);
	assert(m_hBitmap != NULL);
	m_blf.BlendOp = AC_SRC_OVER;
	m_blf.BlendFlags = 0;
	m_blf.SourceConstantAlpha = 255;
	m_blf.AlphaFormat = AC_SRC_ALPHA;//1;
	m_bgAlphaPbo = XMem::createMem<XPBO>();
	assert(m_bgAlphaPbo != NULL);
	m_bgAlphaPbo->init(m_windowWidth, m_windowHeight, 0, 0, m_windowWidth, m_windowHeight, COLOR_BGRA);
	//wglMakeCurrent(m_hMemDc,wCurrentHGLRC);
	m_oldGDIObj = SelectObject(m_hMemDc, m_hBitmap);
	//	bgAlphaFbo.init(windowWidth,windowHeight,COLOR_RGBA);
}
void XFrameWork::releaseBhAlphaData()				//更新屏幕的内容
{
	if (!m_isBGAlphaInited) return;
	if (m_hMemDc != NULL)
	{
		SelectObject(m_hMemDc, m_oldGDIObj);
		DeleteDC(m_hMemDc);
		m_hMemDc = NULL;
	}
	if (m_hBitmap != NULL)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	XMem::XDELETE(m_bgAlphaPbo);
	m_isBGAlphaInited = false;
}
#endif
void XFrameWork::updateScreen(bool withEngin)				//更新屏幕的内容
{
	if (withEngin)
		engineDraw();
	end2DDraw();
#ifdef GET_ALL_PIXEL
	static int maxPixel = 0;
	if (XEE_AllPixel > maxPixel)
	{
		maxPixel = XEE_AllPixel;
		printf("All pixel is %d\n", XEE_AllPixel);
	}
#endif
	if (m_windowData.withAlphaBG)
	{
#if BACKGROUND_ALPHA_MODE == 1
#ifdef ULW_ALPHA
//		m_bgAlphaFbo->removeFBO();
		//这里R和B要反过来颜色才正确
		//方案1、直接读取像素数据通用，但是效率较慢
	//	glReadPixels(0,0,m_windowWidth,m_windowHeight,GL_BGRA,GL_UNSIGNED_BYTE,m_pBits);	//这一句性能消耗很大，需要优化掉
		//方案2、使用fbo，太复杂
//		XGL::EnableTexture2D();
//		XGL::BindTexture2D(bgAlphaFbo.getTexture(0),0);
//		glGetTexImage(GL_TEXTURE_2D,0,GL_BGRA,GL_UNSIGNED_BYTE,pBits);
		//方案3、使用PBO，xp下面好像有问题
		if (m_bgAlphaPbo->getPixel((unsigned char *)m_pBits))
		{
			UpdateLayeredWindow(m_hwnd, m_wHDC, NULL, &m_sizeDest, m_hMemDc, &m_ptSrc, 0, &m_blf, ULW_ALPHA);	//这里设置黑色作为透明色 ULW_ALPHA = 2
			SwapBuffers(m_wHDC);
		}
//		bgAlphaFbo.useFBO();
		//unsigned char *tmp = XWindow.getWindowBuff();
	//	if(!BitBlt(hMemDc,0,0,windowWidth,windowHeight,wHDC,0,0,SRCCOPY))
	//	{
	//		printf("error!\n");
	//	}
	//	SelectObject(hMemDc,hOldBitmap);
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//如果是XP请使用下面的方法
		//glReadPixels(0,0,m_windowWidth,m_windowHeight,GL_BGRA,GL_UNSIGNED_BYTE,m_pBits);
		//UpdateLayeredWindow(m_hwnd,m_wHDC,NULL,&m_sizeDest,m_hMemDc,&m_ptSrc,0,&m_blf,ULW_ALPHA);
		//-------------------------------------------------------------
#endif
#endif
	}
	else
	{
#if CREATE_WINDOW_METHOD == 2
		XWindow.update();		//上下两句是等效的
#else
		SwapBuffers(m_wHDC);
#endif
	}
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
#if AUDIO_MATHOD == 0
	XCurSndCore.setVolume(-1, m_windowData.FWVolume * 0.01f * 128.0f);	//好像没有生效
#endif
#if AUDIO_MATHOD == 1
	XCurSndCore.setVolume(nullptr, m_windowData.FWVolume * 0.01f * 128.0f);	//好像没有生效
#endif
	initGL();
	XGL::showGLInfoToLog();	//显示GL的相关信息
	//初始化第二个上下文
	wglMakeCurrent(m_wHDC, m_wCopyHGLRC);
	initGL();
	wglMakeCurrent(m_wHDC, m_wCurHGLRC);	//恢复上下文
	//2014.4.14这里修改为显示自己的光标
#ifdef WITHXEE_CURSOR
	XWindow.setCurcor(false);
#else
	if (!m_windowData.isShowCursor) XWindow.setCurcor(false);	//隐藏光标
#endif
	//if(screen != NULL)
	//{
	//	XEngineLogo EngineLogo;
	//	XResPack.setOutFileName("XEngine.log");
	//	EngineLogo.init(XVec2(m_windowData.w * 0.5 - 256 + 160,
	//		m_windowData.h * 0.5 - 256 + 128 + 96),RES_LOCAL_PACK);
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
	switch (m_windowData.windowType)
	{
	case WINDOW_TYPE_EX:
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glLoadIdentity();								//变换坐标系函数
		glViewport(0, 0, m_windowData.w, m_windowData.h);
		switch (m_windowData.mode)
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
			if (m_sceneScaleX <= m_sceneScaleY)
			{
				m_sceneWidth = m_windowData.sceneW;
				m_sceneHeight = m_windowData.h * m_sceneScaleX;
				m_scenePosX = m_windowData.positionX;
				m_scenePosY = ((m_windowData.sceneH - m_sceneHeight) >> 1) + m_windowData.positionY;
			}
			else
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
			if (m_sceneScaleX >= m_sceneScaleY)
			{
				m_sceneWidth = m_windowData.sceneW;
				m_sceneHeight = m_windowData.h * m_sceneScaleX;
				m_scenePosX = m_windowData.positionX;
				m_scenePosY = ((m_windowData.sceneH - m_sceneHeight) >> 1) + m_windowData.positionY;
			}
			else
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
		switch (m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			switch (m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_scenePosX, m_sceneWidth + m_scenePosX,
					m_sceneHeight + m_scenePosY, m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_sceneWidth + m_scenePosX, m_scenePosX,
					m_sceneHeight + m_scenePosY, m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_scenePosX, m_sceneWidth + m_scenePosX,
					m_scenePosY, m_sceneHeight + m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_sceneWidth + m_scenePosX, m_scenePosX,
					m_scenePosY, m_sceneHeight + m_scenePosY, -1, 1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_90:
			switch (m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_sceneHeight + m_scenePosY, m_scenePosY,
					m_scenePosX, m_sceneWidth + m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_sceneHeight + m_scenePosY, m_scenePosY,
					m_sceneWidth + m_scenePosX, m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_scenePosY, m_sceneHeight + m_scenePosY,
					m_scenePosX, m_sceneWidth + m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_scenePosY, m_sceneHeight + m_scenePosY,
					m_sceneWidth + m_scenePosX, m_scenePosX, -1, 1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180:
			switch (m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_sceneWidth + m_scenePosX, m_scenePosX,
					m_scenePosY, m_sceneHeight + m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_scenePosX, m_sceneWidth + m_scenePosX,
					m_scenePosY, m_sceneHeight + m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_sceneWidth + m_scenePosX, m_scenePosX,
					m_sceneHeight + m_scenePosY, m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_scenePosX, m_sceneWidth + m_scenePosX,
					m_sceneHeight + m_scenePosY, m_scenePosY, -1, 1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_270:
			switch (m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_scenePosY, m_sceneHeight + m_scenePosY,
					m_sceneWidth + m_scenePosX, m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_scenePosY, m_sceneHeight + m_scenePosY,
					m_scenePosX, m_sceneWidth + m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_sceneHeight + m_scenePosY, m_scenePosY,
					m_sceneWidth + m_scenePosX, m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_sceneHeight + m_scenePosY, m_scenePosY,
					m_scenePosX, m_sceneWidth + m_scenePosX, -1, 1);
				break;
			}
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		switch (m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90:
			switch (m_windowData.mode)
			{
			case WINDOW_SIZE_MODE_CLIP_LP:
				glTranslatef(m_windowData.h, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case WINDOW_SIZE_MODE_CLIP_MID:
			case WINDOW_SIZE_MODE_CLIP_RESIZE:
			case WINDOW_SIZE_MODE_RESIZE_CLIP:
			case WINDOW_SIZE_MODE_RESIZE:
				glTranslatef(m_windowData.sceneH, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180: break;	//do nothing
		case WINDOW_ROTATE_MODE_270:
			switch (m_windowData.mode)
			{
			case WINDOW_SIZE_MODE_CLIP_LP:
				glTranslatef(m_windowData.h, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case WINDOW_SIZE_MODE_CLIP_MID:
			case WINDOW_SIZE_MODE_CLIP_RESIZE:
			case WINDOW_SIZE_MODE_RESIZE_CLIP:
			case WINDOW_SIZE_MODE_RESIZE:
				glTranslatef(m_windowData.sceneH, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				break;
			}
			break;
		}
		break;
	case WINDOW_TYPE_NORMAL:
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glLoadIdentity();								//变换坐标系函数
		glViewport(0, 0, m_windowData.w, m_windowData.h);
		glOrtho(0.0, m_windowData.w, m_windowData.h, 0.0, -1.0, 1.0);					//only for 2D的原始数据
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
		gluPerspective(45.0f, (float)(m_windowData.w) / (float)(m_windowData.h), 1.0f, 10000.0f);		//最后两个数值需要与之前的吻合，不然会出现模型破损的问题
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
	if (m_windowData.withAlphaBG) m_defBGColor.set(0.0f, 0.0f);	//如果是透明背景的话，这列设置为透明
	glClearColor(m_defBGColor.r, m_defBGColor.g, m_defBGColor.b, m_defBGColor.a);				//清除颜色
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);//建议：优化掉这一步，不用每次都调用
	if (m_isMultiSampleSupported)
	{
		glEnable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
		glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
		if (m_isLineSmooth) glEnable(GL_LINE_SMOOTH);
	}
	//3D
	glClearDepth(1.0f);									//Depth Buffer Setup
	glClearStencil(0);
	//	glEnable(GL_DEPTH);	
	glEnable(GL_DEPTH_TEST);							//Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								//The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//Really Nice Perspective Calculations
	XGL::EnableTexture2D();							// Enable 2D Texture Mapping
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping

	glAlphaFunc(GL_GREATER, 0.1f);						//允许进行alpha测试
	glEnable(GL_ALPHA_TEST);
	//下面的代码是为了解决背光面的问题
	glFrontFace(GL_CCW);  //设置正面的判断依据
	glEnable(GL_CULL_FACE);//剔除不必要的背面渲染 ，以便于提升效率
	glCullFace(GL_BACK);

	glEnable(GL_NORMALIZE);
	//+++++++++++++++++++++++++++++
	//注意这里，颜色与法线贴图有冲突
//	glEnable(GL_COLOR_MATERIAL);	//开启这里会造成法线贴图颜色太暗的问题,不开启这个会造成颜色不生效
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//-----------------------------
//	glMaterialfv(GL_FRONT,GL_SPECULAR,XVec4(1.0f,1.0f,1.0f,1.0f));
//	glMaterialf(GL_FRONT,GL_SHININESS,128.0f);

	//GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, XFColor::white);//设置全局环境光
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE); // 将视点设置为局部视点
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glShadeModel(GL_SMOOTH);		//启动反走样，这个好像没有生效
	return XTrue;
}
#pragma comment(lib,"glu32.lib")
void XFrameWork::initGlew()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != 0)
	{//初始化失败
		LogStr("Glew init error!");
		m_isOpenGL2Supported = XFalse;
	}
	else
	{
		//记录显卡的最大贴图尺寸
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);
		//记录最大同时贴图四处
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxTextureSum);
		//检查是否支持FBO
		m_isFboSupported = glewIsSupported("GL_ARB_framebuffer_object");
		if (!m_isFboSupported) LogStr("FBO is not supported.");
		//else LogStr("FBO is supported.");
		//检查硬件是否支持OpenGL2.0
		if (glewIsSupported("GL_VERSION_2_0"))
		{
			//LogStr("Ready for OpenGL 2.0");
			m_isOpenGL2Supported = XTrue;
		}
		else
		{
			LogStr("OpenGL 2.0 not supported");
			m_isOpenGL2Supported = XFalse;
		}
		//开启多重采样抗锯齿
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_MULTISAMPLE);//，初始化的时候开启；2）
		GLint bufSum = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufSum);
		glGetIntegerv(GL_SAMPLES, &m_multiSampleSum);//可以检查多重采样是否可用，前者返回1，后者返回大于1表示可用; 3）
		if (bufSum == 1 && m_multiSampleSum > 1)
		{
			m_isMultiSampleSupported = XTrue;
			glEnable(GL_MULTISAMPLE);	//这里生效需要在显卡的控制面板中设置为最佳图像质量
			//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);	//开启这里会造成alpha混合非常难看
			//glSampleCoverage(0.9f, GL_TRUE);
		}
		else
			m_isMultiSampleSupported = XFalse;
	}
}
XBool XFrameWork::initOpenGL2D()
{
	initWindowMatrix();	//初始化矩阵
	initGlew();			//初始化glew
	glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	if (m_windowData.withAlphaBG) m_defBGColor.set(0.0f, 0.0f);	//如果是透明背景的话，这列设置为透明
	glClearColor(m_defBGColor.r, m_defBGColor.g, m_defBGColor.b, m_defBGColor.a);			//清除颜色
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);//建议：优化掉这一步，不用每次都调用
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	if (m_isMultiSampleSupported)
	{
		glEnable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
		glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
		if (m_isLineSmooth) glEnable(GL_LINE_SMOOTH);
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
		if (tmpEvent.isKeyBoardDown())
		{
			switch (tmpEvent.keyValue)
			{
			case XKEY_ESCAPE: ret = 1; break;
			}
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
#if WITH_LOG
					LogRelease();
#endif
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
#if WITH_LOG
					LogRelease();
#endif
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
bool XFrameWork::isProgramExist(const std::string& name)
{
	m_programPropName = name;
	HWND oldHWnd = NULL;
	EnumWindows(EnumWndProc, (LPARAM)&oldHWnd);	//枚举所有运行的窗口     
	if (oldHWnd != NULL)
	{
		ShowWindow(oldHWnd, SW_SHOWNORMAL);		//激活找到的前一个程序     
		SetForegroundWindow(oldHWnd);			//把它设为前景窗口     
		return true;							//退出本次运行     
	}
	SetProp(m_hwnd, m_programPropName.c_str(), m_programPropValue);
	return false;
}
#endif
//定义自己的消息处理函数
//将消息处理函数下传
LONG_PTR currentWndProc = NULL;
static LRESULT CALLBACK winMsgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (gFrameworkData.pFrameWork == NULL)
		return CallWindowProc((WNDPROC)currentWndProc, hwnd, Msg, wParam, lParam);
	bool end = false;
	//++++++++++++++++++++++++++++++++++++
	//窗口和全屏切换的一个测试
	switch (Msg)
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
			ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
			WINDOWPLACEMENT state;
			state.length = sizeof(WINDOWPLACEMENT);
			state.flags = 0;
			state.showCmd = SW_SHOWNORMAL;
			state.rcNormalPosition.left = 0;
			state.rcNormalPosition.top = 0;
			state.rcNormalPosition.right = getWindowWidth();
			state.rcNormalPosition.bottom = getWindowHeight();
			SetWindowPlacement(hwnd, &state);
			//	printf("激活\n");
		}
		else
		{//最小化窗口，回复分辨率
			ChangeDisplaySettings(0, CDS_RESET);

			WINDOWPLACEMENT state;
			state.length = sizeof(WINDOWPLACEMENT);
			state.showCmd = SW_MINIMIZE;
			SetWindowPlacement(hwnd, &state);
			//	printf("非激活\n");
		}
		break;
#if _WIN32_WINNT >= 0x0501 && WITH_TOUCH_SUPPLY == 1
	case WM_TOUCH:
		int numInputs = (int)wParam;
		TOUCHINPUT ti[256];
		if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs > 256 ? 256 : numInputs, ti, sizeof(TOUCHINPUT)))
		{
			for (unsigned int i = 0; i < numInputs; ++i)
			{
				XEG.touchProc(ti[i]);
			}
		}
		CloseTouchInputHandle((HTOUCHINPUT)lParam);
		break;
		//return (0);
#endif
	}
	//-----------------------------------------------------
	LRESULT ret;
	if (gFrameworkData.pFrameWork->getIsProjectAsDaemon())
	{
		ret = gFrameworkData.pFrameWork->daemonProc(hwnd, Msg, wParam, lParam, end);
		if (end) return ret;
	}
	if (gFrameworkData.pFrameWork->m_customWinProc != NULL)
	{
		ret = gFrameworkData.pFrameWork->m_customWinProc(hwnd, Msg, wParam, lParam, end);
		if (end) return ret;
	}
	//下面处理输入法事件
	ret = XImm::inputMsgProc(hwnd, Msg, wParam, lParam, end);
	if (end) return ret;
	return CallWindowProc((WNDPROC)currentWndProc, hwnd, Msg, wParam, lParam);
}
void XFrameWork::touchProc(const TOUCHINPUT& ti)
{
	XInputEvent tmp;
	tmp.type = EVENT_TOUCH;
	tmp.touchID = ti.dwID;
	XVec2 tmpPos(ti.x * 0.01f, ti.y * 0.01f);
	tmpPos = getClientPos(tmpPos);
	m_touchPosition = getMousePosFromWindow(tmpPos.x, tmpPos.y);
	tmp.mouseX = tmpPos.x;
	tmp.mouseY = tmpPos.y;
	if (ti.dwFlags & TOUCHEVENTF_DOWN)
	{
		tmp.touchState = TOUCH_DOWN;
		gFrameworkData.pFrameWork->inputEvent(tmp);
		return;
	}
	if (ti.dwFlags & TOUCHEVENTF_MOVE)
	{
		tmp.touchState = TOUCH_MOVE;
		gFrameworkData.pFrameWork->inputEvent(tmp);
		return;
	}
	if (ti.dwFlags & TOUCHEVENTF_UP)
	{
		tmp.touchState = TOUCH_UP;
		gFrameworkData.pFrameWork->inputEvent(tmp);
		return;
	}
}
//--------------------------------------
static void registerMyMsgProc()
{
	if (gFrameworkData.pFrameWork == NULL) return;
	//DragAcceptFiles (wHandle, TRUE);	//是否允许向窗口拖拽文件
	//方案1
	//currentWndProc = (WNDPROC)SetWindowLongPtr(wHandle, GWL_WNDPROC, (long)winMsgProc);
	//方案2
	currentWndProc = GetWindowLongPtr(gFrameworkData.pFrameWork->getHWND(), GWLP_WNDPROC);
	SetWindowLongPtr(gFrameworkData.pFrameWork->getHWND(), GWLP_WNDPROC, (LONG_PTR)winMsgProc);
}
bool XFrameWork::createWindow(int width, int height, const char *windowTitle, bool isFullScreen, bool withFrame)
{
	if (!XWindow.createWindow(width, height, windowTitle, isFullScreen, withFrame)) return false;
	m_windowWidth = width;
	m_windowHeight = height;
	//初始化一些变量
	m_wHDC = wglGetCurrentDC();
	m_wCurHGLRC = wglGetCurrentContext();
	m_wCopyHGLRC = wglCreateContext(m_wHDC);
	//wglCopyContext(m_wCurHGLRC,m_wCopyHGLRC,GL_ALL_ATTRIB_BITS);
	wglShareLists(m_wCurHGLRC, m_wCopyHGLRC);
	//	m_hwnd = GetActiveWindow();
	m_hwnd = WindowFromDC(m_wHDC);
	m_mainThreadHandle = GetCurrentThreadId();
#if CREATE_WINDOW_METHOD == 2
	if (!withFrame)
	{
		//下面设置窗口无边框模式
		XVec2 pos;
		if (m_windowData.isInitWindowPos)
			pos.set(m_windowData.windowPosX, m_windowData.windowPosY);
		else
			pos = getWindowPos();
		DWORD style = GetWindowLong(m_hwnd, GWL_STYLE);
		DWORD windowstyle =
			(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		style &= ~windowstyle;
		style |= WS_POPUP;
		SetWindowLong(m_hwnd, GWL_STYLE, style);
		if(m_windowData.isAlwaysTop)
			SetWindowPos(m_hwnd, HWND_TOPMOST, pos.x, pos.y, width, height, SWP_SHOWWINDOW);
		else
			SetWindowPos(m_hwnd, HWND_TOP, pos.x, pos.y, width, height, SWP_SHOWWINDOW);
	}
	else
	{
		if (m_windowData.isInitWindowPos
			|| m_windowData.isAlwaysTop) setWindowPos(m_windowData.windowPosX, m_windowData.windowPosY, m_windowData.isAlwaysTop);
	}
#else
	if (windowTitle != NULL) XWindow.setWindowTitle(windowTitle);	//设置窗口标题
	if (m_windowData.isInitWindowPos
		|| m_windowData.isAlwaysTop) setWindowPos(m_windowData.windowPosX, m_windowData.windowPosY, m_windowData.isAlwaysTop);
#endif

	registerMyMsgProc();
#if _WIN32_WINNT >= 0x0501 && WITH_TOUCH_SUPPLY == 1
	//RegisterTouchWindow(m_hwnd, TWF_WANTPALM);
	RegisterTouchWindow(m_hwnd, 0);
#endif
	XCurSndCore.init();
	return true;
}
void XFrameWork::setStyle(XColorStyle style)
{
	switch (style)
	{
	case COLOR_STYLE_NORMAL:
		XCCS::lightBGColor.set(0.9f, 1.0f);
		XCCS::onColor.set(0.85f, 1.0f);
		XCCS::blackOnColor.set(0.8f, 1.0f);
		XCCS::normalColor.set(0.75f, 1.0f);
		XCCS::blackNormalColor.set(0.7f, 1.0f);
		XCCS::lightSpecialColor.set(0.65f, 1.0f);
		XCCS::specialColor.set(0.6f, 1.0f);
		XCCS::downColor.set(0.5f, 1.0f);
		XCCS::lightMouseColor.set(0.45f, 1.0f);
		XCCS::mouseColor.set(0.4f, 1.0f);
		XCCS::disableColor.set(0.35f, 1.0f);
		XCCS::blackDownColor.set(0.25f, 1.0f);
		XCCS::lightBlackColor.set(0.2f, 1.0f);
		XCCS::blackColor.set(0.1f, 1.0f);
		XCCS::darkColor.set(0.0f, 1.0f);
		setBGColor(XFColor(0.25f, 1.0f));//默认的背景颜色
		m_systemFont->setColor(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	case COLOR_STYLE_LIGHT:
		XCCS::lightBGColor.set(0.9f, 1.0f);
		XCCS::onColor.set(0.875f, 1.0f);
		XCCS::blackOnColor.set(0.85f, 1.0f);
		XCCS::normalColor.set(0.825f, 1.0f);
		XCCS::blackNormalColor.set(0.8f, 1.0f);
		XCCS::lightSpecialColor.set(0.775f, 1.0f);
		XCCS::specialColor.set(0.75f, 1.0f);
		XCCS::downColor.set(0.725f, 1.0f);
		XCCS::lightMouseColor.set(0.7f, 1.0f);
		XCCS::mouseColor.set(0.675f, 1.0f);
		XCCS::disableColor.set(0.65f, 1.0f);
		XCCS::blackDownColor.set(0.625f, 1.0f);
		XCCS::lightBlackColor.set(0.60f, 1.0f);
		XCCS::blackColor.set(0.575f, 1.0f);
		XCCS::darkColor.set(0.55f, 1.0f);
		setBGColor(XFColor(0.95f, 1.0f));//默认的背景颜色
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
LRESULT XFrameWork::daemonProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, bool &end)
{
	end = false;
	static UINT WM_TASKBARCREATED = 0;
	static bool isHide = false;
	static bool isHideConsole = false;
	static bool isHideWindows = false;
	static HMENU hmenu;
	static bool isInited = false;
	if (!isInited)
	{//初始化所有数据
		isInited = true;
		WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
		m_nid.cbSize = sizeof(m_nid);
		m_nid.hWnd = hwnd;
		m_nid.uID = 0;
		m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_nid.uCallbackMessage = WM_USER;
		m_nid.hIcon = LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDI_ICON1);   //IDI_APPLICATION
		lstrcpy(m_nid.szTip, TEXT(m_windowData.windowTitle.c_str()));
		Shell_NotifyIcon(NIM_ADD, &m_nid);
		hmenu = CreatePopupMenu();
		AppendMenu(hmenu, MF_STRING, IDR_HIDE, TEXT("隐藏窗口"));
		CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_CHECKED);
		if (m_deamonWithConsole)
		{
			AppendMenu(hmenu, MF_STRING, IDR_HIDECONSOLE, TEXT("隐藏控制台"));
			CheckMenuItem(hmenu, IDR_HIDECONSOLE, MF_BYCOMMAND | MF_CHECKED);
		}
		AppendMenu(hmenu, MF_STRING, IDR_QUIT, TEXT("退出"));
		//这里设置为第一次调用的时候隐藏窗口
		ShowWindow(hwnd, SW_HIDE);
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		isHide = true;
	}
	switch (Msg)
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
		if (lParam == WM_RBUTTONDOWN)
		{
			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hwnd);
			int menuState = TrackPopupMenu(hmenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, NULL);
			switch (menuState)
			{
			case IDR_HIDE:
				isHideWindows = !isHideWindows;
				if (!isHideWindows)
				{
					ShowWindow(hwnd, SW_HIDE);
					CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_CHECKED);
				}
				else
				{
					ShowWindow(hwnd, SW_NORMAL);
					CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_UNCHECKED);
				}
				break;
			case IDR_HIDECONSOLE:
				isHideConsole = !isHideConsole;
				if (!isHideConsole)
				{
					ShowWindow(GetConsoleWindow(), SW_HIDE);
					CheckMenuItem(hmenu, IDR_HIDECONSOLE, MF_BYCOMMAND | MF_CHECKED);
				}
				else
				{
					ShowWindow(GetConsoleWindow(), SW_NORMAL);
					CheckMenuItem(hmenu, IDR_HIDECONSOLE, MF_BYCOMMAND | MF_UNCHECKED);
				}
				break;
			case IDR_QUIT:
				SendMessage(hwnd, WM_CLOSE, wParam, lParam);
				releaseIcon();
				break;
			case 0:
				PostMessage(hwnd, Msg, NULL, NULL);
				break;
			default:
				PostMessage(hwnd, Msg, wParam, lParam);
				break;
			}
		}
		if (lParam == WM_LBUTTONDBLCLK)
		{//双击处理
			SetForegroundWindow(hwnd);
			isHideWindows = !isHideWindows;
			if (!isHideWindows)
			{
				ShowWindow(hwnd, SW_HIDE);
				CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_CHECKED);
			}
			else
			{
				ShowWindow(hwnd, SW_NORMAL);
				CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_UNCHECKED);
			}
		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
		releaseIcon();
		break;
	default:
		if (Msg == WM_TASKBARCREATED)
		{
			isHide = false;
			SendMessage(hwnd, WM_PAINT, wParam, lParam);
		}
		break;
	}
	return 0;
}
XBool XFrameWork::setSystemFont(const char * filename)
{
	if (m_haveSystemFont) return XFalse;
	if (!m_systemFont->initFromTTF(filename, m_windowData.systemFontSize, RES_LOCAL_FOLDER, false))
	{
		LogStr("系统字体加载失败!");
		return XFalse;
	}
	m_systemFont->setPosition(getSceneW() >> 1, getSceneH() >> 1);
	m_systemFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_systemFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_systemFont->setColor(XFColor(0.75f, 1.0f));
	//m_systemFont->setDistance(-2.0f);	//这样会更好看，但是目前存在bug，会造成XEdit的选择计算出现问题
	m_haveSystemFont = XTrue;
	if (m_windowData.isShowVersion)
	{
		m_showVersionFont->setACopy(*m_systemFont);
		m_showVersionFont->setPosition(XVec2::zero);
		m_showVersionFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_showVersionFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		m_showVersionFont->setColor(XFColor(0.75f, 1.0f));
		m_showVersionFont->setString(m_windowData.versionString.c_str());
	}

	return XTrue;
}
void XFrameWork::toggleFullScreen()
{
	if (m_windowData.isFullScreen) return;
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
	}
	else
	{//设置为窗口
		if (ChangeDisplaySettings(0, CDS_RESET) == DISP_CHANGE_SUCCESSFUL)
		{
			if (state == 0)
			{//没有初始数据
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);	//获取边框的数据
				SetWindowLongPtr(getHWND(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
				MoveWindow(getHWND(), 0, 0, getWinW() + rect.right - rect.left, getWinH() + rect.bottom - rect.top, TRUE);
			}
			else
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
	int buffSize;
	unsigned char *data;
	unsigned char *tmpData;	//用于数据反转
	XScreenShotData()
		:data(NULL)
		, tmpData(NULL)
		, buffSize(0)
	{}
	~XScreenShotData()
	{
		XMem::XDELETE_ARRAY(data);
		XMem::XDELETE_ARRAY(tmpData);
	}
	bool createMem()
	{
		if (buffSize >= w * h * 3) return true;
		buffSize = w * h * 3;
		XMem::XDELETE_ARRAY(data);
		data = XMem::createArrayMem<unsigned char>(buffSize);
		if (data == NULL)
		{
			buffSize = 0;
			return false;
		}
		XMem::XDELETE_ARRAY(tmpData);
		tmpData = XMem::createArrayMem<unsigned char>(buffSize);
		if (tmpData == NULL)
		{
			XMem::XDELETE_ARRAY(data);
			buffSize = 0;
			return false;
		}
		return true;
	}
	void top2Down()	//上下翻转像素
	{
		unsigned char *pd = tmpData + (w * h - w) * 3;
		unsigned char *ps = data;
		int sum = w * 3;
		for (int i = 0; i < h; ++i)	//上下翻转
		{
			memcpy(pd, ps, sum);
			ps += sum;
			pd -= sum;
		}
	}
};
XScreenShotData *gScreenShotData = NULL;	//注意这个数据没有最终被释放掉
bool gIsScreenShotOver = true;
//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入
#ifdef XEE_OS_WINDOWS
DWORD WINAPI saveDataToPNG(void *)
#endif
#ifdef XEE_OS_LINUX
static void *saveDataToPNG(void * pParam)
#endif
{
	if (gScreenShotData == NULL || gScreenShotData->data == NULL)
	{
#ifdef XEE_OS_WINDOWS
		return 1;
#endif
#ifdef XEE_OS_LINUX
		return;
#endif
	}
	//将数据保存成图片
	gScreenShotData->top2Down();
	//	printf("%d\n",getCurrentTicks());
	XPng::savePNG(gScreenShotData->fileName, gScreenShotData->tmpData,
		gScreenShotData->w, gScreenShotData->h, COLOR_RGB, Z_DEFAULT_COMPRESSION);
	//	printf("%d\n",getCurrentTicks());
	gIsScreenShotOver = true;
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
bool XFrameWork::addABaby(XBabyBase *baby,int resetTimer)
{
	if(!m_windowData.withCaregiver || m_pCaregiver == NULL) return false;
	return m_pCaregiver->addABaby(baby,resetTimer);
}
//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入
XBool XFrameWork::gameShot(const char *fileName, int w, int h, bool withThread, float x, float y)
{
	if (fileName == NULL) return XFalse;
	if (x < 0.0f || y < 0.0f) return XFalse;
	if (w <= 0 || h <= 0) return XFalse;
	if (!gIsScreenShotOver) return XFalse;
	gIsScreenShotOver = false;
	if (gScreenShotData == NULL)
	{
		gScreenShotData = XMem::createMem<XScreenShotData>();
		if (gScreenShotData == NULL)
		{
			gIsScreenShotOver = true;
			return XFalse;
		}
	}

	gScreenShotData->h = h;
	gScreenShotData->w = w;
	strcpy_s(gScreenShotData->fileName, MAX_FILE_NAME_LENGTH, fileName);

	if (!gScreenShotData->createMem())
	{
		XMem::XDELETE(gScreenShotData);
		gIsScreenShotOver = true;
		return XFalse;
	}

	//读取数据
	//glReadPixels(0,0,w,h,GL_RGB,GL_BYTE,gScreenShotData->data);
	glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, gScreenShotData->data);	//注意尺寸4字节对齐，否则会造成crash
	if (withThread)
	{//优化之后直接开启一个线程来保存图片
#ifdef XEE_OS_WINDOWS
		DWORD tempThreadP;
		if (CreateThread(0, 0, saveDataToPNG, NULL, 0, &tempThreadP) == 0)
		{
			return XFalse;
		}
#endif
#ifdef XEE_OS_LINUX
		pthread_t tempThreadP;
		if (pthread_create(&tempThreadP, NULL, &(saveDataToPNG), NULL) != 0)
		{
			return XFalse;
		}
#endif
	}
	else
	{
		saveDataToPNG(NULL);
	}
	return XTrue;
}
const XFontUnicode& getDefaultFont()
{
	if (gFrameworkData.pFrameWork != NULL && gFrameworkData.pFrameWork->m_systemFont != NULL)
		return *gFrameworkData.pFrameWork->m_systemFont;
	else
	{
		LogStr("在使用默认字体时请先初始化框架!");
		return XFontUnicode(0);
	}
}
bool getIsImageSupport(const std::string& ext)
{
	return (XFile::fileNameCompare(ext.c_str(), "png") || 
		XFile::fileNameCompare(ext.c_str(), "bmp") || 
		XFile::fileNameCompare(ext.c_str(), "jpg") || 
		XFile::fileNameCompare(ext.c_str(), "tga") || 
		XFile::fileNameCompare(ext.c_str(), "tif") || 
		XFile::fileNameCompare(ext.c_str(), "jpeg") ||
		XFile::fileNameCompare(ext.c_str(), "gif"));
}
bool getIsSoundSupport(const std::string& ext)
{
	return (XFile::fileNameCompare(ext.c_str(), "mp3") ||
		XFile::fileNameCompare(ext.c_str(), "wav") ||
		XFile::fileNameCompare(ext.c_str(), "ogg"));
}
#if !WITH_INLINE_FILE
#include "XFrameWork.inl"
#endif
}