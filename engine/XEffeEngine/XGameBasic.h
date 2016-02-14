#ifndef _JIA_XGAMEBASIC_
#define _JIA_XGAMEBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
namespace XE{
//这是游戏主类的基类
class XGameBasic
{
private:
public:
	virtual bool init() = 0;							//TODO:这里进行游戏的初始化
	virtual void move(float stepTime) = 0;				//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	virtual void draw() = 0;							//TODO:绘制的函数
	virtual void input(const XInputEvent &event) = 0;	//TODO:键盘或者鼠标输入的事件
	virtual void release() = 0;							//TODO:程序结束时的资源释放
	virtual void ctrlEventProc(int id,int eventID) = 0;	//TODO：程序在这个函数中处理控件事件
};
class XGameBasic3D:public XGameBasic
{
private:
public:
	virtual void draw3D(XBool) = 0;							//TODO:绘制的函数
};
}
#endif