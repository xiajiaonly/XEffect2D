#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XCircleGroup.h"
namespace XE{
template<class type>
XBool XCircleGroup<type>::init(int buffsize,int mode)    //初始化，并分配内存空间
{
	if(m_isInited ||    //如果已经初始化，则不能重复初始化
		buffsize <= 0) return XFalse;        //0尺寸是不行滴
	m_insertPoint = 0;                //当前的插入点
	m_popPoint = 0;                    //当前取出点
	m_buffSize = buffsize;            //整个buff的大小
	m_pElement = NULL;                //元素队列的指针
	m_mode = mode;
	m_pElement = XMem::createArrayMem<XElement>(m_buffSize);
	if(m_pElement == NULL) return XFalse;    //内存分配失败
	//初始化所有数据
	for(int i = 0;i < m_buffSize;++ i)
	{
		m_pElement[i].isEnable = XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
template<class type>
int XCircleGroup<type>::insertOneElement(type &element)    //插入一个元素
{
	if(!m_isInited) return 0;
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
template<class type>
XBool XCircleGroup<type>::popOneElement(type &element)    //取出一个元素
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
}