#include "XStdHead.h"
#include "XWindowTitle.h"
#include "XShaderGLSL.h"
#include "XXml.h"
#include "XFile.h"
#include <SHLOBJ.H>
#include "shobjidl.h"
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
namespace XE{
namespace XEE
{
	bool createLink()
	{
		if (!SUCCEEDED(CoInitialize(NULL))) return false;
		IShellLink *pisl;
		if (!SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl)))
		{
			CoUninitialize();
			return false;
		}
		IPersistFile* pIPF;
		std::string tmpStr = XFile::getCurrentExeFileFullPath();
		pisl->SetPath(tmpStr.c_str());
		pisl->SetWorkingDirectory(XFile::getWorkPath().c_str());
		if (!SUCCEEDED(pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF)))
		{
			pisl->Release();
			CoUninitialize();
			return false;
		}
		if (tmpStr.size() >= 3)
		{
			memcpy(&tmpStr[tmpStr.size() - 3], &"lnk", 3);
			//tmpStr[tmpStr.size() - 3] = 'l';
			//tmpStr[tmpStr.size() - 2] = 'n';
			//tmpStr[tmpStr.size() - 1] = 'k';
		}
		else
			return false;

		wchar_t wsz[MAX_PATH]; // 定义Unicode字符串 
		MultiByteToWideChar(CP_ACP, 0, tmpStr.c_str(), -1, wsz, MAX_PATH);
		pIPF->Save(wsz, FALSE);
		pIPF->Release();
		pisl->Release();
		CoUninitialize();
		return true;
	}
	bool isRegRun()
	{
		HKEY hKey;
		//打开指定子键
		DWORD dwDisposition = REG_OPENED_EXISTING_KEY;    // 如果不存在不创建
		if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS)
			return false;

		std::string filename = XFile::getCurrentExeFileFullPath();
		if (filename.size() >= 3)
		{
			memcpy(&filename[filename.size() - 3], &"lnk", 3);
			//filename[filename.size() - 3] = 'l';
			//filename[filename.size() - 2] = 'n';
			//filename[filename.size() - 1] = 'k';
		}
		else
			return false;
		DWORD dataType = REG_SZ;
		DWORD dataSize = 1024;
		char tmpName[1024];
		//读取注册表中该键的键值，如果已经相同则不重复设置。

#if (_WIN32_WINNT >= 0x0601)	//这个方法XP下不支持
		if (RegGetValue(hKey, NULL,
			XEG.m_windowData.windowTitle.c_str(), RRF_RT_REG_SZ, &dataType, tmpName, &dataSize) == ERROR_SUCCESS)
#else
		if (RegQueryValueEx(hKey, XEG.m_windowData.windowTitle.c_str(), NULL, &dataType, (LPBYTE)tmpName, &dataSize) == ERROR_SUCCESS)
