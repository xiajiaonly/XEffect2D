#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
class GGame:public XE::XGameBasic,public XE::XDragObjectBase
{
public:
	virtual ~GGame(){}
	GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();				//TODO:这里进行游戏的初始化
	void move(float stepTime);	//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	void draw();				//TODO:绘制的函数
	void input(const XE::XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
	void ctrlEventProc(int id,int eventID){}//TODO:控件的消息处理函数

	virtual void setPosition(const XE::XVec2& pos) { setPosition(pos.x, pos.y); }
	virtual void setPosition(float x,float y);
	virtual const XE::XVec2& getPosition()const { return m_winPos; }
	virtual XE::XBool getIsInDragRect(const XE::XVec2& pos) { return getIsInDragRect(pos.x, pos.y); }
	virtual XE::XBool getIsInDragRect(float x,float y);//判断位置是否在拖拽区域内
	virtual XE::XVec2 correctPos(const XE::XVec2 &pos) {return pos;}	//更具传入的拖拽位置，修正该位置，当位置出于非法状态时，修正成合法的位置

	XE::XSprite m_sprite;
	XE::XProgressRing m_progress;
	XE::XButton m_button;
	XE::XVec2 m_winPos;
};

#endif