#ifndef _JIA_XBASICFUN_
#define _JIA_XBASICFUN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XOSDefine.h"
#include "XMemoryPool.h"

#include "stdio.h"
//#include <math.h>
#include "string.h"
#include "XCommonDefine.h"
#include "XStringFun.h"
#include "XByteFun.h"
#include "XRandomFun.h"
#include "XMath/XMath.h"

//#define XDELETE_ARRAY(p) { if (p != NULL) { delete [] p; p = NULL; } }
//#define XDELETE(p) { if (p != NULL) { delete p; p = NULL; } }
//#define FILE_ERROR(p) {fclose(p);return XFalse;}
//自己定义new和delete
//A* a = (A*)malloc(sizeof(A));
//new (a) A();	//这样可以吗？
//
//a->~A();
//free(a);

template<typename T>
void XDELETE(T *&p)
{
	if(p != NULL)
	{
		delete p;
		p = NULL;
	}
}
template<typename T>
void XDELETE_ARRAY(T *&p)
{
	if(p != NULL)
	{
		delete [] p;
		p = NULL;
	}
}
template<typename T>
T * createMem()
{
	T * ret = NULL;
	try
	{
		ret = new T;
		if(ret == NULL) return NULL;
	}catch(...)
	{
		return NULL;
	}
	return ret;
}
template<typename T>
T * createArrayMem(int size)
{
	if(size <= 0) return NULL;
	T * ret = NULL;
	try
	{
		ret = new T[size];
		if(ret == NULL) return NULL;
	}catch(...)
	{
		return NULL;
	}
	return ret;
}
inline float squareFloat(float x) {return x * x;}	//求x的平方
//#define SIN_BOOK	//sin函数是否使用查字典方式
#ifdef SIN_BOOK
extern void initSinData();
extern float xSin(float x);
#endif
//#define SQRT_BOOK	//开方是否使用查字典方式

#ifdef SQRT_BOOK
extern void initSqrtData();
extern float xSqrt(float x);
#endif
//获取文件的大小
inline int getFileLen(FILE * fp)
{
	if(fp == NULL) return 0;
	int tempOffset = ftell(fp);	//记录当前的文件位置
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);	//获取文件的长度
	fseek(fp,tempOffset,SEEK_SET);	//恢复之前的文件位置
	return len;
}
template<class type> void xChangeTwoSum(type &sum0,type &sum1)
{//交换两个变量的值
	type temp = sum0;
	sum0 = sum1;
	sum1 = temp;
}
extern int mySleep(unsigned long sleepUSecond);	//这个参数的时间是us及1000000分之1秒，及1000分之1毫秒

//输出HL1与HL2的和的一半，本来想用于声音变慢的，结果好像没用上
extern void getHalfData(unsigned char H1,unsigned char L1,unsigned char H2,unsigned char L2,unsigned char &Ho,unsigned char &Lo);

#ifdef GET_ALL_PIXEL
extern int XEE_AllPixel;	//总的贴图像素
#endif
//图像缩放算法，双线性插值
extern void pixelDataZoomRGB(unsigned char * src,unsigned char * dst,float zoom,	//原始图像数据，目标图像数据，缩放尺寸
					  int sw,int sh,int dw,int dh,	//原始图像尺寸，目标图像尺寸
					  int sOffsetX,int sOffsetY,int dOffsetX,int dOffsetY,	//原始图像裁剪的位置，和目标图像的位置
					  int sClipW,int sClipH);	//原始图像的裁剪大小
//普通图像缩放的应用形式为pixelDataZoomRGB(src,dst,zoom,sw,sh,dw,dh,0,0,0,0,sw,sh);
//mode 0:居中平铺,1:缩放自适应，多出部分黑色
extern void pixelDataConvertRGB(unsigned char * src,unsigned char * dst,
								const _XVector2 &srcSize,const _XVector2 &dstSize,const _XVector2& srcPos,
								const _XVector2 &dstClipSize,int mode = 0);

inline float rectResize(float w,float h,float Rx,float Ry)	//根据Rx与Ry构成的框，将w和h的框放入其中，如果能放进去，则返回1.0f,否则返回自适应大小
{//该函数用于窗口缩放适应，返回缩放的比例
	if(w <= 0.0f || h <= 0.0f || Rx <= 0.0f || Ry <= 0.0f) return 1.0f;
	if(w <= Rx && h <= Ry) return 1.0f;
	float size = Rx / w;
	if(size < Ry / h) return size;
	else return Ry / h;
}

#ifdef XEE_OS_WINDOWS
enum _XShutDownSystemMode
{
	SYS_SD_MODE_G,		//关机
	SYS_SD_MODE_Z,		//注销
	SYS_SD_MODE_C,		//重启
};
//用于系统关机或者重启或者注销的函数
extern void shutDownSystem(_XShutDownSystemMode mode = SYS_SD_MODE_G);
//后台停顿等待键盘输入
inline void waitingForKey() {system("pause");}
//设置控制台输出文字的颜色
inline void setConsoleColor(int index)
{
	HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE); 
	switch(index)
	{
	case 1:
		SetConsoleTextAttribute(consolehwnd,FOREGROUND_BLUE);
		break;
	case 2:
		SetConsoleTextAttribute(consolehwnd,FOREGROUND_GREEN);
		break;
	default:
		SetConsoleTextAttribute(consolehwnd,FOREGROUND_RED);
		break;
	}
}
#endif
/*
//CC 00 00 00 00 00 00 00 00 00 00 00 DD
//需要满足包体中不包含包头特征字
#include <deque>
std::deque<unsigned char *> packetDeque;
unsigned char *dataPacket = NULL;
int dataSum = 0;
int packetSize = 13;
void recvDataProc(unsigned char * data,int len)	//根据简单的协议从数据中提取数据包的函数
{
	if(data == NULL || len <= 0) return ;
	if(dataSum == 0)
	{
		if(data[0] != 0xCC)
		{
			for(int i = 0;i < len;++ i)
			{
				if(data[i] == 0xCC)
				{
					recvDataProc(data + i,len - i);
					break;
				}
			}
		}else
		{
			dataPacket = createArrayMem<unsigned char>(packetSize);
			if(dataPacket == NULL) exit(0);
			if(len >= packetSize)
			{
				memcpy(dataPacket,data,packetSize);
				if(dataPacket[packetSize - 1] != 0xDD) exit(0);
				packetDeque.push_back(dataPacket);
				dataPacket = NULL;
				dataSum = 0;
				recvDataProc(data + packetSize,len - packetSize);
			}else
			{	
				memcpy(dataPacket,data,len);
				dataSum = len;
			}
		}
	}else
	{
		if(dataSum + len >= packetSize)
		{
			int need = packetSize - dataSum;
			memcpy(dataPacket + dataSum,data,need);
			if(dataPacket[packetSize - 1] != 0xDD) exit(0);
			packetDeque.push_back(dataPacket);
			dataPacket = NULL;
			dataSum = 0;
			recvDataProc(data + need,len - need);
		}else
		{
			memcpy(dataPacket + dataSum,data,len);
			dataSum += len;
		}
	}
}*/
struct _XShaderHandle
{
	unsigned int shaderHandle;
	unsigned int shaderF;	//frag
	unsigned int shaderV;	//vert
	unsigned int shaderG;	//geomrtry
	_XShaderHandle()
		:shaderHandle(0)
		,shaderF(0)
		,shaderV(0)
		,shaderG(0)
	{}
};
#endif 
