#ifndef _JIA_XNODELINE_
#define _JIA_XNODELINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XObjectBasic.h"
#include "XMath/XBezierSpline.h"
namespace XE{
#define NODELINE_DATA_FILE_NAME "line/line00000.dat"
#pragma pack(push)
#pragma pack(1)
struct XNode
{
	XVector2 myPoint;		//自身点的位置
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
enum XLeadMode
{
//	LEAD_MODE_NO_ROTATE = 0x01,		//没有角度变化
//	LEAD_MODE_ROTATE = 0x02,			//随着曲线的角度变化而变化
	LEAD_MODE_FOREWORD = 0x04,		//顺向移动曲线
	LEAD_MODE_BACKWORD = 0x08			//逆向移动曲线
};
//引导曲线中的特殊点
struct XNodeSpecialPoint
{
	XBool isEnable;			//特殊点是否有效
	XBool isEnd;				//特殊点是否已经到达曲线的终点
	float curLength;		//特殊点走过的距离
	XVector2 position;		//特殊点的位置
	float angle;			//特殊点的角度
	float upNodeLength;		//上一个节点的里程
	int upNodeOrder;		//上一个节点的编号
	XVector2 divergePoint;	//特殊点的偏离位置
	float speed;			//特殊点的速度(pixel per second)
	XNodeSpecialPoint()
		:isEnable(XFalse)
		,isEnd(XTrue)
	{}
};
//做扩展的时候再使用
//typedef int XNodeSpecialPointHandle	//引导节点中的特殊点的句柄

//注意：下面一些合理性要求，1、线中最少需要两个点，2、线中不能出现连续重合的点。（这两点还没有解决）
//功能方面增加贝塞尔差值，使得曲线更加圆润
//如果需要使用贝塞尔插值的话分段不能太长
class XSCounter;
//节点引导曲线的类
class XNodeLine:public XObjectBasic
{
private:
	static const int m_maxNodeSum = 64;//节点曲线中节点数量的最大限制
	XVector2 *m_bezierPoint;		//用于贝塞尔差值的关键点
	XBool m_needUpdateData;			//是否需要更新贝塞尔差值的点
	XBool m_isLoop;					//曲线是否闭合0不闭合 1闭合，注意闭合的点，首点和尾点不能重合，否则将会失去尾点的方向
	XBool m_isACopy;					//是否是一个副本
	XBool m_isVisible;
	void updateData();				//刷新贝塞尔差值的关键点
	XBezierSpline m_bezierSpline;	//用于贝塞尔差值的线

	XSCounter *m_cp;				//引用计数器
	int m_myOrder;					//本身引导线的编号
	XNode *m_node;					//引导线中的节点
	XLeadMode m_leadMode;					//曲线的引导模式

	XVector2 m_offsetPosition;		//整体曲线的偏移位置
	XVector2 m_scale;				//这个曲线的所放笔需要按照比例进行，不能够使用水平和垂直的不同比例，否则将会造成曲线中角度的失真严重

	float m_lineLength;				//曲线的总长度
	int m_nodeSum;					//引导曲线中的引导节点的数量
public:
	void setVisible() {m_isVisible = XTrue;}					//设置物件可见
	void disVisible() {m_isVisible = XFalse;}					//设置物件不可见
	XBool getVisible() const {return m_isVisible;}			//获取物件是否可见的状态 
	//用于产生引导曲线
	void setOneNode(const XVector2& node,int nodeOrder);	//改变其中某个点的位置
	void setOneNode(float x,float y,int nodeOrder);	//改变其中某个点的位置
	void addOneNode(const XVector2& node);		//向曲线头部添加一个点
	void addOneNode(float x,float y);	//向曲线头部添加一个点
	void decreaseOneNode();				//从曲线头部剔除一个点
	void addOneNode(const XVector2& node,int nodeOrder);		//向曲线中间添加一个点
	void addOneNode(float x,float y,int nodeOrder);		//向曲线中间添加一个点
	void decreaseOneNode(int nodeOrder);	//从曲线中间剔除一个点
	XBool saveNodeLine();					//保存引导曲线，使用何种文件命名方式，还有待磋商
	int getOrder() const;						//获取自身编号的信息
	void setMyOrder(int temp);			//设置曲线的编号，每条曲线都有一个编号，在保存曲线时，用于区分曲线
	void clearUp();						//清除所有的点
	void setLoop();						//设置曲线闭合
	int getNodeOrder(float x,float y,float distance) const	//或者指定点符合距离要求的最近的点的编号，如果没有则返回-1
	{
		if(distance < 0) return -1;
		XVector2 temp(x,y);
		for(int i = 0;i < m_nodeSum;i ++)
		{
			if(temp.getLength(m_node[i].myPoint) <= distance) return i;
		}
		return -1;
	}

