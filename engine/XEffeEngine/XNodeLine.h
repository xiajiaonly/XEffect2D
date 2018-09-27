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
#define NODELINE_DATA_FILE_NAME "line/line%05d.dat"
#pragma pack(push)
#pragma pack(1)
struct XNode
{
	XVec2 myPoint;		//自身点的位置
	//下面是没经过平滑的参数
	float toNextAngle;		//到下一个点的角度
	float toNextSin;		//到下一个点的角度的sin值
	float toNextCos;		//到下一个点的角度的cos值
	float toNextLength;		//到下一个点的距离

	float toPreviousAngle;	//到前一个点的角度
	float toPreviousSin;	//到前一个点的角度的sin值
	float toPreviousCos;	//到前一个点的角度的cos值
	float toPreviousLength;	//到前一个点的距离
	//下面是经过贝塞尔平滑的参数
	float toBezNextAngle;		//到下一个点的角度
	float toBezNextLength;		//到下一个点的距离
	float toBezPreviousAngle;	//到前一个点的角度
	float toBezPreviousLength;	//到前一个点的距离

	XNode(float x = 0.0f, float y = 0.0f)
		:myPoint(x, y)
		, toNextAngle(0.0f)
		, toNextSin(0.0f)
		, toNextCos(0.0f)
		, toNextLength(0.0f)
		, toPreviousAngle(0.0f)
		, toPreviousSin(0.0f)
		, toPreviousCos(0.0f)
		, toPreviousLength(0.0f)
	{}
	XNode(const XVec2& p)
		:myPoint(p)
		, toNextAngle(0.0f)
		, toNextSin(0.0f)
		, toNextCos(0.0f)
		, toNextLength(0.0f)
		, toPreviousAngle(0.0f)
		, toPreviousSin(0.0f)
		, toPreviousCos(0.0f)
		, toPreviousLength(0.0f)
	{}
	void resetToNext()
	{
		toNextAngle = 0.0f;
		toNextSin = 0.0f;
		toNextCos = 0.0f;
		toNextLength = 0.0f;
	}
	void resetToPreviouse()
	{
		toPreviousAngle = 0.0f;
		toPreviousSin = 0.0f;
		toPreviousCos = 0.0f;
		toPreviousLength = 0.0f;
	}
	void setNext(const XVec2& pos)
	{
		toNextAngle = myPoint.getAngle(pos);
		toNextCos = cos(toNextAngle);
		toNextSin = sin(toNextAngle);
		toNextLength = myPoint.getLength(pos);
	}
	void setNext(XNode &node)
	{
		setNext(node.myPoint);
		node.toPreviousAngle = toNextAngle + PI;
		if (node.toPreviousAngle >= PI2) node.toPreviousAngle -= PI2;
		node.toPreviousCos = cos(node.toPreviousAngle);
		node.toPreviousSin = sin(node.toPreviousAngle);
		node.toPreviousLength = toNextLength;
	}
	void setPrevious(const XVec2& pos)
	{
		toPreviousAngle = myPoint.getAngle(pos);
		toPreviousCos = cos(toPreviousAngle);
		toPreviousSin = sin(toPreviousAngle);
		toPreviousLength = myPoint.getLength(pos);
	}
	void setPrevious(XNode &node)
	{
		setPrevious(node.myPoint);
		node.toNextAngle = toPreviousAngle + PI;
		if (node.toNextAngle >= PI2) node.toNextAngle -= PI2;
		node.toNextCos = cos(node.toNextAngle);
		node.toNextSin = sin(node.toNextAngle);
		node.toNextLength = toPreviousLength;
	}
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
	XBool isEnd;			//特殊点是否已经到达曲线的终点
	float curLength;		//特殊点走过的距离
	XVec2 position;		//特殊点的位置
	float angle;			//特殊点的角度
	float upNodeLength;		//上一个节点的里程
	int upNodeOrder;		//上一个节点的编号
	XVec2 divergePoint;	//特殊点的偏离位置
	float speed;			//特殊点的速度(pixel per second)
	XNodeSpecialPoint()
		:isEnable(XFalse)
		, isEnd(XTrue)
	{}
};
struct XNodeCrossInfo
{
	XVec2 pos;	//交叉点的位置
	int indexA;		//顺序位置
	int indexB;
	float angleA;	//角度
	float angleB;
};
struct XNodeMatchInfo
{//两条节点曲线匹配的信息
	int matchLen;	//匹配的长度
	int indexA;		//匹配A起始点的编号
	int indexB;		//匹配B起始点的编号
	bool isOrder;	//是否是顺序匹配
	XNodeMatchInfo()
		:matchLen(0)
	{}
};
struct XNodePointInfo
{
	XVec2 pos;	//插值点的位置
	float angle;	//插值点的角度
	bool isEnd;		//是否结束
	int upIndex;	//最近经过的点的编号
	XNodePointInfo()
		:upIndex(0)
		, isEnd(true)
		, angle(0.0f)
	{}
};
//做扩展的时候再使用
//typedef int XNodeSpecialPointHandle	//引导节点中的特殊点的句柄

