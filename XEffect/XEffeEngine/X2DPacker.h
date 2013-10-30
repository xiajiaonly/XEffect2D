#ifndef _JIA_X2DPACKER_
#define _JIA_X2DPACKER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

#include "XEffeEngine.h"

//2D,不旋转装箱问题
//1、使用第一个能装入及装入的算法

/*角落类型参考下图
|- 0 1-|
|_ 3 2_|
*/
struct _XCorner
{
	int m_isEnable;			//这个角落是否有效
	_XVector2 m_position;	//角落的位置;
	int m_type;				//角落的类型
};

//需要放置的物体
class _XObjectBox
{
public:
	int m_setOrder;			//外部编号，由用户设置的，这个值内部不会改变
    int m_order;			//物件的编号，这个是内部编号
    int m_basicBoxOrder;    //被放置入的封包的编号 -1表示还没有放入
    _XVector2 m_position;	//物件的位置

    _XVector2 m_size;		//物件的尺寸
    float m_acreage;        //物件的面积

	_XRect m_AABB;			//物体的盒子，用于判断是否包含
	bool m_needRotate;		//物体被放入之后是否需要进行旋转
};

//用于放置物体的基础封包
class _XBasicBox
{
public:
    int m_order;				//箱子的编号
    int m_beUsed;				//箱子是否被使用:0表示被装过东西，n表示装入了n个东西
	_XVector2 m_size;			//箱子的尺寸
    float m_releaseAcreage;		//箱子的剩余空间

	int *m_objectBoxesOrder;	//已经放入箱子的盒子的编号列表
	//这是第三种放置方式(没有实现)
//    int *m_xEdge;				//有效可依靠的水平边
//    int *m_yEdge;				//有效可以靠的垂直边
	//这是第一种放置方式
	int m_mayPositionSum;		//有效放入点的数量
	_XVector2 *m_mayPosition;			//可以放入的点
	//这是第二种放置方式
	//优化遍历所有可能的拐角
	int m_mayPositionXSum;		//有可能的X的数量
	int *m_mayPositionX;		//有可能的X
	int m_mayPositionYSum;		//有可能的Y的数量
	int *m_mayPositionY;		//有可能的Y

    int putInRatio(const _XObjectBox &temp);    //箱子放入的合适度
};

//2D非旋转装箱的类
class _X2DPacker
{
private:
	int putInABox(_XObjectBox &objectBox);	//将一个盒子装入箱子，返回是否装入成功
	int canPutIn(_XObjectBox &objectBox,_XBasicBox &basicBox);	//是否能放得下这个盒子
	int m_optimizeLevel;	//使用哪种方式方式0、为第一种放置方式 1、为第二种放置方式

	void orderObjectByWidth();	//通过宽度对物件进行排序
	void orderObjectByArea();	//通过面积对物件进行排序
	void orderObjectByBigEdgeAndArea();	//通过面积对物件进行排序
	void updatePutOperate(_XObjectBox &objectBox,_XBasicBox &basicBox,bool needRotate,float positionX,float positionY);	//完成放入操作，标记相应的属性
	void updateNewPoint(_XBasicBox &basicBox,int sizeX,int sizeY,int pointOrder);	//对于第一种放置方法，更新放入之后的放入信息
	void updateNewXY(_XBasicBox &basicBox,int x,int y);						//对于第二种放置方式，更新新的可以放置的点

	bool m_canRotate;			//是否支持旋转，这种旋转只支持0或者90度的旋转不能支持任意角度的旋转
public:
	int m_basicBoxSum;			//箱子的数量
    _XBasicBox * m_basicBox;	//箱子的指针
	int m_objectBoxSum;			//需要放的物件的数量
    _XObjectBox * m_objectBox;	//需要放的物件的指针
	//float m_wellRate;			//好坏的程度，用于标识解的好坏程度

	//下面为了资源隐藏儿定义的一些数据
	int init(int boxSum,int objectSum);

    int findResult(int orderMode = 0);			//返回是否找到了合适的解,orderMode优先顺序，默认使用面积优先
	int getNeedBoxSum() const;		//返回使用到了多少个箱子

	void setOptimizeLevel(int temp)	//0:初级优化，速度较快，1:高级优化，速度较慢
	{
		if(temp == 0) m_optimizeLevel = 0;
		else m_optimizeLevel = 1;
	}
	int getOptimizeLevel() const
	{
		return m_optimizeLevel;
	}
	//设置放置的时候是否允许将物件旋转90度
	void setCanRotate(bool temp)
	{
		m_canRotate = temp;
	}
	bool getCanRotate() const
	{
		return m_canRotate;
	}
	void reset();	//重置所有信息，以便于可以重新摆放

	_X2DPacker()
		:m_optimizeLevel(0)
		,m_basicBoxSum(0)
		,m_basicBox(NULL)
		,m_objectBoxSum(0)
		,m_objectBox(NULL)
		//,m_wellRate(1.0f)
		,m_canRotate(false)
	{
	}
};

#endif

