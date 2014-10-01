#include "XSerialPort.h"
#include <tchar.h>
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
// 不使用读超时
#define TIMEOUT_READ_INTERVAL			0xFFFFFFFF
#define TIMEOUT_READ_TOTAL_MULTIPLIER	0
#define TIMEOUT_READ_TOTAL_CONSTANT		1000
// 写超时为秒
#define TIMEOUT_WRITE_TOTAL_MULTIPLIER	0
#define TIMEOUT_WRITE_TOTAL_CONSTANT	5000
// 推荐的输入/输出缓冲区（注意：实际值由系统设置）
#define BUFFER_INPUT_RECOMMEND	10000
#define BUFFER_OUTPUT_RECOMMEND	10000
// 异步读取/写入操作时等待事件的超时时间
#define TIMEOUT_READCOMM_EVENT	4000
#define TIMEOUT_WRITECOMM_EVENT	2000
// 一些通讯协议使用的宏
#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

// 打开串口
_XBool _XSerialPort::open(int nPort,int nBaud,int nParity,_XSerialPortMode mode)
{
    if(m_bOpened) return XTrue;	//防止重复打开

    TCHAR szPort[50];
    TCHAR szComParams[50];
    DCB dcb;
	m_mode = mode;

	//if(nPort < 10) wsprintf(szPort,_T("COM%d"),nPort);
	//else wsprintf(szPort,_T("\\\\.\\COM%d"),nPort);
	wsprintf(szPort,_T("\\\\.\\COM%d"),nPort);
	// API：建立文件，Windows中将串口设备当做文件对待
	m_hIDComDev = CreateFile(
					  szPort,
					  GENERIC_READ | GENERIC_WRITE,
					  0,
					  NULL,
					  OPEN_EXISTING,
					  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// 异步读写
					  NULL);

	if(m_hIDComDev == NULL) return XFalse;

	memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
	memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));

	//设置超时
	COMMTIMEOUTS CommTimeOuts;
	//if(m_mode == SP_MODE_AUTO)
	//{
	//	CommTimeOuts.ReadIntervalTimeout		= 0x100;
	//	CommTimeOuts.ReadTotalTimeoutMultiplier = 2;
	//	CommTimeOuts.ReadTotalTimeoutConstant	= 5000;
	//	CommTimeOuts.WriteTotalTimeoutMultiplier= 10;
	//	CommTimeOuts.WriteTotalTimeoutConstant	= 1000;
	//}else
	//{
		CommTimeOuts.ReadIntervalTimeout		= TIMEOUT_READ_INTERVAL;
		CommTimeOuts.ReadTotalTimeoutMultiplier = TIMEOUT_READ_TOTAL_MULTIPLIER;
		CommTimeOuts.ReadTotalTimeoutConstant	= TIMEOUT_READ_TOTAL_CONSTANT;
		CommTimeOuts.WriteTotalTimeoutMultiplier= TIMEOUT_WRITE_TOTAL_MULTIPLIER;
		CommTimeOuts.WriteTotalTimeoutConstant	= TIMEOUT_WRITE_TOTAL_CONSTANT;
	//}

	SetCommTimeouts(m_hIDComDev,&CommTimeOuts);

	wsprintf(szComParams,_T("COM%d:%d,n,8,1" ),nPort,nBaud);
	//设置异步读取/写入监视事件
	m_overlappedRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_overlappedWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	//读取/设置串口设备参数
	dcb.DCBlength = sizeof ( DCB );
	GetCommState ( m_hIDComDev, &dcb );
	dcb.fBinary = true; 
	dcb.BaudRate = nBaud;
	dcb.ByteSize = 8;			// 8 Bits 数据
	dcb.StopBits = ONESTOPBIT;	// 一位停止位
	dcb.Parity = nParity;				//缺省无校验
