#ifndef _JIA_XCIRCLEGROUP_
#define _JIA_XCIRCLEGROUP_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
/****************************************
说明：
这个数据结构使用固定的内存大小
连续的内存空间
先进先出的数据顺序
满队列抛弃的累积处理机制
多线程安全
优化：
可以设计两种方案（这样会带来设计的复杂度骤增，需要慎重考虑）
1、一个数据输入源、一个数据输出源，这种方式尽量不使用互斥锁做到多线程安全
2、多个数据输入源，多个数据输出源，这个必须要使用互斥锁才能保证多线程安全
测试结果：
1、一个数据输入源、一个数据输出源，不抛弃数据，这种方式不使用互斥锁是安全的
2、多个数据输入源，多个数据输出源，这个必须要使用互斥锁才能保证多线程安全
****************************************/
#include "XBasicClass.h"
#include "XOSDefine.h"
#ifdef XEE_OS_LINUX
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#endif

template<class Type> class _XCircleGroup
{
private:
    _XBool m_isInited;    //是否初始化
    int m_insertPoint;    //当前的插入点
    int m_popPoint;        //当前取出点
    int m_buffSize;        //整个buff的大小
    
    struct _XElement    //元素的结构
    {
        _XBool isEnable;
        Type elementData;
    };
    _XElement *m_pElement;    //元素队列的指针
	pthread_mutex_t m_mutex;  
    int m_mode;            //拥堵处理模式：0：拥堵抛弃最老的数据 1：拥堵等待
private:
	_XCircleGroup(const _XCircleGroup & temp)	//重载拷贝构造函数防止不必要的错误
	{
	}
public:
    _XCircleGroup()        //构造函数
    :m_isInited(XFalse)
    ,m_insertPoint(0)    //当前的插入点
    ,m_popPoint(0)        //当前取出点
    ,m_buffSize(0)        //整个buff的大小
    ,m_pElement(NULL)    //元素队列的指针
    ,m_mode(0)
    {
    }
	_XBool init(int buffsize,int mode = 0)    //初始化，并分配内存空间
	{
		if(m_isInited) return XFalse;    //如果已经初始化，则不能重复初始化
		if(buffsize <= 0) return XFalse;        //0尺寸是不行滴
		m_insertPoint = 0;                //当前的插入点
		m_popPoint = 0;                    //当前取出点
		m_buffSize = buffsize;            //整个buff的大小
		m_pElement = NULL;                //元素队列的指针
		m_mode = mode;
		m_pElement = createArrayMem<_XElement>(m_buffSize);
		if(m_pElement == NULL) return XFalse;    //内存分配失败
		//初始化所有数据
		for(int i = 0;i < m_buffSize;++ i)
		{
			m_pElement[i].isEnable = XFalse;
		}

		m_isInited = XTrue;
		return XTrue;
	}
	int insertOneElement(Type &element)    //插入一个元素
	{
		if(!m_isInited) return XFalse;
		pthread_mutex_lock(m_mutex);
		if(!m_pElement[m_insertPoint].isEnable)
		{//有数据可以取出，则取出数据，并作相应的处理
			m_pElement[m_insertPoint].elementData = element;    //注意这里是使用的是赋值操作符，这个在内部成员中有指针操作是会出现错误
			m_pElement[m_insertPoint].isEnable = XTrue;
			++ m_insertPoint;
			if(m_insertPoint >= m_buffSize) m_insertPoint = 0;
			pthread_mutex_unlock(m_mutex);
			return 1;
		}else
		{
	//        printf("buff is full!");
			if(m_mode == 0)
			{
	//            printf(" some data will be demp!\n");
				//满buff进行数据丢弃
				m_pElement[m_insertPoint].elementData = element;    //注意这里是使用的是赋值操作符，这个在内部成员中有指针操作是会出现错误
				m_pElement[m_insertPoint].isEnable = XTrue;
				++ m_insertPoint;
				if(m_insertPoint >= m_buffSize) m_insertPoint = 0;
				++ m_popPoint;
				if(m_popPoint >= m_buffSize) m_popPoint = 0;
			}
	//        printf("\n");
			pthread_mutex_unlock(m_mutex);
			return 2;
		}
	}
	_XBool popOneElement(Type &element)    //取出一个元素
	{
		if(!m_isInited) return XFalse;
		pthread_mutex_lock(m_mutex);
		if(m_pElement[m_popPoint].isEnable != 0)
		{//有数据可以取出，则取出数据，并作相应的处理
			element = m_pElement[m_popPoint].elementData;    //注意这里是使用的是赋值操作符，这个在内部成员中有指针操作是会出现错误
			m_pElement[m_popPoint].isEnable = XFalse;
			++ m_popPoint;
			if(m_popPoint >= m_buffSize) m_popPoint = 0;
			pthread_mutex_unlock(m_mutex);
			return XTrue;
		}
		pthread_mutex_unlock(m_mutex);
		return XFalse;
	}
    _XBool isEmpty() const   //是否为空
    {
        if(!m_isInited) return XTrue;
        return !m_pElement[m_popPoint].isEnable;    //没有可以取出的数据了
    }
	_XBool isFull() const   //是否为满
	{
		if(!m_isInited) return XFalse;
		return m_pElement[m_insertPoint].isEnable;    //没有可以插入的空间了
	}
};
#endif

/********************************************
//for test

#include "stdafx.h"
#include "windows.h"
#include "stdlib.h"
#include "math.h"
#include "XCircleGroup.h"

struct _data
{
    int data1[1024];
    _data& operator=(_data d) 
    {
        if ( &d == this ) return *this;
        for(int i = 0;i < 1024;++ i)
        {
            data1[i] = d.data1[i];
            Sleep(1);
        }
        return *this;
    }
};
_XCircleGroup <_data> tempBuff;

DWORD WINAPI thread_d(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(1)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI thread_e(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(1)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI thread_f(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(1)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}

//测试结果，必须要主线程和辅助线程中都加锁，才能保证不会在锁的范围内跳转。
int main(int avg,char* arg)
{
    HANDLE temp;
    DWORD dw;
    tempBuff.init(100,1);
    temp = CreateThread(NULL,NULL,thread_d,NULL,0,&dw);
    temp = CreateThread(NULL,NULL,thread_e,NULL,0,&dw);
    temp = CreateThread(NULL,NULL,thread_f,NULL,0,&dw);
    _data data;
    int oldData = 0;
    while(1)
    {
        if(tempBuff.popOneElement(&data) == 1)
        {
            for(int i = 1;i < 1024;++ i)
            {
                if(data.data1[0] != data.data1[i])
                {
                    printf("error!\n");
                }
            }
            oldData = data.data1[0];
        }
        Sleep(1);
    }
    return 1;
}

*********************************************/
