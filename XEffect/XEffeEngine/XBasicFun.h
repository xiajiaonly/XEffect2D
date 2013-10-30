#ifndef _JIA_XBASICFUN_
#define _JIA_XBASICFUN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XOSDefine.h"
#include "stdio.h"

#define PI 3.1415926535897932384626433832795f
#define PI_HALF 1.5707963267948966192313216916395f								//(PI * 0.5)
#define PI2 6.283185307179586476925286766558f									//(PI * 2.0)
#define RADIAN_TO_ANGLE	57.295779513082320876798154814114f						//(/PI * 180.0)
#define ANGLE_TO_RADIAN	0.01745329251994329576923690768488f						//(/180.0 * PI)

//#define XDELETE_ARRAY(p) { if (p != NULL) { delete [] p; p = NULL; } }
//#define XDELETE(p) { if (p != NULL) { delete p; p = NULL; } }
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

#define MAX_FILE_NAME_LENGTH 256	//文件名的长度

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

//模板分离编译将会造成编译错误，所以只能如此
template<class type> void xChangeTwoSum(type &sum0,type &sum1)
{
	type temp = sum0;
	sum0 = sum1;
	sum1 = temp;
}
//extern void xChangeTwoSum(int &sum0,int &sum1);			//交换两个整形数的值
//extern void xChangeTwoSum(float &sum0,float &sum1);		//交换两个浮点数的值
//extern void xChangeTwoSum(double &sum0,double &sum1);	//交换两个双精度浮点数的值
extern int random(int x);	//强化这个函数的取值范围0 - 0x07ffffff
extern float randomf(float max);
extern float randomf(float x, float y);
extern float randomPS();	//模拟泊松分布的随即函数产生0.0f - 1.0f的数值
//0 - RAND_MAX(0x7fff)		//返回一个0 - x之间的整数
extern void randomMomery(char *p,int size);	//以字节为单位混乱p所指向的大小为size的内存空间

//bits operate
extern int getBit(unsigned char ch,int pos);			//获取ch某一位的值
extern int setBit(unsigned char &ch,int pos,int state);	//设置ch某一位的值
extern int setBitAnti(unsigned char &ch,int pos);		//将ch某一位的值置反

//角度化简
extern float getAngleXY(float x,float y);	//用于获得x,y构成的角度
extern float angleAbbreviation(float angle);			//弧度角度化简

//从字符串中找到最近的一个符合要求的字符的位置,注意如果这里输入的是一个没有结束符的字符串将会导致严重错误
//返回-1表示找不到这个字符
extern int getCharPosition(const char *p,char temp,int mode = 0);	//mode 0:正序查找，1:倒叙查找

extern int fileNameCompare(const char *p1,const char *p2);	//比较两个路径或者文件名是否一致，不考虑大小写
extern int getPathDeep(const char *p);			//获取路径的深度，获取路径目录的层数 e.g.:c:/x/y/xx.txt return 3
extern int getPathDeepByChar(const char *p);	//获取路径的深度，获取路径目录的字符串深度 e.g.:c:/x/y/xx.txt return 6

extern int getMinWellSize2n(int temp);	//寻找比temp大的最小的符合2的n次方的尺寸

extern int mySleep(unsigned long sleepUSecond);	//这个参数的时间是us及1000000分之1秒，及1000分之1毫秒

//输出HL1与HL2的和的一半，本来想用于声音变慢的，结果好像没用上
extern void getHalfData(unsigned char H1,unsigned char L1,unsigned char H2,unsigned char L2,unsigned char &Ho,unsigned char &Lo);
class _XVector2;
extern void vector4Sort(_XVector2* v,_XVector2* u);//对任意序列的无旋转矩形进行顺时针排序
extern float maping1D(float x,float xmin,float xmax,float ymin,float ymax);//一维映射
//4个点必须要是已经排序了的
extern int getEdgePoint(float angle,_XVector2 * points,const _XVector2* v,const _XVector2* u);

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
    {
    }
};

extern _XSystemTime getTimeMs();

#ifdef GET_ALL_PIXEL
extern int XEE_AllPixel;	//总的贴图像素
#endif

#endif 
