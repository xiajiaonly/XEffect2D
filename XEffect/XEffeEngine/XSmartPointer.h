#ifndef _JIA_XMUSIC_
#define _JIA_XMUSIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.15
//--------------------------------
#include "stdio.h"
#include "XBasicFun.h"
//非数组类型
template<class T> class _XSmartP;

template<class T> class _XBackP  
{  
private:
    friend _XSmartP<T>;  
    T *m_p;			//实际的指针
    size_t m_counter;			//引用计数器
    _XBackP(T *p) 
		:m_p(p)
		,m_counter(1)  
    {  
		printf("_XBackP constructor called !\n");  
    }  
    ~_XBackP()  
    {  
        XDELETE(m_p);  
		printf( "_XBackP distructor called !\n");  
    }  
};  
  
template<class T> class _XSmartP  
{  
public:  
    // 构造函数：p是指向已经动态创建的int对象指针  
    _XSmartP(T *p) 
		:m_backP(new _XBackP<T>(p))
    {  
		printf("_XSmartP constructor called ! use = %d\n",m_backP->m_counter); 
    }  
  
    // 复制构造函数：复制成员并将使用计数加1  
    _XSmartP(const _XSmartP& temp) 
		:m_backP(temp.m_backP)
    {  
		++m_backP->m_counter;    
		printf("_XSmartP copy constructor called ! use = %d\n",m_backP->m_counter); 
    }  
  
    // 赋值操作符  
    _XSmartP<T>& operator=(const _XSmartP<T>&temp)
	{  
		if(this == &temp) return *this;		//防止自生拷贝
		// 增加右操作数中的使用计数  
		++temp.m_backP->m_counter;  
		// 将左操作数对象的使用计数减1，若该对象的使用计数减至0，则删除该对象  
		if (--m_backP->m_counter == 0)  
		{
			XDELETE(m_backP);
		}
		m_backP = temp.m_backP;   // 复制U_Ptr指针  
		return *this;  
	}   
  
    // 析构函数：如果计数为0，则删除U_Ptr对象  
    ~_XSmartP()  
    {  
		printf("_XSmartP distructor called ! use = %d\n",m_backP->m_counter); 
		if (--m_backP->m_counter == 0)  
		{
			XDELETE(m_backP);
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
    _XBackP<T> *m_backP;   //指向使用计数类U_Ptr 
};  

//下面是关于数组的定义
template<class T> class _XSmartPArray;

template<class T> class _XBackPArray  
{  
private:
    friend _XSmartPArray<T>;  
    T *m_p;			//实际的指针
    size_t m_counter;			//引用计数器
    _XBackPArray(T p[]) 
		:m_p(p)
		,m_counter(1)  
    {  
		printf("_XBackPArray constructor called !\n");  
    }  
    ~_XBackPArray()  
    {  
        XDELETE_ARRAY(m_p);  
		printf( "_XBackPArray distructor called !\n");  
    }  
};  
  
template<class T> class _XSmartPArray  
{  
public:  
    // 构造函数：p是指向已经动态创建的int对象指针  
    _XSmartPArray(T p[],int size) 
		:m_backP(new _XBackPArray<T>(p))
		,m_size(size)
    {  
		printf("_XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
    }  
  
    // 复制构造函数：复制成员并将使用计数加1  
    _XSmartPArray(const _XSmartPArray& temp) 
		:m_backP(temp.m_backP)
		,m_size(temp.m_size)
    {  
		++m_backP->m_counter;    
		printf("_XSmartPArray copy constructor called ! use = %d\n",m_backP->m_counter); 
    }  
  
    // 赋值操作符  
    _XSmartPArray<T>& operator=(const _XSmartPArray<T>&temp)
	{  
		if(this == &temp) return *this;		//防止自生拷贝
		// 增加右操作数中的使用计数  
		++temp.m_backP->m_counter;  
		// 将左操作数对象的使用计数减1，若该对象的使用计数减至0，则删除该对象  
		if (--m_backP->m_counter == 0)  
		{
			XDELETE(m_backP);
		}
		m_backP = temp.m_backP;   // 复制U_Ptr指针  
		m_size = temp.m_size;
		return *this;  
	}   

 	T& operator[](int order)const
	{
		if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];
	}
 	T& operator[](int order)
	{
		if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];
	}
 
    // 析构函数：如果计数为0，则删除U_Ptr对象  
    ~_XSmartPArray()  
    {  
		printf("_XSmartPArray distructor called ! use = %d\n",m_backP->m_counter); 
		if (--m_backP->m_counter == 0)  
		{
			XDELETE(m_backP);
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
    _XBackPArray<T> *m_backP;   //指向使用计数类m_backP 
	int m_size;
}; 
#endif