//	unsigned char ucSet;
//	ucSet = (unsigned char)((FC_RTSCTS & FC_DTRDSR) != 0);
//	ucSet = (unsigned char)((FC_RTSCTS & FC_RTSCTS) != 0);
//	ucSet = (unsigned char)((FC_RTSCTS & FC_XONXOFF) != 0);

	if(!SetCommState(m_hIDComDev,&dcb) ||
		!SetupComm(m_hIDComDev,BUFFER_INPUT_RECOMMEND,BUFFER_OUTPUT_RECOMMEND) ||
		m_overlappedRead.hEvent == NULL ||
		m_overlappedWrite.hEvent == NULL) 
	{
		//DWORD dwError = GetLastError();
		if(m_overlappedRead.hEvent != NULL) CloseHandle(m_overlappedRead.hEvent);
		if(m_overlappedWrite.hEvent != NULL) CloseHandle(m_overlappedWrite.hEvent);

		CloseHandle(m_hIDComDev);
		return XFalse;
	}
	// 清空缓冲区
	PurgeComm(m_hIDComDev, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	if(m_mode == SP_MODE_AUTO) //自动模式的时候开启线程
	{
		m_threadState = STATE_BEFORE_START;
		if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
		{//线程建立失败
			printf("Server command thread create error!\n");
			return XFalse;
		}
	}

    m_bOpened = XTrue;
    return m_bOpened;
}
////方案1
//DWORD WINAPI _XSerialPort::recvThread(void * pParam)
//{
//	_XSerialPort &pPar = *(_XSerialPort *)pParam;
//	DWORD dwEvtMask = 0,dwError,nBytesRead;
//	pPar.m_threadState = STATE_START;
//	int buffsize = 1024;
//	char *buff = createArrayMem<char>(buffsize);
//	COMSTAT cs;
//	if(!SetCommMask(pPar.m_hIDComDev,EV_RXCHAR)) printf("Error!\n");
//	while(true)
//	{
//		if(pPar.m_threadState == STATE_SET_TO_END) break;
//		if(WaitCommEvent(pPar.m_hIDComDev, &dwEvtMask,NULL) && (dwEvtMask & EV_RXCHAR))
//		{
//			if(pPar.m_waitingDataTime > 0) Sleep(pPar.m_waitingDataTime);	//等待一段时间，等待数据接收完成
//			ClearCommError(pPar.m_hIDComDev,&dwError,&cs);
//			if(cs.cbInQue)
//			{
//				if(cs.cbInQue > buffsize)
//				{//数据大小超过缓存大小，申请更大的缓存
//					XDELETE_ARRAY(buff);
//					buffsize = (cs.cbInQue << 1);
//					buff = createArrayMem<char>(buffsize);
//				}
//				if(ReadFile(pPar.m_hIDComDev,buff,buffsize,&nBytesRead,&pPar.m_overlappedRead))
//				{
//					pPar.m_recordMutex.Lock();
//					if(pPar.m_withRecord)
//						pPar.recordRecvAData((unsigned char *)buff,nBytesRead);	//如果需要录制的话，这里写入文件
//					pPar.m_recordMutex.Unlock();
//
//					if(pPar.m_funRecv != NULL) 
//						pPar.m_funRecv(pPar.m_pClass,(unsigned char *)buff,nBytesRead);
//					//printf("接收到数据!\n");
//#ifdef SP_WITH_STATISTICS
//					if(pPar.m_withStatistics)
//					{
//						++ pPar.m_recvTimesAll;
//						pPar.m_recvBytesAll += nBytesRead;
//						++ pPar.m_recvTimesNow;
//						pPar.m_recvBytesNow += nBytesRead;
//					}
//#endif
//				}
//			//	PurgeComm(pPar.m_hIDComDev,PURGE_RXCLEAR);
//			}
//		}
//	//	Sleep(1);
//	}
//	XDELETE_ARRAY(buff);
//	pPar.m_threadState = STATE_END;
//	return 0;
//}
//方案2
DWORD WINAPI _XSerialPort::recvThread(void * pParam)
{
	_XSerialPort &pPar = *(_XSerialPort *)pParam;
	DWORD dwEvtMask = 0,dwError,nBytesRead;
	pPar.m_threadState = STATE_START;
	int buffsize = 1024;
	char *buff = createArrayMem<char>(buffsize);
	COMSTAT cs;
	int offset;
	if(!SetCommMask(pPar.m_hIDComDev,EV_RXCHAR)) printf("Error!\n");
	while(true)
	{
		if(pPar.m_threadState == STATE_SET_TO_END) break;
		if(!WaitCommEvent(pPar.m_hIDComDev, &dwEvtMask,NULL) || !(dwEvtMask & EV_RXCHAR))
		{//没有数据
			Sleep(1);
			continue;	//等待接收信号
		}
		//下面开始接收数据直到没有数据接受为之
		offset = 0;
		while(true)
		{				
			ClearCommError(pPar.m_hIDComDev,&dwError,&cs);
			if(cs.cbInQue <= 0) break;	//没有数据需要接收
			if(offset == buffsize || buffsize - offset < cs.cbInQue)
			{//内存不够
				char * tmp = buff;
				buffsize = buffsize << 1;	//内存空间扩大一倍
				if(buffsize - offset < cs.cbInQue) buffsize = offset + cs.cbInQue;	//如果扩大一倍内存空间任然不够用，则扩大到够用为止
				buff = createArrayMem<char>(buffsize);
				memcpy(buff,tmp,offset);
				XDELETE_ARRAY(tmp);
			}
			if(ReadFile(pPar.m_hIDComDev,buff + offset,buffsize - offset,&nBytesRead,&pPar.m_overlappedRead))
			{
				offset += nBytesRead;
				if(pPar.m_waitingDataTime > 0) Sleep(pPar.m_waitingDataTime);	//等待一段时间，等待数据接收完成
				else break;
			}else break;
		}
		if(offset > 0 && !pPar.m_withPlay)	//播放的时候的数据直接丢弃
		{//接收到数据，下面进行处理
			pPar.m_recordMutex.Lock();
			if(pPar.m_withRecord)
				pPar.recordRecvAData((unsigned char *)buff,offset);	//如果需要录制的话，这里写入文件
			pPar.m_recordMutex.Unlock();

			if(pPar.m_funRecv != NULL) 
				pPar.m_funRecv(pPar.m_pClass,(unsigned char *)buff,offset);
			//printf("接收到数据!\n");
#ifdef SP_WITH_STATISTICS
			if(pPar.m_withStatistics)
			{
				++ pPar.m_recvTimesAll;
				pPar.m_recvBytesAll += offset;
				++ pPar.m_recvTimesNow;
				pPar.m_recvBytesNow += offset;
			}
#endif
		}
	//	PurgeComm(pPar.m_hIDComDev,PURGE_RXCLEAR);
	}
	XDELETE_ARRAY(buff);
	pPar.m_threadState = STATE_END;
	return 0;
}
// 关闭串口
_XBool _XSerialPort::close()
{
	if(!m_bOpened || m_hIDComDev == NULL) return XTrue;
	if(m_overlappedRead.hEvent != NULL) CloseHandle(m_overlappedRead.hEvent);
	if(m_overlappedWrite.hEvent != NULL) CloseHandle(m_overlappedWrite.hEvent);
	if(m_mode == SP_MODE_AUTO) //自动模式的时候开启线程
	{
		m_threadState = STATE_SET_TO_END;
		SetCommMask(m_hIDComDev,0);
		while(true)
		{//等待线程结束
			if(m_threadState == STATE_END) break;
			Sleep(1);
		}
	}
	CloseHandle(m_hIDComDev);
	m_bOpened = XFalse;
	m_hIDComDev = NULL;

	return XTrue;
}
// 向串口写入数据，类内部使用
//_XBool _XSerialPort::writeCommByte(unsigned char ucByte)
//{
//	_XBool bWriteStat;
//	DWORD dwBytesWritten;
//
//	bWriteStat = WriteFile(m_hIDComDev,(LPSTR)&ucByte,1,&dwBytesWritten,&m_overlappedWrite);
//
//	// 查询异步写入是否完成，未完成则挂起等待
//	if(!bWriteStat && (GetLastError() == ERROR_IO_PENDING))
//	{
//		if(WaitForSingleObject(m_overlappedWrite.hEvent,TIMEOUT_WRITECOMM_EVENT))
//		{
//			dwBytesWritten = 0;
//		}else 
//		{
//			GetOverlappedResult(m_hIDComDev,&m_overlappedWrite,&dwBytesWritten,XFalse);
//			m_overlappedWrite.Offset += dwBytesWritten;
//		}
//	}
//
//	return XTrue;
//}
// 向串口发送数据
int _XSerialPort::sendData(const unsigned char *buffer,int size)
{
	if(!m_bOpened || m_hIDComDev == NULL) return 0;

	//DWORD dwBytesWritten = 0;
	//for(int i = 0;i < size;++ i) 
	//{
	//	writeCommByte(buffer[i]);
	//	dwBytesWritten++;
	//}

	//return (int)dwBytesWritten;
	_XBool bWriteStat;
	DWORD dwBytesWritten;

	bWriteStat = WriteFile(m_hIDComDev,buffer,size,&dwBytesWritten,&m_overlappedWrite);
	// 查询异步写入是否完成，未完成则挂起等待
	if(!bWriteStat && (GetLastError() == ERROR_IO_PENDING))
	{
		if(WaitForSingleObject(m_overlappedWrite.hEvent,TIMEOUT_WRITECOMM_EVENT))
		{
			dwBytesWritten = 0;
		}else 
		{
			GetOverlappedResult(m_hIDComDev,&m_overlappedWrite,&dwBytesWritten,XFalse);
			m_overlappedWrite.Offset += dwBytesWritten;
		}
	}
#ifdef SP_WITH_STATISTICS
	if(m_withStatistics)
	{
		++ m_sendTimesAll;
		m_sendBytesAll += dwBytesWritten;
		++ m_sendTimesNow;
		m_sendBytesNow += dwBytesWritten;
	}
#endif
	return dwBytesWritten;
}
// 查询接受缓冲区内是否有数据(只查询,不读取)
int _XSerialPort::readDataWaiting()
{
	if(!m_bOpened || m_hIDComDev == NULL) return 0;

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError(m_hIDComDev,&dwErrorFlags,&ComStat);

	return (int)ComStat.cbInQue;
}
//读取来自串口的数据
int _XSerialPort::readData(void *buffer,int limit)
{
    if(!m_bOpened || m_hIDComDev == NULL) return 0;

    BOOL bReadStatus;
    DWORD dwBytesRead, dwErrorFlags;
    COMSTAT ComStat;
	// 读取之前须清楚错误信息
    ClearCommError(m_hIDComDev,&dwErrorFlags,&ComStat);
    if(!ComStat.cbInQue) return 0;

    dwBytesRead = (DWORD)ComStat.cbInQue;

    if(limit < (int)dwBytesRead) dwBytesRead = (DWORD)limit;

    bReadStatus = ReadFile(m_hIDComDev,buffer,dwBytesRead,&dwBytesRead,&m_overlappedRead);
	// 查询异步读取是否完成，未完成则挂起等待
    if(!bReadStatus)
	{
        if(GetLastError() == ERROR_IO_PENDING)
		{
            WaitForSingleObject(m_overlappedRead.hEvent,TIMEOUT_READCOMM_EVENT);
            return (int)dwBytesRead;
        }
        return 0;
    }
#ifdef SP_WITH_STATISTICS
	if(m_withStatistics)
	{
		++ m_recvTimesAll;
		m_recvBytesAll += dwBytesRead;
		++ m_recvTimesNow;
		m_recvBytesNow += dwBytesRead;
	}
#endif

    return (int)dwBytesRead;
}
#include "XBasicWindow.h"
void _XSerialPort::update()
{
#ifdef SP_WITH_STATISTICS
	if(m_withStatistics)
	{
		m_statisticsTimer += XEE::frameTime;
		if(m_statisticsTimer >= 5000)
		{
			float tmp = m_statisticsTimer * 0.001f;
			m_sendTimesAvg = m_sendTimesNow / tmp;	//平均计数
			m_sendBytesAvg = m_sendBytesNow / tmp;
			m_recvTimesAvg = m_recvTimesNow / tmp;
			m_recvBytesAvg = m_recvBytesNow / tmp;
			//重新统计
			m_sendTimesNow = 0;
			m_sendBytesNow = 0;
			m_recvTimesNow = 0;
			m_recvBytesNow = 0;
			m_statisticsTimer = 0;
		}
	}
#endif
	if(m_withPlay)
	{//下面是回放阶段
		m_playTimer += XEE::frameTime;
		//下面检查数据
		int tmpTime = getElapsedTime(m_upTime,m_nextTime);
		if(m_playTimer >= tmpTime)
		{//时间已经溢出，需要处理下一条数据
			m_playTimer -= tmpTime;
			if(m_oparetion == "stop")
			{//播放完成
				LogStr("play over!");
				stopPlay();
				return;
			}
			if(m_oparetion == "recv")
			{//接收到数据
				if(m_funRecv != NULL) 
					m_funRecv(m_pClass,m_nextData,m_nextDataLen);
			}
			//下面读取下一条数据
			m_upTime = m_nextTime;
			if(!readADataFromRecord(m_oparetion,m_nextTime,m_nextData,m_nextDataLen))
			{//如果操作失败则直接关闭
				LogStr("record file error!");
				stopPlay();
				return;
			}
		}
	}
}
void _XSerialPort::setStatistics(bool flag)
{
	if(flag == m_withStatistics) return;
	if(flag)
	{//开启统计
		m_withStatistics = true;
		//数据归零
		m_sendTimesAll = 0;		//总的发送次数
		m_sendBytesAll = 0;		//总的发送字节数
		m_recvTimesAll = 0;		//总的接收次数
		m_recvBytesAll = 0;		//总的接收字节数

		m_statisticsTimer = 0;	//统计计时
		m_sendTimesNow = 0;
		m_sendBytesNow = 0;
		m_recvTimesNow = 0;
		m_recvBytesNow = 0;

		m_sendTimesAvg = 0.0f;	//平均计数
		m_sendBytesAvg = 0.0f;
		m_recvTimesAvg = 0.0f;
		m_recvBytesAvg = 0.0f;
	}else
	{//关闭统计
		m_withStatistics = false;
	}
}