#ifndef _JIA_XLOGBOOK_
#define _JIA_XLOGBOOK_
#include "XOSDefine.h"
#include "XThread.h"
#include "XCritical.h"
#include <string>
namespace XE{
//#ifdef XEE_OS_WINDOWS
//#include "windows.h"
//#endif
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
//下面是一个代码调试时写日志的类
//优化说明：
//日志类将会使用两种运行模式
//A：日志文件会将生成的日志内容保存到指定的文件
//B：及时将日志信息发送给日志服务器，服务器将日志内容保存到适当的文件。
//B方法的优势是日志信息可以及时的保存，即便发生crash，日志信息也不会丢失，较A方式有很多优势
//不过B方式需要服务器支持，如果没有链接到服务器，则默认返回A模式

//对于A方式目前需要等到堆栈满了之后才会将日志信息一次性写入到日志文件，可以改成多线程离线的
//方式将日志信息写入到日志文件。好处是可以减少突发的大流量日志信息写入动作，使得日志输出更
//加平缓，不过多线程会带来一些负面的影响，需要商榷
//信息累积该如何处理呢？丢弃？等待？

//日志内容的格式为：
//年月日时分秒毫秒|LOGID|Message

//日志信息的结构体
struct XLogbookInfo
{
    XBool isEnable;                    //这条日志是否有效
    char *logMessage;					//日志的信息体
//	XLogLevel level;					//日志信息的等级(尚未生效)
	XLogbookInfo()
		:isEnable(XFalse)
		,logMessage(NULL)
//		,level(LOG_LEVEL_MESSAGE)
	{}
};
//++++++++++++++++++++++++++++++++++++++++++++++++++
//logserver的一些定义
enum XLogBookMode
{
    LOGBOOK_MODE_SIMPLE,    //简单模式
    LOGBOOK_MODE_SERVER    //log服务器模式
};
enum XLogBookLevel
{
	LB_LEVEL_NULL = 0,	//无日志输出
	LB_LEVEL_CONSOLE,	//后台日志输出
	LB_LEVEL_FILE,		//文件日志输出
	LB_LEVEL_ALL,		//即在后台输出也在日志文件输出
};
//与服务器的交互协议内容(大小写敏感):
enum XLogServerDataType
{
	LOG_DATA_TYPE_NULL,		//无效的信息
	LOG_DATA_TYPE_ID,		//服务器向客户端发送ID信息
	LOG_DATA_TYPE_INFO,		//客户端向服务器端发送的客户端信息
	LOG_DATA_TYPE_OVER,		//客户端向服务器端发送客户端结束
	LOG_DATA_TYPE_MESSAGE,	//日志内容
	LOG_DATA_TYPE_MAX,		//最大的有效值
};
enum XLogLevel	//日志信息的重要程度(尚未生效)
{
	LOG_LEVEL_NULL,		//最低等级
	LOG_LEVEL_MESSAGE,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
};
//xxxxx:message body
//sys00:file name			//客户端向服务器发送相关客户端的信息
//sys80:file name			//服务器向客户端返回日志文件名(未使用)
//sys01:NULL				//客户端告诉服务器日志文件操作结束
//sys81:NULL				//服务器告诉客户端日志文件操作结束(未使用)
//sys82:ID					//服务器向客户端发送客户端的ID
//msg00:ID message body		//客户端将日志内容发给服务器，ID为客户端的ID
//--------------------------------------------------
class XSocketEx;
class XLogbook
{
public:
	XLogbook();
	virtual ~XLogbook();
protected:
	XLogbook(const XLogbook&);
	XLogbook &operator= (const XLogbook&);
public:
	static const int m_logServerPort = 20086;
private:
#define LOG_SERVER_NAME "XLogBook_Server"	//使用这个可以在局域网内自动连接服务器
#define LOG_SERVER_IP "127.0.0.1"			//使用这个只能在本机连接服务器
	static const int m_maxRecordSum = 256;		//这个日志文件的最大信息数量会造成有些数据丢失
	static const int m_maxRecordLength = 512;	//初始log信息的长度，目前长度会根据实际的需要来变化
	//YYYYMMDD-hhmmss-xxx:
	//01234567890123456789  
	static const int m_recordHeadLength = 20;	//信息时间戳的长度为20

	XLogBookLevel m_logLevel;
	XBool m_isEnable;				//是否使能日志输出
	XLogbookInfo *m_LogInfo;		//日志的内容
	int m_logInfoPoint;
	FILE *m_logBookFile;			//日志文件的文件指针
	XBool m_isInited;				//是否初始化
	XThreadState m_isReleased;            //标记日志文件中是否已经释放过
	//下面是使用log服务器的一些定义
	XLogBookMode m_workMode;		//日志的工作模式
	XSocketEx *m_socket;				//网络连接的套接字	
	char *m_tmpBuff;				//用于临时存储数据
	XCritical m_locker;
	//新加入的属性
	std::string m_logServerFilename;	//服务器的据对路径
	int m_clientID;						//当前日志客户端的唯一ID
	std::string m_filename;

	//+++++++++++++++++++++++++++++++++++++++
	//下面是A方法中建议实现
	int m_logInfoPushPoint;
	int m_logInfoPopPoint;
	XBool pushAMessage(const char * message);    //向队列中推入一条信息，返回数据推入是否成功
	XBool popAMessage(char ** message);    //从队列中取出一条信息 return is there a message.
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
	//XBool doubleStringLength(char ** message,int messageSize);    //扩大内存的宽度一倍
	XBool m_needFlushFile;
public:
	XBool init(const char *fileName);
	void release();
	void addLogInfoStr(const char *p);
	void addLogInfoNull(const char *p, ...);    //目前只能支持到简单的%s %d %c几种输出格式，没做复杂的处理
	void setLogLevel(XLogBookLevel level) { m_logLevel = level; }

	void addLogInfoExp(XBool exp, const char *p, ...);    //目前只能支持到简单的%s %d %c几种输出格式，没做复杂的处理
	//内联函数的形式
	void setWorkMode(XLogBookMode temp)
	{
		if (m_isInited) return;
		m_workMode = temp;
	}
	void setServerFilename(const std::string& filename)
	{
		if (m_isInited) return;
		m_logServerFilename = filename;
	}
	void enable() { m_isEnable = XTrue; }		//使能日志输出
	void disable() { m_isEnable = XFalse; }		//暂时关闭日志输出
};
inline void XLogbook::addLogInfoStr(const char *p)
{
	if(!m_isEnable) return;
	m_locker.Lock();
    pushAMessage(p);
	m_locker.Unlock();
}
}
#endif