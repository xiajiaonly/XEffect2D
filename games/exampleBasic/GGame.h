#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
class GGame:public XE::XGameBasic
{
public:
	GGame(){}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();				//TODO:这里进行游戏的初始化
	void move(float stepTime);	//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	void draw();				//TODO:绘制的函数
	void input(const XE::XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
	void ctrlEventProc(int id,int eventID);//TODO:控件的消息处理函数
};
#endif