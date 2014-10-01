//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XEffeEngine.h"
#include "XEngineLogo.h"

_XBool readSystemConfigData(_XWindowData &data)
{//读取配置数据:策略修改：如果配置项不存在，则使用默认值
	int temp;
	TiXmlDocument doc(XEE_SYSTEM_CONFIG);
	if(!doc.LoadFile()) return XFalse;
	TiXmlNode *rootNode = NULL;
	rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return XFalse;
	getXmlAsInt(rootNode,"windowType",(int &)data.windowType);
	getXmlAsInt(rootNode,"w",data.w);
	getXmlAsInt(rootNode,"h",data.h);
	getXmlAsInt(rootNode,"sceneW",data.sceneW);
	getXmlAsInt(rootNode,"sceneH",data.sceneH);
	getXmlAsInt(rootNode,"positionX",data.positionX);
	getXmlAsInt(rootNode,"positionY",data.positionY);
	getXmlAsInt(rootNode,"isFullScreen",data.isFullScreen);
	getXmlAsInt(rootNode,"isTouch",data.isTouch);
	getXmlAsInt(rootNode,"withLog",data.withLog);
	getXmlAsInt(rootNode,"isShowCursor",data.isShowCursor);
	getXmlAsInt(rootNode,"withFrame",data.withFrame);
	getXmlAsInt(rootNode,"windowMode",(int &)data.mode);
	getXmlAsString(rootNode,"windowTitle",data.windowTitle);
	getXmlAsInt(rootNode,"rotateMode",(int &)data.rotateMode);
	getXmlAsInt(rootNode,"turnOverMode",(int &)data.turnOverMode);
	getXmlAsInt(rootNode,"CFGMode",(int &)data.CFGMode);
	getXmlAsBool(rootNode,"isStartDelay",data.isStartDelay);
	getXmlAsInt(rootNode,"startDelayTimer",data.startDelayTimer);
	getXmlAsBool(rootNode,"isShowVersion",data.isShowVersion);
	getXmlAsString(rootNode,"versionString",data.versionString);
	getXmlAsBool(rootNode,"isAutoShutDown",data.isAutoShutDown);
	getXmlAsInt(rootNode,"shutDownTimer_year",data.autoShutDownYear);
	getXmlAsInt(rootNode,"shutDownTimer_month",data.autoShutDownMonth);
	getXmlAsInt(rootNode,"shutDownTimer_day",data.autoShutDownDay);
	getXmlAsInt(rootNode,"shutDownTimer_hour",data.autoShutDownHour);
	getXmlAsInt(rootNode,"shutDownTimer_minute",data.autoShutDownMinute);
	getXmlAsInt(rootNode,"shutDownTimer_second",data.autoShutDownSecond);
	getXmlAsBool(rootNode,"isShowConsole",data.isShowConsole);
	getXmlAsString(rootNode,"fontFilename",data.systemFontFile);
	getXmlAsString(rootNode,"commonResource",data.commonResourcePos);
	if(getXmlAsInt(rootNode,"defaultResourcePosition",temp) != NULL)
	{
		switch(temp)
		{
		case RESOURCE_LOCAL_FOLDER:
		case RESOURCE_LOCAL_PACK:
		case RESOURCE_WEB:
			data.defResourcePos = (_XResourcePosition)temp;
			break;
		}//其他情况不改变资源位置的设置
	}
	getXmlAsInt(rootNode,"backgroundValume",data.BGVolume);
	getXmlAsInt(rootNode,"formwordValume",data.FWVolume);
	getXmlAsBool(rootNode,"isInitWindowPos",data.isInitWindowPos);
	getXmlAsInt(rootNode,"windowPosX",data.windowPosX);
	getXmlAsInt(rootNode,"windowPosY",data.windowPosY);
	getXmlAsBool(rootNode,"isAlwaysTop",data.isAlwaysTop);
	getXmlAsBool(rootNode,"withCustomTitle",data.withCustomTitle);
	if(data.withFrame != 0 && data.withCustomTitle) data.withCustomTitle = false;
	if(data.withCustomTitle && data.positionY > -32) 
	{
		int tmpH = data.positionY + 32;
		data.positionY = -32;
		data.sceneH += tmpH;
		data.h += tmpH;
	}

	return XTrue;
}
_XBool saveSystemConfigData(const _XWindowData &data)
{//将配置信息保存到配置文件
	TiXmlDocument doc(XEE_SYSTEM_CONFIG);
	TiXmlDeclaration declaration("1.0","gb2312","yes");
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");
	if(!addComment(elmRoot,"窗口类型：0、普通窗口，1、可自适应、旋转和翻转的窗口，2、支持3D渲染的窗口")) return XFalse;
	if(!addLeafNode(elmRoot,"windowType",toString(data.windowType))) return XFalse;
	if(!addComment(elmRoot,"窗口宽度")) return XFalse;
	if(!addLeafNode(elmRoot,"w",toString(data.w))) return XFalse;
	if(!addComment(elmRoot,"窗口高度")) return XFalse;
	if(!addLeafNode(elmRoot,"h",toString(data.h))) return XFalse;
	if(!addComment(elmRoot,"场景宽度")) return XFalse;
	if(!addLeafNode(elmRoot,"sceneW",toString(data.sceneW))) return XFalse;
	if(!addComment(elmRoot,"场景高度")) return XFalse;
	if(!addLeafNode(elmRoot,"sceneH",toString(data.sceneH))) return XFalse;
	if(!addComment(elmRoot,"场景的x偏移")) return XFalse;
	if(!addLeafNode(elmRoot,"positionX",toString(data.positionX))) return XFalse;
	if(!addComment(elmRoot,"场景的y偏移")) return XFalse;
	if(!addLeafNode(elmRoot,"positionY",toString(data.positionY))) return XFalse;
	if(!addComment(elmRoot,"是否全屏：0、不全屏，1、全屏")) return XFalse;
	if(!addLeafNode(elmRoot,"isFullScreen",toString(data.isFullScreen))) return XFalse;
	if(!addComment(elmRoot,"是否支持触摸屏：0、不支持，1、支持")) return XFalse;
	if(!addLeafNode(elmRoot,"isTouch",toString(data.isTouch))) return XFalse;
	if(!addComment(elmRoot,"是否输出日志：0、不输出，1、输出")) return XFalse;
	if(!addLeafNode(elmRoot,"withLog",toString(data.withLog))) return XFalse;
	if(!addComment(elmRoot,"是否显示鼠标：0、不显示，1、显示")) return XFalse;
	if(!addLeafNode(elmRoot,"isShowCursor",toString(data.isShowCursor))) return XFalse;
	if(!addComment(elmRoot,"窗口是否有边框：0、没有边框，1、有边框")) return XFalse;
	if(!addLeafNode(elmRoot,"withFrame",toString(data.withFrame))) return XFalse;
	if(!addComment(elmRoot,"窗口模式：0、左上角对齐并裁剪，1、重点对齐并裁剪，2、短边拉伸长边裁剪，3、长边拉伸短边填空白，4、自动拉伸全屏")) return XFalse;
	if(!addLeafNode(elmRoot,"windowMode",toString(data.mode))) return XFalse;
	if(!addComment(elmRoot,"窗口标题")) return XFalse;
	if(!addLeafNode(elmRoot,"windowTitle",data.windowTitle)) return XFalse;
	if(!addComment(elmRoot,"窗口的旋转模式：0、不旋转，1、90旋转，2、180度旋转，3、270旋转")) return XFalse;
	if(!addLeafNode(elmRoot,"rotateMode",toString(data.rotateMode))) return XFalse;
	if(!addComment(elmRoot,"窗口的反转模式：0、不翻转，1、左右翻转，2、上下翻转，3、左右上下翻转")) return XFalse;
	if(!addLeafNode(elmRoot,"turnOverMode",toString(data.turnOverMode))) return XFalse;
	if(!addComment(elmRoot,"配置的工作模式：0、普通模式，1、服务器模式，2、客户端模式")) return XFalse;
	if(!addLeafNode(elmRoot,"CFGMode",toString(data.CFGMode))) return XFalse;
	if(!addComment(elmRoot,"是否开启启动延迟")) return XFalse;
	if(!addLeafNode(elmRoot,"isStartDelay",data.isStartDelay)) return XFalse;
	if(!addComment(elmRoot,"启动延迟的时间：单位ms")) return XFalse;
	if(!addLeafNode(elmRoot,"startDelayTimer",toString(data.startDelayTimer))) return XFalse;
	if(!addComment(elmRoot,"是否显示版本号")) return XFalse;
	if(!addLeafNode(elmRoot,"isShowVersion",data.isShowVersion)) return XFalse;
	if(!addComment(elmRoot,"版本号")) return XFalse;
	if(!addLeafNode(elmRoot,"versionString",data.versionString)) return XFalse;
	if(!addComment(elmRoot,"是否启动定时关机")) return XFalse;
	if(!addLeafNode(elmRoot,"isAutoShutDown",data.isAutoShutDown)) return XFalse;
	if(!addComment(elmRoot,"定时关机的时间.小于0表示这项无效")) return XFalse;
	if(!addLeafNode(elmRoot,"shutDownTimer_year",toString(data.autoShutDownYear))) return XFalse;
	if(!addLeafNode(elmRoot,"shutDownTimer_month",toString(data.autoShutDownMonth))) return XFalse;
	if(!addLeafNode(elmRoot,"shutDownTimer_day",toString(data.autoShutDownDay))) return XFalse;
	if(!addLeafNode(elmRoot,"shutDownTimer_hour",toString(data.autoShutDownHour))) return XFalse;
	if(!addLeafNode(elmRoot,"shutDownTimer_minut",toString(data.autoShutDownMinute))) return XFalse;
	if(!addLeafNode(elmRoot,"shutDownTimer_second",toString(data.autoShutDownSecond))) return XFalse;
	if(!addComment(elmRoot,"是否显示后台")) return XFalse;
	if(!addLeafNode(elmRoot,"isShowConsole",data.isShowConsole)) return XFalse;
	if(!addComment(elmRoot,"默认资源的位置：0、文件夹，1、资源包，2、网络资源，3、系统默认")) return XFalse;
	if(!addLeafNode(elmRoot,"defaultResourcePosition",toString(data.defResourcePos))) return XFalse;
	if(!addComment(elmRoot,"默认的字体文件")) return XFalse;
	if(!addLeafNode(elmRoot,"fontFilename",data.systemFontFile)) return XFalse;
	if(!addComment(elmRoot,"通用资源的位置")) return XFalse;
	if(!addLeafNode(elmRoot,"commonResource",data.commonResourcePos)) return XFalse;
	if(!addComment(elmRoot,"背景音乐的音量")) return XFalse;
	if(!addLeafNode(elmRoot,"backgroundValume",toString(data.BGVolume))) return XFalse;
	if(!addComment(elmRoot,"前景音乐的音量")) return XFalse;
	if(!addLeafNode(elmRoot,"formwordValume",toString(data.FWVolume))) return XFalse;
	if(!addComment(elmRoot,"是否设置窗口位置")) return XFalse;
	if(!addLeafNode(elmRoot,"isInitWindowPos",data.isInitWindowPos)) return XFalse;
	if(!addComment(elmRoot,"窗口位置X")) return XFalse;
	if(!addLeafNode(elmRoot,"windowPosX",toString(data.windowPosX))) return XFalse;
	if(!addComment(elmRoot,"窗口位置Y")) return XFalse;
	if(!addLeafNode(elmRoot,"windowPosY",toString(data.windowPosY))) return XFalse;
	if(!addComment(elmRoot,"是否置顶窗口")) return XFalse;
	if(!addLeafNode(elmRoot,"isAlwaysTop",data.isAlwaysTop)) return XFalse;
	if(!addComment(elmRoot,"是否使用自己的标题栏")) return XFalse;
	if(!addLeafNode(elmRoot,"withCustomTitle",data.withCustomTitle)) return XFalse;

	if(doc.InsertEndChild(elmRoot) == NULL) return XFalse;
	doc.SaveFile();
	return XTrue;
}
namespace XEE
{
	_XBool engineInit()
	{
		_XResourceManager::GetInstance();	//尝试解决析构函数的调用顺序问题，对于全局的变量这个顺序显然还不够
		XEE::defaultResourcePosition = XEE::windowData.defResourcePos;
		if(XEE::windowData.isStartDelay
			&& XEE::windowData.startDelayTimer > 0) Sleep(XEE::windowData.startDelayTimer);	//程序启动前延迟等待一段时间
		if(!XEE::windowData.isShowConsole) FreeConsole();	//如果需要则隐藏控制后台
		//初始化一些变量的值
		XEE::windowWidth = XEE::windowData.w;
		XEE::windowHeight = XEE::windowData.h;
		XEE::sceneX = XEE::windowData.positionX;
		XEE::sceneY = XEE::windowData.positionY;
		XEE::sceneWidth = XEE::windowData.sceneW;
		XEE::sceneHeight = XEE::windowData.sceneH;
		XEE::sceneSizeX = 1.0f;		//窗口的缩放比例
		XEE::sceneSizeY = 1.0f;		//窗口的缩放比例
		srand((unsigned)time(0)); //引入随机因子

		return XTrue;
	}
}
inline _XBool initGL()
{
	switch(XEE::windowData.windowType)
	{
	case WINDOW_TYPE_EX:
	case WINDOW_TYPE_NORMAL:
		if(!initOpenGL2D()) return XFalse;
		break;
	case WINDOW_TYPE_3D:
		if(!initOpenGL3D()) return XFalse;
		break;
	}
	return XTrue;
}
_XBool initWindowEx(const _XWindowData& windowData)
{
	//atexit(XEE::errProc);
	XEE::windowData = windowData;
	LogStr("程序开始!");
	if(!XEE::engineInit()) return XFalse;
	//if(XEE::windowData.w <= 0 || XEE::windowData.h <= 0) return XFalse;//检查输入参数是否正确
	//srand((unsigned)time(0)); //引入随机因子
	bool ret = false;
	switch(XEE::windowData.windowType)
	{
	case WINDOW_TYPE_EX:
		if(XEE::windowData.rotateMode == WINDOW_ROTATE_MODE_90
			|| XEE::windowData.rotateMode == WINDOW_ROTATE_MODE_270)
			ret = createWindow(XEE::windowData.h,XEE::windowData.w,
			XEE::windowData.windowTitle.c_str(),XEE::windowData.isFullScreen,XEE::windowData.withFrame);
		else ret = createWindow(XEE::windowData.w,XEE::windowData.h,
			XEE::windowData.windowTitle.c_str(),XEE::windowData.isFullScreen,XEE::windowData.withFrame);
		break;
	case WINDOW_TYPE_NORMAL:
		ret = createWindow(XEE::windowData.w,XEE::windowData.h,
			XEE::windowData.windowTitle.c_str(),XEE::windowData.isFullScreen,XEE::windowData.withFrame);
		break;
	case WINDOW_TYPE_3D:
		ret = createWindow(XEE::windowData.w,XEE::windowData.h,
			XEE::windowData.windowTitle.c_str(),XEE::windowData.isFullScreen,XEE::windowData.withFrame);
		break;
	}
	if(!ret) return XFalse;
	_XSoundHandle.setMusicVolume(XEE::windowData.BGVolume * 0.01f * 128.0f);
	_XSoundHandle.setVolume(-1,XEE::windowData.FWVolume * 0.01f * 128.0f);	//好像没有生效
	initGL();
	//初始化第二个上下文
	wglMakeCurrent(XEE::wHDC,XEE::wCopyHGLRC);
	initGL();
	wglMakeCurrent(XEE::wHDC,XEE::wCurrentHGLRC);	//恢复上下文
	//2014.4.14这里修改为显示自己的光标
#ifdef WITH_XEE_CURSOR
	_XWindow.setCurcor(false);
#else
	if(XEE::windowData.isShowCursor == 0) _XWindow.setCurcor(false);	//隐藏光标
#endif
	//if(XEE::screen != NULL)
	//{
	//	_XEngineLogo EngineLogo;
	//	_XResourcePack::GetInstance().setOutFileName("XEngine.log");
	//	EngineLogo.init(_XVector2(XEE::XEE::windowData.w * 0.5 - 256 + 160,
	//		XEE::XEE::windowData.h * 0.5 - 256 + 128 + 96),RESOURCE_LOCAL_PACK);
	//	_XResourcePack::GetInstance().setOutFileName(NULL);
	//	EngineLogo.reset();
	//	EngineLogo.setBackSize(XEE::XEE::windowData.w/8.0f,XEE::XEE::windowData.h/8.0f);
	//	int interval = 0;
	//	while(true) 
	//	{
	//		interval = XEE::engineIdle();
	//		EngineLogo.move(interval);
	//		EngineLogo.draw();
	//		if(EngineLogo.getIsEnd() != 0) break;
	//	}
	//	EngineLogo.release();
	//}
//	setSystemFont(XEE_SYSTEM_FONT_PATH);
//#if WITH_OBJECT_MANAGER
//	_XObjManger.init();
//#endif
	XEE::engineSysInit();
	//atexit(XEE::release);
	return XTrue;
}
namespace XEE
{
	void engineMove(int delay)
	{
		if(XEE::showVersionTimer < 61000)	//版本号显示一分钟
			XEE::showVersionTimer += delay;
		//下面是自动关机的控制
		if(XEE::windowData.isAutoShutDown)
		{//下面控制自动关机
			autoShutDownTimer += delay;
			if(autoShutDownTimer > 1000)
			{//一秒钟判断一次
				autoShutDownTimer = 0;
				_XSystemTime tempT;
				getTimeMs(tempT);
				if((tempT.year >= XEE::windowData.autoShutDownYear || XEE::windowData.autoShutDownYear < 0)
					&& (tempT.month >= XEE::windowData.autoShutDownMonth || XEE::windowData.autoShutDownMonth < 0)
					&& (tempT.day >= XEE::windowData.autoShutDownDay || XEE::windowData.autoShutDownDay < 0)
					&& (tempT.hour >= XEE::windowData.autoShutDownHour || XEE::windowData.autoShutDownHour < 0)
					&& (tempT.minute >= XEE::windowData.autoShutDownMinute || XEE::windowData.autoShutDownMinute < 0)
					&& (tempT.second >= XEE::windowData.autoShutDownSecond || XEE::windowData.autoShutDownSecond < 0))
				{//符合条件触发自动关机
					if(autoShutDownState == 0) shutDownSystem();
				}else
				{
					autoShutDownState = 0;
				}
			}
		}
		_XSoundHandle.update(delay);
		_XCtrlManger.update(delay);
#if WITH_OBJECT_MANAGER
		_XObjManger.move(delay);
#endif
		if(XEE::windowData.withCustomTitle) XEE::customWinTitle.move(delay);
#ifdef GET_ALL_PIXEL
		XEE_AllPixel = 0;
#endif
	}
	void (*drawAfterCtrl)() = NULL;
	int engineIdle()
	{
//		static _XTimer tempTimer;
//		static Uint32 current,last = 0,five = 0,nframes = 0;
//		current = tempTimer.getCurrentTime();
//		++nframes;
//		if(current - five > 5000) 
//		{//将帧率显示出来
//			XEE::nowFPS = nframes * 0.2f;
////#ifdef DEBUG_MODE
//			if(XEE::manualFPS > 0) printf("%.1f FPS@ %d\n",XEE::nowFPS,XEE::manualFPS);
//			else printf("%.1f FPS\n",XEE::nowFPS);
////#endif
//			nframes = 0;
//			five = current;
//		}
//		XEE::frameTime = current - last;
//		last = current;
		//上下两种方式在低帧率时差距较大，不知道为什么
		static Uint32 current,last = 0,five = 0,nframes = 0;
		current = getCurrentTicks();
		++nframes;
		if(current - five > 5000) 
		{//将帧率显示出来
			XEE::nowFPS = nframes * 0.2f;
//#ifdef DEBUG_MODE
			if(XEE::manualFPS > 0) LogNull("%.1f FPS@ %d",XEE::nowFPS,XEE::manualFPS);
			else LogNull("%.1f FPS",XEE::nowFPS);
//#endif
			nframes = 0;
			five = current;
		}	
		XEE::frameTime = current - last;
		last = current;
		//注意：这里需要2000才能保证控制的准确，不知道为什么
		if(XEE::manualFPS > 0 && XEE::frameTime < XEE::manualFrameTime)
		{
			//SDL_Delay(XEE_setFrameTime - XEE_frameTime);
			Sleep(XEE::manualFrameTime - XEE::frameTime);
		}
		if(_XErrorReporter::GetInstance().isErrored()
			&& !_XErrorReporter::GetInstance().getIsMainProc())
		{
			_XErrorReporter::GetInstance().setMainProc();
			_XErrorReporter::GetInstance().errorProc();
			exit(1);
		}
		engineMove(XEE::frameTime);

		return XEE::frameTime;	//返回两次调用之间的时间差
	}
}	
int suspendKeyProc()
{
	int ret = 0;
	_XInputEvent tmpEvent;
	while(getInputEvent(tmpEvent)) 
	{
		switch(tmpEvent.type)
		{
		case EVENT_KEYBOARD:
			if(tmpEvent.keyState == KEY_STATE_DOWN)
			{
				switch(tmpEvent.keyValue)
				{
				case XKEY_ESCAPE: ret = 1;break;
				}
			}
			break;
		}
	}
	return ret;
}
void suspend(const char * str)
{
	XEE::isSuspend = XTrue;
	if(XEE::haveSystemFont)
	{//有自己的字体
		if(str == NULL)
		{
			while(true)
			{
				if(suspendKeyProc() != 0) exit(1);
				XEE::clearScreen();
				XEE::updateScreen();
				Sleep(1);
			}
		}else
		{
		//	string tempStr = str;
		//	tempStr += "\n";
		//	LogStr(tempStr.c_str());
			LogStr(str);
			XEE::systemFont.setString(str);
			while(true)
			{
				if(suspendKeyProc() != 0) 
				{
#if WITH_LOG
					_XLogbook::GetInstance().releaseLog();
#endif
					exit(1);
				}
				XEE::clearScreen();
				XEE::systemFont.draw();
				XEE::updateScreen();
				Sleep(1);
			}
		}
	}else
	{//没有自己的字体
		if(str == NULL)
		{//啥也不干就是死锁
			while(true)
			{
				if(suspendKeyProc() != 0) exit(1);
				Sleep(1);
			}
		}else
		{//显示错误信息并进入死锁
		//	string tempStr = str;
		//	tempStr += "\n";
		//	LogStr(tempStr.c_str());
			LogStr(str);
			while(true) 
			{
				if(suspendKeyProc() != 0)
				{
#if WITH_LOG
					_XLogbook::GetInstance().releaseLog();
#endif
					exit(1);
				}
				Sleep(1);
			}
		}
	}
}
_XBool setSystemFont(const char * filename)
{
	if(XEE::haveSystemFont) return XFalse;
	if(!XEE::systemFont.initFromTTF(filename,30,RESOURCE_LOCAL_FOLDER,false))
	{
		LogStr("系统字体加载失败!");
		return XFalse;
	}
	XEE::systemFont.setPosition(XEE::windowWidth >> 1,XEE::windowHeight >> 1);
	XEE::systemFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	XEE::systemFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	XEE::systemFont.setColor(0.75f,0.75f,0.75f,1.0f);
	XEE::haveSystemFont = XTrue;
	if(XEE::windowData.isShowVersion)
	{
		XEE::showVersionStr.setACopy(XEE::systemFont);
		XEE::showVersionStr.setPosition(0.0f,0.0f);
		XEE::showVersionStr.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		XEE::showVersionStr.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		XEE::showVersionStr.setColor(0.75f,0.75f,0.75f,1.0f);
		XEE::showVersionStr.setString(XEE::windowData.versionString.c_str());
	}
	
	return XTrue;
}

