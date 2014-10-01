#ifndef _JIA_SERIALPORT_
#define _JIA_SERIALPORT_
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
#include <windows.h>
#include "XOSDefine.h"
#include "XBasicClass.h"
#include "../XTimer.h"
#include "../XLogBook.h"
//串口数据的接受模式
enum _XSerialPortMode
{
	SP_MODE_MANUAL,	//查询模式
	SP_MODE_AUTO,	//事件模式
};

#define SP_WITH_STATISTICS	//是否支持统计
#define SP_RECORDE_FILENAME ("serialPortRecv.dat")

class _XSerialPort
{
public:
	_XSerialPort()
		:m_hIDComDev(NULL)
		,m_bOpened(XFalse)
		,m_mode(SP_MODE_MANUAL)
		,m_funRecv(NULL)
		,m_pClass(NULL)
		,m_withStatistics(false)
		,m_withRecord(XFalse)
		,m_withPlay(XFalse)
		,m_recordFile(NULL)
		,m_playFile(NULL)
		,m_waitingDataTime(0)
	{
		memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
		memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));
	}
	~_XSerialPort()
	{
		endRecord();
		stopPlay();
		close();
	}

	_XBool open(int nPort = 1,int nBaud = 9600,int nParity = 0,_XSerialPortMode mode = SP_MODE_MANUAL);	//打开串口设备
	_XBool close();//关闭设备

	int readData(void *buff, int limit);
	int sendData(const unsigned char *,int size);
	int readDataWaiting();	// 查询缓冲区内是否有未读取的数据

	_XBool isOpened() const {return m_bOpened;}
	void setCallBackFun(void (* funRecv)(void *,unsigned char *,int),void *pClass)
	{
		m_funRecv = funRecv;
		m_pClass = pClass;
	}
protected:
	//_XBool writeCommByte(unsigned char);	// 内部实现，向串口写数据

	HANDLE m_hIDComDev;
	OVERLAPPED m_overlappedRead;
	OVERLAPPED m_overlappedWrite;
	_XBool m_bOpened;
	_XSerialPortMode m_mode;

	_XThreadState m_threadState;
	pthread_t m_recvThread;
	static DWORD WINAPI recvThread(void * pParam);		//接收线程

	void (*m_funRecv)(void *,unsigned char *,int);	//收到数据的回调函数
	void *m_pClass;
	int m_waitingDataTime;	//等待数据接收的时间
public:
	void setWaitingDataTime(int tmp){m_waitingDataTime = tmp;}
	int getWaitingDataTime() const{return m_waitingDataTime;}
private:
	//下面是统计信息
	bool m_withStatistics;	//是否启用统计
	int m_sendTimesAll;		//总的发送次数
	int m_sendBytesAll;		//总的发送字节数
	int m_recvTimesAll;		//总的接收次数
	int m_recvBytesAll;		//总的接收字节数

	int m_statisticsTimer;	//统计计时
	int m_sendTimesNow;
	int m_sendBytesNow;
	int m_recvTimesNow;
	int m_recvBytesNow;

	float m_sendTimesAvg;	//平均计数
	float m_sendBytesAvg;
	float m_recvTimesAvg;
	float m_recvBytesAvg;
public:
	int getSendTimesAll()const{return m_sendTimesAll;}
	int getSendBytesAll()const{return m_sendBytesAll;}
	int getRecvTimesAll()const{return m_recvTimesAll;}
	int getRecvBytesAll()const{return m_recvBytesAll;}
	float getSendTimesAvg()const{return m_sendTimesAvg;}
	float getSendBytesAvg()const{return m_sendBytesAvg;}
	float getRecvTimesAvg()const{return m_recvTimesAvg;}
	float getRecvBytesAvg()const{return m_recvBytesAvg;}
	void setStatistics(bool flag);
	void update();
	//下面实现串口数据的录制的功能