#endif
		{
			RegCloseKey(hKey);
			//下面判断字符串是否相同
			//if(dataSize - 1 != filename.size()) return false;
			return XFile::fileNameCompare(tmpName, filename.c_str());
		}

		// 关闭子键句柄
		RegCloseKey(hKey);
		return false;
	}
	bool setRegRun(bool stat)
	{
		HKEY hKey;
		//打开指定子键
		DWORD dwDisposition = REG_OPENED_EXISTING_KEY;    // 如果不存在不创建
		if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS)
			return false;
		//创建一个新的键值，设置键值数据为文件名
		if (stat)
		{
			if (createLink())
			{
				std::string filename = XFile::getCurrentExeFileFullPath();
				if (filename.size() >= 3)
				{
					memcpy(&filename[filename.size() - 3], &"lnk", 3);
					//filename[filename.size() - 3] = 'l';
					//filename[filename.size() - 2] = 'n';
					//filename[filename.size() - 1] = 'k';
				}
				else
					return false;
				//读取注册表中该键的键值，如果已经相同则不重复设置。
				if (RegSetValueEx(hKey, XEG.m_windowData.windowTitle.c_str(), 0, REG_SZ, (BYTE*)filename.c_str(), filename.length()) == ERROR_SUCCESS)
					LogStr("自动启动设置成功!");
			}
			else
			{
				RegCloseKey(hKey);
				return false;
			}
		}
		else
		{
			if (RegDeleteValue(hKey, XEG.m_windowData.windowTitle.c_str()) == ERROR_SUCCESS)
				LogStr("取消自动启动!");
		}
		// 关闭子键句柄
		RegCloseKey(hKey);
		return true;
	}
	int sleep(unsigned long sleepUSecond)
	{//注意到这里的最小延迟设置为1ms，凡是小于1ms的延迟都模认为1ms
#ifdef XEE_OS_WINDOWS
		int delay = (int)(sleepUSecond * 0.001f);
		if (delay <= 0) Sleep(1);
		else Sleep(delay);
#endif
#ifdef XEE_OS_LINUX
		//方案1
		//	if(sleepUSecond < 1000) sleepUSecond = 1000;
		//	usleep(sleepUSecond);
		//方案2
		/*	if(sleepUSecond < 1000) sleepUSecond = 1000;
			timespec t_timeval;
			if(sleepUSecond >= 1000000)
			{//超过秒的延迟直接用秒
				t_timeval.tv_sec = sleepUSecond /1000000;
				t_timeval.tv_nsec = (sleepUSecond % 1000000) * 1000;
			}else
			{
				t_timeval.tv_sec = 0;
				t_timeval.tv_nsec = sleepUSecond * 1000;
			}
			if(nanosleep(&t_timeval,NULL) == -1)
			{
				//DSHOW("sleep error!\n");
			}*/
			//方案3
		if (sleepUSecond < 1000) sleepUSecond = 1000;
		timeval t_timeval;
		if (sleepUSecond >= 1000000)
		{//超过秒的延迟直接用秒    
			t_timeval.tv_sec = sleepUSecond / 1000000;
			t_timeval.tv_usec = sleepUSecond % 1000000;
		}
		else
		{
			t_timeval.tv_sec = 0;
			t_timeval.tv_usec = sleepUSecond;
		}
		if (select(0, NULL, NULL, NULL, &t_timeval) == -1)
		{
			//DSHOW("sleep error!\n");
		}
		//方案4
		/*	pthread_cond_t mycond;
			pthread_mutex_t mymutex;

			pthread_cond_init(&mycond, NULL);
			pthread_mutex_init(&mymutex, NULL);

			pthread_mutex_lock(&mymutex);

			if(sleepUSecond < 10000) sleepUSecond = 10000;
			struct timeval now;
			gettimeofday(&now, NULL);

			timespec abstime;
			abstime.tv_nsec = now.tv_usec * 1000 + (sleepUSecond % 1000000) * 1000;
			abstime.tv_sec = now.tv_sec + sleepUSecond / 1000000;
			pthread_cond_timedwait(&mycond, &mymutex, &abstime);
			//DSHOW("time out dispatch something...\n");

			pthread_mutex_unlock(&mymutex);*/
#endif
		return 0;
	}
	inline ULONGLONG subtractTime(const FILETIME &a, const FILETIME &b)
	{
		LARGE_INTEGER la, lb;
		la.LowPart = a.dwLowDateTime;
		la.HighPart = a.dwHighDateTime;
		lb.LowPart = b.dwLowDateTime;
		lb.HighPart = b.dwHighDateTime;

		return la.QuadPart - lb.QuadPart;
	}
	float getCpuUsage()	//获取CPU的占用率
	{
#if _WIN32_WINNT < 0x0501
		return 0.0f;
#else
		static FILETIME prevSysKernel, prevSysUser;
		static FILETIME prevProcKernel, prevProcUser;
		static bool isFirstRun = true;

		FILETIME sysIdle, sysKernel, sysUser;
		FILETIME procCreation, procExit, procKernel, procUser;

		if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
			!GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
		{// can't get time info so return
			return -1.;
		}
		// check for first call
		if (isFirstRun)
		{
			isFirstRun = false;
			// save time info before return
			prevSysKernel.dwLowDateTime = sysKernel.dwLowDateTime;
			prevSysKernel.dwHighDateTime = sysKernel.dwHighDateTime;

			prevSysUser.dwLowDateTime = sysUser.dwLowDateTime;
			prevSysUser.dwHighDateTime = sysUser.dwHighDateTime;

			prevProcKernel.dwLowDateTime = procKernel.dwLowDateTime;
			prevProcKernel.dwHighDateTime = procKernel.dwHighDateTime;

			prevProcUser.dwLowDateTime = procUser.dwLowDateTime;
			prevProcUser.dwHighDateTime = procUser.dwHighDateTime;

			return -1.;
		}

		ULONGLONG sysKernelDiff = subtractTime(sysKernel, prevSysKernel);
		ULONGLONG sysUserDiff = subtractTime(sysUser, prevSysUser);

		ULONGLONG procKernelDiff = subtractTime(procKernel, prevProcKernel);
		ULONGLONG procUserDiff = subtractTime(procUser, prevProcUser);

		ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
		ULONGLONG procTotal = procKernelDiff + procUserDiff;

		return (float)((100.0 * procTotal) / sysTotal);
#endif
	}
	XMemUsageInfo getMemUsageInfo()
	{
		XMemUsageInfo ret;
		HANDLE handle = GetCurrentProcess();
		if (handle == NULL) return ret;
		PROCESS_MEMORY_COUNTERS pmc;
		if (!GetProcessMemoryInfo(handle, &pmc, sizeof(pmc))) return ret;
		ret.peakWorkSet = pmc.PeakWorkingSetSize;	//峰值内存占用
		ret.curWorkingSet = pmc.WorkingSetSize;		//当前内存占用
		return ret;
		//Members
		//cb
		//Size of the structure, in bytes.
		//PageFaultCount
		//Number of page faults.		//缺页中断次数
		//PeakWorkingSetSize		//任务管理器:峰值工作集
		//Peak working set size.		//使用内存高峰
		//WorkingSetSize			//任务管理器:工作集
		//Current working set size.		//当前使用的内存
		//QuotaPeakPagedPoolUsage
		//Peak paged pool usage.		//使用页面缓存池高峰
		//QuotaPagedPoolUsage		//任务管理器:页面缓冲池
		//Current paged pool usage.		//使用页面缓存池
		//QuotaPeakNonPagedPoolUsage
		//Peak nonpaged pool usage.		//使用非分页缓存池高峰
		//QuotaNonPagedPoolUsage	//任务管理器:非页面缓冲池
		//Current nonpaged pool usage. //使用非分页缓存池
		//PagefileUsage				//任务管理器:提交大小(虚拟内存)
		//Current space allocated for the pagefile. Those pages may or may not be in memory. //使用分页文件
		//PeakPagefileUsage			//最大虚拟内存
		//Peak space allocated for the pagefile. //使用分页文件高峰
	}

	//void winMsgRelease()
	//{
	//	if(customWinProc == daemonProc)
	//	{
	//		Shell_NotifyIcon(NIM_DELETE, &nid);	//资源释放
	//	}
	//}
	void simulateFullScreen(HWND hwnd)
	{
		//方案1
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
		newment.rcNormalPosition.left = -DialogX;      //(0,0)点重合
		newment.rcNormalPosition.top = -DialogY;
		newment.rcNormalPosition.bottom = ScreenX - DialogY; //尺寸一致
		newment.rcNormalPosition.right = ScreenY - DialogX;
		SetWindowPlacement(hwnd, &newment);
		//方案2
		//	SetWindowLongPtr(hwnd, GWL_STYLE, 
		//		WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
		//	//MoveWindow(hwnd, 0, 0, windowWidth, windowHeight, TRUE);
		//	setWindowPos(0,0);
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	XBool startProgram(const char* programPath)
	{
		if (programPath == NULL) return XFalse;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		//ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
			programPath,// 程序的全路径,如："../Debug/x.exe"
			NULL, // Command line.  
			NULL, // Process handle not inheritable.  
			NULL, // Thread handle not inheritable.  
			FALSE, // Set handle inheritance to FALSE.  
			0, // No creation flags.  
			NULL, // Use parent's environment block.  
			NULL, // Use parent's starting directory.  
			&si, // Pointer to STARTUPINFO structure.
			&pi) // Pointer to PROCESS_INFORMATION structure.
			)
		{
			LogNull("CreateProcess failed (%d).", GetLastError());
			return XFalse;
		}
		//不使用的句柄最好关掉
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return XTrue;
	}
	XBool startProgram(const char* programPath, HANDLE &hProcess)
	{
		if (programPath == NULL) return XFalse;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		//ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
			programPath,// 程序的全路径,如："../Debug/x.exe"
			NULL, // Command line.  
			NULL, // Process handle not inheritable.  
			NULL, // Thread handle not inheritable.  
			FALSE, // Set handle inheritance to FALSE.  
			0, // No creation flags.  
			NULL, // Use parent's environment block.  
			NULL, // Use parent's starting directory.  
			&si, // Pointer to STARTUPINFO structure.
			&pi) // Pointer to PROCESS_INFORMATION structure.
			)
		{
			LogNull("CreateProcess failed (%d).", GetLastError());
			return XFalse;
		}
		//不使用的句柄最好关掉
		CloseHandle(pi.hThread);
		hProcess = pi.hProcess;
		return XTrue;
	}
	XBool startProgramEx(const char* programPath,int mode)//启动一个程序
	{
		return ShellExecuteA(0, "open", programPath, "", "", mode) > (HINSTANCE)32;
	}
	XBool exitProgram(HANDLE hProcess)
	{
		return TerminateProcess(hProcess, 0);
	}
	XBool activateGame(HWND h)
	{
		//方案1：//不行
		//return BringWindowToTop(XEG.getHWND());
		//方案2：//可以
		//typedef void (WINAPI *PROCSWITCHTOTHISWINDOW) (HWND, BOOL);
		//PROCSWITCHTOTHISWINDOW SwitchToThisWindow;
		//HMODULE hUser32 = GetModuleHandle("user32");
		//SwitchToThisWindow = ( PROCSWITCHTOTHISWINDOW)GetProcAddress(hUser32, "SwitchToThisWindow");  
		//if(SwitchToThisWindow == NULL) return false;
		////接下来只要用任何现存窗口的句柄调用这个函数即可，第二个参数指定如果窗口极小化，是否恢复其原状态。
		//SwitchToThisWindow(XEG.getHWND(), TRUE);
		//return true;
		//方案3：//可行
		HWND hCurWnd = GetForegroundWindow();
		DWORD dwMyID = GetCurrentThreadId();
		DWORD dwCurID = GetWindowThreadProcessId(hCurWnd, NULL);
		AttachThreadInput(dwCurID, dwMyID, TRUE);
		SetForegroundWindow(h);
		AttachThreadInput(dwCurID, dwMyID, FALSE);
		return  true;
		//方法4：//可行
		//ShowWindow(XEG.getHWND(),SW_SHOWNA);//简单的显示主窗口完事儿 
		//SetActiveWindow(XEG.getHWND());
		//SetForegroundWindow(XEG.getHWND());    
		////this->SetWindowPos(this,LOWORD(lParam),HIWORD(lParam),c.Width(),c.Height(),SWP_NOACTIVATE);
		//BringWindowToTop(XEG.getHWND());
		//return true;
	}
	//--------------------------------------------------------------
