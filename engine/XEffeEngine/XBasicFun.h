#ifndef _JIA_XBASICFUN_
#define _JIA_XBASICFUN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//#include "XMemoryPool.h"

//#include "stdio.h"
//#include <math.h>
//#include "string.h"
//#include "XCommonDefine.h"
//#include "XStringFun.h"
//#include "XMath/XByteFun.h"
//#include "XMath/XRandomFun.h"
//#include "XMath/XMath.h"
//#include "XMemory.h"
//#include "XFile.h"
//+++++++++++++++++++++++++++++++++++++++++++++++
//说明：目前的SDL在VS2015中最后在资源时候时会由于clearup当前资源而造成crash
//下面的问题是为了解决VS2015中提示未定义__iob_func而增加的
//方案1：老方案必须只能包含一次
//#include "stdio.h"
//extern "C" FILE __iob_func[3] = { __acrt_iob_func(0),__acrt_iob_func(1),__acrt_iob_func(2) };
//方案2：可以多次包含
#if _MSC_VER>=1900
//#include "XOSDefine.h"
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus 
extern "C"
#endif 
FILE* __cdecl __iob_func(unsigned i);
#endif //_MSC_VER>=1900
//-----------------------------------------------
namespace XE{
class XVec2;

//输出HL1与HL2的和的一半，本来想用于声音变慢的，结果好像没用上
extern void getHalfData(unsigned char H1,unsigned char L1,unsigned char H2,unsigned char L2,unsigned char &Ho,unsigned char &Lo);

namespace XPixel
{
	#ifdef GET_ALL_PIXEL
	extern int XEE_AllPixel;	//总的贴图像素
	#endif
	extern void pixelsZoomRGBA(unsigned char * src,unsigned char * dst,	//原始图像数据，目标图像数据
						  int sw,int sh,int dw,int dh);	//原始图像的裁剪大小
	extern void pixelsZoomRGB(unsigned char * src,unsigned char * dst,	//原始图像数据，目标图像数据
						  int sw,int sh,int dw,int dh);	//原始图像的裁剪大小
	//将指定矩阵的数据缩放到目标矩阵中
	extern void datasZoom(int* src, int* dst, int sw, int sh, int dw, int dh);
	//处理掉0的特殊值
	extern void datasZoomEx(int* src, int* dst, int sw, int sh, int dw, int dh);
	//图像缩放算法，双线性插值
	extern void pixelDataZoomRGB(unsigned char * src,unsigned char * dst,float zoom,	//原始图像数据，目标图像数据，缩放尺寸
						  int sw,int sh,int dw,int dh,	//原始图像尺寸，目标图像尺寸
						  int sOffsetX,int sOffsetY,int dOffsetX,int dOffsetY,	//原始图像裁剪的位置，和目标图像的位置
						  int sClipW,int sClipH);	//原始图像的裁剪大小
	//普通图像缩放的应用形式为pixelDataZoomRGB(src,dst,zoom,sw,sh,dw,dh,0,0,0,0,sw,sh);
	//mode 0:居中平铺,1:缩放自适应，多出部分黑色
	enum XPixelFillMode	//图像填充模式
	{
		PIXEL_MODE_CENTER_TILE,	//居中平铺
		PIXEL_MODE_ZOOM,		//自适应缩放
	};
	extern void pixelDataConvertRGB(unsigned char * src,unsigned char * dst,
									const XVec2& srcSize,const XVec2& dstSize,const XVec2& srcPos,
									const XVec2& dstClipSize,XPixelFillMode mode = PIXEL_MODE_CENTER_TILE);
	//使用居中平铺的方式将src的数据填充到dst部分
	template<class T>
	void pixelFillRGBA(T *src,T *dst,int sw,int sh,int dw,int dh)	//这个函数没有经过验证
	{
		//位置
		XVec2 resSize((std::min)(sw,dw),(std::min)(sh,dh));				//最终确定的源目标尺寸
		XVec2 resDstPos((XVec2(dw,dh) - resSize) * 0.5f);		//最终确定的目标位置
		XVec2 resSrcPos((XVec2(sw,sh) - resSize) * 0.5f);	//最终确定的源位置
		//下面开始拷贝数据
		int w = resSize.x * 4 * sizeof(T);
		int wS = (sw * sizeof(T)) << 2;
		int wD = (dw * sizeof(T)) << 2;
		int tmpD = int(resDstPos.x) * 4 * sizeof(T);
		int tmpS = int(resSrcPos.x) * 4 * sizeof(T);
		unsigned char *s = (unsigned char *)src;
		unsigned char *d = (unsigned char *)dst;
		for(int h = 0;h < resSize.y;++ h)
		{
			memcpy(d + (int)(h + resDstPos.y) * wD + tmpD,
				s + (int)(h + resSrcPos.y) * wS + tmpS,w);
		}
	}
}

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
			dataPacket = XMem::createArrayMem<unsigned char>(packetSize);
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
struct XShaderHandle
{
	unsigned int shaderHandle;
	unsigned int shaderF;	//frag
	unsigned int shaderV;	//vert
	unsigned int shaderG;	//geomrtry
	XShaderHandle()
		:shaderHandle(0)
		,shaderF(0)
		,shaderV(0)
		,shaderG(0)
	{}
};
}
#endif 
