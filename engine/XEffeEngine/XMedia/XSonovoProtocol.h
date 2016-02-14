#ifndef _JIA_XSONOVOPROTOCOL_
#define _JIA_XSONOVOPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.8.20
//--------------------------------
//#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"
#include "../XLogBook.h"
#include <deque>
namespace XE{
//工作模式
enum XSPDevType
{
	DEV_MASTER,		//主设备
	DEV_SLAVE,		//从设备
};
enum XSPCommand
{
	CMD_WRITE = 0x00,		//写指令
	CMD_READ = 0x01,		//读指令
	CMD_WRITE_ACK = 0x10,	//写指令的回复
	CMD_READ_ACK = 0x11,		//读指令的回复
};
struct XSPData
{
	int dataLen;
	unsigned char * data;
	XSPData()
		:dataLen(0)
		,data(NULL)
	{}
};
//用于描述当前协议信息的结构
struct XSPInfo
{
	XSerialPortInfo serialPortInfo;
	XSPDevType devType;
	unsigned short * buff;	//存储单元的指针
	int buffSize;			//存储单元中可以存储的元素数量
	XSPInfo()
		:devType(DEV_MASTER)
		,buff(NULL)
		,buffSize(0)
	{}
};
class XSonovoProtocol
{
private:
	XBool m_isInited;
	std::deque<XSPData> m_recvData;	//收到的数据
	std::deque<XSPData> m_sendData;	//发送的数据

	XBool m_needRecv;	//是否要等待回复
	int m_delayTime;	//等待回复的时间

	XSerialPort m_serialPort;
	XThreadState m_sendThreadState;
	static DWORD WINAPI sendThread(void * pParam);		//发送线程
	XCritical m_recvMutex;
	XCritical m_sendMutex;

	static void SPRecvCB(void *pClass,unsigned char * data,int len);	//数据接收到的回调函数
	void (*m_callBackFun)(const XSPData & data,void *p);	//增加一个回调函数，当接收到数据时调用该回调函数
	void * m_pClass;

	XBool pushAData(XSPData &data);			//向发送队列中推入一个数据(注意，这里会复制数据)
	void answerProc(XSPData &data);			//根据指定的问题回答主机

	XSPInfo m_info;
	XSPData m_curSendData;	//当前发送的数据包
	XBool m_withLog;

