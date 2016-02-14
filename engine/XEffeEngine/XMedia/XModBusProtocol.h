#ifndef _JIA_XMODBUSPROTOCOL_
#define _JIA_XMODBUSPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.25
//--------------------------------
#include <deque>
//#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"

//这是一个用于串口通讯的协议的实现
namespace XE{
//协议的工作模式
enum XModBusWorkMode
{
	MODE_ASCII,		//16进制工作模式
	MODE_RTU,		//2进制工作模式
};
enum XModBusType
{
	TYPE_MASTER,	//主设备
	TYPE_SLAVE,		//从设备
};

struct XModBusData
{
	int delayTime;		//接收到数据的延迟时间
	unsigned char * data;
	unsigned short addr;	//上读次操作的地址
	int dataLen;
	XModBusData()
		:data(NULL)
		,dataLen(0)
	{}
};
enum XModeBusCMD
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
enum XModeBusErrorCode
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
struct XModbusState
{
	XSerialPortInfo serialPortInfo;
	unsigned char deviceID;			//主机编号
	XModBusWorkMode workMode;		//工作模式，16进制或者2进制
	XModBusType workType;			//工作类型，主机或者从机
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
	int maxR2STime;					//接收到数据之后再次发送命令的时间间隔（ms）
	XModbusState()
		:workMode(MODE_RTU)
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
		,maxR2STime(5)
	{}
};
class XModBusProtocol
{
private:
	XBool m_isInited;
	std::deque<XModBusData> m_recvData;	//收到的数据
	std::deque<XModBusData> m_sendData;	//发送的数据

	XSerialPort m_serialPort;
	//static DWORD WINAPI recvThread(void * pParam);		//接收线程
	static DWORD WINAPI sendThread(void * pParam);		//发送线程

	//int m_tempDataOffset;
	//int m_tempDataLen;
	//unsigned char *m_tempDataBuff;
	//unsigned char m_modbusHead[MODBUS_HEAD_LEN];
	//int m_modbusNeedHeadLen;
	//int m_headLen;
	//void recvDataProc(unsigned char * data,int len);		//解析数据
	XBool m_needRecv;	//是否等待回复
	int m_delayTime;	//等待回复的时间
	int m_delaySendTime;	//等待发送的时间差

	XCritical m_sendMutex;
	XCritical m_recvMutex;
//	XThreadState m_recvThreadState;
	XThreadState m_sendThreadState;

