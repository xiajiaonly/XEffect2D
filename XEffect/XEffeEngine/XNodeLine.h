#ifndef _JIA_XNODELINE_
#define _JIA_XNODELINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "stdlib.h"
#include "XBasicClass.h"
#include "XBasicFun.h"
#include "XObjectBasic.h"

#define NODELINE_DATA_FILE_NAME ("line/line00000.dat")

#pragma pack(push)
#pragma pack(1)
struct _XNode
{
	_XVector2 myPoint;		//自身点的位置
	float toNextAngle;		//到下一个点的角度
	float toNextSin;		//到下一个点的角度的sin值
	float toNextCos;		//到下一个点的角度的cos值
	float toNextLength;		//到下一个点的距离

	float toPreviousAngle;	//到前一个点的角度
	float toPreviousSin;	//到前一个点的角度的sin值
	float toPreviousCos;	//到前一个点的角度的cos值
	float toPreviousLength;	//到前一个点的距离
};
#pragma pack(pop)

//引导线的引导模式
enum
{
//	_XLEAD_MODE_NO_ROTATE = 0x01,		//没有角度变化
//	_XLEAD_MODE_ROTATE = 0x02,			//随着曲线的角度变化而变化
	_XLEAD_MODE_FOREWORD = 0x04,		//顺向移动曲线
	_XLEAD_MODE_BACKWORD = 0x08			//逆向移动曲线
};

//引导曲线中的特殊点
struct _XNodeSpecialPoint
{
	char isEnable;			//特殊点是否有效
	char isEnd;				//特殊点是否已经到达曲线的终点
	float nowLength;		//特殊点走过的距离
	_XVector2 position;		//特殊点的位置
	float angle;			//特殊点的角度
	float upNodeLength;		//上一个节点的里程
	int upNodeOrder;		//上一个节点的编号
	_XVector2 divergePoint;	//特殊点的偏离位置
	float speed;			//特殊点的速度
	_XNodeSpecialPoint()
		:isEnable(0)
		,isEnd(1)
	{
	}
};
//节点曲线中节点数量的最大限制
#define MAX_NODE_SUM 64
//做扩展的时候再使用
//typedef int XNodeSpecialPointHandle	//引导节点中的特殊点的句柄

//注意：下面一些合理性要求，1、线中最少需要两个点，2、线中不能出现连续重合的点。（这两点还没有解决）

class _XSCounter;
//节点引导曲线的类
class _XNodeLine:public _XObjectBasic
{
private:
	_XSCounter *m_cp;				//引用计数器

	int m_myOrder;					//本身引导线的编号
	_XNode *m_node;					//引导线中的节点
	int m_leadMode;					//曲线的引导模式
	int m_isLoop;					//曲线是否闭合0不闭合 1闭合，注意闭合的点，首点和尾点不能重合，否则将会失去尾点的方向

	_XVector2 m_offsetPosition;		//整体曲线的偏移位置
	_XVector2 m_size;					//这个曲线的所放笔需要按照比例进行，不能够使用水平和垂直的不同比例，否则将会造成曲线中角度的失真严重
	char m_isACopy;					//是否是一个副本
	char m_isVisiable;

