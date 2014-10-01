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
    //memset(&stime,0,sizeof(stime));
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
//const static dayInMonth[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
const int dayInMonth[12] = {0,31,60,91,121,152,182,213,244,274,305,335};
bool getDateIsCorrect(const _XSystemTime &t)
{
	if(t.year < 0 ||
		t.day <= 0) return false;
	switch(t.month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if(t.day > 31) return false;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		if(t.day > 30) return false;
		break;
	case 2:
		if(t.day > 29) return false;
		if(!isLeapYear(t.year)
			&& t.day > 28) return false;
		break;
	default:
		return false;
		break;
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
void getDateData(int year,int day,_XSystemTime &t,int &week)
{
	while(1)
	{//前几年的事情
		if(day <= 0)
		{
			if(isLeapYear(year - 1)) day = 366 + day;
			else day = 365 + day;
			year -= 1;
			continue;
		}
		break;
	}
	while(1)
	{//后几年的事情
		if(isLeapYear(year))
		{
			if(day > 366)
			{
				day -= 366;
				year += 1;
				continue;
			}
		}else
		{
			if(day > 365)
			{
				day -= 365;
				year += 1;
				continue;
			}
		}
		break;
	}
	t.year = year;
	if(day < dayInMonth[2] || (day == dayInMonth[2] && isLeapYear(t.year)))
	{
		if(day <= dayInMonth[1])
		{
			t.month = 1;
			t.day = day;
			week = getDataWeek(t);
			return;
		}else
		{
			t.month = 2;
			t.day = day - dayInMonth[1];
			week = getDataWeek(t);
			return;
		}
	}else
	{
		if(!isLeapYear(t.year)) day += 1;
		for(int i = 3;i < 12;++ i)
		{
			if(day <= dayInMonth[i])
			{
				t.month = i;
				t.day = day - dayInMonth[i - 1];
				week = getDataWeek(t);
				return;
			}
		}
		t.month = 12;
		t.day = day - dayInMonth[11];
		week = getDataWeek(t);
		return;
	}
}
//算法整理自：http://blog.csdn.net/a252228180/article/details/9138281
//阳历1900.1.1 
//在阳历为1900.1.31  时阴历为1900.1.1 即1900年正月初一   
//1900-2050年的农历数据 
//数据格式说明： 
//5位十六进制数字 例:04bd8 
//  1    位: 1表示闰月30天，0表示29天 
//2、3、4位: 转换二进制为:0100 1011 1101(1为30天，0为29天) 
//           04bd8表示为(13个月):29,30,29,29,30,29,30,30,30(闰8月),30,30,29,30; 
//  5    位: 如果有闰月，则为月份，没有则为0
unsigned long int lunarInfo[] =  
{  
    0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,  
    0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,  
    0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,  
    0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,  
    0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,  
    0x06ca0,0x0b550,0x15355,0x04da0,0x0a5b0,0x14573,0x052b0,0x0a9a8,0x0e950,0x06aa0,  
    0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,  
    0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b6a0,0x195a6,  
    0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,	//1980  
    0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,  
    0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,	//2004 07552  
    0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,  
    0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530,	//2028  
    0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,  
    0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0,  
    0x14b63  
};  
inline unsigned char getLeapMonth(unsigned int lunarYear) 
{//确定是否存在农历的闰月 并返回闰月  
    return lunarInfo[lunarYear - 1900]&0xf;  
}  
inline unsigned char getLeapMonthDay(unsigned int lunarYear)
{//若存在闰月,返回闰月的天数,30?29  
    if(getLeapMonth(lunarYear))  
        return(((lunarInfo[lunarYear - 1900]) & 0x10000)?30:29);  
    else  
        return(0);  
}  
inline unsigned char getLunarMonthTotal(unsigned int lunarYear, unsigned char lunarMonth) 
{//确定农历当月天数,30?29    
    return((lunarInfo[lunarYear - 1900] & (0x10000 >> lunarMonth)  )?30:29);  
}   
unsigned int getLunarYearTotal(unsigned int lunarYear)
{// 农历当年总天数,354?355 384 383     
	/*sum=12*29 */   //12个月 29天一月  
	unsigned int sum=348;  
	unsigned int i;  
	for(i=0x8000;i>0x8; i>>=1)   
		sum += (lunarInfo[lunarYear - 1900]&i)?1:0;		//把大月的1加进去  
	return(sum + getLeapMonthDay(lunarYear));			//判断这年是否有闰月  
} 
inline unsigned int getDaysInYear(unsigned int year)
{//获取一年中有多少天
    if(isLeapYear(year)) return 366;   
    else return 365;   
}  
unsigned char getDaysInMonth(unsigned int year,unsigned char month) //判断当年当月天数  
{//获取指定月有多少天
	switch(month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		if(isLeapYear(year)) return 29;
		return 28;
	default:
		return 0;	//非法数据
	} 
}  
inline unsigned int getSolarTotal(unsigned int solarYear, unsigned char solarMonth)  
{//计算1900.1.1 到  输入年月的天数    
	//方案1
	//unsigned int total = 0;  
	//unsigned int tempNum;
	//for(tempNum = 1900;tempNum < solarYear;++ tempNum)  
	//	total += getDaysInYear(tempNum);  
	//for(tempNum = 1;tempNum < solarMonth;++ tempNum)  
	//	total += getDaysInMonth(solarYear,tempNum); 
	//return total;
	//优化方案
	static int daysTo1900 = getTotalDays(_XSystemTime(1900,1,1));
	return getTotalDays(_XSystemTime(solarYear,solarMonth,1)) - daysTo1900;  
} 
void solar2lunar(const _XSystemTime &in,_XSystemTime &out)	//阳历转阴历
{
	if(in.year < 1900 || in.year > 2050 || 
		in.month > 12 || in.month <= 0 || 
		(in.year == 1900 && in.month == 1))  
        return;	//错误的数据  
    if(in.day > getDaysInMonth(in.year,in.month) || in.day == 0)  
        return; //错误的数据 
    unsigned int totalDay = 0;		/*记录农历1900.1.1日到今天相隔的天数 */  
    unsigned char runYueFlag = 0,runYue = 0; 
  
    totalDay = getSolarTotal(in.year,in.month) + in.day - 30; /* 阳历从1900.1.31(农历1900.1.1)到今天的总天数(减30 实际少了一天)。 */  
    out.year = 1900;  
	//这段代码可以优化(既然是查表，完全可以优化到更快，尚未进行，考虑到阳历转农历使用的比较少)
    while(totalDay > 385) //385大于一年  留出一年多的时间用于条件计算  
    {  
        totalDay -= getLunarYearTotal(out.year); //  
        ++ out.year;  
    }  
    if(totalDay > getLunarYearTotal(out.year))  //排除out.year有闰月的情况  
    {  
		totalDay -= getLunarYearTotal(out.year);  
		++ out.year;  
    }  
    runYue = getLeapMonth(out.year);  //当前闰哪个月  
    if(runYue) runYueFlag = 1;	//有闰月则一年为13个月  
    else runYueFlag = 0;		//没闰月则一年为12个月  
  
    if(totalDay == 0)   //刚好一年  
    {  
        out.day = getLunarMonthTotal(out.year,12);  
        out.month = 12;  
    }else  
    {  
		out.month = 1;  
		while(out.month <= 12)  
		{  
			if(runYueFlag == 1 && out.month == (runYue + 1))  //闰月处理  
			{  
				if(totalDay > getLeapMonthDay(out.year))   
					totalDay -= getLeapMonthDay(out.year);  //该年闰月天数  
				//out.month--;  
				runYueFlag = 0;  
				continue;  
			}  
			if( totalDay> getLunarMonthTotal(out.year,out.month ) )  
			{  
				totalDay = totalDay - getLunarMonthTotal(out.year,out.month);  //该年该月天数  
				++ out.month;  
			}else  
			{  
				out.day = totalDay;  
				break;  
			}  
		}  
    }  
}
void lunar2solar(const _XSystemTime &in,_XSystemTime &out)	//阴历转阳历（尚未实现）
{}