#ifndef _JIA_XOBJECTBASIC_
#define _JIA_XOBJECTBASIC_

#include "XBasicClass.h"

//这个是能被物件管理器管理的物件的基类，能被管理的所有物件都是这个类的派生类
class _XObjectBasic
{
//private:
protected:
public:
	virtual int isInRect(float x,float y) = 0;		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	virtual _XVector2 getBox(int order) = 0;			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	virtual void setPosition(float x,float y) = 0;			//设置物件的位置
	virtual void setPosition(const _XVector2& pos) = 0;		//设置物件的位置
	virtual _XVector2 getPosition() const  = 0;				//获取物件的尺寸

	virtual _XVector2 getSize() const  = 0;					//获取物件的尺寸
	virtual void setSize(float x,float y) = 0;				//设置物件的尺寸
	virtual void setSize(const _XVector2& size) = 0;		//设置物件的尺寸

	virtual float getAngle() const = 0;						//获取物件的角度
	virtual void setAngle(float angle) = 0;					//设置物件的角度
	//virtual ~_XObjectBasic();
	virtual void setVisiable() = 0;					//设置物件可见
	virtual void disVisiable() = 0;					//设置物件不可见
	virtual char getVisiable() const = 0;					//获取物件是否可见的状态 
};
#endif
