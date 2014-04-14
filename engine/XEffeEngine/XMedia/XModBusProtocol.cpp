#include "XModBusProtocol.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.25
//--------------------------------
//CRC的高位
#include "XTimer.h"
const unsigned char auchCRCHi[] = { 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40 }; 
//CRC的低位
const unsigned char auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40 };

unsigned short CRC16(unsigned char * p,unsigned short len)     
{
    unsigned char uchCRCHi = 0xFF ;
    unsigned char uchCRCLo = 0xFF ;
    unsigned short uIndex;
	if(p == NULL || len <= 0) return (uchCRCHi << 8 | uchCRCLo);
    while(len --) 
	{
        uIndex = uchCRCLo ^ *p++;
        uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
        uchCRCHi = auchCRCLo[uIndex];
	}
    return (uchCRCHi << 8 | uchCRCLo);
}
void recvCallback(void *pClass,unsigned char * data,int len)
{
	_XModBusProtocol * p = (_XModBusProtocol *)pClass;
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = len;
	tempData.delayTime = p->m_delayTime;
	if(data[0] == p->m_modbusState.deviceID)
	{//自己的数据
		for(int i = 0;i < tempData.dataLen;++ i)
		{
			printf("0x%02x ",tempData.data[i]);
		}
		unsigned short crcCheck = CRC16(tempData.data,tempData.dataLen - 2);
		if(tempData.data[tempData.dataLen - 1] != (crcCheck >> 8)
			|| tempData.data[tempData.dataLen - 2] != (crcCheck & 0xff))
		{
			printf(" - CRC ERROR\n");
			return;
		}else
		{
			printf(" - \n");
		}
		if(p->m_modbusState.workType == TYPE_MASTER)
		{//主的方式
			p->m_recvMutex.Lock();
			p->m_recvData.push_back(tempData);
			p->m_recvMutex.Unlock();
			p->m_sendMutex.Lock();
			p->m_needRecv = XFalse;
			XDELETE_ARRAY(p->m_nowSendData.data);
			p->m_sendMutex.Unlock();
		}else
		{//从的方式
			p->answerProc(tempData);
		}
	}else
	{//别人的数据
		printf("别人的数据:");
		for(int i = 0;i < tempData.dataLen;++ i)
		{
			printf("0x%02x ",tempData.data[i]);
		}
		printf(" - \n");
	}
}
_XBool _XModBusProtocol::openDevice(_XModbusState &modbusState)
{
	if(m_isInited) return XFalse;
	m_modbusState = modbusState;

	//if(!m_serialPort.open(m_modbusState.nPort,m_modbusState.nBaud,m_modbusState.nParity)) return XFalse;
	if(!m_serialPort.open(m_modbusState.nPort,m_modbusState.nBaud,m_modbusState.nParity,SP_MODE_AUTO)) return XFalse;
	m_serialPort.setCallBackFun(recvCallback,this);
	//下面开启接受线程和发送线程
//	m_recvThreadState = STATE_BEFORE_START;
	m_sendThreadState = STATE_BEFORE_START;
//	m_tempDataLen = 0;
//	m_headLen = 0;
	m_needRecv = XFalse;
	m_connectState = XTrue;
//	if(m_modbusState.workType == TYPE_MASTER) m_modbusNeedHeadLen = 3;
//	if(m_modbusState.workType == TYPE_SLAVE) m_modbusNeedHeadLen = 7;

//	if(CreateThread(0,0,recvThread,this,0,NULL) == 0) return XFalse;	//开启接受线程
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//开启发送线程

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XModBusProtocol::pushAData(_XModBusData &data)
{
	if(data.data == NULL || data.dataLen <= 0) return XFalse;
	_XModBusData tempData;
	tempData.dataLen = data.dataLen;
	tempData.data = createArrayMem<unsigned char>(data.dataLen);
	if(tempData.data == NULL) return XFalse;
	memcpy(tempData.data,data.data,data.dataLen);
	m_sendMutex.Lock();
	m_sendData.push_back(tempData);
	m_sendMutex.Unlock();
	return XTrue;
}
void _XModBusProtocol::answerProc(_XModBusData &data)
{
	switch(data.data[1])
	{
	case CMD_READ_COIL_VOLUME:
		{
			unsigned short start = (data.data[2] << 8) + data.data[3] - m_modbusState.coilsOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.coilsBuffSize == 0 || m_modbusState.coilsBuffSize < start + sum)
			{//直接返回错误
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x81,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//返回数据
				int len = 0;
				if(sum % 8 == 0) len = sum / 8;
				else len = sum / 8 + 1;
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(len + 5);
				if(tempData.data == NULL)
				{//返回失败
					unsigned char retData[] = {0x00,0x81,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				tempData.dataLen = len + 5;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_READ_COIL_VOLUME;
				tempData.data[2] = len;
				//下面取值
				int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					temp = getBit(m_modbusState.coilsBuff[(i + start) >> 3],(i + start) % 8);
					setBit(tempData.data[3 + (i >> 3)],i % 8,temp);
				}
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_READ_INPUT:
		{
			unsigned short start = (data.data[2] << 8) + data.data[3] - m_modbusState.inputOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.inputBuffSize == 0 || m_modbusState.inputBuffSize < start + sum)
			{//直接返回错误
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x82,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//返回数据
				int len = 0;
				if(sum % 8 == 0) len = sum / 8;
				else len = sum / 8 + 1;
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(len + 5);
				if(tempData.data == NULL)
				{//返回失败
					unsigned char retData[] = {0x00,0x82,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				tempData.dataLen = len + 5;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_READ_INPUT;
				tempData.data[2] = len;
				//下面取值
				int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					temp = getBit(m_modbusState.inputBuff[(i + start) >> 3],(i + start) % 8);
					setBit(tempData.data[3 + (i >> 3)],i % 8,temp);
				}
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_READ_HOLD_REGISTER:
		{
			unsigned short start = (data.data[2] << 8) + data.data[3] - m_modbusState.hRegisterOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.hRegisterBuffSize == 0 || m_modbusState.hRegisterBuffSize < start + sum)
			{//直接返回错误
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x83,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//返回数据
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(sum * 2 + 5);
				if(tempData.data == NULL)
				{//返回失败
					unsigned char retData[] = {0x00,0x83,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				tempData.dataLen = sum * 2 + 5;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_READ_HOLD_REGISTER;
				tempData.data[2] = sum * 2;
				//下面取值
				//int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					//memcpy(&tempData.data[3 + i * 2],&m_modbusState.hRegisterBuff[start + i],2);
					tempData.data[3 + i * 2 + 0] = m_modbusState.hRegisterBuff[start + i] >> 8;
					tempData.data[3 + i * 2 + 1] = m_modbusState.hRegisterBuff[start + i] & 0xff;
				}
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_READ_INPUT_REGISTER:
		{
			unsigned short start = (data.data[2] << 8) + data.data[3] - m_modbusState.iRegisterOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.iRegisterBuffSize == 0 || m_modbusState.iRegisterBuffSize < start + sum)
			{//直接返回错误
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x84,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//返回数据
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(sum * 2 + 5);
				if(tempData.data == NULL)
				{//返回失败
					unsigned char retData[] = {0x00,0x84,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				tempData.dataLen = sum * 2 + 5;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_READ_INPUT_REGISTER;
				tempData.data[2] = sum * 2;
				//下面取值
				//int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					//memcpy(&tempData.data[3 + i * 2],&m_modbusState.iRegisterBuff[start + i],2);
					tempData.data[3 + i * 2 + 0] = m_modbusState.iRegisterBuff[start + i] >> 8;
					tempData.data[3 + i * 2 + 1] = m_modbusState.iRegisterBuff[start + i] & 0xff;
				}
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_SET_ONE_COIL:
		{
			unsigned short pos = (data.data[2] << 8) + data.data[3] - m_modbusState.coilsOffset;
			unsigned short value = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.coilsBuffSize == 0 || m_modbusState.coilsBuffSize < pos)
			{//直接返回错误
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x85,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//返回数据
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(8);
				if(tempData.data == NULL)
				{//返回失败
					unsigned char retData[] = {0x00,0x85,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				if(value == 0x0000) setBit(m_modbusState.coilsBuff[pos >> 3],pos % 8,0);
				if(value == 0xff00) setBit(m_modbusState.coilsBuff[pos >> 3],pos % 8,1);
				value = getBit(m_modbusState.coilsBuff[pos >> 3],pos % 8);
				if(value != 0) value = 0xff00;
				tempData.dataLen = 8;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_SET_ONE_COIL;
				pos += m_modbusState.coilsOffset;
				tempData.data[2] = pos >> 8;
				tempData.data[3] = pos & 0xff;
				tempData.data[4] = value >> 8;
				tempData.data[5] = value & 0xff;
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_SET_ONE_REGISTER:
		{
			unsigned short pos = (data.data[2] << 8) + data.data[3] - m_modbusState.hRegisterOffset;
			unsigned short value = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.hRegisterBuffSize == 0 || m_modbusState.hRegisterBuffSize < pos)
			{//直接返回错误
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x86,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//返回数据
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(8);
				if(tempData.data == NULL)
				{//返回失败
					unsigned char retData[] = {0x00,0x86,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}

				m_modbusState.hRegisterBuff[pos] = value;
				tempData.dataLen = 8;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_SET_ONE_REGISTER;
				pos += m_modbusState.hRegisterOffset;
				tempData.data[2] = pos >> 8;
				tempData.data[3] = pos & 0xff;
				tempData.data[4] = value >> 8;
				tempData.data[5] = value & 0xff;
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_SET_COILS:		///++++++++++
		{
			unsigned short pos = (data.data[2] << 8) + data.data[3] - m_modbusState.coilsOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.coilsBuffSize == 0 || m_modbusState.coilsBuffSize < pos + sum)
			{//直接返回错误
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x8f,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//返回数据
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(8);
				if(tempData.data == NULL)
				{//返回失败
					unsigned char retData[] = {0x00,0x8f,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				//设置值
				int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					temp = getBit(data.data[7 + (i >> 3)],i % 8);
					setBit(m_modbusState.coilsBuff[(i + pos) >> 3],(i + pos) % 8,temp);
				}

				tempData.dataLen = 8;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_SET_COILS;
				pos += m_modbusState.coilsOffset;
				tempData.data[2] = pos >> 8;
				tempData.data[3] = pos & 0xff;
				tempData.data[4] = sum >> 8;
				tempData.data[5] = sum & 0xff;
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_SET_REGISTERS:
		{
			unsigned short pos = (data.data[2] << 8) + data.data[3] - m_modbusState.hRegisterOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.hRegisterBuffSize == 0 || m_modbusState.hRegisterBuffSize < pos + sum)
			{//直接返回错误
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x90,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//返回数据
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(8);
				if(tempData.data == NULL)
				{//返回失败
					unsigned char retData[] = {0x00,0x90,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				//设置值
				//int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					//memcpy(&m_modbusState.hRegisterBuff[pos + i],&data.data[7 + i * 2],2);
					m_modbusState.hRegisterBuff[pos + i] = (data.data[7 + i * 2] << 8) + data.data[7 + i * 2 + 1];
				}

				tempData.dataLen = 8;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_SET_REGISTERS;
				pos += m_modbusState.hRegisterOffset;
				tempData.data[2] = pos >> 8;
				tempData.data[3] = pos & 0xff;
				tempData.data[4] = sum >> 8;
				tempData.data[5] = sum & 0xff;
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	}
}
_XModBusData _XModBusProtocol::popData()
{
	_XModBusData tempData;
	if(m_modbusState.workType == TYPE_MASTER)
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
void _XModBusProtocol::release()
{
	if(!m_isInited) return;
	//等待线程退出
	//if(m_recvThreadState == STATE_START)
	//{
	//	m_recvThreadState = STATE_SET_TO_END;
	//	while(1)
	//	{
	//		if(m_recvThreadState == STATE_END) break;
	//		Sleep(1);
	//	}
	//}
	if(m_sendThreadState == STATE_START)
	{
		m_sendThreadState = STATE_SET_TO_END;
		while(1)
		{
			if(m_sendThreadState == STATE_END) break;
			Sleep(1);
		}
	}
	//清除所有的数据
	for(int i = 0;i < m_sendData.size();++ i)
	{
		XDELETE_ARRAY(m_sendData[i].data);
	}
	m_sendData.clear();
	for(int i = 0;i < m_sendData.size();++ i)
	{
		XDELETE_ARRAY(m_recvData[i].data);
	}
	m_recvData.clear();
//	XDELETE_ARRAY(m_tempDataBuff);
//	m_tempDataLen = 0;

	m_serialPort.close();
	m_isInited = XFalse;
}
//#define MODBUS_MAX_DATA_LEN (1024)
//void _XModBusProtocol::recvDataProc(unsigned char * data,int len)
//{
//	if(len == 0) return;
//
//	if(m_tempDataLen == 0)
//	{//头尚不完整
//		if(m_headLen + len >= m_modbusNeedHeadLen)
//		{//头完整
//			int offset = m_modbusNeedHeadLen - m_headLen;
//			memcpy(m_modbusHead + m_headLen,data,offset);
//			m_headLen = 0;
//			//解析头
//			if(m_modbusState.workType == TYPE_MASTER)
//			{
//				switch(m_modbusHead[1])
//				{
//					//正确返回
//				case CMD_READ_COIL_VOLUME:
//				case CMD_READ_INPUT:
//				case CMD_READ_HOLD_REGISTER:
//				case CMD_READ_INPUT_REGISTER:
//					m_tempDataLen = m_modbusHead[2] + 5;
//					break;
//				case CMD_SET_ONE_COIL:
//				case CMD_SET_ONE_REGISTER:
//				case CMD_SET_COILS:;
//				case CMD_SET_REGISTERS:
//					m_tempDataLen = 8;
//					break;
//					//错误返回
//				case ERROR_READ_COIL_VOLUME:
//				case ERROR_READ_INPUT:
//				case ERROR_READ_HOLD_REGISTER:
//				case ERROR_READ_INPUT_REGISTER:
//				case ERROR_SET_ONE_COIL:
//				case ERROR_SET_ONE_REGISTER:
//				case ERROR_SET_COILS:
//				case ERROR_SET_REGISTERS:
//					m_tempDataLen = 5;
//					break;
//				}
//			}else
//			{
//				switch(m_modbusHead[1])
//				{
//					//正确返回
//				case CMD_READ_COIL_VOLUME:
//				case CMD_READ_INPUT:
//				case CMD_READ_HOLD_REGISTER:
//				case CMD_READ_INPUT_REGISTER:
//				case CMD_SET_ONE_COIL:
//				case CMD_SET_ONE_REGISTER:
//					m_tempDataLen = 8;
//					break;
//
//				case CMD_SET_COILS:
//				case CMD_SET_REGISTERS:
//					m_tempDataLen = m_modbusHead[6] + 9;
//					break;
//				}
//			}
//
//			m_tempDataBuff = createArrayMem<unsigned char>(m_tempDataLen);
//			if(m_tempDataBuff == NULL)
//			{//丢弃数据
//				m_tempDataLen = 0;
//				m_headLen = 0;
//				return;
//			}
//			memcpy(m_tempDataBuff,m_modbusHead,m_modbusNeedHeadLen);
//			m_tempDataOffset = m_modbusNeedHeadLen;
//			recvDataProc(data + offset,len - offset);
//		}else
//		{//头不完整
//			memcpy(m_modbusHead + m_headLen,data,len);
//			m_headLen += len;
//			return;
//		}
//	}else
//	{//头已经完成
//		if(m_tempDataOffset + len >= m_tempDataLen)
//		{//数据包已经完整
//			int need = m_tempDataLen - m_tempDataOffset;
//			memcpy(m_tempDataBuff + m_tempDataOffset,data,need);
//			if(m_tempDataBuff[0] == m_modbusState.deviceID)
//			{//是发给我的数据包则将包推入队列
//				if(m_modbusState.workType == TYPE_MASTER)
//				{//主的方式
//					if(m_needRecv && (m_nowSendData.data[1] == m_tempDataBuff[1] || m_nowSendData.data[1] == m_tempDataBuff[1] + 0x80))
//					{//正确的应答(这里逻辑不严密)
//						_XModBusData tempData;
//						tempData.data = m_tempDataBuff;
//						tempData.dataLen = m_tempDataLen;
//						tempData.delayTime = m_delayTime;
//						//这里要进行CRC16校验
//						unsigned short crcCheck = CRC16(tempData.data,tempData.dataLen - 2);
//						if(tempData.data[tempData.dataLen - 1] != (crcCheck >> 8)
//							|| tempData.data[tempData.dataLen - 2] != (crcCheck & 0xff))
//						{//校验失败丢弃数据
//							XDELETE_ARRAY(m_tempDataBuff);
//						}else
//						{//校验成功
//							m_recvMutex.Lock();
//							m_recvData.push_back(tempData);
//							m_recvMutex.Unlock();
//							m_sendMutex.Lock();
//							m_needRecv = XFalse;
//							XDELETE_ARRAY(m_nowSendData.data);
//							m_sendMutex.Unlock();
//						}
//					}else
//					{//多余的回答直接丢弃数据
//						XDELETE_ARRAY(m_tempDataBuff);
//					}
//				}else
//				{//从的方式(直接处理接收到的数据，不推入队列)(尚未完成)
//					_XModBusData tempData;
//					tempData.data = m_tempDataBuff;
//					tempData.dataLen = m_tempDataLen;
//					tempData.delayTime = m_delayTime;
//					for(int i = 0;i < tempData.dataLen;++ i)
//					{
//						printf("0x%02x ",tempData.data[i]);
//					}
//					printf(" - \n");
//					answerProc(tempData);
//					//释放内存空间
//					XDELETE_ARRAY(m_tempDataBuff);
//				}
//			}else
//			{//丢弃数据包
//				XDELETE_ARRAY(m_tempDataBuff);
//			}
//			//继续处理余下的数据
//			len -= need;
//			m_tempDataOffset = 0;
//			m_tempDataLen = 0;
//			m_tempDataBuff = NULL;
//			recvDataProc(data + need,len);
//
//			return;
//		}else
//		{//数据包尚不完整
//			memcpy(m_tempDataBuff + m_tempDataOffset,data,len);
//			m_tempDataOffset += len;
//			return;
//		}
//	}
//}
//DWORD WINAPI _XModBusProtocol::recvThread(void * pParam)
//{
//	_XModBusProtocol *pPar = (_XModBusProtocol *)pParam;
//	pPar->m_recvThreadState = STATE_START;
//	unsigned char tempBuff[MODBUS_MAX_DATA_LEN];	//假设数据长度不会超过
//	int ret = 0;
//	while(1)
//	{
//		if(pPar->m_recvThreadState == STATE_SET_TO_END) break;
//		//if(!pPar->m_needRecv) continue;
//		//这里接受数据
//		ret = pPar->m_serialPort.readData(tempBuff,MODBUS_MAX_DATA_LEN);
//		if(ret > 0)
//		{//读取到有效数据
//			pPar->recvDataProc(tempBuff,ret);
//		}else
//		if(ret < 0)
//		{
//			pPar->m_connectState = XFalse;
//			break;
//		}
//		Sleep(1);
//	}
//	pPar->m_recvThreadState = STATE_END;
//	return 0;
//}
void _XModBusProtocol::sendNowData()
{
	++ m_sendTime;
	unsigned short crcCheck = CRC16(m_nowSendData.data,m_nowSendData.dataLen - 2);	//最后两个字节为校验位
	m_nowSendData.data[0] = m_modbusState.deviceID;
	m_nowSendData.data[m_nowSendData.dataLen - 1] = (crcCheck >> 8);
	m_nowSendData.data[m_nowSendData.dataLen - 2] = (crcCheck & 0xff);
	printf("%d : ",m_sendTime);
	for(int i = 0;i < m_nowSendData.dataLen;++ i)
	{
		printf("0x%02x ",m_nowSendData.data[i]);
	}
	printf(" + \n");
	m_serialPort.sendData(m_nowSendData.data,m_nowSendData.dataLen);
	m_needRecv = XTrue;
	m_delayTime = 0;
}
DWORD WINAPI _XModBusProtocol::sendThread(void * pParam)
{
	_XModBusProtocol *pPar = (_XModBusProtocol *)pParam;
	pPar->m_sendThreadState = STATE_START;
	int upTime = getCurrentTicks();
	while(1)
	{
		if(pPar->m_sendThreadState == STATE_SET_TO_END) break;
		if(pPar->m_modbusState.workType == TYPE_MASTER)
		{
			pPar->m_sendMutex.Lock();
			if(pPar->m_needRecv)  
			{
				pPar->m_delayTime += getCurrentTicks() - upTime;
				upTime = getCurrentTicks();

				if(pPar->m_delayTime >= pPar->m_modbusState.outTime)
				{//如果超时则重新发送
					if(pPar->m_sendTime == pPar->m_modbusState.maxResendTimes)
					{
						printf("超过重发次数\n");
						pPar->m_connectState = XFalse;
						break;
					}else
					{//重发
						pPar->sendNowData();
					}
				}
				pPar->m_sendMutex.Unlock();
				continue;
			}else
			{
				pPar->m_sendMutex.Unlock();

				upTime = getCurrentTicks();
			}
		}
		//这里接收数据
		pPar->m_sendMutex.Lock();
		if(pPar->m_sendData.size() > 0)
		{//有数据需要发送,这里发送数据
			pPar->m_nowSendData = pPar->m_sendData[0];
			pPar->m_sendData.pop_front();
			pPar->m_sendMutex.Unlock();
			pPar->m_sendTime = 0;
			pPar->sendNowData();
			if(pPar->m_modbusState.workType == TYPE_SLAVE)
			{//做从时发送一次就丢弃数据
				XDELETE_ARRAY(pPar->m_nowSendData.data);
			}
		}else
		{
			pPar->m_sendMutex.Unlock();
		}
		Sleep(1);
	}
	pPar->m_sendThreadState = STATE_END;
	return 0;
}
int getModbusCMDDataSum(const _XModBusData &CMD)	//从命令中解析数据数量
{
	if(CMD.dataLen <= 0) return 0;
	switch(CMD.data[1])
	{
	case CMD_READ_COIL_VOLUME:
	case CMD_READ_INPUT:
	case CMD_READ_HOLD_REGISTER:
	case CMD_READ_INPUT_REGISTER:
	case CMD_SET_ONE_COIL:
	case CMD_SET_ONE_REGISTER:
	case CMD_SET_COILS:
		return 0;
		break;
	case CMD_SET_REGISTERS:
		return (CMD.data[4] << 8) + CMD.data[5];	//上面的都不支持
		break;
	}
	return 0;
}
bool getModbusCMDAddress(const _XModBusData &CMD,unsigned int &addr)
{//根据协议获取命令的操作地址
	if(CMD.dataLen <= 0) return false;
	switch(CMD.data[1])
	{
	case CMD_READ_COIL_VOLUME:
	case CMD_READ_INPUT:
	case CMD_READ_HOLD_REGISTER:
	case CMD_READ_INPUT_REGISTER:
	case CMD_SET_ONE_COIL:
	case CMD_SET_ONE_REGISTER:
	case CMD_SET_COILS:
	case CMD_SET_REGISTERS:
		addr = (CMD.data[2] << 8) + CMD.data[3];
		break;
	}
	return true;
}
bool getModbusCMDData(const _XModBusData &CMD,int index,unsigned int &data)
{//根据协议获取命令的操作地址
	if(CMD.dataLen <= 0) return false;
	if(index < 0 || index >= getModbusCMDDataSum(CMD)) return false;
	switch(CMD.data[1])
	{
	case CMD_READ_COIL_VOLUME:
	case CMD_READ_INPUT:
	case CMD_READ_HOLD_REGISTER:
	case CMD_READ_INPUT_REGISTER:
	case CMD_SET_ONE_COIL:
	case CMD_SET_ONE_REGISTER:
	case CMD_SET_COILS:
		return false;	//上面的都不支持
		break;
	case CMD_SET_REGISTERS:
		data = (CMD.data[7 + (index << 1)] << 8) + CMD.data[8 + (index << 1)];
		return true;
		break;
	}
	return true;
}