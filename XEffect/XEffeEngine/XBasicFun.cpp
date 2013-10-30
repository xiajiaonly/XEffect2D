//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicFun.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "XBasicClass.h"
#include "string.h"

//void xChangeTwoSum(int &sum0,int &sum1)
//{
//	int temp = sum0;
//	sum0 = sum1;
//	sum1 = temp;
//}
//
//void xChangeTwoSum(float &sum0,float &sum1)
//{
//	float temp = sum0;
//	sum0 = sum1;
//	sum1 = temp;
//}
//
//void xChangeTwoSum(double &sum0,double &sum1)
//{
//	double temp = sum0;
//	sum0 = sum1;
//	sum1 = temp;
//}

int random(int x)	
{
	if(x == 0) return 0;
	if(x < 0) x = -x;
	if(x <= RAND_MAX)
	{
		return rand()%x;
	}else
	{
		return ((rand() << 12) + (rand() % 0x0fff)) % x;
	}
}

float randomf(float max) 
{
	return max * rand() / (RAND_MAX + 1.0f);
}

float randomf(float x, float y) 
{
	float high = 0;
	float low = 0;
	float randNum = 0;
	if (x == y) return x; 
	high = max(x,y);
	low = min(x,y);
	randNum = low + ((high-low) * rand()/(RAND_MAX + 1.0));
	return randNum;
}
float randomPS()
{
	if((rand() % 100) < 50)
	{
		return (1.0f - cos((rand() % 100) / 100.0f * PI2)) * 0.25f;
	}else
	{
		return 1.0f - (1.0f - cos((rand() % 100) / 100.0f * PI2)) * 0.25f;
	}
}

void randomMomery(char *p,int size)
{
	if(p == NULL) return;
	if(size <= 0) return;
	int order;
	char temp;
	for(int i = 0;i < size; ++ i)
	{
		order = random(size);
		if(order >= 0 && order < size && order != i)
		{
			temp = p[i];
			p[i] = p[order];
			p[order] = temp;
		}
	}
}

int getBit(unsigned char ch,int pos)
{
	if(pos < 0 || pos >=8) return 0;
	//unsigned char temp = 0x01;
	//temp = temp << pos;
	//temp = temp & ch;
	//if(temp == 0) return 0;
	//else return 1;
	return ((0x01 << pos) & ch);
}

int setBit(unsigned char &ch,int pos,int state)
{
	if(pos < 0 || pos >= 8) return 0;
	if(state != 0) state =1;
	
	unsigned char temp;
	if(state == 0)
	{
		temp = 0x01;
		temp = temp << pos;
		temp = ~temp;
		ch = ch & temp;
	}else
	{
		temp = 0x01;
		temp = temp << pos;
		ch = ch | temp;
	}
	return 1;
}

int setBitAnti(unsigned char &ch,int pos)
{
	if(pos < 0 || pos >= 8) return 0;
	unsigned char temp = 0x01;
	temp = temp << pos;
	//if(temp & (* ch) != 0) (* ch) = (* ch) | temp;
	if(getBit(ch,pos) == 0) ch = ch | temp;
	else ch = ch & (~temp);
	return 1;
}

float angleAbbreviation(float angle)
{
	while(1)
	{
		if(angle >= PI2) angle -= (float)(PI2);
		else if(angle < 0) angle += (float)(PI2);
		else break;
	}
	return angle;
}

float getAngleXY(float x,float y)
{
	float angle = 0.0f;
    if(x == 0.0f)
    {
        if(y == 0.0f)
        {
            angle = 0.0f;
        }else
        if(y > 0.0f)
        {
            angle = (float)(PI_HALF);
        }else
        {
            angle = (float)(PI + PI_HALF);
        }
    }else
    if(y == 0.0f)
    {
        if(x > 0.0f)
        {
            angle = 0.0f;
        }else
        {
            angle = (float)(PI);
        }
    }else
    {
        angle = atan(y / x);        //atan() return a value (-PI_HALF , PI_HALF)
        
        if(x < 0.0f)
        {
            angle += (float)(PI);
        }
        if(angle < 0.0f) angle += (float)(PI2);
    }
    return angle;
}