private:	//注意目前只录制接收功能，不录制发送
	_XBool m_withRecord;	//是否录制
	_XBool m_withPlay;		//是否播放
	FILE *m_recordFile;		//录制文件的指针
	FILE *m_playFile;		//播放文件的指针
	_XCritical m_recordMutex;
	void recordRecvAData(const unsigned char *data,int len)
	{//向文件中写入读取到一条数据
		if(!m_withRecord || m_recordFile == NULL) return;
		fprintf(m_recordFile,"recv:");	//写入标志
		fprintf(m_recordFile,(sysTime2String(getTimeMs()) + ":").c_str());	//写入时间
		for(int i = 0;i < len;++ i)
		{
			fprintf(m_recordFile,"0x%02x ",data[i]);
		}
		fprintf(m_recordFile,"\n");
	}
	_XBool readADataFromRecord(std::string &oparetion,_XSystemTime &t,unsigned char *date,int &len)	//从记录文件中读取一条数据
	{
		len = 0;
		if(m_playFile == NULL) return XFalse;
		static char lineData[2048];
		if(fgets(lineData,2048,m_playFile) == NULL) return XFalse;
		int ret = getCharPosition(lineData,':');
		if(ret <= 0) return XFalse;	//错误
		lineData[ret] = '\0';
		oparetion = lineData;
		int timeEnd = getCharPosition(lineData + ret + 1,':');
		if(timeEnd <= 0) return XFalse;	//错误
		lineData[ret + 1 + timeEnd] = '\0';
		t = string2sysTime(lineData + ret + 1);
		if(oparetion == "recv")
		{//下面读取时间和数据
			int offset = ret + 1 + timeEnd;
			for(len = 0;;++ len)
			{
				if(sscanf(lineData + offset + 1,"0x%02x ",&date[len]) != 1) break;
				ret = getCharPosition(lineData + offset + 1,' ');
				if(ret <= 0) break;
				offset += ret + 1;
			}
		}
		return true;
	}
	int m_playTimer;	//回放的计时器
	_XSystemTime m_upTime;
	_XSystemTime m_nextTime;
	unsigned char m_nextData[1024];
	int m_nextDataLen;
	std::string m_oparetion;
public:
	_XBool getWithRecord() const {return m_withRecord;}
	_XBool getWithPlay() const {return m_withPlay;}
	_XBool startRecord()	//开始录制
	{
		if(m_withRecord || m_withPlay) return XFalse;
		m_recordMutex.Lock();
		if((m_recordFile = fopen(SP_RECORDE_FILENAME,"w")) == NULL) return XFalse;	//打开录制文件
		fprintf(m_recordFile,("start:" + sysTime2String(getTimeMs()) + ":\n").c_str());	//写入时间
		m_withRecord = XTrue;
		m_recordMutex.Unlock();
		return XTrue;
	}
	_XBool endRecord()		//结束录制
	{
		if(!m_withRecord) return XFalse;
		m_recordMutex.Lock();
		fprintf(m_recordFile,("stop:" + sysTime2String(getTimeMs()) + ":\n").c_str());	//写入时间
		fclose(m_recordFile);
		m_recordFile = NULL;
		m_withRecord = XFalse;
		m_recordMutex.Unlock();
		return XTrue;
	}
	_XBool startPlay()		//开始播放
	{
		if(m_withPlay || m_withRecord) return XFalse;
		m_playTimer = 0;
		//打开录制文件(尚未完成)
		if((m_playFile = fopen(SP_RECORDE_FILENAME,"r")) == NULL) return XFalse;	//打开录制文件
		m_withPlay = XTrue;
		if(!readADataFromRecord(m_oparetion,m_upTime,m_nextData,m_nextDataLen))
		{//如果操作失败则直接关闭
			LogStr("record file error!");
			stopPlay();
		}
		if(m_oparetion != "start")
		{
			LogStr("record file error!");
			stopPlay();
		}
		//读取下一条数据
		if(!readADataFromRecord(m_oparetion,m_nextTime,m_nextData,m_nextDataLen))
		{//如果操作失败则直接关闭
			LogStr("record file error!");
			stopPlay();
		}
		return XTrue;
	}
	_XBool stopPlay()		//结束播放
	{
		if(!m_withPlay) return XFalse;
		//关闭数据文件(尚未完成)
		fclose(m_playFile);
		m_withPlay = XFalse;
		return XTrue;
	}
};

#endif