//注意：下面一些合理性要求，1、线中最少需要两个点，2、线中不能出现连续重合的点。（这两点还没有解决）
//对于贝塞尔的优化，取所有线段的平均长度作为标准对于超过长度的线进行对拆，直到所有长度都小于或等于平均长度为止
//功能方面增加贝塞尔插值，使得曲线更加圆润
//如果需要使用贝塞尔插值的话分段不能太长，可以程序进行优化，
class XSCounter;
//节点引导曲线的类
class XNodeLine :public XObjectBasic
{
private:
	//	static const int m_maxNodeSum = 512;//节点曲线中节点数量的最大限制
	//	XVec2 *m_bezierPoint;		//用于贝塞尔插值的关键点
	XBool m_needUpdateData;			//是否需要更新贝塞尔插值的点
	XBool m_isLoop;					//曲线是否闭合0不闭合 1闭合，注意闭合的点，首点和尾点不能重合，否则将会失去尾点的方向
	XBool m_isVisible;
public:
	void updateData();				//刷新贝塞尔插值的关键点
private:
	std::vector<XVec2> m_bezierPoint;
	XBezierSpline m_bezierSpline;	//用于贝塞尔插值的线

//	XSCounter *m_cp;				//引用计数器
	int m_myOrder;					//本身引导线的编号
//	XNode *m_node;					//引导线中的节点
	std::vector<XNode> m_node;
	XLeadMode m_leadMode;					//曲线的引导模式

	XVec2 m_offsetPosition;		//整体曲线的偏移位置
	XVec2 m_scale;				//这个曲线的所放笔需要按照比例进行，不能够使用水平和垂直的不同比例，否则将会造成曲线中角度的失真严重

	float m_lineLength;				//曲线的总长度，直线长度和
	float m_bezLineLength;			//曲线经过贝塞尔平滑之后的长度和
//	int m_nodeSum;					//引导曲线中的引导节点的数量
	bool m_withAngleSmooth;			//是否使用角度平滑
public:
	bool getWithAngleSmooth()const { return m_withAngleSmooth; }
	void setWithAngleSmooth(bool flag) { m_withAngleSmooth = flag; }
	void setVisible() { m_isVisible = XTrue; }					//设置物件可见
	void disVisible() { m_isVisible = XFalse; }					//设置物件不可见
	XBool getVisible() const { return m_isVisible; }			//获取物件是否可见的状态 
	//用于产生引导曲线
	void setOneNode(const XVec2& node, int nodeOrder);	//改变其中某个点的位置
//	void setOneNode(float x,float y,int nodeOrder);	//改变其中某个点的位置
	void addOneNode(const XVec2& node);		//向曲线头部添加一个点
//	void addOneNode(float x,float y);	//向曲线头部添加一个点
	void decreaseOneNode();				//从曲线尾部剔除一个点
	void addOneNode(const XVec2& node, int nodeOrder);		//向曲线中间添加一个点
//	void addOneNode(float x,float y,int nodeOrder);		//向曲线中间添加一个点
	void decreaseOneNode(int nodeOrder);	//从曲线中间剔除一个点
	void decreaseNode(int nodeS, int nodeE);	//删除节点S到节点E中的点，并包含S和E(尚未实现)
	XBool saveNodeLine();					//保存引导曲线，使用何种文件命名方式，还有待磋商
	int getOrder() const;						//获取自身编号的信息
	void setMyOrder(int temp);			//设置曲线的编号，每条曲线都有一个编号，在保存曲线时，用于区分曲线
	void clearUp();						//清除所有的点
	void setLoop(bool flag);						//设置曲线闭合
	bool getIsLoop()const { return m_isLoop; }
	//	int getNodeOrder(float x,float y,float distance) const	//或者指定点符合距离要求的最近的点的编号，如果没有则返回-1
	//	{
	//		return getNodeOrder(XVec2(x,y), distance);
	//	}
	int getNodeOrder(const XVec2& p, float distance) const	//或者指定点符合距离要求的最近的点的编号，如果没有则返回-1
	{
		if (distance < 0) return -1;
		distance *= distance;
		for (auto it = m_node.begin(); it != m_node.end(); ++it)
		{
			if (p.getLengthSqure(it->myPoint) <= distance) return it - m_node.begin();
		}
		return -1;
	}

	int getNodeSum() const { return m_node.size(); }//返回节点曲线中的节点数量
	XVec2 getNode(int order) const	//获取节点曲线中某个节点的位置
	{
		if (order < 0 || order >= m_node.size()) return XVec2::zero;
		return m_node[order].myPoint;
	}
	XNode getNodeInfo(int order) const
	{
		if (order < 0 || order >= m_node.size()) return XNode();
		return m_node[order];
	}
	//int setSize(float size);			//设置曲线的尺寸
	using XObjectBasic::setPosition;
	void setPosition(const XVec2& p);	//设置曲线的偏移位置
	const XVec2& getPosition() const { return m_offsetPosition; }
	const XVec2& getScale() const { return m_scale; }
	using XObjectBasic::setScale;
	void setScale(const XVec2& s) { m_scale = s; }

