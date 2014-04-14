#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"

class _GGame
{
protected:
	_GGame(){}
	_GGame(const _GGame&);
	_GGame &operator= (const _GGame&);
	virtual ~_GGame(){}
public:
	static _GGame& GetInstance()
	{
		static _GGame m_instance;
		return m_instance;
	}
public:
	bool init();				//TODO:这里进行游戏的初始化
	void move(int stepTime);	//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	void draw();				//TODO:绘制的函数
	void input(const _XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
};

#endif