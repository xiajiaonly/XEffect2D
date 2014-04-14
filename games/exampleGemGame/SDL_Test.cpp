//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XEffeEngine.h"
#include "XResourcePack.h"
#include "XLoading.h"
#include "XBasicFun.h"

#include "XGun.h"
#include "XGem.h"

#ifdef OS_WINDOWS
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

#define WINDOWS_WIDTH (1280)
#define WINDOWS_HIGHT (720)

int inputEvent();	//下面是事件处理函数，例如：键盘事件或者鼠标事件

_XGun gameGun;
_XGemMatrix gameGemMatrix;
_XLoading Loading;

char keyShootDownState = 0;
int keyShootTimer;
#define MAX_GUN_ROTATE_TIMER (100)	//枪炮转动的最慢速度
#define MIN_GUN_ROTATE_TIMER (7)	//枪炮转动的最快速度
float nowGunRotateTimer = MAX_GUN_ROTATE_TIMER;
char keyMoveUpDownState = 0;
int keyMoveUpTimer;
char keyMoveDownDownState = 0;
int keyMoveDownTimer;
char mouseMoveStateUD = 0;
char mouseMoveStateLR = 0;
char mouseMoveStateLRC = 0;
void keyProc(int interval);

#if MAP_EDIT_MODE
_XVector2 mousePoint;
#endif

#if PC_FREE_VERSION		//给个人玩的版本，有些功能被关闭
GLuint m_gameAllTex;
#endif

int main(int argc, char **argv)
{
	Uint32 interval;	//两次调用之间的时间差			
	//建立窗体
	_XWindowData tmp(WINDOWS_WIDTH,WINDOWS_HIGHT);
	tmp.windowTitle = "Destruction of precious stones";
#if PC_FREE_VERSION	
	tmp.isFullScreen = 0;
#else
	//tmp.isFullScreen = 1;
	tmp.isFullScreen = 0;
#endif
	if(!initWindowEx(tmp)) REPORT_ERROR("建立窗体失败");
	//下面是关于游戏手柄方面的数据
	SDL_Joystick *g_pJoy00 = NULL;
	if(SDL_NumJoysticks() > 0)
	{
		g_pJoy00 = SDL_JoystickOpen(0);
		if(g_pJoy00 != NULL)
		{
			SDL_JoystickEventState(SDL_ENABLE);
		}else
		{
			printf("没有检测到手柄设备!\n");
		}
	}

	Loading.init(_XVector2(WINDOWS_WIDTH,WINDOWS_HIGHT),RESOURCE_LOCAL_PACK);
	Loading.setStart();

#if PC_FREE_VERSION
	m_gameAllTex =  EmptyTexture(2048,1024);
	_XSprite m_gameAllSprite;
	m_gameAllSprite.init(2048,1024);
	m_gameAllSprite.setPosition(180.0f - 1024.0f - 76.0f + 460.0f,320.0f - 512.0f - 192.0f + 40.0f);
	m_gameAllSprite.setAngle(90.0f);
	m_gameAllSprite.setSize(0.5f,0.5f);
	m_gameAllSprite.setOverturn(1);
#endif


	if(_XAllTexture::GetInstance().init(RESOURCE_LOCAL_PACK) == 0) return 0;

	if(gameGemMatrix.init(RESOURCE_LOCAL_PACK) == 0) 
	{
		Loading.setEnd();
		return 0;
	}
	if(gameGun.init(&gameGemMatrix,RESOURCE_LOCAL_PACK) == 0) 
	{
		Loading.setEnd();
		return 0;
	}
	_XResourcePack::GetInstance().setOutFileName("GameData.dat");
	if(gameGemMatrix.initInsideData() == 0) 
	{
		Loading.setEnd();
		return 0;
	}
	Loading.setEnd();

	_XResourcePack::GetInstance().release();
#ifdef DEBUG_DATA
	int maxPixel = 0;
#endif

	while(!inputEvent()) 
	{
		interval = XEE::engineIdle();
		interval = (float)(interval) * gameGun.buffTimerRate();
		keyProc(interval);

		gameGun.crashProc(&gameGemMatrix);
		gameGun.move(interval);
		gameGun.moveFBO(interval);
		gameGemMatrix.move(interval);
		
		//清除屏幕
#ifdef DEBUG_DATA
		XEE_AllPixel = 0;
#endif
#if PC_FREE_VERSION
		clearScreen();
#endif

		gameGun.drawDown();
		gameGun.useFBO();

		gameGemMatrix.draw();
		gameGun.draw();
		gameGemMatrix.drawUp();
		gameGun.drawUp();

		gameGun.removeFBO();
		gameGun.drawFBO();
#if MAP_EDIT_MODE
		drawLine(mousePoint.x,mousePoint.y,mousePoint.x + 10,mousePoint.y + 10,5);
#endif
#if PC_FREE_VERSION
		glBindTexture(GL_TEXTURE_2D,m_gameAllTex);
		glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,2048,1024);
		glClear(GL_COLOR_BUFFER_BIT);
		drawLine(460,40 - 1,820,40 - 1,3);
		drawLine(820 + 2,40,820 + 2,680,3);
		drawLine(820,680 + 2,460,680 + 2,3);
		drawLine(460 - 1,680,460 - 1,40,3);
		m_gameAllSprite.draw(&m_gameAllTex);
#endif

		XEE::updateScreen();
#ifdef DEBUG_DATA
		if(XEE_AllPixel > maxPixel)
		{
			maxPixel = XEE_AllPixel;
			printf("Max:%f\n",maxPixel/(1280.0f * 720.0f));
		}
#endif
	}

	if(SDL_JoystickOpened(0))
	{
		SDL_JoystickClose(g_pJoy00);
		g_pJoy00 = NULL;
	}

	SDL_Quit();
	return 0;	
}