	XModbusState m_modbusState;
	XModBusData m_curSendData;	//当前发送的数据包
	void sendCurData();			//发送当前需要发送的数据
	int m_sendTime;				//发送次数
	XBool m_connectState;		//连接状态，如果连接状态为XFalse的话，会退出收发线程
	XBool pushAData(XModBusData &data);			//向发送队列中推入一个数据,这里会复制数据（注意）
	void answerProc(XModBusData &data);			//根据指定的问题回答主机
	void (*m_callBackFun)(const XModBusData & data,void *p);	//增加一个回调函数，当接收到数据时调用该回调函数
	void * m_pClass;
	XBool m_withLog;
public:
	XModBusType getWorkType() const {return m_modbusState.workType;}
	void setWithLog(XBool flag) {m_withLog = flag;}
	XBool getWithLog() const {return m_withLog;}
	void setCallBackFun(void (*p)(const XModBusData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}
	XBool getIsOpen() const {return m_isInited;}

	XBool openDevice(XModbusState &modbusState);	//说明：mode和type的功能尚未实现
	int getSendBuffSize();							//获取发送队列的长度
	XBool pushData(XModBusData &data)				//向发送队列中推入一个数据
	{
		if(m_modbusState.workType == TYPE_MASTER)
			return pushAData(data);
		return XFalse;
	}
	int getRecvBuffSize();							//获取接受队列的长度
	XModBusData popData();							//从接受队列中取出一个数据

	void release();
	XModBusProtocol()
		:m_isInited(XFalse)
	//	,m_tempDataBuff(NULL)
		,m_withLog(XTrue)
		,m_withStatistics(false)
	{}
	~XModBusProtocol(){release();}
	static void recvCallback(void *pClass,unsigned char * data,int len);
	//下面需要增加的接口
	//做主时
	//下面四中操作尚未封装
	//下面这些接口都未经过测试
	void readCoilState(int startAddr,int sum,int arm);
	void readInputState(int startAddr,int sum,int arm);
	void writeOneCoilState(int Addr,int value,int arm);
	void writeCoilsState(int startAddr,int sum,unsigned char *value,int arm);
	//读取某个寄存器的值
	void readHoldRegisters(int startAddr,int sum,int arm);	//读取多个保持寄存器
	void readInputRegisters(int startAddr,int sum,int arm);	//读取多个输入寄存器
	//写入某个寄存器的值
	void writeRegisters(int startAddr,int sum,const unsigned short *data,int arm);		//写入多个寄存器
	void writeOneRegister(int addr,int value,int arm);	//写入一个寄存器
	int getHoldRegisterOffset(){return m_modbusState.hRegisterOffset;}	//获取做从时的地址的偏移量
private:
	//下面是统计信息
	bool m_withStatistics;	//是否启用统计
	int m_statisticsTimer;	//统计计时
	int m_comunicateTimesCur;	//交互次数
	int m_delayTimeCur;			//总延迟
	float m_delayTimeAvg;	//平均延迟
public:
	int getSendTimesAll()const{return m_serialPort.getSendTimesAll();}
	int getSendBytesAll()const{return m_serialPort.getSendBytesAll();}
	int getRecvTimesAll()const{return m_serialPort.getRecvTimesAll();}
	int getRecvBytesAll()const{return m_serialPort.getRecvBytesAll();}
	float getSendTimesAvg()const{return m_serialPort.getSendTimesAvg();}
	float getSendBytesAvg()const{return m_serialPort.getSendBytesAvg();}
	float getRecvTimesAvg()const{return m_serialPort.getRecvTimesAvg();}
	float getRecvBytesAvg()const{return m_serialPort.getRecvBytesAvg();}

	float getDelayTimeAvg()const{return m_delayTimeAvg;}
	void setStatistics(bool flag)
	{
		if(flag == m_withStatistics) return;
		if(flag)
		{//开启统计
			m_withStatistics = true;
			//数据归零
			m_statisticsTimer = 0;	//统计计时
			m_comunicateTimesCur = 0;	//交互次数
			m_delayTimeCur = 0;			//总延迟
			m_delayTimeAvg = 0.0f;	//平均延迟
			m_serialPort.setStatistics(true);
		}else
		{//关闭统计
			m_withStatistics = false;
			m_serialPort.setStatistics(false);
		}
	}
	void update();
};
inline int XModBusProtocol::getRecvBuffSize()
{
	m_recvMutex.Lock();
	int ret = m_recvData.size();
	m_recvMutex.Unlock();
	return ret;
}
inline int XModBusProtocol::getSendBuffSize()
{
	m_sendMutex.Lock();
	int ret = m_sendData.size();
	m_sendMutex.Unlock();
	return ret;
}
namespace XModBus{
//做主时从接受的数据中提取数据
//注意下面的接口是做主时从接收到的数据中提取相关的数据
extern int getModbusCMDDataSum(const XModBusData &CMD);	//从命令中解析数据数量
inline bool getModbusCMDAddress(const XModBusData &CMD,unsigned int &addr)
{//根据协议获取命令的操作地址
	if(CMD.dataLen <= 0) return false;
	addr = CMD.addr;
	return true;
}
extern bool getModbusCMDData(const XModBusData &CMD,int index,unsigned int &data);
inline int getModbusCMDID(const XModBusData &CMD){return CMD.data[0];}	//从modbus命令中获取从设备的设备号
}
}
#endif