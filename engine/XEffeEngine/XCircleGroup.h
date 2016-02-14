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
//#include "XThread.h"
#include "XCritical.h"
#ifdef XEE_OS_LINUX
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#endif
namespace XE{
template<class Type> class XCircleGroup
{
private:
    XBool m_isInited;    //是否初始化
    int m_insertPoint;    //当前的插入点
    int m_popPoint;        //当前取出点
    int m_buffSize;        //整个buff的大小
    
    struct XElement    //元素的结构
    {
        XBool isEnable;
        Type elementData;
    };
    XElement *m_pElement;    //元素队列的指针
	pthread_mutex_t m_mutex;  
    int m_mode;            //拥堵处理模式：0：拥堵抛弃最老的数据 1：拥堵等待
private:
	XCircleGroup(const XCircleGroup & temp){}	//重载拷贝构造函数防止不必要的错误
public:
    XCircleGroup()        //构造函数
    :m_isInited(XFalse)
    ,m_insertPoint(0)    //当前的插入点
    ,m_popPoint(0)        //当前取出点
    ,m_buffSize(0)        //整个buff的大小
    ,m_pElement(NULL)    //元素队列的指针
    ,m_mode(0)
    {}
	XBool init(int buffsize,int mode = 0);    //初始化，并分配内存空间
	int insertOneElement(Type &element);    //插入一个元素
	XBool popOneElement(Type &element);    //取出一个元素
    XBool isEmpty() const   //是否为空
    {
        if(!m_isInited) return XTrue;
        return !m_pElement[m_popPoint].isEnable;    //没有可以取出的数据了
    }
	XBool isFull() const   //是否为满
	{
		if(!m_isInited) return XFalse;
		return m_pElement[m_insertPoint].isEnable;    //没有可以插入的空间了
	}
};
}
#endif

/********************************************
//for test

#include "stdafx.h"
//#include "windows.h"
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
XCircleGroup <_data> tempBuff;

DWORD WINAPI thread_d(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(true)
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
    while(true)
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
    while(true)
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
    while(true)
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