	float getAngle() const { return 0.0f; }	//这两个接口对于这个类没有实际的意义
	float getNodeLineLength() const
	{
		if (m_withBezier) return m_bezLineLength;
		else return m_lineLength;
	}//获取节点曲线的总长度
	void setAngle(float) {}	//这两个接口对于这个类没有实际的意义
	//为了物件管理而实现下面的接口
	XBool isInRect(const XVec2& p);
	XVec2 getBox(int order);

private:
	//引导曲线的使用
	XNodeSpecialPoint m_specialPoint;	//曲线中的特殊点
	void release();		//释放资源
	bool loadFromFolder(const char *filename);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源
public:
	XBool getNodeLine(XResPos resPos = RES_SYS_DEF);					//从文件中读取引导曲线
	void moveSpecialPoint(float timeDelay);	//移动曲线中的特种点，增加功能是否使用贝塞尔差值
	void resetSpecialPoint();				//重置特征点
	void resetSpecialPt(int index, float distance = 0.0f);		//重置特征点从某一点开始移动，并附带一段距离
	void updateBezierInfo();	//更新贝塞尔的数据
private:
	bool m_withBezier;	//是否使用贝塞尔插值
public:
	bool getWithBezier()const { return m_withBezier; }
	void setWithBezier(bool flag);
	//目前这个算法存在问题
	void bezierOptimization();	//针对于贝塞尔对曲线进行优化以便于曲线更加表现合理
	void bezierOptimizationEx()	//针对于贝塞尔对曲线进行优化以便于曲线更加表现合理
	{
		while (getDeformityRate() >= 3.0f)
		{
			bezierOptimization();
		}
	}

	XLeadMode getLeadMode()const { return m_leadMode; }
	void setLeadMode(XLeadMode mode);				//设置曲线的类型，顺向运行的曲线，或者逆向运行的曲线
	void setSpecialPointSpeed(float speed);	//设置曲线中的点的速度
	XBool getSpecialPointIsEnd() const;				//获取曲线中的点是否已经抵达终点
	XVec2 getSpecialPointPosition() const;		//获取曲线中的点的位置
	float getSpecialPointAngle() const;			//获取曲线中的点的当前角度[0 - PI]
	XVec2 getRatePosition(float rate, int *index = NULL, bool needBezier = false);	//rate[0 - 1],返回百分比处的坐标
	XVec2 getRate(const XVec2& pos, int *index = NULL, bool needBezier = false);	//计算与pos最接近的点的百分比和距离

	//setACopy这个函数是利用目标的资源，所以如果目标被提前释放掉，则会造成问题，这里需要慎重使用
	XBool setACopy(const XNodeLine &temp);			//建立一个副本，副本拥有和原本一样的资源，但是其拥有自己的个性，副本的资源是不能修改的
	XNodeLine(const XNodeLine &temp);				//重载的拷贝构造函数，完全独立的两个东西，只是属性值相同而已
	XNodeLine& operator = (const XNodeLine& temp);//重载的赋值操作符，完全独立的两个东西，只是属性值相同而已

	virtual void setColor(const XFColor&) {}
	virtual const XFColor& getColor() const { return XFColor::white; }
	virtual void setAlpha(float) {}

	void draw();
	XNodeLine();
	~XNodeLine();	//这里用析构函数释放资源

	//virtual void justForTest() {}
	//计算两条线的相交信息
	void getCrossInfo(const XNodeLine &line, std::vector<XNodeCrossInfo> &ret);
	std::vector<XNodeCrossInfo> getCrossInfo(const XNodeLine &line)
	{
		std::vector<XNodeCrossInfo> ret;
		getCrossInfo(line, ret);
		return ret;
	}
	XNodeMatchInfo simpleMatch(const XNodeLine &line);	//比较两条线中顺序吻合的节点数量（该方法尚未实现）

	XNodePointInfo getNodePointInfo(float distance);	//计算距离开始点距离为distance的点的信息
	float getDistanceFromStart(int index);				//计算某个节点距离起始点之间的距离
	XNodePointInfo getNodePointInfo(int index, float distance)	//距离节点index的距离为distance的点的信息
	{
		return getNodePointInfo(getDistanceFromStart(index) + distance);
	}
public:
	//isBack是否反方向
	void setBezierSplinePar(XBezierSpline &sp, int index, bool isBack = false)const;	//设置第index个点的贝塞尔曲线的参数
	//获取变形系数，返回值越大畸形系数越大
	float getDeformityRate()const;
};
#if WITH_INLINE_FILE
#include "XNodeLine.inl"
#endif
}
#endif