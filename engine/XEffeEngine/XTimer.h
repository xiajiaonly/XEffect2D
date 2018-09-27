#ifndef _JIA_XTIMER_
#define _JIA_XTIMER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"
#include <string>
#include <time.h>
namespace XE {
//这是一个高精度计时器的类
class XTimer
{
public:
	XTimer();
	~XTimer() {}

	void start();	//设置计时的起点
	void end();		//设置计时结束
	double getElapsedTime();	//获取时间差，单位为秒，如果没有停止则是返回的当前的时间差
	double getElapsedTimeInSec();	//获取时间差，单位为秒
	double getElapsedTimeInMilliSec();	//获取时间差，单位为毫秒
	double getElapsedTimeInMicroSec();	//获取时间差，单位为微秒

	double getUpElapsedTime();	//获取两次调用之间的时间差,单位为毫秒
	double getCurrentTime();	//获取当前的时间，单位为毫秒

	bool getIsStoped()const { return m_stopped; }
private:
	double m_upTime;	//上次调用的时间
	double m_startTime;
	double m_endTime;
#ifdef XEE_OS_WINDOWS
	double m_perFreq;
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_startCount;
	LARGE_INTEGER m_endCount;
	LARGE_INTEGER m_upCount;
	LARGE_INTEGER m_curCount;
#else
	timeval m_startCount;
	timeval m_endCount;
	timeval m_upCount;
	timeval m_curCount;
#endif
	bool m_stopped;
};
inline void XTimer::start()
{
	m_stopped = false; // reset stop flag
#ifdef XEE_OS_WINDOWS
	QueryPerformanceCounter(&m_startCount);
#else
	gettimeofday(&m_startCount, NULL);
#endif
}
inline void XTimer::end()
{
	m_stopped = true; // set timer stopped flag
#ifdef XEE_OS_WINDOWS
	QueryPerformanceCounter(&m_endCount);
#else
	gettimeofday(&m_endCount, NULL);
#endif
}
inline double XTimer::getElapsedTimeInMilliSec()
{
	return getElapsedTimeInMicroSec() * 0.001;
}
inline double XTimer::getElapsedTimeInSec()
{
	return getElapsedTimeInMicroSec() * 0.000001;
}
inline double XTimer::getElapsedTime()
{
	return getElapsedTimeInSec();
}
inline double XTimer::getCurrentTime()		//获取当前的时间
{
#ifdef XEE_OS_WINDOWS
	QueryPerformanceCounter(&m_curCount);
	return m_curCount.QuadPart * m_perFreq * 0.001;
#else
	gettimeofday(&m_curCount, NULL);
	return (m_curCount.tv_sec * 1000000.0) + m_curCount.tv_usec;
#endif
}
struct XSystemTime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int millisecond;
	XSystemTime()
		:year(0)
		, month(0)
		, day(0)
		, hour(0)
		, minute(0)
		, second(0)
		, millisecond(0)
	{}
	XSystemTime(int y, int m, int d, int h = 0, int mi = 0, int s = 0, int ms = 0)
		:year(y)
		, month(m)
		, day(d)
		, hour(h)
		, minute(mi)
		, second(s)
		, millisecond(ms)
	{}
};
#define MILLISECOND_IN_ONE_DAY (24 * 60 * 60 * 1000)	//一天有多少毫秒
#define DAY_IN_ONE_LEAP_YEAR (366)
namespace XTime
{
	inline bool isLeapYear(int year)
	{//判断是会否为闰年
		//方案1
		//return (((year % 4) == 0 && year % 100 != 0) || year % 400 == 0);
		//方案2,年不为负数
		return (((year & 0x03) == 0 && year % 100 != 0) || year % 400 == 0);
	}
	//获取从程序运行到当前的时间差，单位为ms
	extern long getCurrentTicks();
	extern bool getDateIsCorrect(const XSystemTime &t);			//判断时间格式的正确性
	extern void getTimeMs(XSystemTime &t);							//下面是一个通用的方法用于获取系统时间，单位到ms
	inline XSystemTime getTimeMs()
	{
		XSystemTime tmp;
		getTimeMs(tmp);
		return tmp;
	}
	extern bool isBefore(const XSystemTime &t1, const XSystemTime &t2);		//判断t1是否在t2之前
	extern long long getElapsedTime(const XSystemTime &ts, const XSystemTime &te);	//计算两个时间之间的时间差，单位为ms
	extern const int dayInMonth[12];
	//下面的函数都可以进行优化，但是尚未进行
	inline long long getMsInThisYear(const XSystemTime &t)	//经过验证
	{//计算这个日期在这一年中的毫秒值
		if (!getDateIsCorrect(t)) return 0;
		long long ret = t.hour * 60 * 60 * 1000 + t.minute * 60 * 1000 + t.second * 1000 + t.millisecond;
		if (t.month <= 2) return (dayInMonth[t.month - 1] + t.day) * MILLISECOND_IN_ONE_DAY + ret;
		if (isLeapYear(t.year))//闰年
			return (dayInMonth[t.month - 1] + t.day) * MILLISECOND_IN_ONE_DAY + ret;
		else//非闰年
			return (dayInMonth[t.month - 1] - 1 + t.day) * MILLISECOND_IN_ONE_DAY + ret;
	}
	//计算指定时间是一年中的第几天,假设这个日期是合法的
	inline int getDateDaysInYear(const XSystemTime &t)	//经过验证
	{
		if (t.month <= 2) return dayInMonth[t.month - 1] + t.day;
		if (isLeapYear(t.year)) return dayInMonth[t.month - 1] + t.day;	//闰年
		else return dayInMonth[t.month - 1] - 1 + t.day;//非闰年
	}
	//计算从公元元年以来到指定日期的天数
	inline int getTotalDays(const XSystemTime &t)
	{
		int tmpY = t.year - 1;
		int tmp = 365 * tmpY + getDateDaysInYear(t);	//计算历史以来的时间
		//计算闰年的数量
		//方案1
	//	for(int i = 1;i < t.year;++ i)
	//	{//这里每次都要算，岂不是浪费计算量
	//		if(isLeapYear(i)) ++tmp;
	//	}
		//方案2
		tmp += (tmpY >> 2);
		tmpY /= 100;
		tmp += (tmpY >> 2) - tmpY;
		return tmp;
	}
	//计算指定时间是星期几
	inline int getDateWeek(const XSystemTime &t)	//经过验证
	{
		return getTotalDays(t) % 7;
	}
	inline int getCurrentWeek()	//获取今天是星期几
	{
		time_t t = time(NULL);
		return localtime(&t)->tm_wday;
	}
	//计算指定年的第几天是什么日期，星期几
	extern void getDateData(int year, int day, XSystemTime &t, int &week);
	//目前使用网络上的算法只能演算1900年-2050年的农历
	extern void solar2lunar(const XSystemTime &in, XSystemTime &out);	//阳历转阴历
	extern void lunar2solar(const XSystemTime &in, XSystemTime &out);	//阴历转阳历（尚未实现）
	inline std::string sysTime2String(const XSystemTime &in)	//时间转换成string 20140915-095643-377
	{
		char tmp[20];
		sprintf_s(tmp, 20, "%04d%02d%02d-%02d%02d%02d-%03d", in.year, in.month, in.day,
			in.hour, in.minute, in.second, in.millisecond);
		return tmp;
	}
	inline XSystemTime string2sysTime(const std::string& str)
	{
		XSystemTime in;
		sscanf(str.c_str(), "%04d%02d%02d-%02d%02d%02d-%03d", &in.year, &in.month, &in.day,
			&in.hour, &in.minute, &in.second, &in.millisecond);
		return in;
	}
}
//这是一个简单的计数器，可以用于记录次数，以及频率
class XSimpleCounter
{
private:
	float m_frequency;	//频率，次每秒
	int m_times;		//次数
	int m_upTimes;		//上次统计时的次数
	float m_cycle;		//统计周期,频率的计算步长,单位秒,周期小于等于0时为不统计
	float m_timer;		//计时器,单位秒
	float m_upTimer;	//上一次的时间点，单位秒
	int m_upTicks;		//上一次的时间信号
	int m_lastFrameTime;	//上一帧的时间,单位ms
public:
	XSimpleCounter()
		:m_frequency(0.0f)
		, m_times(0)
		, m_upTimes(0)
		, m_cycle(5.0f)
		, m_upTicks(-1)
		, m_upTimer(0.0f)
		, m_timer(0.0f)
		, m_lastFrameTime(0)
	{}
	void setCycle(float cycle) { m_cycle = cycle; }	//单位秒，统计的步长
	int getTotalTimes()const { return m_times; }		//获取总的调用次数
	float getTotalTimer()const { return m_timer; }		//获取总的调用时长
	float getFrequency()const { return m_frequency; }	//获取当前的频率
	int getLastFrameTime()const { return m_lastFrameTime; }//返回上一帧的时间:单位 毫秒
	//void reset();	//复位，尚未实现
	bool update()//返回是否产生新的频率数据
	{
		++m_times;
		if (m_upTicks < 0)
		{//第一次运行
			m_upTicks = XTime::getCurrentTicks();
			return false;
		}
		else
		{
			int curTicks = XTime::getCurrentTicks();
			m_lastFrameTime = curTicks - m_upTicks;
			m_timer += m_lastFrameTime * 0.001f;
			m_upTicks = curTicks;
		}
		if (m_timer - m_upTimer >= m_cycle && m_cycle > 0.0f)
		{
			m_frequency = (float)(m_times - m_upTimes) / (m_timer - m_upTimer);
			m_upTimes = m_times;
			m_upTimer = m_timer;
			return true;
		}
		return false;
	}
};
//步进计时器
class XStepTimer
{
private:
	int m_upTimer;
	int m_curTimer;
	float m_stepTime;	//一步的时间，单位为毫秒
	bool m_isFirst;
public:
	float update()	//返回两次调用之间的时间差
	{
		if (m_isFirst)
		{
			m_isFirst = false;
			m_upTimer = m_curTimer = XTime::getCurrentTicks();
			m_stepTime = 0.0f;
			return m_stepTime;
		}
		else
		{
			m_curTimer = XTime::getCurrentTicks();
			m_stepTime = m_curTimer - m_upTimer;
			m_upTimer = m_curTimer;
			return m_stepTime;
		}
	}
	float getStepTime()const { return m_stepTime; }
	XStepTimer()
		:m_isFirst(true)
		, m_stepTime(0.0f)
	{}
};
//这是一个通用时间的内，主要是用于记录一天的时间最小单位为分钟
class XGenTimer
{
public:
	int m_hour;
	int m_minute;
public:
	bool getIsInArea(const XGenTimer &t, int minutes)const	//判断时间t是否在当前时间之后的minutes秒内
	{
		return getIsInArea(t.getInMinutes(), minutes);
	}
	bool getIsInArea(int t, int minutes)const	//判断时间t是否在当前时间之后的minutes秒内
	{
		//下面的算法在跨越零时时会造成bug
	//	return (t >= getInMinutes() &&
	//		t < getInMinutes() + minutes);
		//下面为了解决零时bug而设计的算法
		int m = getInMinutes();
		if (m + minutes > 24 * 60)
		{//跨越零时
			if (t >= m && t < 24 * 60) return true;
			return (t >= 0 && t < m + minutes - 24 * 60);
		}
		else
		{//不跨越零时
			return (t >= m && t < m + minutes);
		}
	}
	//判断当前时间否在在t的后面
	bool getIsLaterThan(const XGenTimer &t)const { return getInMinutes() > t.getInMinutes(); }
	int getInMinutes()const { return m_hour * 60 + m_minute; }	//将时间转换为分钟单位
	int getInSeconds()const { return getInMinutes() * 60; }	//将时间转换为秒单位
	bool getIsValid()const { return (m_hour >= 0 && m_hour < 24 && m_minute >= 0 && m_minute < 60); }	//判断时间是否是合法的
	XGenTimer()
		:m_hour(-1)
		, m_minute(-1)
	{}
	XGenTimer(int hour, int minute)
		:m_hour(hour)
		, m_minute(minute)
	{}
};
//这是一个简单计时器的类，用于简单的计时需要而定义的计时器
class XSimpleTimer
{
private:
	float m_timer;
	float m_stepTime;	//步进时间,小于0为不计时,单位为毫秒
	bool m_isWorking;	//是否处于工作状态
	bool m_noGapMode;	//是否使用无间隙模式，无间隙模式的时候update返回true的次数与时间严格一致，否则会抛弃多余的时间
	//另外一种自计时的模式
	int m_curTicks;
public:
	XSimpleTimer()
		:m_timer(0)
		, m_stepTime(0)
		, m_isWorking(true)
		, m_noGapMode(true)
		, m_curTicks(-1)
	{}
	XSimpleTimer(float stepTime)
		:m_timer(0)
		, m_stepTime(stepTime)
		, m_isWorking(true)
	{}
	bool getIsNoGapMode()const { return m_noGapMode; }
	void setIsNoGapMode(bool flag) { m_noGapMode = flag; }
	void setWorking(bool flag) { m_isWorking = flag; }
	bool getIsWorking()const { return m_isWorking; }
	void setStepTime(float stepTime) { m_stepTime = stepTime; }
	float getStepTime() const { return m_stepTime; }
	bool update(float stepTime)	//返回是否触发时间事件
	{
		if (!m_isWorking) return false;
		if (m_stepTime <= 0.0f) return true;
		m_timer += stepTime;
		if (m_timer >= m_stepTime)
		{
			if (m_noGapMode)
				m_timer -= m_stepTime;
			else
				m_timer = 0.0f;
			return true;
		}
		return false;
	}
	bool update()	//返回是否触发时间事件
	{
		if (m_curTicks < 0)
		{
			m_curTicks = XTime::getCurrentTicks();
			return false;
		}
		int tmp = m_curTicks;
		m_curTicks = XTime::getCurrentTicks();
		return update(m_curTicks - tmp);
	}
	float getCurTime()const { return m_timer; }	//获取当前记录的时间
	void reset() { m_timer = 0.0f; }
};
}
#endif