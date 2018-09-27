#ifndef _JIA_XSMARTPOINTER_
#define _JIA_XSMARTPOINTER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.15
//--------------------------------
#include "stdio.h"
//#include "XBasicFun.h"
namespace XE{
//非数组类型
template<class T> class XSmartP;

template<class T> class XBackP  
{  
private:
    friend XSmartP<T>;  
    T *m_p;			//实际的指针
    size_t m_counter;			//引用计数器
    XBackP(T *p) 
		:m_p(p)
		,m_counter(1)  
    {  
	//	printf("XBackP constructor called !\n");  
    }  
public:
    ~XBackP()  
    {  
        XMem::XDELETE(m_p);  
	//	printf( "XBackP distructor called !\n");  
    }  
};    
template<class T> class XSmartP  
{  
public:  
    // 构造函数：p是指向已经动态创建的T对象指针  
    XSmartP(T *p) 
		:m_backP(new XBackP<T>(p))
    {  
	//	printf("XSmartP constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    XSmartP() 
		:m_backP(new XBackP<T>(new T()))
    {  
	//	printf("XSmartP constructor called ! use = %d\n",m_backP->m_counter); 
    }  
  
    // 复制构造函数：复制成员并将使用计数加1  
    XSmartP(const XSmartP& temp) 
		:m_backP(temp.m_backP)
    {  
		++m_backP->m_counter;    
	//	printf("XSmartP copy constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // 赋值操作符  
    XSmartP<T>& operator=(const XSmartP<T>&temp)
	{  
		if(this == &temp) return *this;		//防止自生拷贝
		// 增加右操作数中的使用计数  
		++temp.m_backP->m_counter;  
		// 将左操作数对象的使用计数减1，若该对象的使用计数减至0，则删除该对象  
		if (--m_backP->m_counter == 0)  
		{
			XMem::XDELETE(m_backP);
		}
		m_backP = temp.m_backP;   // 复制m_backP指针  
		return *this;  
	}   
    // 析构函数：如果计数为0，则删除m_backP对象  
    ~XSmartP()  
    {  
	//	printf("XSmartP distructor called ! use = %d\n",m_backP->m_counter); 
		if (--m_backP->m_counter == 0)  
		{
			XMem::XDELETE(m_backP);
		}
    }  
    // 获取数据成员  
    T *getPtr() const  
    {  
        return m_backP->m_p;  
    } 
//	// 修改数据成员  
//	void setPtr(T *p) const  
//	{  
//		m_backP->m_p = p;  
//	}
    // 返回或修改对象  
    T getVal() const  
    {  
        return *m_backP->m_p;  
    }  
    void setVal(T val)  
    {  
        *m_backP->m_p = val;  
    }  
private:  
    XBackP<T> *m_backP;   //指向使用计数类m_backP 
};  

//下面是关于数组的定义
template<class T> class XSmartPArray;

template<class T> class XBackPArray  
{  
private:
    friend XSmartPArray<T>;  
    T *m_p;			//实际的指针
    size_t m_counter;			//引用计数器
    XBackPArray(T p[]) 
		:m_p(p)
		,m_counter(1)  
    {  
	//	printf("XBackPArray constructor called !\n");  
    }  
public:
    ~XBackPArray()  
    {  
        XMem::XDELETE_ARRAY(m_p);  
	//	printf( "XBackPArray distructor called !\n");  
    }  
};  
  
template<class T> class XSmartPArray  
{  
public:  
	XSmartPArray()
		:m_backP(new XBackPArray<T>(new T[2]))
		,m_size(2)
	{
	//	printf("XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
	}
    // 构造函数：p是指向已经动态创建的int对象指针  
    XSmartPArray(T p[],int size) 
		:m_backP(new XBackPArray<T>(p))
		,m_size(size)
    {  
	//	printf("XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    XSmartPArray(int size) 
		:m_backP(new XBackPArray<T>(new T[size]))
		,m_size(size)
    {  
	//	printf("XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // 复制构造函数：复制成员并将使用计数加1  
    XSmartPArray(const XSmartPArray& temp) 
		:m_backP(temp.m_backP)
		,m_size(temp.m_size)
    {  
		++m_backP->m_counter;    
	//	printf("XSmartPArray copy constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // 赋值操作符  
    XSmartPArray<T>& operator=(const XSmartPArray<T>&temp)
	{  
		if(this == &temp) return *this;		//防止自生拷贝
		// 增加右操作数中的使用计数  
		++temp.m_backP->m_counter;  
		// 将左操作数对象的使用计数减1，若该对象的使用计数减至0，则删除该对象  
		if (--m_backP->m_counter == 0)  
		{
			XMem::XDELETE(m_backP);
		}
		m_backP = temp.m_backP;   // 复制m_backP指针  
		m_size = temp.m_size;
		return *this;  
	}   
 	T& operator[](int order)const
	{
	//	if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];
	}
 	T& operator[](int order)
	{
	//	if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];
	}
    // 析构函数：如果计数为0，则删除m_backP对象  
    ~XSmartPArray()  
    {  
	//	printf("XSmartPArray distructor called ! use = %d\n",m_backP->m_counter); 
		if (--m_backP->m_counter == 0)  
		{
			XMem::XDELETE(m_backP);
		}
    }  
    // 获取数据成员  
    T *getPtr() const  
    {  
        return m_backP->m_p;  
    } 
//	// 修改数据成员  
//	void setPtr(T *p) const  
//	{  
//		m_backP->m_p = p;  
//	}
    // 返回或修改对象  
    T getVal(int order) const  
    {  
		if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];  
    }  
    void setVal(T val,int order)  
    {  
		if(order < 0 || order >= m_size) order = 0;
        m_backP->m_p[order] = val;  
    }  
private:  
    XBackPArray<T> *m_backP;   //指向使用计数类m_backP 
	int m_size;
}; 
}
#endif