#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "X3D/X3DWorld.h"

class GGame:public XE::XGameBasic3D
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
	void draw();				//TODO:绘制2D的函数
	void draw3D(XE::XBool withTex);				//TODO:绘制3D的函数
	void input(const XE::XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
	void ctrlEventProc(int id,int eventID);

	XE::XSprite tempSprite;
	XE::XModelObj tempModel;
	XE::XSlider tmpXSld;
	XE::XSlider tmpYSld;
	XE::XSlider tmpZSld;
	XE::XVector3 angle;

	//说明：这个VBO效果需要在release模式下才能看到，目前原因不明
	XE::XVBO tmpVBO;
	float *v;
	float *t;
	float *n;
	unsigned int *index;
	XE::XVector2 mouseVector;
};

#endif