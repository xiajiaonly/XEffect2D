#include "XStdHead.h"
#include "XException.h"
#include "XErrorReporter.h"
//#include "XLogBook.h"

#include "Dbghelp.h"
#pragma comment(lib, "Dbghelp.lib")
namespace XE{
namespace XException
{
		//下面是关于异常调试的问题
	void dumpMemory(const char *filename, void *p)
	{
#if _WIN32_WINNT < 0x0501
		return;
#else
		std::string tmpFilename;
		if (filename == NULL)//这里以当前的时间作为文件名
			tmpFilename = "dump_" + XTime::sysTime2String(XTime::getTimeMs()) + ".dmp";
		else
			tmpFilename = filename;

		_EXCEPTION_POINTERS *pExcPointers = (_EXCEPTION_POINTERS *)p;
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId(); //把需要的信息添进去
		eInfo.ExceptionPointers = pExcPointers;
		eInfo.ClientPointers = FALSE;
		HANDLE hDumpFile = CreateFile(tmpFilename.c_str(), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
		MINIDUMP_TYPE MiniDumpWithDataSegs = (MINIDUMP_TYPE)(MiniDumpNormal
			| MiniDumpWithHandleData
			| MiniDumpWithUnloadedModules
			| MiniDumpWithIndirectlyReferencedMemory
			| MiniDumpScanMemory
			| MiniDumpWithProcessThreadData
			| MiniDumpWithThreadInfo);
		//bool bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		//	hDumpFile, MiniDumpWithDataSegs, pExcPointers ? &eInfo : NULL, NULL, NULL);
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hDumpFile, MiniDumpWithDataSegs, pExcPointers ? &eInfo : NULL, NULL, NULL);
		CloseHandle(hDumpFile);
#endif
	}
	LONG WINAPI unhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionPointers)
	{//这里是异常处理函数 
		//SetErrorMode( SEM_NOGPFAULTERRORBOX );  
		//printf("Error address %x/n",pExceptionPointers->ExceptionRecord->ExceptionAddress);   
		//printf("CPU register:/n");   
		//printf("eax %x ebx %x ecx %x edx %x/n",pExceptionPointers->ContextRecord->Eax,   
		//	pExceptionPointers->ContextRecord->Ebx,pExceptionPointers->ContextRecord->Ecx,   
		//	pExceptionPointers->ContextRecord->Edx); 
		dumpMemory(NULL,pExceptionPointers);	//输出内存
		LogStr("发生运行异常");					//输出日志
		LogRelease();	//保持日志文件的完整性
		if(!XEG.getIsGameExit() && XEG.m_windowData.isRestartWhenCrash) XEG.restartGame();	//重新启动程序
		XEG.setGameExit();				//防止由于退出时再次产生异常
		exit(1);								//退出当前程序
//		return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH; //或者 EXCEPTION_EXECUTE_HANDLER 关闭程序  
	}
	BOOL WINAPI consoleHandler(DWORD consoleEvent)
	{
		switch(consoleEvent)
		{
		case CTRL_C_EVENT:		//关闭
		case CTRL_BREAK_EVENT:	//暂停
		case CTRL_CLOSE_EVENT:	//关闭
		case CTRL_LOGOFF_EVENT:	//用户退出
		case CTRL_SHUTDOWN_EVENT:	//关闭
			LogRelease();	//窗口退出时，保证日志的完整性
			//XEE::release();	//捕获到退出事件，但是未解决问题，需要进一步的处理
			break;
		}
		return TRUE;
	}
	bool initException()
	{
		SetUnhandledExceptionFilter(XException::unhandledExceptionFilter); 
		if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)XException::consoleHandler,TRUE))
		{
			LogStr("Unable to install handler!");
			return false;
		}
		return true;
	}
}
}