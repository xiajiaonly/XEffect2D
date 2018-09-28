#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "XResourcePack.h"
#include "XLoading.h"
#include "XBasicFun.h"

#include "XGun.h"
#include "XGem.h"

#define WINDOWS_WIDTH (1280)
#define WINDOWS_HIGHT (720)
#define MAX_GUN_ROTATE_TIMER (100)	//枪炮转动的最慢速度
#define MIN_GUN_ROTATE_TIMER (7)	//枪炮转动的最快速度

class GGame:public XE::XGameBasic
{
public:
	GGame()
		:keyShootDownState(0)
		, nowGunRotateTimer(MAX_GUN_ROTATE_TIMER)
		, keyMoveUpDownState(0)
		, keyMoveDownDownState(0)
		, mouseMoveStateUD(0)
		, mouseMoveStateLR(0)
		, mouseMoveStateLRC(0)
		, g_pJoy00(NULL)
	{}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
//	static GGame& GetInstance()
//	{
//		static GGame m_instance;
//		return m_instance;
//	}
public:
	bool init();				//TODO:这里进行游戏的初始化
	void move(float stepTime);	//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	void draw();				//TODO:绘制的函数
	void input(const XE::XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
	void ctrlEventProc(int id,int eventID){}//TODO:控件的消息处理函数

	XE::XGun gameGun;
	XE::XGemMatrix gameGemMatrix;
	XE::XLoading Loading;

	char keyShootDownState;
	int keyShootTimer;
	float nowGunRotateTimer;
	char keyMoveUpDownState;
	int keyMoveUpTimer;
	char keyMoveDownDownState;
	int keyMoveDownTimer;
	char mouseMoveStateUD;
	char mouseMoveStateLR;
	char mouseMoveStateLRC;
	void keyProc(int interval);
#if MAP_EDIT_MODE
	XE::XVector2 mousePoint;
#endif
#if PC_FREE_VERSION		//给个人玩的版本，有些功能被关闭
	GLuint m_gameAllTex;
#endif
	SDL_Joystick *g_pJoy00;
};

#define _GGameMain GGame::GetInstance()

#endif