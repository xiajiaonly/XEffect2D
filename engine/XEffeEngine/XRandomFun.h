#ifndef _JIA_XRANDOMFUN_
#define _JIA_XRANDOMFUN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "stdlib.h"
#include "math.h"
#include "XMath/XMath.h"

inline int random(int x)	//强化这个函数的取值范围0 - 0x07ffffff
{
	if(x == 0) return 0;
	if(x < 0) x = -x;
	if(x <= RAND_MAX) return rand() % x;
	else return ((rand() << 12) + (rand() % 0x0fff)) % x;
}
inline float randomf(float tempMax) {return tempMax * rand() / (RAND_MAX + 1.0f);}
inline float randomf(float x, float y)
{
	if(x == y) return x; 
	float high = XEE_Max(x,y);
	float low = XEE_Min(x,y);
	return low + ((high-low) * rand() / (RAND_MAX + 1.0));
}
inline float gaussRand()	//模拟正态分布的随即函数产生0.0f - 1.0f的数值
{
	if((rand() % 100) < 50) return sin(randomf(PI_HALF)) * 0.5f;
	else return 0.5f + (1.0f - sin(randomf(PI_HALF))) * 0.5f;
}
//0 - RAND_MAX(0x7fff)		//返回一个0 - x之间的整数
extern void randomMomery(char *p,int size);	//以字节为单位混乱p所指向的大小为size的内存空间
template<class type> void randomArray(type *p,int size) //混乱一个数组
{
	if(p == NULL) return;
	if(size <= 1) return;
	int order;
	for(int i = 0;i < size; ++ i)
	{
		order = random(size);
		if(order >= 0 && order < size && order != i)
			xChangeTwoSum<type>(p[i],p[order]);
	}
}
#endif