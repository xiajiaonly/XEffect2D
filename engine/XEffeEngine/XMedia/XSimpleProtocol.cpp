#include "XSimpleProtocol.h"

void simpleProctocolRecvCB(void *pClass,unsigned char * data,int len)
{
	_XSimpleProtocol &pPar = *(_XSimpleProtocol *)pClass;
	pPar.recvDataProc(data,len);	//下面解析数据
}
void _XSimpleProtocol::recvDataProc(unsigned char *data,int len)
{
	if(data == NULL || len <= 0) return;
	if(m_recvedDataLen != 0)
	{//原有数据不完整
		switch(m_recvedDataLen)
		{
		case 1:
			if(data[0] >= 0x80)
			{//之前收到的数据是不完整的包
				printf("Error Data:0x%02x\n",m_recvedData[0]);
				m_recvedDataLen = 0;
				recvDataProc(data,len);
			}else
			{//正确的数据
				m_recvedData[1] = data[0];
				++ m_recvedDataLen;
				recvDataProc(data + 1,len - 1);
			}
			break;
		case 2:
			if(data[0] >= 0x80)
			{//之前收到的数据是不完整的包
				printf("Error Data:0x%02x,0x%02x\n",m_recvedData[0],m_recvedData[1]);
				m_recvedDataLen = 0;
				recvDataProc(data,len);
			}else
			{//正确的数据
				m_recvedData[2] = data[0];
				//收到一个完整的数据包
				m_data[m_recvedData[0] - 0xa0] = (m_recvedData[1] << 7) + m_recvedData[2];
				if(m_callBackFun != NULL)
				{
					_XSimProData tmp;
					tmp.type = SIMPRO_TYPE_DATA;
					tmp.index = m_recvedData[0] - 0xa0;
					tmp.data = (m_recvedData[1] << 7) + m_recvedData[2];
					m_callBackFun(tmp,m_pClass);
				}

				m_recvedDataLen = 0;;
				recvDataProc(data + 1,len - 1);
			}
			break;
		}
	}else
	{//没有堆叠数据
		if(data[0] >= 0x80)
		{//有效的数据
			if((data[0] & 0xf0) >= 0xA0)
			{//数据数据
				m_recvedData[0] = data[0];
				++ m_recvedDataLen;
				recvDataProc(data + 1,len - 1);
			}else
			{//状态数据
				m_state[(data[0] & 0x0f)] = ((data[0] & 0xf0) == 0x80)?1:0;
				if(m_callBackFun != NULL)
				{
					_XSimProData tmp;
					tmp.type = SIMPRO_TYPE_STATE;
					tmp.index = (data[0] & 0x0f);
					tmp.data = ((data[0] & 0xf0) == 0x80)?1:0;
					m_callBackFun(tmp,m_pClass);
				}
			}
		}else
		{//不完整的头数据
			printf("Error Data:");
			for(int i = 0;i < len;++ i)
			{
				if(data[i] >= 0x80)
				{
					printf("\n");
					recvDataProc(data + i,len - i);
					return;
				}else
				{//错误的数据
					printf("0x%02x ",data[i]);
				}
			}
			//所有数据都被抛弃
			printf("\n");
		}
	}
}
DWORD WINAPI _XSimpleProtocol::sendThread(void * pParam)
{
	_XSimpleProtocol &pPar = *(_XSimpleProtocol *)pParam;
	pPar.m_sendThreadState = STATE_START;

	while(true)
	{
		if(pPar.m_sendThreadState == STATE_SET_TO_END) break;
		//这里发送数据
		if(pPar.m_sendData.size() > 0)
		{//这里发送一个数据
			pPar.m_sendMutex.Lock();
			_XSimProData tmp = pPar.m_sendData[0];
			pPar.m_sendData.pop_front();
			pPar.m_sendMutex.Unlock();
			//下面发送tmp
			pPar.sendData(tmp);
		}

		Sleep(1);
	}
	pPar.m_sendThreadState = STATE_END;
	return 0;
}
void _XSimpleProtocol::sendData(const _XSimProData&tmp)
{
	if(tmp.type == SIMPRO_TYPE_STATE)
	{
		if(tmp.index < 0 || tmp.index >= getMaxStateSum()) return;	//数据不合法
		unsigned char data;
		if(tmp.data == 0)
		{
			data = 0x90 + tmp.index;
		}else
		{
			data = 0x80 + tmp.index;
		}
		m_serialPort.sendData(&data,1);
	}else
	{
		if(tmp.index < 0 || tmp.index >= getMaxDataSum()) return;	//数据不合法
		unsigned char data[3];
		data[0] = tmp.index + 0xa0;
		data[1] = (tmp.data / 128) % 128;
		data[2] = (tmp.data) % 128;
		m_serialPort.sendData(data,3);
	}
}