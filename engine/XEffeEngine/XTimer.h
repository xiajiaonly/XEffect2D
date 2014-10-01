#ifndef _JIA_XTIMER_
#define _JIA_XTIMER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"
#include "SDL.h"
//这是一个高精度计时器的类
class _XTimer
{
public:
    _XTimer();
	~_XTimer(){}

    void start();	//设置计时的起点
    void end();		//设置计时结束
    double getElapsedTime();	//获取时间差，单位为秒
    double getElapsedTimeInSec();	//获取时间差，单位为秒
    double getElapsedTimeInMilliSec();	//获取时间差，单位为毫秒
    double getElapsedTimeInMicroSec();	//获取时间差，单位为微秒

	double getUpElapsedTime();	//获取两次调用之间的时间差,单位为毫秒
	double getCurrentTime();	//获取当前的时间，单位为毫秒
private:
	double m_upTime;	//上次调用的时间
    double m_startTime;
    double m_endTime;
    bool m_stopped;
#ifdef XEE_OS_WINDOWS
	double m_perFreq;
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startCount;
    LARGE_INTEGER m_endCount;
	LARGE_INTEGER m_upCount;
	LARGE_INTEGER m_nowCount;
#else
    timeval m_startCount;
    timeval m_endCount;
	timeval m_upCount;
	timeval m_nowCount;
#endif
};
inline void _XTimer::start()
{
    m_stopped = false; // reset stop flag
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_startCount);
#else
    gettimeofday(&m_startCount,NULL);
#endif
}
inline void _XTimer::end()
{
    m_stopped = true; // set timer stopped flag
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_endCount);
#else
    gettimeofday(&m_endCount,NULL);
#endif
}
inline double _XTimer::getElapsedTimeInMilliSec()
{
    return getElapsedTimeInMicroSec() * 0.001;
}
inline double _XTimer::getElapsedTimeInSec()
{
    return getElapsedTimeInMicroSec() * 0.000001;
}
inline double _XTimer::getElapsedTime()
{
    return getElapsedTimeInSec();
}
inline double _XTimer::getCurrentTime()		//获取当前的时间
{
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_nowCount);
    return m_nowCount.QuadPart * m_perFreq * 0.001;
#else
	gettimeofday(&m_nowCount,NULL);
	return (m_nowCount.tv_sec * 1000000.0) + m_nowCount.tv_usec;
#endif
}
struct _XSystemTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
    _XSystemTime()
        :year(0)
        ,month(0)
        ,day(0)
        ,hour(0)
        ,minute(0)
        ,second(0)
        ,millisecond(0)
    {}
    _XSystemTime(int y,int m,int d)
        :year(y)
        ,month(m)
        ,day(d)
        ,hour(0)
        ,minute(0)
        ,second(0)
        ,millisecond(0)
    {}
};
#define MILLISECOND_IN_ONE_DAY (24 * 60 * 60 * 1000)	//一天有多少毫秒
#define DAY_IN_ONE_LEAP_YEAR (366)
inline bool isLeapYear(int year)
{//判断是会否为闰年
	//方案1
	//return (((year % 4) == 0 && year % 100 != 0) || year % 400 == 0);
	//方案2,年不为负数
	return (((year & 0x03) == 0 && year % 100 != 0) || year % 400 == 0);
}
//获取从程序运行到当前的时间差，单位为ms
inline long getCurrentTicks()
{
	return SDL_GetTicks();	//在SDL环境下
#if WITH_64BIT_SUPPLY		//windows下得API
	return GetTickCount64();
#else 
	return GetTickCount();
#endif
}
extern bool getDateIsCorrect(const _XSystemTime &t);			//判断时间格式的正确性
extern void getTimeMs(_XSystemTime &t);							//下面是一个通用的方法用于获取系统时间，单位到ms
inline _XSystemTime getTimeMs()
{
	_XSystemTime tmp;
	getTimeMs(tmp);
	return tmp;
}
extern bool isBefore(const _XSystemTime &t1,const _XSystemTime &t2);		//判断t1是否在t2之前
extern long getElapsedTime(const _XSystemTime &ts,const _XSystemTime &te);	//计算两个时间之间的时间差，单位为ms
extern const int dayInMonth[12];
//下面的函数都可以进行优化，但是尚未进行
inline int getMsInThisYear(const _XSystemTime &t)	//经过验证
{//计算这个日期在这一年中的毫秒值
	if(!getDateIsCorrect(t)) return 0;
	int ret = t.hour * 60 * 60 * 1000 + t.minute * 60 * 1000 + t.second * 1000 + t.millisecond;
	if(t.month <= 2) return (dayInMonth[t.month - 1] + t.day) * MILLISECOND_IN_ONE_DAY + ret;
	if(isLeapYear(t.year))//闰年
		return (dayInMonth[t.month - 1] + t.day) * MILLISECOND_IN_ONE_DAY + ret;
	else//非闰年
		return (dayInMonth[t.month - 1] - 1 + t.day) * MILLISECOND_IN_ONE_DAY + ret;
}
//计算指定时间是一年中的第几天,假设这个日期是合法的
inline int getDateDaysInYear(const _XSystemTime &t)	//经过验证
{
	if(t.month <= 2) return dayInMonth[t.month - 1] + t.day;
	if(isLeapYear(t.year)) return dayInMonth[t.month - 1] + t.day;	//闰年
	else return dayInMonth[t.month - 1] - 1 + t.day;//非闰年
}
//计算从公元元年以来到指定日期的天数
inline int getTotalDays(const _XSystemTime &t)
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
inline int getDataWeek(const _XSystemTime &t)	//经过验证
{
	return getTotalDays(t) % 7;
}
//计算指定年的第几天是什么日期，星期几
extern void getDateData(int year,int day,_XSystemTime &t,int &week);
//目前使用网络上的算法只能演算1900年-2050年的农历
extern void solar2lunar(const _XSystemTime &in,_XSystemTime &out);	//阳历转阴历
extern void lunar2solar(const _XSystemTime &in,_XSystemTime &out);	//阴历转阳历（尚未实现）
#include <string>
inline std::string sysTime2String(const _XSystemTime &in)	//时间转换成string 20140915-095643-377
{
	char tmp[20];
	sprintf(tmp,"%04d%02d%02d-%02d%02d%02d-%03d",in.year,in.month,in.day,
        in.hour,in.minute,in.second,in.millisecond);
	return tmp;
}
inline _XSystemTime string2sysTime(const std::string &str)
{
	_XSystemTime in;
	sscanf(str.c_str(),"%04d%02d%02d-%02d%02d%02d-%03d",&in.year,&in.month,&in.day,
        &in.hour,&in.minute,&in.second,&in.millisecond);
	return in;
}
#endif