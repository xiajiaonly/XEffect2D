#ifndef _JIA_XOBJECTBASIC_
#define _JIA_XOBJECTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.1
//--------------------------------
#include "XBasicClass.h"
#include "XOprateDes.h"
#include <vector>
enum _XObjectType
{
	OBJ_NULL,			//无效的物件
	OBJ_SPRITE,			//精灵的物件
	OBJ_FRAME,			//序列帧的物件
	OBJ_FRAMEEX,		//序列帧的物件
	OBJ_NUMBER,			//数字字符串的物件
	OBJ_FONTUNICODE,	//显示字符的物件
	OBJ_FONTX,			//显示字符的物件
	OBJ_NODELINE,		//节点曲线的物件
	OBJ_CONTROL,		//控件的物件
};
extern string objectTypeStr[];
//这个是能被物件管理器管理的物件的基类，能被管理的所有物件都是这个类的派生类
class _XObjectManager;
class _XObjectBasic
{
//private:
	friend _XObjectManager;
protected:
	_XObjectType m_objType;
public:
	virtual _XObjectType getObjectType() const {return m_objType;}
	virtual _XBool isInRect(float x,float y) = 0;		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	virtual _XVector2 getBox(int order) = 0;			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	virtual void setPosition(float x,float y) = 0;		//设置物件的位置	+Child处理
	virtual void setPosition(const _XVector2& pos) {setPosition(pos.x,pos.y);}
	virtual _XVector2 getPosition() const  = 0;				//获取物件的尺寸

	virtual void setSize(float x,float y) = 0;			//设置物件的尺寸	+Child处理
	virtual void setSize(const _XVector2& size) {setSize(size.x,size.y);}
	virtual void setSize(float size) {setSize(size,size);}
	virtual _XVector2 getSize() const  = 0;					//获取物件的尺寸

	virtual void setAngle(float angle) = 0;					//设置物件的角度	+Child处理
	virtual float getAngle() const = 0;						//获取物件的角度
	//virtual ~_XObjectBasic();
	virtual void setVisible() = 0;					//设置物件可见		+Child处理
	virtual void disVisible() = 0;					//设置物件不可见	+Child处理
	virtual _XBool getVisible() const = 0;					//获取物件是否可见的状态 
	//新加入的接口
	virtual void draw() = 0;

	virtual void setColor(float r,float g,float b,float a) = 0;		//+Child处理
	virtual void setColor(const _XFColor& color) {setColor(color.fR,color.fG,color.fB,color.fA);}
	virtual _XFColor getColor() const = 0;				
	virtual void setAlpha(float a) = 0;								//+Child处理

	_XObjectBasic()
		:m_parent(NULL)
		,m_objType(OBJ_NULL)
	{}
	virtual ~_XObjectBasic()
	{
		if(m_parent != NULL)
		{//从他的父物体中注销自己
			m_parent->popChild(this);
			m_parent = NULL;
			clearAllChild();	//删除它的所有子物体
		}
	}

	//下面增加对物件从属关系的控制
protected:
	_XObjectBasic *m_parent;	//父物体
	std::vector<_XObjectBasic *> m_childList;	//孩子列表,任何一个物体只能有一个父，任何一个物体可以有多个子物体
	std::vector<_XVector2> m_childRelativePos;	//相对位置
	std::vector<_XVector2> m_childRelativeSize;	//相对大小
	std::vector<float> m_childRelativeAngle;	//相对角度
												//为了设计的简单，这里暂时使用颜色和透明度的不做相对变化而是统一变化
	virtual void updateChildPos();
	virtual void updateChildSize();
	virtual void updateChildAngle();
	virtual void updateChildAlpha();
	virtual void updateChildColor();
	virtual void updateChildVisible();
public:
	virtual void clearAllChild();	//删除所有的子物体
	virtual void pushChild(_XObjectBasic * child);	//推入
	virtual void popChild(_XObjectBasic * child);	//推出
	virtual _XBool getIsChild(_XObjectBasic * child);	//检查目标是否为自己的子成员
	//这个测试尚未完成，所以这一行还保留着
	//virtual void justForTest() = 0;	//这一行的目的是为了通过开启这一行以便于通过编译器查找需要修改的地方
};

#endif