#include <wincon.h>
#define IDR_HIDE         16
#define IDR_HIDECONSOLE  18
#define IDR_QUIT         20
#define IDI_ICON1 (101)
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();
//定义自己的消息处理函数
//将消息处理函数下传
static LRESULT daemonProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam,bool &end) 
{
	end = false;
	static NOTIFYICONDATA nid;			//当关闭控制台时会造成这里菜单没有正常关闭
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
		nid.cbSize = sizeof(nid);
		nid.hWnd = hwnd;
		nid.uID = 0;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = WM_USER;
		nid.hIcon = LoadIcon(GetModuleHandle(NULL),(LPCTSTR)IDI_ICON1);   //IDI_APPLICATION
		lstrcpy(nid.szTip, TEXT("xiajia"));
		Shell_NotifyIcon(NIM_ADD, &nid);
		hmenu = CreatePopupMenu();
		AppendMenu(hmenu,MF_STRING,IDR_HIDE,TEXT("隐藏窗口"));
		CheckMenuItem(hmenu,IDR_HIDE,MF_BYCOMMAND|MF_CHECKED);
		AppendMenu(hmenu,MF_STRING,IDR_HIDECONSOLE,TEXT("隐藏控制台"));
		CheckMenuItem(hmenu,IDR_HIDECONSOLE,MF_BYCOMMAND|MF_CHECKED);
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
				Shell_NotifyIcon(NIM_DELETE,&nid);
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
		Shell_NotifyIcon(NIM_DELETE, &nid);	//资源释放
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
//void winMsgRelease()
//{
//	if(XEE::customWinProc == daemonProc)
//	{
//		Shell_NotifyIcon(NIM_DELETE, &nid);	//资源释放
//	}
//}
void setProjectAsDaemon()
{
	XEE::customWinProc = daemonProc;
	//atexit(winMsgRelease);
}
