#ifndef _JIA_XMEMORYPOOL_
#define _JIA_XMEMORYPOOL_

//这是一个线程池的类，尚未有好的实现思路
//目前通过使用第三方的内存池来代替
//这里推荐使用libtcmalloc_minimal.lib库
//连接https://code.google.com/p/gperftools/
//使用方法：
//1、包含lib文件
#pragma comment(lib, "MemoryPool/libtcmalloc_minimal.lib")
//加入强制符号引用__tcmalloc
#ifdef XEE_OS_WINDOWS
//#pragma comment( linker, "/INCLUDE:__tcmalloc" )	//使用内存池的编译说明
#endif

//#include "stdio.h"
//#include "stdlib.h"
//extern int allMemory;
//inline void* operator new(size_t sz) 
//{
//	allMemory += sz;
//	printf("global op new called, size = %d\n",allMemory);
//	return malloc(sz);
//}
//inline void* operator new[](size_t sz) 
//{
//	allMemory += sz;
//	printf("global op new[] called, size = %d\n",allMemory);
//	return malloc(sz);
//}
//inline void operator delete(void* ptr,size_t sz)
//{
//	allMemory -= sz;
//	printf("global op delete called:%d\n",allMemory);
//	free(ptr);
//}
//inline void operator delete[](void* ptr,size_t sz)
//{
//	allMemory -= sz;
//	printf("global op delete[] called:%d\n",allMemory);
//	free(ptr);
//}
//class  A
//{
//public:
//	A()
//	{
//		printf("构造函数!\n");
//	}
//	~A()
//	{
//		printf("析构函数!\n");
//	}
//	//static void * operator new(size_t size)
//	//{
//	//	void *p = malloc(sizeof(A));
//	//	printf("new! %d\n",size);
//	//	return p;
//	//}
//	//static void * operator new[](size_t size)
//	//{
//	//	void *p = malloc(size);
//	//	printf("new[]! %d\n",size);
//	//	return p;
//	//}
//	//static void operator delete(void *p,size_t size)
//	//{
//	//	free(p);
//	//	printf("delete! %d\n",size);
//	//}
//	//static void operator delete[](void *p,size_t size)
//	//{
//	//	free(p);
//	//	printf("delete[]! %d\n",size);
//	//}
//};
//void main()
//{
//	A *p = new A();
//	A *q = new A[2];
//	delete p;
//	delete[] q;
//}
#endif