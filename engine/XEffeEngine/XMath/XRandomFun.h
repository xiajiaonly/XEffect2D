#ifndef _JIA_XRANDOMFUN_
#define _JIA_XRANDOMFUN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "stdlib.h"
#include "XMath/XMath.h"
namespace XE{
namespace XRand
{
	inline int random(int x)	//强化这个函数的取值范围0 - 0x7fffffff
	{
		if (x < 0) return -random(-x);
		if (x <= 1) return 0;
		if (x <= RAND_MAX) return rand() % x;
		else if (x <= 0x3fffffff) return ((rand() << 15) + rand()) % x;
		else return ((rand() << 16) + (rand() << 1) + (rand() & 0x01)) % x;
	}
	inline float randomf(float tempMax) {return tempMax * static_cast<float>(rand()) / (RAND_MAX + 1.0f);}
	inline float randomf(float x, float y)
	{
		if(x == y) return x; 
	//	float high = (std::max)(x,y);
	//	float low = (std::min)(x,y);
		//float high,low;
		//if(x > y) 
		//{
		//	high = x;
		//	low = y;
		//}else
		//{
		//	high = y;
		//	low = x;
		//}
		//return (float)(low + ((high-low) * rand() / (RAND_MAX + 1.0)));
		return (float)(y + ((x-y) * rand() / (RAND_MAX + 1.0)));
	}
	inline float gaussRand()	//模拟正态分布的随即函数产生0.0f - 1.0f的数值,数据集中于0.5左右
	{
		if((rand() % 100) < 50) return sin(randomf(PI_HALF)) * 0.5f;
		else return 1.0f - sin(randomf(PI_HALF)) * 0.5f;
	}
	inline float gaussRandMax()	//模拟正态分布的随即函数产生0.0f - 1.0f的数值,数据集中于1.0左右
	{
		return sin(randomf(PI_HALF));
	}
	inline float gaussRandMin()	//模拟正态分布的随即函数产生0.0f - 1.0f的数值,数据集中于0.0左右
	{
		return 1.0f - sin(randomf(PI_HALF));
	}
	//0 - RAND_MAX(0x7fff)		//返回一个0 - x之间的整数
	extern void randomMomery(void *p,int size);//以字节为单位向内存中写入随机的内容
	template<class type> void randomArray(type *p,int size) //混乱一个数组
	{
		if(p == NULL ||
			size <= 1) return;
	//	int order;
		for(int i = 0;i < size; ++ i)
		{
	//		order = random(size);
	//	//	//if(order >= 0 && order < size && order != i)
	//	//	if(order != i)
	//			XMath::swapTwoSum<type>(p[i],p[order]);
			XMath::swapTwoSum<type>(p[i], p[random(size)]);
		}
	}
	//当所有数据都为0时返回的是无效的数据
	//圆桌随机，data为各个比例的份额，sum为份额的数量，返回随机到的份额编号
	inline int randRoundTable(const int *datas,int sum)
	{
		if(sum <= 0 || datas == NULL) return 0;
		int maxSum = 0;
		for(int i = 0;i < sum;++ i)
		{
			if(datas[i] < 0) return 0;	//非法的数据，份额的比例不能为负数
			maxSum += datas[i];
		}
		if(maxSum <= 0) return 0;
		int curData = random(maxSum) + 1;
		for(int i = 0;i < sum;++ i)
		{
			curData -= datas[i];
			if(curData <= 0) return i;
		}
		return sum - 1;
	}
}
//这个类的作用是在指定的时长内会发生一次随机事件，也就是每X秒随机发生一次E事件
class XRandByTime
{
private:
	float m_settingTime;	//设置的时长
	float m_timer;	//计时器
	float m_curHappenTime;	//这一次的发生时间
	bool m_isHappened;//是否已经发生
public:
	XRandByTime()
		:m_settingTime(1000)
		, m_timer(0.0f)
		, m_isHappened(false)
		, m_curHappenTime(0.5f)
	{}
	void reset()
	{
		m_timer = 0.0f;
		m_isHappened = false;
		m_curHappenTime = XRand::randomf(0.0f,1.0f) * m_settingTime;
	}
	//注意这个函数会重置内部的计时机制
	void setTime(float time)	//设置发生一次的平均时长
	{
		if(time <= 0.0f) return;	//非法的参数
		m_settingTime = time;
		reset();
	}
	bool update(float stepTime)
	{
		m_timer += stepTime;
		if(!m_isHappened && m_timer >= m_curHappenTime)
		{
			m_isHappened = true;
			return true;
		}
		if(m_timer >= m_settingTime)
		{//重置随机事件
			m_timer -= m_settingTime;
			m_isHappened = false;
			m_curHappenTime = XRand::randomf(0.0f,1.0f) * m_settingTime;
		}
		return false;
	}
};
}
#endif