#ifndef _JIA_SERIALPORT_
#define _JIA_SERIALPORT_
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
#include "../XCommonDefine.h"
#include "../XTimer.h"
#include "../XXml.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
//串口数据的接受模式
enum XSerialPortMode
{
	SP_MODE_MANUAL,	//程序员手动调用数据接收函数
	SP_MODE_AUTO,	//多线程自动接收数据
};
#define SP_WITH_STATISTICS	//是否支持统计
#define SP_RECORDE_FILENAME "serialPortRecv.dat"
struct XSerialPortInfo	//这个是计划(尚未加入)
{
	XSerialPortMode mode;	//主动模式还是被动模式
	int byteSize;	//数据位的宽度4 - 8
	int nPort;	//端口号
	int nBaud;	//比特率
	int nParity;	//校验方式 0-4
	XBool withEventMode;	//是否支持事件模式,当withOverLaped为false时withEventMode不能为true会造成不能发送和不能退出的bug
	XBool withOverLaped;	//是否使用异步的方式
	int stopBits;	//停止位
	int dtr;
	int rts;
	int waitingDataTime;	//数据字节之间的等待时间(毫秒)
	XSerialPortInfo()
		:mode(SP_MODE_MANUAL)
		, nPort(1)
		, nBaud(9600)
		, nParity(NOPARITY)
		, withEventMode(XFalse)
		, withOverLaped(XFalse)
		, stopBits(ONESTOPBIT)
		, dtr(DTR_CONTROL_ENABLE)
		, rts(RTS_CONTROL_ENABLE)
		, waitingDataTime(0)
		, byteSize(8)
	{}
	bool readFromFile(const char * filename);		//从xml配置文件中读取	//尚未完成
	bool writeToFile(const char * filename);		//写入到配置文件中		//尚未完成
	bool readFromFile(TiXmlNode * node);			//从xml配置文件中读取	//尚未完成
	bool writeToFile(TiXmlElement &element);		//写入到配置文件中		//尚未完成
	bool checkData() const;		//检查数据的合法性
};
class XSerialPort
{
public:
	XSerialPort()
		:m_hIDComDev(NULL)
		, m_bOpened(XFalse)
		, m_funRecv(NULL)
		, m_pClass(NULL)
		, m_withStatistics(false)
		, m_withRecord(XFalse)
		, m_withPlay(XFalse)
		, m_recordFile(NULL)
		, m_playFile(NULL)
	{
		memset(&m_overlappedRead, 0, sizeof(OVERLAPPED));
		memset(&m_overlappedWrite, 0, sizeof(OVERLAPPED));
	}
	~XSerialPort()
	{
		endRecord();
		stopPlay();
		close();
	}

	XBool open(int nPort = 1, int nBaud = 9600, int nParity = 0,
		XSerialPortMode mode = SP_MODE_MANUAL, XBool withEventMode = XFalse);	//打开串口设备
	XBool open(const XSerialPortInfo &info);	//打开串口设备
	XBool close();//关闭设备

	int readData(void *buff, int limit);
	int sendData(const void *, int size);
	int readDataWaiting();	// 查询接受缓冲区内是否有数据(只查询,不读取)

	XBool isOpened() const { return m_bOpened; }
	void setCallBackFun(void(*funRecv)(void *, unsigned char *, int), void *pClass);
protected:
	//XBool writeCommByte(unsigned char);	// 内部实现，向串口写数据
	//XBool m_withEventMode;	//是否使用事件模式,实际使用中会发现，现场硬件使用事件模式有时候不会触发,目前原因尚不明确
	XSerialPortInfo m_serialPortInfo;

	HANDLE m_hIDComDev;
	OVERLAPPED m_overlappedRead;
	OVERLAPPED m_overlappedWrite;
	XBool m_bOpened;
	//XSerialPortMode m_mode;

	XThreadState m_threadState;
	pthread_t m_recvThread;
	static DWORD WINAPI recvThread(void * pParam);		//接收线程

	void(*m_funRecv)(void *, unsigned char *, int);	//收到数据的回调函数
	void *m_pClass;
	//int m_waitingDataTime;	//等待数据接收的时间
public:
	void setWaitingDataTime(int tmp) { m_serialPortInfo.waitingDataTime = tmp; }
	int getWaitingDataTime() const { return m_serialPortInfo.waitingDataTime; }
private:
	//下面是统计信息
	bool m_withStatistics;	//是否启用统计
	int m_sendTimesAll;		//总的发送次数
	int m_sendBytesAll;		//总的发送字节数
	int m_recvTimesAll;		//总的接收次数
	int m_recvBytesAll;		//总的接收字节数

	int m_statisticsTimer;	//统计计时
	int m_sendTimesCur;
	int m_sendBytesCur;
	int m_recvTimesCur;
	int m_recvBytesCur;

	float m_sendTimesAvg;	//平均计数
	float m_sendBytesAvg;
	float m_recvTimesAvg;
	float m_recvBytesAvg;
public:
	int getSendTimesAll()const { return m_sendTimesAll; }
	int getSendBytesAll()const { return m_sendBytesAll; }
	int getRecvTimesAll()const { return m_recvTimesAll; }
	int getRecvBytesAll()const { return m_recvBytesAll; }
	float getSendTimesAvg()const { return m_sendTimesAvg; }
	float getSendBytesAvg()const { return m_sendBytesAvg; }
	float getRecvTimesAvg()const { return m_recvTimesAvg; }
	float getRecvBytesAvg()const { return m_recvBytesAvg; }
	void setStatistics(bool flag);
	void update();
	//下面实现串口数据的录制的功能
private:	//注意目前只录制接收功能，不录制发送
	XBool m_withRecord;	//是否录制
	XBool m_withPlay;		//是否播放
	FILE *m_recordFile;		//录制文件的指针
	FILE *m_playFile;		//播放文件的指针
	XCritical m_recordMutex;
	void recordRecvAData(const unsigned char *data, int len);//向文件中写入读取到一条数据
	XBool readADataFromRecord(std::string &oparetion, XSystemTime &t, unsigned char *date, int &len);	//从记录文件中读取一条数据
	int m_playTimer;	//回放的计时器
	XSystemTime m_upTime;
	XSystemTime m_nextTime;
	unsigned char m_nextData[1024];
	int m_nextDataLen;
	std::string m_oparetion;
public:
	XBool getWithRecord() const { return m_withRecord; }
	XBool getWithPlay() const { return m_withPlay; }
	XBool startRecord();	//开始录制
	XBool endRecord();		//结束录制
	XBool startPlay();		//开始播放
	XBool stopPlay();		//结束播放
};
#if WITH_INLINE_FILE
#include "XSerialPort.inl"
#endif
}
#endif