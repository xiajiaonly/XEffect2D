#ifndef _JIA_XLOGBOOK_
#define _JIA_XLOGBOOK_
#include "stdio.h"
#include <memory>
#include "string.h"
#include "XOSDefine.h"
#include "XBasicClass.h"
#include "XBasicFun.h"
#include "XSocket.h"

#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
using namespace std;
//下面是一个代码调试时写日志的类
//优化说明：
//日志类将会使用两种运行模式
//A：日志文件会将生成的日志内容保存到指定的文件
//B：及时将日志信息发送给日志服务器，服务器将日志内存保存到适当的文件。
//B方法的优势是日志信息可以及时的保存，即便发生crash，日志信息也不会丢失，较A方式有很多优势
//不过B方式需要服务器支持，如果没有链接到服务器，则默认返回A模式

//对于A方式目前需要等到堆栈满了之后才会将日志信息一次性写入到日志文件，可以改成多线程离线的
//方式将日志信息写入到日志文件。好处是可以减少突发的大流量日志信息写入动作，使得日志输出更
//加平缓，不过多线程会带来一些负面的影响，需要商榷
//信息累积该如何处理呢？丢弃？等待？

//日志内容的格式为：
//年月日时分秒毫秒|LOGID|Message

#define MAX_RECORD_SUM 16
#define MAX_RECORD_LENGTH 512    //初始log信息的长度，目前长度会根据实际的需要来变化
//YYYYMMDD-hhmmss-xxx:
//01234567890123456789
#define RECORD_HEAD_LENGTH 20    //信息时间戳的长度为20

//日志信息的结构体
struct _XLogbookInfo
{
    _XBool isEnable;                        //这条日志是否有效
    char *logMessage;                    //日志的信息体
	_XLogbookInfo()
		:isEnable(XFalse)
		,logMessage(NULL)
	{
	}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++
//logserver的一些定义
enum _XLogBookMode
{
    LOGBOOK_MODE_SIMPLE,    //简单模式
    LOGBOOK_MODE_SERVER    //log服务器模式
};
#define LOG_SERVER_NAME "XLogBook_Server"
#define LOG_SERVER_PORT 20086
#define LOG_SERVER_IP "127.0.0.1"
//与服务器的交互协议内容(大小写敏感):
//xxxxx:message body
//sys00:file name        :客户端向服务器发送日志文件名
//sys01:file name        :服务器向客户端返回日志文件名
//sys02:NULL            :客户端告诉服务器日志文件操作结束
//sys03:NULL            :服务器告诉客户端日志文件操作结束
//msg00:message body    :客户端将日志内容发给服务器
//--------------------------------------------------
class _XLogbook
{
    //+++++++++++++++++++++++++++++++++++++++++++
    //下面需要将其设计为Singleton模式
protected:
    _XLogbook();
    _XLogbook(const _XLogbook&);
	_XLogbook &operator= (const _XLogbook&);
    virtual ~_XLogbook(); 
public:
    static _XLogbook& GetInstance();
    //-------------------------------------------
private:
    _XLogbookInfo *m_LogInfo;    //日志的内容
    int m_logInfoPoint;
    FILE *m_logBookFile;		//日志文件的文件指针
    _XBool m_isInited;				//是否初始化
    _XThreadState m_isReleased;            //标记日志文件中是否已经释放过
    //下面是使用log服务器的一些定义
    _XLogBookMode m_workMode;    //日志的工作模式
    _XSocket m_socket;            //网络部分的一些定义

    //+++++++++++++++++++++++++++++++++++++++
    //下面是A方法中建议实现
    int m_logInfoPushPoint;
    int m_logInfoPopPoint;
    _XBool pushAMessage(const char * message);    //向队列中推入一条信息，返回数据推入是否成功
    _XBool popAMessage(char ** message);    //从队列中取出一条信息 return is there a message.
    //线程句柄
    pthread_t m_outputLogMessageThreadP;
#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI outputLogMessageThread(void * pParam);    //输出日志信息到文件或者log服务器的线程
#endif

#ifdef XEE_OS_LINUX
    static void *outputLogMessageThread(void * pParam);    //输出日志信息到文件或者log服务器的线程
#endif
    //---------------------------------------

    void popAllLogMessage();    //从队列中的信息输出到日志文件中
    _XBool doubleStringLength(char ** message,int messageSize);    //扩大内存的宽度一倍
public:
    _XBool initLog(const char *fileName);
    void releaseLog();
    void addLogInfoStr(const char *p);
    void addLogInfoNull(const char *p,...);    //目前只能支持到简单的%s %d %c几种输出格式，没做复杂的处理
     
    void addLogInfoExp(_XBool exp,const char *p,...);    //目前只能支持到简单的%s %d %c几种输出格式，没做复杂的处理
    //内联函数的形式
    void setWorkMode(_XLogBookMode temp)
    {
        if(m_isInited) return;
        m_workMode = temp;
    }
};

#ifdef DEBUG_MODE
#define AddLogInfoStr(p) _XLogbook::GetInstance().addLogInfoStr(p)
#define AddLogInfoNull _XLogbook::GetInstance().addLogInfoNull
#define AddLogInfoExp _XLogbook::GetInstance().addLogInfoExp
#else
#define AddLogInfoStr(p) (void)(p)
#define AddLogInfoNull (void)
#define AddLogInfoExp (void)
#endif

inline void _XLogbook::addLogInfoStr(const char *p)
{
    if(!m_isInited) initLog("logBook.txt");
	printf(p);
    pushAMessage(p);
}
#endif