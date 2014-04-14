#ifndef _JIA_XMODBUSPROTOCOL_
#define _JIA_XMODBUSPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.25
//--------------------------------
//这是一个用于串口通讯的协议的实现

extern unsigned short CRC16(unsigned char *p,unsigned short len);

//协议的工作模式
enum _XModBusWorkMode
{
	MODE_ASCII,		//16进制工作模式
	MODE_RTU,		//2进制工作模式
};
enum _XModBusType
{
	TYPE_MASTER,	//主设备
	TYPE_SLAVE,		//从设备
};

#include <deque>
#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"

struct _XModBusData
{
	int delayTime;		//接收到数据的延迟时间
	unsigned char * data;
	int dataLen;
	_XModBusData()
		:data(NULL)
		,dataLen(0)
	{
	}
};
enum _XModeBusCMD
{
	CMD_READ_COIL_VOLUME = 1,
	CMD_READ_INPUT = 2,
	CMD_READ_HOLD_REGISTER = 3,
	CMD_READ_INPUT_REGISTER = 4,
	CMD_SET_ONE_COIL = 5,
	CMD_SET_ONE_REGISTER = 6,
	CMD_SET_COILS = 15,
	CMD_SET_REGISTERS = 16,
};
enum _XModeBusErrorCode
{
	ERROR_READ_COIL_VOLUME = 0x81,
	ERROR_READ_INPUT = 0x82,
	ERROR_READ_HOLD_REGISTER = 0x83,
	ERROR_READ_INPUT_REGISTER = 0x84,
	ERROR_SET_ONE_COIL = 0x85,
	ERROR_SET_ONE_REGISTER = 0x86,
	ERROR_SET_COILS = 0x8f,
	ERROR_SET_REGISTERS = 0x90,
};
#define MODBUS_HEAD_LEN (8)
struct _XModbusState
{
	int nPort;		//通讯串口的端口号
	int nBaud;		//通讯频率
	int nParity;	//奇偶校验
	unsigned char deviceID;			//主机编号
	_XModBusWorkMode workMode;		//工作模式，16进制或者2进制
	_XModBusType workType;			//工作类型，主机或者从机
	unsigned char *coilsBuff;		//状态寄存器
	int coilsBuffSize;				//可变数量，这个是bit为单位
	int coilsOffset;				//状态的偏移,偏移之前的不使用
	unsigned short *hRegisterBuff;	//保持寄存器
	int hRegisterBuffSize;			
	int hRegisterOffset;			
	unsigned short *iRegisterBuff;	//输入寄存器
	int iRegisterBuffSize;
	int iRegisterOffset;
	unsigned char *inputBuff;		//输入状态寄存器
	int inputBuffSize;
	int inputOffset;
	int outTime;					//超时时间单位：ms
	int maxResendTimes;				//最大重发次数
	_XModbusState()
		:nPort(0)
		,nBaud(19200)
		,nParity(0)
		,workMode(MODE_RTU)
		,workType(TYPE_MASTER)
		,coilsBuff(NULL)
		,coilsBuffSize(0)
		,hRegisterBuff(NULL)
		,hRegisterBuffSize(0)
		,iRegisterBuff(NULL)
		,iRegisterBuffSize(0)
		,inputBuff(NULL)
		,inputBuffSize(0)
		,outTime(300)
		,maxResendTimes(3)
		,deviceID(1)
		,coilsOffset(0)
		,hRegisterOffset(0)
		,iRegisterOffset(0)
		,inputOffset(0)
	{
	}
};
class _XModBusProtocol
{
private:
	_XBool m_isInited;
	std::deque<_XModBusData> m_recvData;	//收到的数据
	std::deque<_XModBusData> m_sendData;	//发送的数据

	_XSerialPort m_serialPort;
	//static DWORD WINAPI recvThread(void * pParam);		//接收线程
	static DWORD WINAPI sendThread(void * pParam);		//发送线程

	//int m_tempDataOffset;
	//int m_tempDataLen;
	//unsigned char *m_tempDataBuff;
	//unsigned char m_modbusHead[MODBUS_HEAD_LEN];
	//int m_modbusNeedHeadLen;
	//int m_headLen;
	//void recvDataProc(unsigned char * data,int len);		//解析数据
	_XBool m_needRecv;	//是否等待回复
	int m_delayTime;	//等待回复的时间

	_XCritical m_sendMutex;
	_XCritical m_recvMutex;
//	_XThreadState m_recvThreadState;
	_XThreadState m_sendThreadState;

	_XModbusState m_modbusState;
	_XModBusData m_nowSendData;	//当前发送的数据包
	void sendNowData();			//发送当前需要发送的数据
	int m_sendTime;				//发送次数
	_XBool m_connectState;		//连接状态，如果连接状态为XFalse的话，会退出收发线程
	_XBool pushAData(_XModBusData &data);			//向发送队列中推入一个数据
	void answerProc(_XModBusData &data);			//根据指定的问题回答主机
	void (*m_callBackFun)(const _XModBusData & data,void *p);	//增加一个回调函数，当接收到数据时调用该回调函数
	void * m_pClass;
public:
	void setCallBackFun(void (*p)(const _XModBusData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}

	_XBool openDevice(_XModbusState &modbusState);	//说明：mode和type的功能尚未实现
	int getSendBuffSize();							//获取发送队列的长度
	_XBool pushData(_XModBusData &data)				//向发送队列中推入一个数据
	{
		if(m_modbusState.workType == TYPE_MASTER)
			return pushAData(data);
		return XFalse;
	}
	int getRecvBuffSize();							//获取接受队列的长度
	_XModBusData popData();							//从接受队列中取出一个数据

	void release();
	_XModBusProtocol()
		:m_isInited(XFalse)
	//	,m_tempDataBuff(NULL)
	{
	}
	~_XModBusProtocol(){release();}
	friend void recvCallback(void *pClass,unsigned char * data,int len);
};
inline int _XModBusProtocol::getRecvBuffSize()
{
	m_recvMutex.Lock();
	int ret = m_recvData.size();
	m_recvMutex.Unlock();
	return ret;
}
inline int _XModBusProtocol::getSendBuffSize()
{
	m_sendMutex.Lock();
	int ret = m_sendData.size();
	m_sendMutex.Unlock();
	return ret;
}
//下面的接口尚未完成
extern int getModbusCMDDataSum(const _XModBusData &CMD);	//从命令中解析数据数量
extern bool getModbusCMDAddress(const _XModBusData &CMD,unsigned int &addr);
extern bool getModbusCMDData(const _XModBusData &CMD,unsigned int &data);

#endif