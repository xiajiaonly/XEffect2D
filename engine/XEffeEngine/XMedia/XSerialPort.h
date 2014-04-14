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
//串口数据的接受模式
enum _XSerialPortMode
{
	SP_MODE_MANUAL,	//查询模式
	SP_MODE_AUTO,	//事件模式
};

class _XSerialPort
{
public:
	_XSerialPort()
		:m_hIDComDev(NULL)
		,m_bOpened(XFalse)
		,m_mode(SP_MODE_MANUAL)
		,m_funRecv(NULL)
		,m_pClass(NULL)
	{
		memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
		memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));
	}
	~_XSerialPort(){close();}

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
};

#endif