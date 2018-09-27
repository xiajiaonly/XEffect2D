INLINE void XFrameWork::setStop(int temp)	//停止 0,play,1,stop,-1,change state
{
	if (temp > 0) m_stopFlag = XTrue; else
		if (temp == 0) m_stopFlag = XFalse; else
			m_stopFlag = !m_stopFlag;
}
INLINE void XFrameWork::setFPS(int temp)		//设置游戏帧率，小于0为不限制帧率(目前这个函数好像没有起到应有的作用)
{
	if (temp <= 0) return;	//不合理的数据
	m_manualFPS = temp;
	//注意：这里需要2000才能保证控制的准确，不知道为什么
	m_manualFrameTime = 2000.0f / temp;
	//	m_fpsTimer.setStepTime(1000.0f / temp);
}
INLINE void XFrameWork::setRendFPS(int tmp)
{
	m_windowData.drawFps = tmp;
	if (m_windowData.drawFps <= 0) m_rendDelay = -1;
	else m_rendDelay = 1000.0f / m_windowData.drawFps;
}
INLINE void XFrameWork::setBGColor(const XFColor& c)	//设置背景的颜色
{
	m_defBGColor = c;
	glClearColor(m_defBGColor.r, m_defBGColor.g, m_defBGColor.b, m_defBGColor.a);
}
INLINE XBool XFrameWork::initWindowEx(const XWindowInfo& wData)	//这个接口已经废弃
{
	m_windowData = wData;
	return initWindow();
}
INLINE XBool XFrameWork::initWindowEx(void)	//这个接口已经废弃
{
	//if(!readSystemConfigData(windowData)) return XFalse;f
	if (!readSystemConfigData(m_windowData,true)) LogStr("读取配置文件失败!");	//2014.4.13修改为读取配置文件失败之后不退出，而是使用默认值建立窗口
	return initWindow();
}
INLINE XBool XFrameWork::getWindowNeedDraw()	//获取当前窗口是否需要刷新
{
	if (m_forceToPaint) return XTrue;	//如果设置为强制更新则这里直接跳过
	//窗口最小化或者显示为图标时不需要绘制
	return !(!(GetWindowLong(m_hwnd, GWL_STYLE) & WS_VISIBLE) ||
		IsIconic(m_hwnd));
}
INLINE void XFrameWork::restartGame()		//重新运行当前程序
{
	char szFileName[255];
	GetModuleFileNameA(NULL, szFileName, 255);
	XEE::startProgram(szFileName);
}
INLINE void XFrameWork::clearScreen()		//清除屏幕的内容
{
	if (m_windowData.windowType == WINDOW_TYPE_3D)
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//3D
	else glClear(GL_COLOR_BUFFER_BIT);	//only 2D
}
INLINE void XFrameWork::clearScreen(const XFColor& c)
{
	glClearColor(c.r, c.g, c.b, c.a);			//清除颜色
	clearScreen();
	glClearColor(m_defBGColor.r, m_defBGColor.g, m_defBGColor.b, m_defBGColor.a);			//清除颜色
}
#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif
//#ifndef LWA_ALPHA
//#define LWA_ALPHA 0x00000002
//#endif
INLINE void XFrameWork::setWindowAlpha(float a)
{
	static bool isFirst = true;
	if (isFirst)
	{
		if (SetWindowLong(m_hwnd, GWL_EXSTYLE, GetWindowLong(m_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED) == 0)
		{
			printf("SetWindowLong error:%lu!\n", GetLastError());
			return;
		}
		isFirst = false;
	}
#ifdef LWA_ALPHA
	//	if(!SetLayeredWindowAttributes(wHandle,RGB(255,255,255),0,LWA_COLORKEY))
	//	{
	//		printf("SetLayeredWindowAttributes error!\n");
	//	}
	SetLayeredWindowAttributes(m_hwnd, 0, a * 255, LWA_ALPHA);
	//SetLayeredWindowAttributes(wHandle,RGB(0,0,0),a * 255,LWA_ALPHA | LWA_COLORKEY);
#endif
}
INLINE void XFrameWork::getWindowPos(int &x, int &y)
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	x = rect.left;
	y = rect.top;
}
INLINE XVec2 XFrameWork::getWindowPos()
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	return XVec2(rect.left, rect.top);
}
INLINE XVec2 XFrameWork::getClientPos(const XVec2& screenPos)
{
	POINT tmpPos;
	tmpPos.x = screenPos.x;
	tmpPos.y = screenPos.y;
	ScreenToClient(m_hwnd, &tmpPos);
	return XVec2(tmpPos.x, tmpPos.y);
}
INLINE void XFrameWork::getWindowSize(int &w, int &h)
{//这里应该只返回客户区的大小，有待测试
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	//GetClientRect(m_hwnd, &rect);	//OSC工具中的窗口动画会造成问题
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
}
INLINE void XFrameWork::setWindowSize(float x, float y)
{//这个也有待测试
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	//下面的代码在OSC工具中的窗口动画会造成问题
	//RECT rectC;
	//GetClientRect(m_hwnd, &rectC);
	//int frameW = (rect.right - rect.right) - (rectC.right - rectC.left);
	//int frameH = (rect.bottom - rect.top) - (rectC.bottom - rectC.top);
	//if (m_windowData.isAlwaysTop)
	//	SetWindowPos(m_hwnd, HWND_TOPMOST, (int)rect.left, (int)rect.top, (int)x + frameW, (int)y + frameH, SWP_SHOWWINDOW);
	//else
	//	SetWindowPos(m_hwnd, HWND_TOP, (int)rect.left, (int)rect.top, (int)x + frameW, (int)y + frameH, SWP_SHOWWINDOW);
	if (m_windowData.isAlwaysTop)
		SetWindowPos(m_hwnd, HWND_TOPMOST, (int)rect.left, (int)rect.top, (int)x, (int)y, SWP_SHOWWINDOW);
	else
		SetWindowPos(m_hwnd, HWND_TOP, (int)rect.left, (int)rect.top, (int)x, (int)y, SWP_SHOWWINDOW);
}
INLINE void XFrameWork::setWindowPos(int x, int y, bool always)
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	m_windowData.isAlwaysTop = always;
	if (m_windowData.isAlwaysTop)
		SetWindowPos(m_hwnd, HWND_TOPMOST, x, y, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
	else
		SetWindowPos(m_hwnd, HWND_TOP, x, y, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
}