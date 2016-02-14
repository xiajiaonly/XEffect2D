#include "XStdHead.h"
#include "XMemory.h"
namespace XE{
namespace XMem
{
	unsigned char *spliceData(unsigned char * baseBuff, int &baseLen, int &offset, const unsigned char * addBuff, int len)
	{
		if (addBuff == NULL) return baseBuff;
		if (offset + len > baseLen || baseBuff == NULL)
		{//数据超过长度
			if (baseLen == 0) baseLen = 1;
			while (true)
			{
				baseLen = baseLen << 1;
				if (offset + len <= baseLen) break;
			}
			unsigned char *temp = XMem::createArrayMem<unsigned char>(baseLen);
			if (baseBuff != NULL) memcpy(temp, baseBuff, offset);
			XMem::XDELETE_ARRAY(baseBuff);
			memcpy(temp + offset, addBuff, len);
			offset += len;
			return temp;
		}
		else
		{//数据没有超过长度
			memcpy(baseBuff + offset, addBuff, len);
			offset += len;
			return baseBuff;
		}
	}
}
	bool XBuffer::pushData(const unsigned char *data,int len)	//向队列中添加数据
	{
		if(data == NULL || len <= 0) return false;
		//下面将数据加入
		if(m_buffSize - m_usage < len)
		{//内存空间不够，这里需要分配内存空间
			if(!XMem::resizeMem(m_pData,m_usage,len + m_usage)) return false;
			m_buffSize = len + m_usage;
		}
		memcpy(m_pData + m_usage,data,len);
		m_usage += len;
		return true;
	}
	bool XBuffer::pushAData(int data)
	{
		//下面将数据加入
		if(m_buffSize - m_usage < 1)
		{//内存空间不够，这里需要分配内存空间
			if(!XMem::resizeMem(m_pData,m_usage,1 + m_usage)) return false;
			m_buffSize = 1 + m_usage;
		}
		m_pData[m_usage] = data;
		//memcpy(m_pData + m_usage,data,len);
		++ m_usage;
		return true;
	}
	bool XBuffer::resize(int size)
	{
		if(size <= 0) return false;
		if(size == m_buffSize) return true;
		unsigned char *tmp = m_pData;
		m_pData = XMem::createArrayMem<unsigned char>(size);
		if(m_pData == NULL)
		{//内存分配失败
			m_pData = tmp;
			return false;
		}
		//目前的策略，丢弃以前的数据，比较合适的策略是尽可能多的保存原有的数据(尚未实现)
		m_usage = 0;
		XMem::XDELETE_ARRAY(tmp);
		m_buffSize = size;
		return true;
	}
	bool XBuffer::decreaseData(int len)
	{
		if(len <= 0 || len > m_usage) return false;	//非法的数据
		if(len == m_usage)
		{
			m_usage = 0;
			return true;
		}
		memmove(m_pData,m_pData + len,m_usage - len);
		m_usage -= len;
		return true;
	}
}