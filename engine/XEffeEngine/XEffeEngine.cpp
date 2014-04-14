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
	TiXmlNode *keyNode = NULL;
	keyNode = rootNode->FirstChildElement("windowType");
	if(keyNode != NULL)
		data.windowType = (_XWindowType)atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("w");
	if(keyNode != NULL)
		data.w = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("h");
	if(keyNode != NULL)
		data.h = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("sceneW");
	if(keyNode != NULL)
		data.sceneW = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("sceneH");
	if(keyNode != NULL)
		data.sceneH = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("positionX");
	if(keyNode != NULL)
		data.positionX = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("positionY");
	if(keyNode != NULL)
		data.positionY = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("isFullScreen");
	if(keyNode != NULL)
		data.isFullScreen = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("isShowCursor");
	if(keyNode != NULL)
		data.isShowCursor = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("withFrame");
	if(keyNode != NULL)
		data.withFrame = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("windowMode");
	if(keyNode != NULL)
		data.mode = (_XWindowSizeMode)atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("windowTitle");
	if(keyNode != NULL)
		data.windowTitle = keyNode->ToElement()->GetText();
	keyNode = rootNode->FirstChildElement("rotateMode");
	if(keyNode != NULL)
		data.rotateMode = (_XWindowRotateMode)atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("turnOverMode");
	if(keyNode != NULL)
		data.turnOverMode = (_XWindowTurnOverMode)atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("CFGMode");
	if(keyNode != NULL)
		data.CFGMode = (_XWindowTurnOverMode)atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("isStartDelay");
	if(keyNode != NULL)
	{
		temp = atoi(keyNode->ToElement()->GetText());
		if(temp != 0) data.isStartDelay = true;
		else data.isStartDelay = false;
	}
	keyNode = rootNode->FirstChildElement("startDelayTimer");
	if(keyNode != NULL)
		data.startDelayTimer = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("isShowVersion");
	if(keyNode != NULL)
	{
		temp = atoi(keyNode->ToElement()->GetText());
		if(temp != 0) data.isShowVersion = true;
		else data.isShowVersion = false;
	}
	keyNode = rootNode->FirstChildElement("versionString");
	if(keyNode != NULL)
		data.versionString = keyNode->ToElement()->GetText();
	keyNode = rootNode->FirstChildElement("isAutoShutDown");
	if(keyNode != NULL)
	{
		temp = atoi(keyNode->ToElement()->GetText());
		if(temp != 0) data.isAutoShutDown = true;
		else data.isAutoShutDown = false;
	}
	keyNode = rootNode->FirstChildElement("shutDownTimer_year");
	if(keyNode != NULL)
		data.autoShutDownYear = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("shutDownTimer_month");
	if(keyNode != NULL)
		data.autoShutDownMonth = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("shutDownTimer_day");
	if(keyNode != NULL)
		data.autoShutDownDay = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("shutDownTimer_hour");
	if(keyNode != NULL)
		data.autoShutDownHour = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("shutDownTimer_minute");
	if(keyNode != NULL)
		data.autoShutDownMinute = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("shutDownTimer_second");
	if(keyNode != NULL)
		data.autoShutDownSecond = atoi(keyNode->ToElement()->GetText());
	keyNode = rootNode->FirstChildElement("isShowConsole");
	if(keyNode != NULL)
	{
		temp = atoi(keyNode->ToElement()->GetText());
		if(temp != 0) data.isShowConsole = true;
		else data.isShowConsole = false;
	}
	keyNode = rootNode->FirstChildElement("defaultResourcePosition");
	if(keyNode != NULL)
	{
		temp = atoi(keyNode->ToElement()->GetText());
		switch(temp)
		{
		case RESOURCE_LOCAL_FOLDER:
			data.defResourcePos = RESOURCE_LOCAL_FOLDER;
			break;
		case RESOURCE_LOCAL_PACK:
			data.defResourcePos = RESOURCE_LOCAL_PACK;
			break;
		case RESOURCE_WEB:
			data.defResourcePos = RESOURCE_WEB;
			break;
		}//其他情况不改变资源位置的设置
	}

	return XTrue;
}
_XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const char* pszText)  
{  
    TiXmlElement elmNode(pszNode);  
    TiXmlText elmText(pszText);  
    if(elmNode.InsertEndChild(elmText) == NULL) return XFalse;  
    if(pElmParent.InsertEndChild(elmNode) == NULL) return XFalse;  
    return XTrue;  
} 
_XBool saveSystemConfigData(const _XWindowData &data)
{//将配置信息保存到配置文件
	TiXmlDocument doc(XEE_SYSTEM_CONFIG);
	TiXmlDeclaration declaration( "1.0","", "" ); 
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");
	char tempStr[1024];
	sprintf(tempStr,"%d",data.windowType);
	if(!addLeafNode(elmRoot,"windowType",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.w);
	if(!addLeafNode(elmRoot,"w",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.h);
	if(!addLeafNode(elmRoot,"h",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.sceneW);
	if(!addLeafNode(elmRoot,"sceneW",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.sceneH);
	if(!addLeafNode(elmRoot,"sceneH",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.positionX);
	if(!addLeafNode(elmRoot,"positionX",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.positionY);
	if(!addLeafNode(elmRoot,"positionY",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.isFullScreen);
	if(!addLeafNode(elmRoot,"isFullScreen",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.isShowCursor);
	if(!addLeafNode(elmRoot,"isShowCursor",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.withFrame);
	if(!addLeafNode(elmRoot,"withFrame",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.mode);
	if(!addLeafNode(elmRoot,"windowMode",tempStr)) return XFalse;
	if(!addLeafNode(elmRoot,"windowTitle",data.windowTitle.c_str())) return XFalse;
	sprintf(tempStr,"%d",data.rotateMode);
	if(!addLeafNode(elmRoot,"rotateMode",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.turnOverMode);
	if(!addLeafNode(elmRoot,"turnOverMode",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.CFGMode);
	if(!addLeafNode(elmRoot,"CFGMode",tempStr)) return XFalse;
	if(data.isStartDelay)
	{
		if(!addLeafNode(elmRoot,"isStartDelay","1")) return XFalse;
	}else
	{
		if(!addLeafNode(elmRoot,"isStartDelay","0")) return XFalse;
	}
	sprintf(tempStr,"%d",data.startDelayTimer);
	if(!addLeafNode(elmRoot,"startDelayTimer",tempStr)) return XFalse;
	if(data.isShowVersion)
	{
		if(!addLeafNode(elmRoot,"isShowVersion","1")) return XFalse;
	}else
	{
		if(!addLeafNode(elmRoot,"isShowVersion","0")) return XFalse;
	}
	if(!addLeafNode(elmRoot,"versionString",data.versionString.c_str())) return XFalse;
	if(data.isAutoShutDown)
	{
		if(!addLeafNode(elmRoot,"isAutoShutDown","1")) return XFalse;
	}else
	{
		if(!addLeafNode(elmRoot,"isAutoShutDown","0")) return XFalse;
	}
	sprintf(tempStr,"%d",data.autoShutDownYear);
	if(!addLeafNode(elmRoot,"shutDownTimer_year",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.autoShutDownMonth);
	if(!addLeafNode(elmRoot,"shutDownTimer_month",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.autoShutDownDay);
	if(!addLeafNode(elmRoot,"shutDownTimer_day",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.autoShutDownHour);
	if(!addLeafNode(elmRoot,"shutDownTimer_hour",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.autoShutDownMinute);
	if(!addLeafNode(elmRoot,"shutDownTimer_minut",tempStr)) return XFalse;
	sprintf(tempStr,"%d",data.autoShutDownSecond);
	if(!addLeafNode(elmRoot,"shutDownTimer_second",tempStr)) return XFalse;
	if(data.isShowConsole)
	{
		if(!addLeafNode(elmRoot,"isShowConsole","1")) return XFalse;
	}else
	{
		if(!addLeafNode(elmRoot,"isShowConsole","0")) return XFalse;
	}
	sprintf(tempStr,"%d",data.defResourcePos);
	if(!addLeafNode(elmRoot,"defaultResourcePosition",tempStr)) return XFalse;

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
		if(initOpenGLEx(XEE::windowData) == 0) return XFalse;
		break;
	case WINDOW_TYPE_NORMAL:
		if(initOpenGL(XEE::windowData.w,XEE::windowData.h) == 0) return XFalse;
		break;
	case WINDOW_TYPE_3D:
		if(initOpenGL3D(XEE::windowData.w,XEE::windowData.h) == 0) return XFalse;
		break;
	}
	return XTrue;
}
_XBool initWindowEx(const _XWindowData& windowData)
{
	//atexit(XEE::errProc);
	XEE::windowData = windowData;
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
	initGL();
	//初始化第二个上下文
	wglMakeCurrent(XEE::wHDC,XEE::wCopyHGLRC);
	initGL();
	wglMakeCurrent(XEE::wHDC,XEE::wCurrentHGLRC);	//恢复上下文
	if(XEE::windowData.isShowCursor == 0) _XWindow.setCurcor(false);	//隐藏光标
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
	//	while(1) 
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
//	_XObjectManager::GetInstance().init();
//#endif
	XEE::engineSysInit();
	return XTrue;
}
namespace XEE
{
	_XBool stopFlag = XFalse;	//是否暂停
	int manualFPS = -1;	//-1是不设置帧率
	float manualFrameTime;	//设置的每帧的时间
	_XBool needReset3DParam = XFalse;

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
		_XControlManager::GetInstance().update(delay);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().move(delay);
#endif
#ifdef GET_ALL_PIXEL
		XEE_AllPixel = 0;
#endif
	}
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
			if(XEE::manualFPS > 0) printf("%.1f FPS@ %d\n",XEE::nowFPS,XEE::manualFPS);
			else printf("%.1f FPS\n",XEE::nowFPS);
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
	_XBool haveSystemFont = XFalse;
	_XFontUnicode systemFont;
	_XBool isSuspend = XFalse;

	int showVersionTimer = 0;			//显示版本号的计时器
	_XFontUnicode showVersionStr;	//显示版本号的字符串
	int autoShutDownTimer = 0;
	char autoShutDownState = -1;
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
			while(1)
			{
				if(suspendKeyProc() != 0) exit(1);
				XEE::clearScreen();
				XEE::updateScreen();
				Sleep(1);
			}
		}else
		{
			string tempStr = str;
			tempStr += "\n";
			AddLogInfoStr(tempStr.c_str());
			XEE::systemFont.setString(str);
			while(1)
			{
				if(suspendKeyProc() != 0) 
				{
#ifdef DEBUG_MODE
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
			while(1)
			{
				if(suspendKeyProc() != 0) exit(1);
				Sleep(1);
			}
		}else
		{//显示错误信息并进入死锁
			string tempStr = str;
			tempStr += "\n";
			AddLogInfoStr(tempStr.c_str());
			while(1) 
			{
				if(suspendKeyProc() != 0)
				{
#ifdef DEBUG_MODE
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
	if(!XEE::systemFont.initFromTTF(filename,30,RESOURCE_LOCAL_FOLDER,false)) return XFalse;
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
			::SetForegroundWindow(hwnd);
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
				SendMessage(hwnd, WM_CLOSE, wParam, lParam);
				Shell_NotifyIcon(NIM_DELETE, &nid);
				break;
			case 0: 
				PostMessage(hwnd,WM_LBUTTONDOWN,NULL,NULL);
				break;
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