#ifdef XEE_OS_WINDOWS
	bool getSysPrivilege(const char * value)
	{
		HANDLE hToken;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			TOKEN_PRIVILEGES tkp;
			LookupPrivilegeValue(NULL, value, &tkp.Privileges[0].Luid);
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
			{
				CloseHandle(hToken);
				return true;
			}
		}
		CloseHandle(hToken);
		return false;
	}
	void shutDownSystem(XShutDownSystemMode mode)
	{
		if (!getSysPrivilege(SE_SHUTDOWN_NAME)) return;
		switch (mode)
		{
		case SYS_SD_MODE_G:
			//InitiateSystemShutdownEx(NULL,NULL,0,TRUE,FALSE,SHTDN_REASON_MAJOR_APPLICATION);	//关闭自身的计算机
			//InitiateSystemShutdownEx("192.168.0.1",NULL,0,TRUE,FALSE,SHTDN_REASON_MAJOR_APPLICATION);	//关闭远程的计算机
			ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF, 0);
			break;
		case SYS_SD_MODE_Z:
			ExitWindowsEx(EWX_LOGOFF, 0);
			break;
		case SYS_SD_MODE_C:
			ExitWindowsEx(EWX_REBOOT, 0);
			break;
		}
	}
	bool turnOnSystem(const void *MAC)
	{
		if (MAC == NULL) return false;
		unsigned char data[128];
		int offset = 6;
		memset(data, 0xff, 6);
		for (int i = 0; i < 16; ++i)
		{
			memcpy(data + offset, MAC, 6);
			offset += 6;
		}
		//启动WSA
		WSADATA WSAData;
		if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
		{
			LogNull("WSAStartup failed: %d\n", GetLastError());
			return false;
		}

		//创建socket
		SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock == INVALID_SOCKET)
		{
			LogNull("Socket create error: %d\n", GetLastError());
			return false;
		}

		//设置为广播发送
		BOOL bOptVal = TRUE;
		int iOptLen = sizeof(BOOL);
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, iOptLen) == SOCKET_ERROR)
		{
			LogNull("setsockopt error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return false;
		}

		sockaddr_in to;
		to.sin_family = AF_INET;
		to.sin_port = htons(0);
#ifdef WITH_LOCAL_BOARDCAST_IP
		to.sin_addr.s_addr = inet_addr(BOARDCASR_IP);
#else
		to.sin_addr.s_addr = htonl(INADDR_BROADCAST);
#endif

		//发送Magic Packet
		if (sendto(sock, (const char *)data, offset, 0, (const struct sockaddr *)&to, sizeof(to)) == SOCKET_ERROR)
			LogNull("Magic packet send error: %d", WSAGetLastError());
		else
			LogStr("Magic packet send!");

		closesocket(sock);
		WSACleanup();
		return true;
	}
#endif
}
}