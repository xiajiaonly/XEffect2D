#ifndef _JIA_XMEMORY_
#define _JIA_XMEMORY_
//这里封装所有关于内存操作的相关函数
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "string.h"
namespace XE {
	//#define XMem::XDELETE_ARRAY(p) { if (p != NULL) { delete [] p; p = NULL; } }
	//#define XMem::XDELETE(p) { if (p != NULL) { delete p; p = NULL; } }
	//#define FILE_ERROR(p) {fclose(p);return XFalse;}
	//自己定义new和delete
	//A* a = (A*)malloc(sizeof(A));
	//new (a) A();	//这样可以吗？
	//
	//a->~A();
	//free(a);
	namespace XMem
	{//内存管理的相关方法
		template<typename T>
		inline void XDELETE(T *&p)
		{
			if (p == NULL) return;
			delete p;
			p = NULL;
		}
		template<typename T>
		inline void XDELETE_ARRAY(T *&p)
		{
			if (p == NULL) return;
			delete[] p;
			p = NULL;
		}
		template<typename T>
		inline T * createMem()
		{
			try
			{
				return new T;
			}
			catch (...)
			{
				return NULL;
			}
		}
		template<typename T>
		inline T * createArrayMem(int size)
		{
			if (size <= 0) return NULL;
			try
			{
				return new T[size];
			}
			catch (...)
			{
				return NULL;
			}
		}

		//将addBuff的数据接到baseBuff之后，如果内存空间不够则分配足够的内存空间
		extern unsigned char *spliceData(unsigned char * baseBuff, int &baseLen, int &offset,
			const void * addBuff, int len);
		//将p的内存大小改成size，并保留usage的数据
		template<typename T>
		bool resizeMem(T *&p, int usage, int size)
		{
			if (size <= 0 || usage > size) return false;	//无效的数据
			T * tmp = XMem::createArrayMem<T>(size);
			if (tmp == NULL) return false;	//内存分配失败，什么也不改变
			if (usage > 0)
				memcpy(tmp, p, usage * sizeof(T));
			XMem::XDELETE_ARRAY(p);
			p = tmp;
			return true;
		}
		extern void randomMem(void *mem, int size);	//混乱一块内存中的数据
	}
	//用于存储的数据结构
	class XBuffer
	{
	private:
		unsigned char* m_pData;	//存储空间的指针
		int m_buffSize;			//存储的大小
		int m_usage;			//空间的使用量
		//这个类需要禁止拷贝构造函数和复制操作符
		XBuffer(const XBuffer &temp);
		XBuffer& operator = (const XBuffer& temp);
	public:
		XBuffer()
			:m_pData(NULL)
			, m_buffSize(0)
			, m_usage(0)
		{}
		~XBuffer() { release(); }
		int getBuffSize()const { return m_buffSize; }
		int getUsage()const { return m_usage; }
		//这个函数不要随便调用，一定要在明确操作意图时才调用，容易造成内部数据错误
		void setUsage(int u)
		{
			if (u > m_buffSize) m_usage = m_buffSize;
			else m_usage = u;
		}
		//向队列中推入指定数据
		bool pushData(const void *data, int len);	//向队列中添加数据
		bool pushAData(unsigned char data);
		//只分配大小，不做内容拷贝
		bool resize(int size);
		//保留原有内容
		bool resizeEx(int size);
		void release()
		{
			XMem::XDELETE_ARRAY(m_pData);
			m_buffSize = 0;
			m_usage = 0;
		}
		//只分配大小，不做内容拷贝
		void fitSize(int size)	//这个函数不考虑已经使用的内存空间，策略是直接丢弃
		{
			if (size <= 0 || m_buffSize >= size) return;	//如果任然有足够的内存空间则不需要额外分配
			resize(size);
		}
		//保留原有内容
		void fitSizeEx(int size)	//这个函数不考虑已经使用的内存空间，策略是直接丢弃
		{
			if (size <= 0 || m_buffSize >= size) return;	//如果任然有足够的内存空间则不需要额外分配
			resizeEx(size);
		}
		//从队列中取出指定数据
		bool popData(unsigned char *data, int len)
		{
			if (data == NULL || len <= 0) return false;
			if (len > m_usage) return false;
			memcpy(data, m_pData, len);
			return decreaseData(len);
		}
		//下面是另一种调用方式
		const unsigned char *getBuffer()const { return m_pData; }
		unsigned char *getBuffer() { return m_pData; }	//这是一种比较危险的使用方法，请谨慎使用这个接口
		bool decreaseData(int len);
		void clear() { m_usage = 0; }
	};
}
#endif