	float m_lineLength;					//曲线的总长度
	int m_nodeSum;						//引导曲线中的引导节点的数量
public:
	void setVisiable() {m_isVisiable = 1;}					//设置物件可见
	void disVisiable() {m_isVisiable = 0;}						//设置物件不可见
	char getVisiable() const {return m_isVisiable;}					//获取物件是否可见的状态 
	//用于产生引导曲线
	void setOneNode(const _XVector2& node,int nodeOrder);	//改变其中某个点的位置
	void setOneNode(float x,float y,int nodeOrder);	//改变其中某个点的位置
	void addOneNode(const _XVector2& node);		//向曲线头部添加一个点
	void addOneNode(float x,float y);	//向曲线头部添加一个点
	void decreaseOneNode();				//从曲线头部剔除一个点
	void addOneNode(const _XVector2& node,int nodeOrder);		//向曲线中间添加一个点
	void addOneNode(float x,float y,int nodeOrder);		//向曲线中间添加一个点
	void decreaseOneNode(int nodeOrder);	//从曲线中间剔除一个点
	int saveNodeLine();					//保存引导曲线，使用何种文件命名方式，还有待磋商
	int getOrder() const;						//获取自身编号的信息
	void setMyOrder(int temp);			//设置曲线的编号，每条曲线都有一个编号，在保存曲线时，用于区分曲线
	void clearUp();						//清除所有的点
	void setLoop();						//设置曲线闭合
	int getNodeOrder(float x,float y,float distance) const	//或者指定点符合距离要求的最近的点的编号，如果没有则返回-1
	{
		if(distance < 0) return -1;
		_XVector2 temp(x,y);
		for(int i = 0;i < m_nodeSum;i ++)
		{
			if(temp.getLength(m_node[i].myPoint) <= distance) return i;
		}
		return -1;
	}

	int getNodeSum() const	//返回节点曲线中的节点数量
	{
		return m_nodeSum;
	}
	_XVector2 getNode(int order) const	//获取节点曲线中某个节点的位置
	{
		if(order < 0 || order >= m_nodeSum) return _XVector2(0.0f,0.0f);
		return m_node[order].myPoint;
	}
	//int setSize(float size);			//设置曲线的尺寸
	void setPosition(const _XVector2& position);	//设置曲线的偏移位置
	void setPosition(float x,float y);	//设置曲线的偏移位置
	_XVector2 getPosition() const
	{
		return m_offsetPosition;
	}
	_XVector2 getSize() const
	{
		return m_size;
	}
	void setSize(float x,float y)
	{
		m_size.set(x,y);
	}
	void setSize(float x)
	{
		m_size.set(x,x);
	}
	void setSize(const _XVector2& size)
	{
		m_size = size;
	}
	float getAngle() const	//这两个接口对于这个类没有实际的意义
	{
		return 0;
	}
	float getNodeLineLength() const	//获取节点曲线的总长度
	{
		return m_lineLength;
	}
	void setAngle(float angle)	//这两个接口对于这个类没有实际的意义
	{//啥也不做
	}
	//为了物件管理而实现下面的接口
	int isInRect(float x,float y);
	_XVector2 getBox(int order);

private:
	//引导曲线的使用
	_XNodeSpecialPoint m_specialPoint;	//曲线中的特殊点
	void release();		//释放资源
public:
	int getNodeLine(int resoursePosition = 0);					//从文件中读取引导曲线
	void moveSpecialPoint(int timeDelay,int isLoop);	//移动曲线中的特种点
	void resetSpecialPoint();				//重置特征点

	void setLeadMode(int mode);				//设置曲线的类型，顺向运行的曲线，或者逆向运行的曲线
	void setSpecialPointSpeed(float speed);	//设置曲线中的点的速度
	int getSpecialPointIsEnd() const;				//获取曲线中的点是否已经抵达终点
	_XVector2 getSpecialPointPosition() const;		//获取曲线中的点的位置
	float getSpecialPointAngle() const;			//获取曲线中的点的当前角度

	//setACopy这个函数是利用目标的资源，所以如果目标被提前释放掉，则会造成问题，这里需要慎重使用
	int setACopy(const _XNodeLine &temp);			//建立一个副本，副本拥有和原本一样的资源，但是其拥有自己的个性，副本的资源是不能修改的
	_XNodeLine(const _XNodeLine &temp);				//重载的拷贝构造函数，完全独立的连个东西，只是属性值相同而已
	_XNodeLine& operator = (const _XNodeLine& temp);//重载的赋值操作符，完全独立的连个东西，只是属性值相同而已

	_XNodeLine();
	~_XNodeLine();	//这里用析构函数释放资源
};

#include "XNodeLine.inl"

#endif