	void sendCurData();			//发送当前需要发送的数据
	unsigned char *m_sendBuff;	//用于发送的临时数据
	int m_sendBuffSize;	
public:
	XSPDevType getWorkType() const {return m_info.devType;}
	void setWithLog(XBool flag) {m_withLog = flag;}
	XBool getWithLog() {return m_withLog;}
	void setCallBackFun(void (*p)(const XSPData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}

	XBool getIsOpen() const {return m_isInited;}
	XBool openDevice(const XSPInfo &info);	//打开设备
	int getSendBuffSize();							//获取发送队列的长度

	XSonovoProtocol()
		:m_isInited(false)
		,m_withStatistics(false)
		,m_callBackFun(NULL)
		,m_pClass(NULL)
	{}
	~XSonovoProtocol(){release();}
	void release()
	{
		if(!m_isInited) return;
		waitThreadEnd(m_sendThreadState);
		//清除所有的数据
		for(unsigned int i = 0;i < m_sendData.size();++ i)
		{
			XMem::XDELETE_ARRAY(m_sendData[i].data);
		}
		m_sendData.clear();
		for(unsigned int i = 0;i < m_sendData.size();++ i)
		{
			XMem::XDELETE_ARRAY(m_recvData[i].data);
		}
		m_recvData.clear();

		m_serialPort.close();
		m_isInited = XFalse; 
	}
	XSPData popData()							//从接受队列中取出一个数据
	{
		XSPData tempData;
		if(m_info.devType == DEV_MASTER)
		{
			m_recvMutex.Lock();
			if(m_recvData.size() > 0)
			{
				tempData = m_recvData[0];
				m_recvData.pop_front();
			}
			m_recvMutex.Unlock();
		}
		return tempData;
	}
	XBool pushData(XSPData &data)				//向发送队列中推入一个数据
	{
		if(m_info.devType == DEV_MASTER)
			return pushAData(data);
		return XFalse;
	}

	void showData(const XSPData &data,const std::string &title);
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

	void writeOneRegister(int addr,int value);	//写一个寄存器
	void readRegisters(int startAddr,int sum);	//读取多个保持寄存器
};

inline void XSonovoProtocol::showData(const XSPData &data,const std::string &title)
{
	std::string tmp = title + "data:";
	char tmpStr[32];
	for(int i = 0;i < data.dataLen;++ i)
	{
		sprintf(tmpStr,"0x%02x ",data.data[i]);
		tmp += tmpStr;
	}
	LogStr(tmp.c_str());
}
inline int XSonovoProtocol::getSendBuffSize()
{
	m_sendMutex.Lock();
	int ret = m_sendData.size();
	m_sendMutex.Unlock();
	return ret;
}
inline bool getSPCMDAddress(const XSPData &CMD,unsigned int &addr)
{//根据协议获取命令的操作地址
	if(CMD.dataLen <= 0) return 0;
	switch(CMD.data[0])
	{
	case CMD_WRITE:
	//	addr = (CMD.data[1] << 8) + CMD.data[2];
		addr = CMD.data[1];
		return true;
	case CMD_WRITE_ACK:
		return false;
	case CMD_READ_ACK:
		//addr = (CMD.data[3] << 8) + CMD.data[4];
		addr = CMD.data[2];
		return true;
	}
	return false;
}
inline int getSPCMDDataSum(const XSPData &CMD)	//从命令中解析数据数量
{
	if(CMD.dataLen <= 0) return 0;
	switch(CMD.data[0])
	{
	case CMD_WRITE:
		return (CMD.dataLen - 4) >> 1;
	case CMD_READ:
		return 0;
	case CMD_WRITE_ACK:
		return 0;
	case CMD_READ_ACK:
		return (CMD.dataLen - 5) >> 1;
	}
	return false;
}
inline bool getSPCMDData(const XSPData &CMD,int index,unsigned int &data)
{//根据协议获取命令的操作地址
	if(CMD.dataLen <= 0) return false;
	if(index < 0 || index >= getSPCMDDataSum(CMD)) return false;
	int offset;
	switch(CMD.data[0])
	{
	case CMD_WRITE:
		offset = (index << 1) + 2;
		data = (CMD.data[offset] << 8) + CMD.data[offset + 1];
		return true;
	case CMD_READ:
		return false;
	case CMD_WRITE_ACK:
		return false;
	case CMD_READ_ACK:
		offset = (index << 1) + 3;
		data = (CMD.data[offset] << 8) + CMD.data[offset + 1];
		return true;
	}
	return false;
}
#if WITH_INLINE_FILE
#include "XSonovoProtocol.inl"
#endif
/****************************协议说明文档******************************
寄存单元：2Bytes连续内存空间为一个存储单元，每个存储单元有一个序号，0到n
存储空间：由n个存储单元构成，存储空间中存储单元的编号从0开始到n-1结束

主机：没有存储空间
从机：拥有存储空间

系统结构：一个系统中包含一个主机一个从机，目前不支持超过2台机器的情况
通讯协议：

主机指令
写指令
1Byte	|1Byte			|2*nBytes		|2Bytes
0x00    |起始寄存器序号 |n个寄存器数值 	|校验字节(CRC16校验)
读指令
1Byte	|1Byte			|1Byte					|2Bytes
0x01    |起始寄存器序号 |需要读取的寄存器数量 	|校验字节(CRC16校验)

从机指令
回应写指令
1Byte	|1Byte							|2Bytes
0x10    |写操作结果:0x00失败，0x01成功	|校验字节(CRC16校验)
回应读指令
1Byte	|1Byte							|1Byte			|2*nBytes		|2Bytes
0x11    |读操作结果:0x00失败，0x01成功	|起始寄存器序号 |n个寄存器数值 	|校验字节(CRC16校验)

默认的串口设置规则为:比特率：9600，奇偶校验：无，停止位：1，
主机指令需要等待从机回应之后才能继续下一条指令，超时时间为300ms，超时处理方式丢弃或者重发根据具体情况确定
CRC16校验使用的公式为：x16 + x12 + x5 + 1(XModen)
确认使用CRC16校验，命令(头字节)需要参与校验。
重码可能会造成通讯错误，如果对协议进行修订需要考虑解决该问题。
***********************************************************************/
}
#endif