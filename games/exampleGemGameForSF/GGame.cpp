#include "GGame.h"
using namespace XE;
bool GGame::init()
{
	//下面是关于游戏手柄方面的数据
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
	Loading.init(XVec2(WINDOWS_WIDTH,WINDOWS_HIGHT),RES_LOCAL_PACK);
	Loading.setStart();
#if PC_FREE_VERSION
	m_gameAllTex = EmptyTexture(2048,1024);
	_XSprite m_gameAllSprite;
	m_gameAllSprite.init(2048,1024);
	m_gameAllSprite.setPosition(180.0f - 1024.0f - 76.0f + 460.0f,320.0f - 512.0f - 192.0f + 40.0f);
	m_gameAllSprite.setAngle(90.0f);
	m_gameAllSprite.setSize(0.5f,0.5f);
	m_gameAllSprite.setOverturn(1);
#endif
	if(!XTexManager.init(RES_LOCAL_PACK)) return XFalse;

	if(gameGemMatrix.init(RES_LOCAL_PACK) == 0)
	{
		Loading.setEnd();
		return false;
	}
	if(gameGun.init(&gameGemMatrix, RES_LOCAL_PACK) == 0)
	{
		Loading.setEnd();
		return false;
	}
	XResPack.setOutFileName("GameData.dat");
	if(gameGemMatrix.initInsideData() == 0) 
	{
		Loading.setEnd();
		return false;
	}
	Loading.setEnd();

	XResPack.release();
#ifdef DEBUG_DATA
	int maxPixel = 0;
#endif
	return true;
}
void GGame::move(float stepTime)
{
	stepTime = (float)(stepTime)* gameGun.buffTimerRate();
	keyProc(stepTime);

	gameGun.crashProc(&gameGemMatrix);
	gameGun.move(stepTime);
#if USE_FBO
	gameGun.moveFBO(stepTime);
#endif
	gameGemMatrix.move(stepTime);
}
void GGame::draw()
{
#ifdef DEBUG_DATA
	XEE_AllPixel = 0;
#endif
#if PC_FREE_VERSION
	XEE::clearScreen();
#endif

	gameGun.drawDown();
#if USE_FBO
	gameGun.useFBO();
#endif
	gameGemMatrix.draw();
	gameGun.draw();
	gameGemMatrix.drawUp();
	gameGun.drawUp();
#if USE_FBO
	gameGun.removeFBO();
	gameGun.drawFBO();
#endif
#if MAP_EDIT_MODE
	drawLine(mousePoint.x, mousePoint.y, mousePoint.x + 10, mousePoint.y + 10, 5);
#endif
#if PC_FREE_VERSION
	glBindTexture(GL_TEXTURE_2D, m_gameAllTex);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 2048, 1024);
	glClear(GL_COLOR_BUFFER_BIT);
	drawLine(460, 40 - 1, 820, 40 - 1, 3);
	drawLine(820 + 2, 40, 820 + 2, 680, 3);
	drawLine(820, 680 + 2, 460, 680 + 2, 3);
	drawLine(460 - 1, 680, 460 - 1, 40, 3);
	m_gameAllSprite.draw(&m_gameAllTex);
#endif

#ifdef DEBUG_DATA
	if (XEE_AllPixel > maxPixel)
	{
		maxPixel = XEE_AllPixel;
		printf("Max:%f\n", maxPixel / (1280.0f * 720.0f));
	}
#endif
}
void GGame::input(const XInputEvent &event)
{
	switch(event.type)
	{
	case EVENT_KEYBOARD:
		switch(event.keyState)
		{
		case KEY_STATE_DOWN:
			switch(event.keyValue)
			{
			case XKEY_S:
				gameGun.upgradeGun();
				gameGun.pressChangeGun();
				break;
			case XKEY_A:
				gameGun.addCredit();
				break;
			//case XKEY_Z:
			//	gameGun.setFaceStart();
			//	break;
			case XKEY_Q:
				gameGun.addEnergy();
				break;
#if PC_FREE_VERSION
			//case XKEY_C:
			//	gameGun.enterConfig();
			//	break;
#else
			case XKEY_C:
				gameGun.enterConfig();
				break;
#endif
			case XKEY_X:
				keyShootDownState = 1;
				gameGun.inputName(INPUT_KEY_ENTER);
				gameGun.configKeyProc(CONFIG_KEY_SHOOT);
				gameGun.pressShoot();
				break;
			case XKEY_G:
				gameGun.initRecordData();
				break;
			case XKEY_J:
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
			case XKEY_1:
				gameGun.setTicketWrong();
				break;
			case XKEY_2:
				gameGun.setTicketWell();
				break;
			case XKEY_4:
				gameGun.addTicket(20);
				break;
			case XKEY_5:
				gameGun.decreaseTicket(1);
				break;
			case XKEY_H:
				gameGemMatrix.writeMatrix(100);
				break;
			case XKEY_K:
				gameGemMatrix.clearMatrix();
				break;
			case XKEY_L:
				gameGemMatrix.readMatrixFromTxt(100);
				break;
			case XKEY_LALT:
				//gameGemMatrix.readMatrix(0);
				break;
			case XKEY_RIGHT:
				keyMoveUpDownState = 1;
			//	gameGun.inputName(INPUT_KEY_RIGHT);
				gameGun.configKeyProc(CONFIG_KEY_RIGHT);
				break;
			case XKEY_LEFT:
				keyMoveDownDownState = 1;
			//	gameGun.inputName(INPUT_KEY_LEFT);
				gameGun.configKeyProc(CONFIG_KEY_LEFT);
				break;
			case XKEY_UP:
				gameGun.configKeyProc(CONFIG_KEY_UP);
				break;
			case XKEY_DOWN:
				gameGun.configKeyProc(CONFIG_KEY_DOWN);
				break;
			}
			break;
		case KEY_STATE_UP:
			switch(event.keyValue)
			{
			case XKEY_X:
				keyShootDownState = 0;
				keyShootTimer = 0;
				break;
			case XKEY_RIGHT:
				keyMoveUpDownState = 0;
				keyMoveUpTimer = 0;
				break;
			case XKEY_LEFT:
				keyMoveDownDownState = 0;
				keyMoveDownTimer = 0;
				break;
			}
			break;
		}
		break;
	}
}
void GGame::release()
{
	if(SDL_JoystickOpened(0))
	{
		SDL_JoystickClose(g_pJoy00);
		g_pJoy00 = NULL;
	}
}
void GGame::keyProc(int interval)
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
