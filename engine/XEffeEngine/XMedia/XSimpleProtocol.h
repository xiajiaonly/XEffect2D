#ifndef _JIA_XSIMPLEPROTOCOL_
#define _JIA_XSIMPLEPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.9.5
//--------------------------------
/***********************************协议文案说明如下**************************************
我曾经在一个工业流水线项目中用过一个通讯协议，现在感觉可以套用
大致描述如下：
通讯内容分成两大部分：特征命令字和参数，其中命令字选用>80H（即128）的二进制数，参数采用2字节模数编码，每字节均<80H（128），
举例：编码后2字节为m,n，其中m为模数，解码后D=m*128+n，这样保证了m和n均不会>80H
命令字分为带参数和不带参数两种，后者适用于按钮或者传感器输入，现在可以约定如下：
80H，81H，....8FH共16个特征码分别表示1-16号按钮（传感器）闭合，单字节，后面不带参数
90H，91H，....9FH共16个特征码分别表示1-16号按钮（传感器）释放，单字节，后面不带参数，该特征码一般情况下不用。
A0H，A1H，....FFH共96个特征码分别表示1-96号设备号，后面带2字节参数（以模数方式编码）
例如：    接收到85H，表示第六个按钮（传感器）闭合；
              接收到A3H，7EH，35H，表示4号设备参数为 16181，[7EH（126）*128+35H（53）]；126*128+53=16181
              参数最大值为2字节模数 16383，一般情况下够用了，如果需要大于这个数字时，可以另外开一个设备号，例如这次移动屏如果数字很大的话，
              把它分割成若干段（比如1m/段）， 每段起一个设备号，可以很方便的传输。
              另外，接收三字节数据包时每字节间隔不会超过2毫秒，放10倍，超时20毫秒该数据放弃。
              所有通讯不带校验，通讯项目做到如今从未碰到通讯出错，除非硬件条件不满足。
******************************************************************************************/
#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"
#include "XLogBook.h"
enum _XSimProDataType
{
	SIMPRO_TYPE_STATE,	//状态数据
	SIMPRO_TYPE_DATA,	//数值数据
};
struct _XSimProData
{
	_XSimProDataType type;	//数据类型
	int index;	//数据索引号
	int data;	//数据值
};
#include <deque>
class _XSimpleProtocol
{
private:
	_XBool m_isInited;
	char m_state[16];			//16个状态寄存器的状态
	unsigned short m_data[96];	//96个数据寄存器的数据
	unsigned char m_recvedData[3];	//已经接收的数据
	int m_recvedDataLen;		//已经接受的数据长度

	_XSerialPort m_serialPort;	//串口设备
	friend void simpleProctocolRecvCB(void *pClass,unsigned char * data,int len);	//数据接收到的回调函数
	void (*m_callBackFun)(const _XSimProData &data,void *p);	//增加一个回调函数，当接收到数据时调用该回调函数
	void * m_pClass;

	void recvDataProc(unsigned char *data,int len);
public:
	void setCallBackFun(void (*p)(const _XSimProData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}
	_XBool openDevice(int portIndex,int bitRate,int parity = 0)
	{
		if(m_isInited) return XFalse;
		//打开串口设备
		if(!m_serialPort.open(portIndex,bitRate,parity,SP_MODE_AUTO)) return XFalse;
		m_serialPort.setCallBackFun(simpleProctocolRecvCB,this);
		//开启相关线程
		m_sendThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//开启发送线程

		m_isInited = XTrue;
		return XTrue;
	}
	_XSimpleProtocol()
		:m_isInited(XFalse)
		,m_recvedDataLen(0)
	{}
	~_XSimpleProtocol(){release();}
	void release()
	{
		if(!m_isInited) return;
		m_sendThreadState = STATE_SET_TO_END;
		m_sendData.clear();	//清空发送队列
		waitThreadEnd(m_sendThreadState);
		m_serialPort.close();
		m_isInited = false;
	}
	char getState(int index) const
	{
		if(index < 0 || index >= 16) return 0;
		return m_state[index];
	}
	int getData(int index) const
	{
		if(index < 0 || index >= 96) return 0;
		return m_data[index];
	}
	int getMaxStateSum() const{return 16;}
	int getMaxDataSum() const{return 96;}
	//下面是为发送数据而定义的
private:
	std::deque<_XSimProData> m_sendData;	//发送的数据
	_XThreadState m_sendThreadState;
	_XCritical m_sendMutex;
	static DWORD WINAPI sendThread(void * pParam);		//发送线程
	void sendData(const _XSimProData&tmp);
public:
	_XBool pushAData(_XSimProData &data)
	{
		m_sendMutex.Lock();
		m_sendData.push_back(data);
		m_sendMutex.Unlock();
		return XTrue;
	}
	//录制和回放的几个接口
	_XBool getWithRecord() const {return m_serialPort.getWithRecord();}
	_XBool getWithPlay() const {return m_serialPort.getWithPlay();}
	_XBool startRecord() {return m_serialPort.startRecord();}	//开始录制
	_XBool endRecord() {return m_serialPort.endRecord();}		//结束录制
	_XBool startPlay() {return m_serialPort.startPlay();}		//开始播放
	_XBool stopPlay() {return m_serialPort.stopPlay();}		//结束播放
	void update() {m_serialPort.update();}
};
#endif