int getCharPosition(const char *p,char temp,int mode)
{
	if(p == NULL) return 0;
	if(mode == 0)
	{//正序查找
		for(int i = 0;;++ i)
		{
			if(*(p + i) == '\0') return -1;	//结束
			if(*(p + i) == temp) return i;
		}
	}else
	{//逆序查找
		for(int i = strlen(p) - 1;i >= 0;-- i)
		{
			if(*(p + i) == temp) return i;
		}
	}
	return -1;
}

int fileNameCompare(const char *p1,const char *p2)
{
	if(p1 == NULL || p2 == NULL) return 0; 
	int len1 = strlen(p1);
	int len2 = strlen(p2);
	if(len1 != len2) return 0;
	//int temp = 'A' - 'a';
	for(int i = 0;i < len1;++ i)
	{
		if(p1[i] == p2[i]) continue;
		if(p1[i] >= 'a' && p1[i] <= 'z' && p1[i] - 'a' + 'A' == p2[i]) continue;else
		if(p1[i] >= 'A' && p1[i] <= 'Z' && p1[i] - 'A' + 'a' == p2[i]) continue;else
		if((p1[i] == '/' && p2[i] == '\\') || (p1[i] == '\\' && p2[i] == '/') ) continue;
		return 0;
	}
	return 1;
}

int getPathDeep(const char *p)
{
	if(p == NULL) return 0;
	int len = strlen(p);
	if(len >= MAX_FILE_NAME_LENGTH) return 0;
	int deep = 0;
	for(int i = 0;i < len;++ i)
	{
		if(p[i] == '\\' || p[i] == '/') 
		{
			deep ++;
		}
	}
	return deep;
}

int getPathDeepByChar(const char *p)
{
	if(p == NULL) return 0;
	if(strlen(p) >= MAX_FILE_NAME_LENGTH) return 0;
	for(int i = strlen(p) - 1;i >= 0;-- i)
	{
		if(p[i] == '\\' || p[i] == '/') 
		{
			return i;
		}
	}
	return 0;
	//int len = strlen(p);
	//int deepByChar = 0;
	//for(int i = 0;i < len;++ i)
	//{
	//	if(p[i] == '\\' || p[i] == '/') 
	//	{
	//		deepByChar = i;
	//	}
	//}
	//return deepByChar;
}

//寻找符合要求的最小的尺寸
int getMinWellSize2n(int  temp)
{
	return (int)powf(2.0, ceilf(logf((float)temp)/logf(2.0f)));
/*	int i = 1;
	while(1)
	{
		if(i > temp) break;
		i = i << 1;
	}
	return i;*/
}

#ifdef SIN_BOOK
float sinData[628] = {0};

void initSinData()
{
	for(int i = 0;i < 628;++i)
	{
		sinData[i] = (float)(sin(PI2 * i /628.0));
	}
}

float xSin(float x)
{
	int temp = (int)(x * 100.0f);
	while(1)
	{
		if(temp < 0) temp += 628;
		else break;
	}
	if(temp >= 628) temp = temp % 628;

	return sinData[temp];
}
#endif

#ifdef SQRT_BOOK
float sqrtData[1000000] = {0};

void initSqrtData()
{
	sinData[0] = 0;
	for(int i = 1;i < 1000000;++i)
	{
		sqrtData[i] = sqrt((double)i);
	}
}

float xSqrt(float x)
{
	if(x < 0 || x >= 1000000) return 0;
	return sqrtData[(int)x];
}
#endif

#ifdef OS_WINDWOS
#include "windows.h"
#include "string.h"
#endif
#ifdef OS_LINUX
#include <unistd.h>           //usleep()
#include <sys/time.h>
#endif


