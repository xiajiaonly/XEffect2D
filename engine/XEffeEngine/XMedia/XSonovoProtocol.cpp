#include "XSonovoProtocol.h"
#include "XTimer.h"
#include "XMath/XMath.h"

void SPRecvCB(void *pClass,unsigned char * data,int len)
{
	_XSonovoProtocol &pPar = *(_XSonovoProtocol *)pClass;
	//这里是接收数据
	if(pPar.m_sendThreadState == STATE_SET_TO_END ||
		pPar.m_sendThreadState == STATE_END) return;	//数据直接丢弃
	_XSPData tempData;
	tempData.data = createArrayMem<unsigned char>(len);
	if(tempData.data == NULL) return;
	memcpy(tempData.data,data,len);
	tempData.dataLen = len;
	//接收到数据下面对数据进行处理
	if(tempData.dataLen < 3)
	{
		LogStr(" - data ERROR");
		pPar.showData(tempData,"接收: ");
		XDELETE_ARRAY(tempData.data);
		return;
	}
	unsigned short crcCheck = CRC16_XModen(tempData.data,tempData.dataLen - 2);
	if(tempData.data[tempData.dataLen - 2] != (crcCheck >> 8)
		|| tempData.data[tempData.dataLen - 1] != (crcCheck & 0xff))
	{
		LogStr(" - CRC ERROR");
		pPar.showData(tempData,"接收: ");
		XDELETE_ARRAY(tempData.data);
		return;
	}

	if(pPar.m_withLog) pPar.showData(tempData,"接收: ");

	if(pPar.m_info.devType == DEV_MASTER)
	{//主
		if(pPar.m_withStatistics) 
		{
			++ pPar.m_comunicateTimesNow;
			pPar.m_delayTimeNow += pPar.m_delayTime;
		}
		pPar.m_recvMutex.Lock();
		pPar.m_recvData.push_back(tempData);
		pPar.m_recvMutex.Unlock();

		pPar.m_sendMutex.Lock();	//发送锁提前好像不必要，需要进一步考察
		pPar.m_needRecv = XFalse;
		pPar.m_sendMutex.Unlock();
	}else
	{//从
		pPar.answerProc(tempData);		//回复
		XDELETE_ARRAY(tempData.data);	//这里需要释放资源
	}
}
DWORD WINAPI _XSonovoProtocol::sendThread(void * pParam)
{
	_XSonovoProtocol &pPar = *(_XSonovoProtocol *)pParam;
	pPar.m_sendThreadState = STATE_START;
	int upTime = getCurrentTicks();
	while(pPar.m_sendThreadState != STATE_SET_TO_END)
	{
		//这里发送数据
		if(pPar.m_info.devType == DEV_MASTER)
		{
			pPar.m_sendMutex.Lock();
			if(pPar.m_needRecv)  
			{
				pPar.m_delayTime += getCurrentTicks() - upTime;
				upTime = getCurrentTicks();
			}else
			{
				upTime = getCurrentTicks();
			}
			pPar.m_sendMutex.Unlock();
		}
		pPar.m_sendMutex.Lock();
		if(pPar.m_sendData.size() > 0 && !pPar.m_needRecv)	//如果需要接收的话需要等待就收完成才能继续发送
		{//有数据需要发送,这里发送数据
			pPar.m_nowSendData = pPar.m_sendData[0];
			pPar.m_sendData.pop_front();
			pPar.sendNowData();
			XDELETE_ARRAY(pPar.m_nowSendData.data);	//发送完成之后将数据丢弃
		}
		pPar.m_sendMutex.Unlock();
		Sleep(1);
	}
	pPar.m_sendThreadState = STATE_END;
	return 0;
}
_XBool _XSonovoProtocol::openDevice(const _XSPInfo &info)	//打开设备
{
	if(m_isInited) return XFalse;
	m_info = info;

	if(!m_serialPort.open(m_info.nPort,m_info.nBaud,m_info.nParity,SP_MODE_AUTO)) return XFalse;
	m_serialPort.setCallBackFun(SPRecvCB,this);

	m_needRecv = XFalse;
	//开启相关线程
	m_sendThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//开启发送线程

	m_isInited = XTrue;
	return XTrue;
}
void _XSonovoProtocol::answerProc(_XSPData &data)			//根据指定的问题回答主机
{
	switch(data.data[0])
	{
	case CMD_WRITE:
		{//对数据进行处理
			if(data.dataLen <= 5) 
			{
				LogStr("错误的数据!");
				return;
			}
			int start = data.data[1];
			int sum = (data.dataLen - 4) >> 1;
			if(sum <= 0 || sum > m_info.buffSize - start || m_info.buff == NULL)
			{//数据非法，返回写错误
				_XSPData tempData;
				unsigned char tmp[] = {CMD_WRITE_ACK,0x00,0x00,0x00};
				tempData.data = tmp;
				tempData.dataLen = sizeof(tmp);
				pushAData(tempData);
			}else
			{//处理数据
				for(int i = 0;i < sum;++ i)
				{
					m_info.buff[start + i] = (data.data[2 + (i << 1)] << 8) + data.data[2 + (i << 1) + 1];
				}
				//memcpy(m_info.buff + start,data.data + 3,sum << 1);
				//返回数据成功
				_XSPData tempData;
				unsigned char tmp[] = {CMD_WRITE_ACK,0x01,0x00,0x00};
				tempData.data = tmp;
				tempData.dataLen = sizeof(tmp);
				pushAData(tempData);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_READ:
		{//对数据进行处理
			if(data.dataLen != 5)
			{
				LogStr("错误的数据!");
				return;
			}
			int start = data.data[1];
			int sum = data.data[2];
			if(sum <= 0 || sum > m_info.buffSize - start || m_info.buff == NULL)
			{//读取数据错误
				_XSPData tempData;
				unsigned char tmp[] = {CMD_READ_ACK,0x00,0x00,0x00};
				tempData.data = tmp;
				tempData.dataLen = sizeof(tmp);
				pushAData(tempData);
			}else
			{//处理数据
				//返回数据成功
				_XSPData tempData;
				tempData.dataLen = (sum << 1) + 3 + 2;
				tempData.data = createArrayMem<unsigned char>(tempData.dataLen);
				tempData.data[0] = CMD_READ_ACK;
				tempData.data[1] = 0x01;
				tempData.data[2] = start;
				for(int i = 0;i < sum;++ i)
				{
					tempData.data[3 + (i << 1)] = (m_info.buff[start + i] >> 8) % 256;
					tempData.data[3 + (i << 1) + 1] = m_info.buff[start + i] % 256;
				}
				//memcpy(tempData.data + 3,m_info.buff + start,sum << 1);
				pushAData(tempData);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	default:	//收到错误的数据
		LogStr("收到错误的命令!");
		break;
	}
}
_XBool _XSonovoProtocol::pushAData(_XSPData &data)			//向发送队列中推入一个数据
{
	if(data.data == NULL || data.dataLen <= 0) return XFalse;
	_XSPData tempData;
	tempData.dataLen = data.dataLen;
	tempData.data = createArrayMem<unsigned char>(data.dataLen);
	if(tempData.data == NULL) return XFalse;
	memcpy(tempData.data,data.data,data.dataLen);
	m_sendMutex.Lock();
	m_sendData.push_back(tempData);
	m_sendMutex.Unlock();
	return XTrue;
}
void _XSonovoProtocol::sendNowData()
{
	if(m_nowSendData.data == NULL) return;
	unsigned short crcCheck = CRC16_XModen(m_nowSendData.data,m_nowSendData.dataLen - 2);	//最后两个字节为校验位
//	m_nowSendData.data[0] = m_modbusState.deviceID;
	m_nowSendData.data[m_nowSendData.dataLen - 2] = (crcCheck >> 8);
	m_nowSendData.data[m_nowSendData.dataLen - 1] = (crcCheck & 0xff);
#if WITH_LOG
	if(m_withLog)  showData(m_nowSendData,"发送：");
#endif
	m_serialPort.sendData(m_nowSendData.data,m_nowSendData.dataLen);
	if(m_info.devType == DEV_MASTER) m_needRecv = XTrue;	//只有做主时才标记需要等待接收
	m_delayTime = 0;
}
#include "XBasicWindow.h"
void _XSonovoProtocol::update()
{
	if(!m_withStatistics) return;
	m_statisticsTimer += XEE::frameTime;
	if(m_statisticsTimer >= 5000)
	{
		if(m_comunicateTimesNow > 0.0f)
		{
			float tmp = m_comunicateTimesNow;
			m_delayTimeAvg = m_delayTimeNow / tmp;	//平均计数
		}
		//重新统计
		m_delayTimeNow = 0;
		m_comunicateTimesNow = 0;
		m_statisticsTimer = 0;
	}
	m_serialPort.update();
}
void _XSonovoProtocol::writeOneRegister(int addr,int value)
{
	if(m_info.devType == DEV_SLAVE) return;
	unsigned char data[6];
	data[0] = CMD_WRITE;
	data[1] = addr;
	data[2] = (value >> 8) % 256;
	data[3] = value % 256;
	_XSPData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
void _XSonovoProtocol::readRegisters(int startAddr,int sum)	//读取多个保持寄存器
{
	if(m_info.devType == DEV_SLAVE) return;
	if(sum <= 0) return;
	unsigned char data[5];
	data[0] = CMD_READ;
	data[1] = startAddr;
	data[2] = sum;
	_XSPData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}