void keyProc(int interval)
{//下面是关于按键的连续处理
	if(gameGun.m_configData.isAutoShoot == 0)
	{
		if(keyShootDownState == 1)
		{
			gameGun.shoot();
			keyShootDownState = 0;
		}
	}else
	{
		if(keyShootDownState == 1)
		{
			keyShootTimer -= interval;
			if(keyShootTimer <= 0)
			{
				gameGun.shoot();
				keyShootTimer = gameGun.m_gunShootSpeed * gameGun.buffShootTimer();
			}
		}
	}
	if(keyMoveUpDownState == 1)
	{
		keyMoveUpTimer -= interval;
		if(keyMoveUpTimer <= 0)
		{
			gameGun.whirlGun(1);	
			nowGunRotateTimer *= 0.85f;
			if(nowGunRotateTimer < MIN_GUN_ROTATE_TIMER) nowGunRotateTimer = MIN_GUN_ROTATE_TIMER;
			keyMoveUpTimer = nowGunRotateTimer;

			gameGun.inputName(INPUT_KEY_RIGHT);
			//gameGun.configKeyProc(CONFIG_KEY_RIGHT);
		}
	}
	if(keyMoveDownDownState == 1)
	{
		keyMoveDownTimer -= interval;
		if(keyMoveDownTimer <= 0)
		{
			gameGun.whirlGun(-1);
			nowGunRotateTimer *= 0.85f;
			if(nowGunRotateTimer < MIN_GUN_ROTATE_TIMER) nowGunRotateTimer = MIN_GUN_ROTATE_TIMER;
			keyMoveDownTimer = nowGunRotateTimer;

			gameGun.inputName(INPUT_KEY_LEFT);
			//gameGun.configKeyProc(CONFIG_KEY_LEFT);
		}
	}
	if(keyMoveUpDownState == 0 && keyMoveDownDownState == 0)
	{
		nowGunRotateTimer *= 1.02f;
		if(nowGunRotateTimer > MAX_GUN_ROTATE_TIMER) nowGunRotateTimer = MAX_GUN_ROTATE_TIMER;
	}
}

