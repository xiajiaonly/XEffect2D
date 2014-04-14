//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XTimer.h"

_XTimer::_XTimer()
:m_stopped(false)
,m_startTime(0)
,m_endTime(0)
,m_upTime(0)
{
#ifdef XEE_OS_WINDOWS
    QueryPerformanceFrequency(&m_frequency);
	m_perFreq = 1000000.0 / m_frequency.QuadPart;
    m_startCount.QuadPart = 0;
    m_endCount.QuadPart = 0;
#else
    m_startCount.tv_sec = m_startCount.tv_usec = 0;
    m_endCount.tv_sec = m_endCount.tv_usec = 0;
#endif
	getUpElapsedTime();
}
//计算时间差，单位为毫秒
double _XTimer::getElapsedTimeInMicroSec()
{
#ifdef XEE_OS_WINDOWS
    if(!m_stopped)
        QueryPerformanceCounter(&m_endCount);

    m_startTime = m_startCount.QuadPart * m_perFreq;
    m_endTime = m_endCount.QuadPart * m_perFreq;
#else
    if(!m_stopped)
        gettimeofday(&m_endCount, NULL);

    m_startTime = (m_startCount.tv_sec * 1000000.0) + m_startCount.tv_usec;
    m_endTime = (m_endCount.tv_sec * 1000000.0) + m_endCount.tv_usec;
#endif

    return m_endTime - m_startTime;
}
double _XTimer::getUpElapsedTime()
{
	double tempTime = m_upTime;
#ifdef XEE_OS_WINDOWS
	QueryPerformanceCounter(&m_upCount);
	m_upTime = m_upCount.QuadPart * m_perFreq;
#else
    gettimeofday(&m_upCount);
    m_upTime = (m_upCount.tv_sec * 1000000.0) + m_upCount.tv_usec;
#endif
	return (m_upTime - tempTime) * 0.001;
}
//下面是一个通用的方法用于获取系统时间，单位到ms
void getTimeMs(_XSystemTime &t)
{
#ifdef XEE_OS_WINDOWS
    SYSTEMTIME stime;
    memset(&stime,0,sizeof(stime));
    GetSystemTime(&stime);
    t.year = stime.wYear;
    t.month = stime.wMonth;
    t.day = stime.wDay;
    t.hour = stime.wHour + 8;    //中国为东8时区
    t.minute = stime.wMinute;
    t.second = stime.wSecond;
    t.millisecond = stime.wMilliseconds;
	//GetTickCount(); //返回毫秒的时间计时
#endif
#ifdef XEE_OS_LINUX
    time_t timep;
    struct tm *p;
    struct timeval nowTime;
    time(&timep);
    p = gmtime(&timep); 
    t.year = p->tm_year + 1900;
    t.month = p->tm_mon + 1;    //0为一月
    t.day = p->tm_mday;
    t.hour = p->tm_hour + 8;
    t.minute = p->tm_min;
    t.second = p->tm_sec;
    gettimeofday(&nowTime,NULL);
    t.millisecond = nowTime.tv_usec/1000.0f;
#endif
}
long getElapsedTime(const _XSystemTime &ts,const _XSystemTime &te)
{//尚未测试
	if(!getDateIsCorrect(ts) || !getDateIsCorrect(te)) return 0;
	_XSystemTime t1 = ts;
	_XSystemTime t2 = te;
	if(!isBefore(ts,te))
	{
		t1 = te;
		t2 = ts;
	}
	long ret = 0;
	for(int i = t1.year;i < t2.year;++ i)
	{
		if(isLeapYear(i)) ret += DAY_IN_ONE_LEAP_YEAR * MILLISECOND_IN_ONE_DAY;
		else ret += (DAY_IN_ONE_LEAP_YEAR - 1) * MILLISECOND_IN_ONE_DAY;
	}
	ret += getMsInThisYear(t2) - getMsInThisYear(t1);

	return ret;
}
bool getDateIsCorrect(const _XSystemTime &t)
{
	if(t.year < 0) return false;
	if(t.month <= 0 || t.month > 12) return false;
	if(t.day <= 0) return false;
	if((t.month == 1 || t.month == 3 || t.month == 5 || 
		t.month == 7 || t.month == 8 || t.month == 10 || t.month == 12) && t.day > 31) return false;
	if((t.month == 4 || t.month == 6 || t.month == 9 || t.month == 11) && t.day > 30) return false;
	if(t.month == 2)
	{
		if(isLeapYear(t.year))
		{
			if(t.day > 29) return false;
		}else 
			if(t.day > 28) return false;
	}
	if(t.hour <= 0 || t.hour >= 24) return false;
	if(t.minute <= 0 || t.minute >= 60) return false;
	if(t.second <= 0 || t.second >= 60) return false;
	if(t.millisecond <= 0 || t.millisecond >= 1000) return false;
	return true;
}
bool isBefore(const _XSystemTime &t1,const _XSystemTime &t2)
{
	if(t1.year < t2.year) return true;
	if(t1.year > t2.year) return false;
	if(t1.month < t2.month) return true;
	if(t1.month > t2.month) return false;
	if(t1.day < t2.day) return true;
	if(t1.day > t2.day) return false;
	if(t1.hour < t2.hour) return true;
	if(t1.hour > t2.hour) return false;
	if(t1.minute < t2.minute) return true;
	if(t1.minute > t2.minute) return false;
	if(t1.second < t2.second) return true;
	if(t1.second > t2.second) return false;
	if(t1.millisecond < t2.millisecond) return true;
	if(t1.millisecond > t2.millisecond) return false;
	//两个时间完全相同
	return false;
}