	int getNodeSum() const {return m_nodeSum;}//返回节点曲线中的节点数量
	XVector2 getNode(int order) const	//获取节点曲线中某个节点的位置
	{
		if(order < 0 || order >= m_nodeSum) return XVector2::zero;
		return m_node[order].myPoint;
	}
	//int setSize(float size);			//设置曲线的尺寸
	using XObjectBasic::setPosition;
	void setPosition(float x,float y);	//设置曲线的偏移位置
	XVector2 getPosition() const {return m_offsetPosition;}
	XVector2 getScale() const {return m_scale;}
	using XObjectBasic::setScale;
	void setScale(float x,float y) {m_scale.set(x,y);}

	float getAngle() const {return 0.0f;}	//这两个接口对于这个类没有实际的意义
	float getNodeLineLength() const {return m_lineLength;}//获取节点曲线的总长度
	void setAngle(float) {}	//这两个接口对于这个类没有实际的意义
	//为了物件管理而实现下面的接口
	XBool isInRect(float x,float y);
	XVector2 getBox(int order);

private:
	//引导曲线的使用
	XNodeSpecialPoint m_specialPoint;	//曲线中的特殊点
	void release();		//释放资源
	bool loadFromFolder(const char *filename);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源
public:
	XBool getNodeLine(XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);					//从文件中读取引导曲线
	void moveSpecialPoint(int timeDelay,int isLoop,XBool needBezier = XFalse);	//移动曲线中的特种点，增加功能是否使用贝塞尔差值
	void resetSpecialPoint(XBool needBezier = XFalse);				//重置特征点

	void setLeadMode(XLeadMode mode);				//设置曲线的类型，顺向运行的曲线，或者逆向运行的曲线
	void setSpecialPointSpeed(float speed);	//设置曲线中的点的速度
	XBool getSpecialPointIsEnd() const;				//获取曲线中的点是否已经抵达终点
	XVector2 getSpecialPointPosition() const;		//获取曲线中的点的位置
	float getSpecialPointAngle() const;			//获取曲线中的点的当前角度[0 - PI]

	//setACopy这个函数是利用目标的资源，所以如果目标被提前释放掉，则会造成问题，这里需要慎重使用
	XBool setACopy(const XNodeLine &temp);			//建立一个副本，副本拥有和原本一样的资源，但是其拥有自己的个性，副本的资源是不能修改的
	XNodeLine(const XNodeLine &temp);				//重载的拷贝构造函数，完全独立的两个东西，只是属性值相同而已
	XNodeLine& operator = (const XNodeLine& temp);//重载的赋值操作符，完全独立的两个东西，只是属性值相同而已

	virtual void setColor(float,float,float,float){}
	virtual XFColor getColor() const {return XFColor::white;}
	virtual void setAlpha(float){}

	void draw();

	XNodeLine();
	~XNodeLine();	//这里用析构函数释放资源

	//virtual void justForTest() {}
};
#if WITH_INLINE_FILE
#include "XNodeLine.inl"
#endif
}
#endif