int mySleep(unsigned long sleepUSecond)
{//注意到这里的最小延迟设置为1ms，凡是小于1ms的延迟都模认为1ms
#ifdef OS_WINDOWS
	int delay = (int)(sleepUSecond / 1000);
    if(delay <=0) delay =1;
    Sleep(delay);
#endif
#ifdef OS_LINUX
//方案1
//	if(sleepUSecond < 1000) sleepUSecond = 1000;
//	usleep(sleepUSecond);
//方案2
/*	if(sleepUSecond < 1000) sleepUSecond = 1000;
	timespec t_timeval;
	if(sleepUSecond >= 1000000)
	{//超过秒的延迟直接用秒    
		t_timeval.tv_sec = sleepUSecond /1000000;
		t_timeval.tv_nsec = (sleepUSecond % 1000000) * 1000; 
	}else
	{
		t_timeval.tv_sec = 0;
		t_timeval.tv_nsec = sleepUSecond * 1000; 
	}
	if(nanosleep(&t_timeval,NULL) == -1)
	{
		//DSHOW("sleep error!\n");
	}*/
//方案3
	if(sleepUSecond < 1000) sleepUSecond = 1000;
	timeval t_timeval;
	if(sleepUSecond >= 1000000)
	{//超过秒的延迟直接用秒    
		t_timeval.tv_sec = sleepUSecond /1000000;
		t_timeval.tv_usec = sleepUSecond % 1000000; 
	}else
	{
		t_timeval.tv_sec = 0;
		t_timeval.tv_usec = sleepUSecond; 
	}
	if(select(0, NULL, NULL, NULL, &t_timeval) == -1)
	{
		//DSHOW("sleep error!\n");
	}
//方案4
/*	pthread_cond_t mycond;     
	pthread_mutex_t mymutex;
	
	pthread_cond_init(&mycond, NULL);
   	pthread_mutex_init(&mymutex, NULL);
	
	pthread_mutex_lock(&mymutex); 
	
	if(sleepUSecond < 10000) sleepUSecond = 10000;
	struct timeval now; 
    gettimeofday(&now, NULL); 
    
    timespec abstime;
    abstime.tv_nsec = now.tv_usec * 1000 + (sleepUSecond % 1000000) * 1000; 
    abstime.tv_sec = now.tv_sec + sleepUSecond / 1000000;
	pthread_cond_timedwait(&mycond, &mymutex, &abstime);      
  	//DSHOW("time out dispatch something...\n");   
 
	pthread_mutex_unlock(&mymutex);*/       
#endif
	return 0;	
}

//下面是一个通用的方法用于获取系统时间，单位到ms
_XSystemTime getTimeMs()
{
    _XSystemTime result;
#ifdef OS_WINDOWS
    SYSTEMTIME stime;
    memset(&stime,0,sizeof(stime));
    GetSystemTime(&stime);
    result.year = stime.wYear;
    result.month = stime.wMonth;
    result.day = stime.wDay;
    result.hour = stime.wHour + 8;    //中国为东8时区
    result.minute = stime.wMinute;
    result.second = stime.wSecond;
    result.millisecond = stime.wMilliseconds;
	//GetTickCount(); //返回毫秒的时间计时
#endif
#ifdef OS_LINUX
    time_t timep;
    struct tm *p;
    struct timeval nowTime;
    time(&timep);
    p = gmtime(&timep); 
    result.year = p->tm_year + 1900;
    result.month = p->tm_mon + 1;    //0为一月
    result.day = p->tm_mday;
    result.hour = p->tm_hour + 8;
    result.minute = p->tm_min;
    result.second = p->tm_sec;
    gettimeofday(&nowTime,NULL);
    result.millisecond = nowTime.tv_usec/1000.0f;
#endif
    return result;
}

#ifdef GET_ALL_PIXEL
int XEE_AllPixel = 0;
#endif

void getHalfData(unsigned char H1,unsigned char L1,unsigned char H2,unsigned char L2,unsigned char &Ho,unsigned char &Lo)
{
	int temp = 0;
	if(H1 >= 0x80)
	{
		temp = -(L1 + ((H1 & 0x7f) << 8));
	}else
	{
		temp = L1 + (H1 << 8);
	}
	int temp1 = 0;
	if(H2 >= 0x80)
	{
		temp1 = -(L2 + ((H2 & 0x7f) << 8));
	}else
	{
		temp1 = L2 + (H2 << 8);
	}
	temp = ((temp + temp1) >> 1);
	if(temp < 0)
	{
		Ho = (((- temp) >> 8) | 0x80);
		Lo = (- temp) % 256;
	}else
	{
		Ho = (temp >> 8);
		Lo = (temp % 256);
	}
}