int inputEvent()
{
	int flag =0;
	SDL_Event event;		//SDL事件句柄

	while(SDL_PollEvent(&event)) 
	{
		switch(event.type)
		{case SDL_JOYAXISMOTION:
			if(event.jaxis.which == 0 && event.jaxis.axis == 0)
			{
				if(event.jaxis.value < - 2000)
				{
					keyMoveDownDownState = 1;
					keyMoveUpDownState = 0;
				//	gameGun.inputName(INPUT_KEY_LEFT);
					gameGun.configKeyProc(CONFIG_KEY_LEFT);
				}else
				if(event.jaxis.value >  2000)
				{
					keyMoveDownDownState = 0;
					keyMoveUpDownState = 1;
				//	gameGun.inputName(INPUT_KEY_RIGHT);
					gameGun.configKeyProc(CONFIG_KEY_RIGHT);
				}else
				{
					keyMoveUpDownState = 0;
					keyMoveUpTimer = 0;
					keyMoveDownDownState = 0;
					keyMoveDownTimer = 0;
				}
			}
			if(event.jaxis.which == 0 && event.jaxis.axis == 1)
			{
				if(event.jaxis.value < - 2000)
				{
					gameGun.configKeyProc(CONFIG_KEY_UP);
				}else
				if(event.jaxis.value >  2000)
				{
					gameGun.configKeyProc(CONFIG_KEY_DOWN);
				}
			}
			break;
		case SDL_JOYBUTTONDOWN: 
			if(event.jbutton.which == 0)
			{
				switch(event.jbutton.button)
				{
				case 0:
					gameGun.addCredit();
					break;
				case 1:
					gameGun.addEnergy();
					break;
				case 2:
					gameGun.enterConfig();
					break;
				case 3:
					keyShootDownState = 1;
					gameGun.inputName(INPUT_KEY_ENTER);
					gameGun.configKeyProc(CONFIG_KEY_SHOOT);
					gameGun.pressShoot();
					break;
				case 4:
					gameGun.upgradeGun();
					gameGun.pressChangeGun();
					break;
				}
			}
			break;
		case SDL_JOYBUTTONUP: 
			if(event.jbutton.which == 0)
			{
				switch(event.jbutton.button)
				{
				case 3:
					keyShootDownState = 0;
					keyShootTimer = 0;
					break;
				}
			}
			break;
		case SDL_QUIT:
			flag = 1;
			break;
		case SDL_KEYUP:
			switch(event.key.keysym.sym )
			{
			case SDLK_x:
				keyShootDownState = 0;
				keyShootTimer = 0;
				break;
			case SDLK_RIGHT:
				keyMoveUpDownState = 0;
				keyMoveUpTimer = 0;
				break;
			case SDLK_LEFT:
				keyMoveDownDownState = 0;
				keyMoveDownTimer = 0;
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym )
			{
			case SDLK_ESCAPE:
				flag = 1;
				break;
			case SDLK_s:
				gameGun.upgradeGun();
				gameGun.pressChangeGun();
				break;
			case SDLK_a:
				gameGun.addCredit();
				break;
			//case SDLK_z:
			//	gameGun.setFaceStart();
			//	break;
			case SDLK_q:
				gameGun.addEnergy();
				break;
#if PC_FREE_VERSION
			//case SDLK_c:
			//	gameGun.enterConfig();
			//	break;
#else
			case SDLK_c:
				gameGun.enterConfig();
				break;
#endif
			case SDLK_x:
				keyShootDownState = 1;
				gameGun.inputName(INPUT_KEY_ENTER);
				gameGun.configKeyProc(CONFIG_KEY_SHOOT);
				gameGun.pressShoot();
				break;
			case SDLK_g:
				gameGun.initRecordData();
				break;
			case SDLK_j:
#if MAP_EDIT_MODE
				if(gameGemMatrix.m_isShowReseau == 0)
				{
					gameGemMatrix.m_isShowReseau = 1;
				}else
				{
					gameGemMatrix.m_isShowReseau = 0;
				}
#endif
				break;
			case SDLK_1:
				gameGun.setTicketWrong();
				break;
			case SDLK_2:
				gameGun.setTicketWell();
				break;
			case SDLK_4:
				gameGun.addTicket(20);
				break;
			case SDLK_5:
				gameGun.decreaseTicket(1);
				break;
			case SDLK_h:
				gameGemMatrix.writeMatrix(100);
				break;
			case SDLK_k:
				gameGemMatrix.clearMatrix();
				break;
			case SDLK_l:
				gameGemMatrix.readMatrixFromTxt(100);
				break;
			case SDLK_LALT:
				//gameGemMatrix.readMatrix(0);
				break;
			case SDLK_RIGHT:
				keyMoveUpDownState = 1;
			//	gameGun.inputName(INPUT_KEY_RIGHT);
				gameGun.configKeyProc(CONFIG_KEY_RIGHT);
				break;
			case SDLK_LEFT:
				keyMoveDownDownState = 1;
			//	gameGun.inputName(INPUT_KEY_LEFT);
				gameGun.configKeyProc(CONFIG_KEY_LEFT);
				break;
			case SDLK_UP:
				gameGun.configKeyProc(CONFIG_KEY_UP);
				break;
			case SDLK_DOWN:
				gameGun.configKeyProc(CONFIG_KEY_DOWN);
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			//下面是游戏中的控制
			//if(event.motion.xrel > 5) 
			//{
			//	if(mouseMoveStateLR == 0)
			//	{
			//		keyMoveUpDownState = 1;
			//		mouseMoveStateLR = 1;
			//	}
			//}else
			//if(event.motion.xrel < -5) 
			//{
			//	if(mouseMoveStateLR == 0)
			//	{
			//		keyMoveDownDownState = 1;
			//		mouseMoveStateLR = 1;
			//	}
			//}else
			//{
			//	keyMoveUpDownState = 0;
			//	keyMoveDownDownState = 0;
			//	mouseMoveStateLR = 0;
			//}
			//这两种是使用不同的控制方式
			if(event.motion.xrel > 1 || event.motion.xrel < -1)
			{
				if(event.motion.xrel > 1)
				{
					for(int i = 0;i < (event.motion.xrel - 1) * 0.05f; ++ i)
					{
						gameGun.whirlGun(1);
					}
				}else
				{
					for(int i = 0;i < (-event.motion.xrel - 1) * 0.05f; ++ i)
					{
						gameGun.whirlGun(-1);
					}
				}
				if(event.motion.xrel > 1)
				{
					for(int i = 0;i < (event.motion.xrel - 1) * 0.1f; ++ i)
					{
						gameGun.inputName(INPUT_KEY_RIGHT);
					}
				}else
				{
					for(int i = 0;i < (-event.motion.xrel - 1) * 0.1f; ++ i)
					{
						gameGun.inputName(INPUT_KEY_LEFT);
					}
				}
			}
			///下面是配置界面中的控制
			if(event.motion.xrel > 6) 
			{
				if(mouseMoveStateLRC == 0)
				{
					mouseMoveStateLRC = 1;
					gameGun.configKeyProc(CONFIG_KEY_RIGHT);
				}
			}else
			if(event.motion.xrel < -6) 
			{
				if(mouseMoveStateLRC == 0)
				{
					mouseMoveStateLRC = 1;
					gameGun.configKeyProc(CONFIG_KEY_LEFT);
				}
			}else
			{
				if(event.motion.xrel < 2 && event.motion.xrel > -2) 
				{
					mouseMoveStateLRC = 0;
				}

				if(event.motion.yrel > 6) 
				{
					if(mouseMoveStateUD == 0)
					{
						gameGun.configKeyProc(CONFIG_KEY_DOWN);
						mouseMoveStateUD = 1;
					}
				}else
				if(event.motion.yrel < -6) 
				{
					if(mouseMoveStateUD == 0)
					{
						gameGun.configKeyProc(CONFIG_KEY_UP);
						mouseMoveStateUD = 1;
					}
				}else
				if(event.motion.yrel < 2 && event.motion.yrel > -2) 
				{
					mouseMoveStateUD = 0;
				}
			}

#if MAP_EDIT_MODE
			mousePoint.x = 1280 - event.motion.y * 2.0f;
			mousePoint.y = (event.motion.x) * 2.0f;
#endif
			break;
		case SDL_MOUSEBUTTONDOWN:
#if MAP_EDIT_MODE
			if(event.button.button == 1)
			{//左键
				gameGemMatrix.changeGem(mousePoint.x,mousePoint.y,0);
			}else
			if(event.button.button == 2)
			{//右键
				gameGemMatrix.changeGem(mousePoint.x,mousePoint.y,2);
			}else
			if(event.button.button == 3)
			{//右键
				gameGemMatrix.changeGem(mousePoint.x,mousePoint.y,1);
			}
#endif
			break;
		}
		//inputEvent(event);	//注意这里有问题
	}
	return flag;
}