#ifndef _JIA_XENGINECOMMON_
#define _JIA_XENGINECOMMON_
//这里用于定义引擎中所有公用的部分，将原来C方式的语法定义方式，转换成C++的方式进行归总
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.2
//--------------------------------
#include "tlhelp32.h"
#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XWindowCommon.h"
namespace XE{
namespace XEE //+
{
	//stat是否开机启动
	extern bool setRegRun(bool stat);	//设置程序为开机启动
	extern bool isRegRun();				//获取程序是否为开机启动(尚未实现)
	extern int sleep(unsigned long sleepUSecond);	//这个参数的时间是us及1000000分之1秒，及1000分之1毫秒
	inline void setVSync(bool sync)	//开启和关闭垂直同步，关闭的话，可以放开帧率，开启的话会限制帧率
	{	
		// Function pointer for the wgl extention function we need to enable/disable
		// vsync
		typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
		PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

		const char *extensions = (char*)glGetString( GL_EXTENSIONS );

		if(strstr(extensions,"WGL_EXT_swap_control") == 0) return;
		else
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
			if(wglSwapIntervalEXT) wglSwapIntervalEXT(sync);
		}
		//说明
		//In Linux / GLX
		//Use the GLX_EXT_swap_control extension to control swap interval. Check the GLX-specific extensions string via glXQueryExtensionsString() to verify that the extension is actually present.
		//The extension provides glXSwapIntervalEXT(), which also directly specifies the swap interval. glXSwapIntervalEXT(1) is used to enable vsync; glXSwapIntervalEXT(0) to disable vsync.
	}
	inline XBool getCapsLockState(){return (GetKeyState(VK_CAPITAL) == 1);}
	inline XBool getNumLockState(){return (GetKeyState(VK_NUMLOCK) == 1);}
	//设置按键状态
	inline void setCapsLockState(XBool state)
	{
		BYTE btKeyState[256];
		if(!GetKeyboardState((LPBYTE)&btKeyState)) return;
	 
		if((!(btKeyState[VK_CAPITAL] & 1) && state) ||
			((btKeyState[VK_CAPITAL] & 1) && !state))
		{//如果按键没有亮，则设置为亮
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}
	inline void setNumLockState(XBool state)
	{
		BYTE btKeyState[256];
		if(!GetKeyboardState((LPBYTE)&btKeyState)) return;
	 
		if((!(btKeyState[VK_NUMLOCK] & 1) && state) ||
			((btKeyState[VK_NUMLOCK] & 1) && !state))
		{//如果按键没有亮，则设置为亮
			keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}

	inline void makeABeep(){printf("\a");}	//发出一个规格化的声音
	inline void makeCrash()	//产生crash操作，用于测试crash的情况
	{
		int *a = 0;
		*a = 0;
	}
	extern XBool startProgram(const char * programPath);//启动一个程序
	extern XBool startProgram(const char * programPath,HANDLE &hProcess);//启动一个程序
	extern XBool exitProgram(HANDLE hProcess);
	extern XBool activateGame(HWND h);	//激活当前窗口为最前端
	struct XMemUsageInfo
	{//内存使用情况的数据
		unsigned int peakWorkSet;		//峰值内存使用量
		unsigned int curWorkingSet;		//当前内存使用量

		XMemUsageInfo()
			:peakWorkSet(0)
			, curWorkingSet(0)
		{}
	};
	extern XMemUsageInfo getMemUsageInfo();	//获取内存使用信息
	extern float getCpuUsage();				//获取CPU的占用率
	extern void simulateFullScreen(HWND hwnd);		//将指定窗口设置为全屏

#ifdef XEE_OS_WINDOWS
	enum XShutDownSystemMode
	{
		SYS_SD_MODE_G,		//关机
		SYS_SD_MODE_Z,		//注销
		SYS_SD_MODE_C,		//重启
	};
	//用于系统关机或者重启或者注销的函数
	extern void shutDownSystem(XShutDownSystemMode mode = SYS_SD_MODE_G);
	extern bool getSysPrivilege(const char * value);	//提升系统权限
	//后台停顿等待键盘输入
	inline void waitingForKey() {system("pause");}
	//设置控制台输出文字的颜色
	inline void setConsoleColor(int index)
	{
		switch(index)
		{
		case 1:SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE);break;
		case 2:SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);break;
		default:SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);break;
		}
	}
	//获取当前进程的ID
	inline DWORD getCurProcessID(){return GetCurrentProcessId();}
	//判断指定进程是否结束
	inline bool getIsProcessOver(DWORD id)
	{
	//	HANDLE hApp = OpenProcess(PROCESS_VM_OPERATION|SYNCHRONIZE,FALSE,id);
	//	return !WaitForSingleObject(hApp,0);	//返回指定进程的状态
		if(id == NULL) return true;
		return !WaitForSingleObject(OpenProcess(PROCESS_VM_OPERATION|SYNCHRONIZE,FALSE,id),0);	//返回指定进程的状态
	}
	//获取指定名称的进程ID
#ifdef UNICODE
	inline std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
#endif
	inline DWORD getProcesssID(const std::string &name)
	{
		PROCESSENTRY32  pe32;	
 
		HANDLE hSnaphot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); //获取进程快照
		Process32First(hSnaphot, &pe32); //指向第一个进程
		do
		{
#ifdef UNICODE
			if (lstrcmpi(pe32.szExeFile, s2ws(name).c_str()) == 0) //查找进程名称为NotePad.exe（记事本的程序）
#else
			if (lstrcmpi(pe32.szExeFile, name.c_str()) == 0) //查找进程名称为NotePad.exe（记事本的程序）
#endif
			{
				return pe32.th32ProcessID;
				break;
			}
		}while(Process32Next(hSnaphot, &pe32)); // 不断循环直到取不到进程
		return NULL;
	}
#endif

}
}

#endif