//对任意序列的无旋转矩形进行顺时针排序
void vector4Sort(_XVector2* v,_XVector2* u)
{
	bool isChange;
	for(int j = 0;j < 3;++ j)
	{
		isChange = false;
		for(int i = 0;i < 3;++ i)
		{
			if(v[i].x > v[i + 1].x)
			{
				xChangeTwoSum<_XVector2>(v[i],v[i + 1]);
				xChangeTwoSum<_XVector2>(u[i],u[i + 1]);
				isChange = true;
			}
		}
		if(!isChange) break;
	}
	if(v[2].y < v[3].y)
	{
		xChangeTwoSum<_XVector2>(v[2],v[3]);
		xChangeTwoSum<_XVector2>(u[2],u[3]);
	}
}
//一维映射
float maping1D(float x,float xmin,float xmax,float ymin,float ymax)
{
	return  (x - xmin) / (xmax - xmin) * (ymax - ymin) + ymin;
}
//4个点必须要是已经排序了的
int getEdgePoint(float angle,_XVector2 * points,const _XVector2* v,const _XVector2* u)
{
	int sum = 0;
	_XVector2 centerV((v[0].x + v[2].x) * 0.5f,(v[0].y + v[2].y) * 0.5f);	//提取必要的参数
	_XVector2 centerU((u[0].x + u[2].x) * 0.5f,(u[0].y + u[2].y) * 0.5f);	//提取必要的参数
	float wV = -(v[0].x - v[2].x);
	float hV = -(v[0].y - v[2].y);
	float tempAngle = _XVector2(fabs(wV),fabs(hV)).getAngle(); 
	//默认为顺时针垂直边模式及第一二个点是左边
	points[0] = centerV;
	points[1] = centerU;

	points[2].set(centerV.x,v[0].y); 
	points[3].set((u[0].x + u[3].x) * 0.5f,(u[0].y + u[3].y) * 0.5f); 
	if(angle <= PI_HALF - tempAngle)
	{
		points[4].set(centerV.x - hV * 0.5f * tan(angle),v[0].y);
		points[5].set(maping1D(points[4].x,v[3].x,v[0].x,u[3].x,u[0].x),
			maping1D(points[4].x,v[3].x,v[0].x,u[3].y,u[0].y));//@	//不好映射列
		sum = 3;
	}else
	if(angle <= PI_HALF + tempAngle)
	{
		points[4] = v[0];
		points[5] = u[0];

		points[6].set(v[0].x,centerV.y - wV * 0.5f * tan(PI_HALF - angle));
		points[7].set(maping1D(points[6].y,v[1].y,v[0].y,u[1].x,u[0].x),
			maping1D(points[6].y,v[1].y,v[0].y,u[1].y,u[0].y));	
		sum = 4;
	}else
	if(angle <= PI * 1.5f - tempAngle)
	{
		points[4] = v[0];
		points[5] = u[0];
		points[6] = v[1];
		points[7] = u[1];

		points[8].set(centerV.x - hV * 0.5f * tan(PI - angle),v[1].y);
		points[9].set(maping1D(points[8].x,v[2].x,v[1].x,u[2].x,u[1].x),
			maping1D(points[8].x,v[2].x,v[1].x,u[2].y,u[1].y));	
		sum = 5;
	}else
	if(angle <= PI * 1.5f + tempAngle)
	{
		points[4] = v[0];
		points[5] = u[0];
		points[6] = v[1];
		points[7] = u[1];
		points[8] = v[2];
		points[9] = u[2];
		points[10].set(v[2].x,centerV.y + wV * 0.5f * tan(PI * 1.5f - angle));
		points[11].set(maping1D(points[10].y,v[3].y,v[2].y,u[3].x,u[2].x),
			maping1D(points[10].y,v[3].y,v[2].y,u[3].y,u[2].y));	
		sum = 6;
	}else
	{
		points[4] = v[0];
		points[5] = u[0];
		points[6] = v[1];
		points[7] = u[1];
		points[8] = v[2];
		points[9] = u[2];
		points[10] = v[3];
		points[11] = u[3];
		points[12].set(centerV.x + hV * 0.5f * tan(PI2 - angle),v[3].y);
		points[13].set(maping1D(points[12].x,v[0].x,v[3].x,u[0].x,u[3].x),
			maping1D(points[12].x,v[0].x,v[3].x,u[0].y,u[3].y));	
		sum = 7;
	}
	//points[sum] = center;
	return sum;
}