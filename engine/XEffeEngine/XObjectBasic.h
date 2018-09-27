#ifndef _JIA_XOBJECTBASIC_
#define _JIA_XOBJECTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.1
//--------------------------------
#include "XCommonDefine.h"
#include <string>
#include <vector>
#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
namespace XE{
enum XObjectType
{
	OBJ_NULL,			//无效的物件
	OBJ_SPRITE,			//精灵的物件
	OBJ_FRAME,			//序列帧的物件
	OBJ_FRAMEEX,		//序列帧的物件
	OBJ_NUMBER,			//数字字符串的物件
	OBJ_FONTUNICODE,	//显示字符的物件
	OBJ_FONTX,			//显示字符的物件
	OBJ_NODELINE,		//节点曲线的物件
	OBJ_TURNBOARD,		//翻板子的控件
	OBJ_CONTROL,		//控件的物件
};
extern std::string objectTypeStr[];
//这个是能被物件管理器管理的物件的基类，能被管理的所有物件都是这个类的派生类
class XObjectManager;
class XObjectBasic
{
	//private:
	friend XObjectManager;
protected:
	XObjectType m_objType;
public:
	virtual XObjectType getObjectType() const { return m_objType; }
	virtual XBool isInRect(const XVec2&) = 0;		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	virtual XVec2 getBox(int order) = 0;			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	virtual XVec2 getRectSize() { return getBox(2) - getBox(0); }
	virtual float getRectWidth() { return getBox(2).y - getBox(0).y; }
	virtual float getRectHeight() { return getBox(2).x - getBox(0).x; }

	virtual void setPosition(float x, float y) { setPosition(XVec2(x, y)); }		//设置物件的位置	+Child处理
	virtual void setPosition(const XVec2& pos) = 0;
	virtual const XVec2& getPosition() const = 0;				//获取物件的尺寸

	virtual void setScale(float x, float y) { setScale(XVec2(x, y)); }		//设置物件的尺寸	+Child处理
	virtual void setScale(const XVec2& scale) = 0;
	virtual void setScale(float scale) { setScale(XVec2(scale)); }
	virtual const XVec2& getScale() const = 0;					//获取物件的尺寸

	virtual void setAngle(float angle) = 0;					//设置物件的角度	+Child处理
	virtual float getAngle() const = 0;						//获取物件的角度
	//virtual ~XObjectBasic();
	virtual void setVisible() = 0;					//设置物件可见		+Child处理
	virtual void disVisible() = 0;					//设置物件不可见	+Child处理
	virtual XBool getVisible() const = 0;					//获取物件是否可见的状态 
	//新加入的接口
	virtual void draw() = 0;

	virtual void setColor(float r, float g, float b, float a) { setColor(XFColor(r, g, b, a)); }		//+Child处理
	virtual void setColor(const XFColor& color) = 0;
	virtual const XFColor& getColor() const = 0;
	virtual void setAlpha(float a) = 0;								//+Child处理

	XObjectBasic()
		:m_parent(NULL)
		, m_objType(OBJ_NULL)
	{}
	virtual ~XObjectBasic()
	{
		if (m_parent == NULL) return;
		//从他的父物体中注销自己
		m_parent->popChild(this);
		m_parent = NULL;
		clearAllChild();	//删除它的所有子物体
	}
	//下面增加对物件从属关系的控制
protected:
	XObjectBasic *m_parent;	//父物体
	std::vector<XObjectBasic *> m_childList;	//孩子列表,任何一个物体只能有一个父，任何一个物体可以有多个子物体
	std::vector<XVec2> m_childRelativePos;	//相对位置
	std::vector<XVec2> m_childRelativeSize;	//相对大小
	std::vector<float> m_childRelativeAngle;	//相对角度
												//为了设计的简单，这里暂时使用颜色和透明度的不做相对变化而是统一变化
	virtual void updateChildPos();
	virtual void updateChildScale();
	virtual void updateChildAngle();
	virtual void updateChildAlpha();
	virtual void updateChildColor();
	virtual void updateChildVisible();
public:
	virtual void clearAllChild();	//删除所有的子物体
	virtual void pushChild(XObjectBasic * child);	//推入
	virtual void popChild(XObjectBasic * child);	//推出
	virtual XBool getIsChild(XObjectBasic * child);	//检查目标是否为自己的子成员
	//这个测试尚未完成，所以这一行还保留着
	//virtual void justForTest() = 0;	//这一行的目的是为了通过开启这一行以便于通过编译器查找需要修改的地方
};
}
#endif
