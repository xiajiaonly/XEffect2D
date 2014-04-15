//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XGun.h"
#include "XResourcePack.h"

XMusicHandle BGMusicHandle[6];	//声音的句柄
//_XMusic _XMusic::GetInstance();					//播放背景音乐的类

#define GOLD_FLY_ARM_X (98)
#define GOLD_FLY_ARM_Y (150)
#define DEBUG00 (0)

#if USE_FBO
#if USE_SHADER
//int m_shaderHandle;			//shader的句柄
//int textureHandle;			//shader中贴图的句柄
//void useShader()			//具体使用shader的函数
//{
//	glUseProgram(m_shaderHandle);
//	static char flag = 0;
//	if(0 == flag)
//	{
//		//设置shader的参数
//		textureHandle = glGetUniformLocation(m_shaderHandle, "Texture");
//		glUniform1i(textureHandle,0);
//		flag = 1;
//	}
//}
void _XGun::initShaderFromText()	//从文件中初始化sheder的代码
{
//	setShader("ClearAlpha.vrt", "ClearAlpha.frg",m_shaderHandle,m_resoursePosition);
//	setShader("ClearAlpha.vrt", "ClearAlpha.frg",m_shaderHandle,0);
	m_tmpSader.init("ResourcePack/ClearAlpha.vrt", "ResourcePack/ClearAlpha.frg",m_resoursePosition);
}
#endif
#endif

int _XGun::init(_XGemMatrix *gems,_XResourcePosition resoursePosition)
{
	if(m_isInited != 0) return 1;
	if(gems == NULL) return 0;
	m_gems = gems;
	m_resoursePosition = resoursePosition;
	if(m_cameraLogo.init("ResourcePack/pic/UI/camera.png",m_resoursePosition) == 0) return 0;	//初始化摄像头的背景
	m_cameraLogo.setPosition(1200.0f,638.0f);
	m_cameraLogo.setAngle(90);
	if(m_cameraNotSupply.init("ResourcePack/pic/UI/notsupply.png",m_resoursePosition) == 0) return 0;	//初始化摄像头的背景
	m_cameraNotSupply.setPosition(1200.0f,638.0f);
	m_cameraNotSupply.setAngle(90);
	m_cameraNotSupply.setAlpha(0.0f);
	m_notSupplyAlpha = 0.0f;

	if(m_camera.init(CAMERA_WIDTH,CAMERA_HIGHT) == 0)
	{
		m_isCameraSupply = 0;
	}else
	{
		m_isCameraSupply = 1;
		if(m_camera.getBuffSize() != MAX_GAME_ORDER_CAMERA_DATA) 
		{
#if DEBUG00
			printf("The texture size of camera inited is different from default!\n");
#endif
			return 0;
		}
	}
	m_camera.m_cameraSprite.setAngle(90);
	m_camera.m_cameraSprite.setPosition(388,240);
	m_camera.setStop();
	if(m_takePhotoAttention.init("ResourcePack/pic/UI/takePhotoTitle.png",m_resoursePosition) == 0) return 0;	//初始化摄像头的背景
	m_takePhotoAttention.setPosition(290,290);
	m_takePhotoAttention.setAngle(90);
	if(m_cameraBG.init("ResourcePack/pic/UI/cameraBG.png",m_resoursePosition) == 0) return 0;	//初始化摄像头的背景
	m_cameraBG.setPosition(388 - 96,240 - 136);
	m_cameraBG.setAngle(90);
	if(m_takePhotoTex00.init("ResourcePack/pic/UI/TakePhoto00.png",m_resoursePosition) == 0) return 0;	//初始化摄像头的背景
	m_takePhotoTex00.setPosition(388 + 96,240 + 56);
	m_takePhotoTex00.setAngle(90);
	if(m_takePhotoTex01.init("ResourcePack/pic/UI/TakePhoto01.png",m_resoursePosition) == 0) return 0;	//初始化摄像头的背景
	m_takePhotoTex01.setPosition(388 + 96,240 + 56);
	m_takePhotoTex01.setAngle(90);
	if(m_takePhotoTex02.init("ResourcePack/pic/UI/TakePhoto02.png",m_resoursePosition) == 0) return 0;	//初始化摄像头的背景
	m_takePhotoTex02.setPosition(388 + 96,240 + 56);
	m_takePhotoTex02.setAngle(90);
	if(m_takePhotoTex03.init("ResourcePack/pic/UI/TakePhoto03.png",m_resoursePosition) == 0) return 0;	//初始化摄像头的背景
	m_takePhotoTex03.setPosition(388 - 96,240 - 8);
	m_takePhotoTex03.setAngle(90);
//	if(m_cameraHead.init(CAMERA_WIDTH,CAMERA_HIGHT,0) == 0) return 0;
	if(m_cameraHead.init(CAMERA_WIDTH,CAMERA_HIGHT) == 0) return 0;
	m_cameraHead.setAngle(90);
	m_cameraHead.setSize(0.3f,0.3f);
	try
	{
		m_gameOrderCameraData = new unsigned char[MAX_GAME_ORDER_CAMERA_DATA * MAX_GAME_ORDER_SUM];
		if(m_gameOrderCameraData == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
	{
		glGenTextures(1,&(m_cameraRecordTex[i]));
		glBindTexture(GL_TEXTURE_2D, m_cameraRecordTex[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CAMERA_TEX_WIDTH, CAMERA_TEX_HIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}
	for(int i = 0;i < 4;++ i)
	{
		if(m_userHeadSprite[i].init(CAMERA_WIDTH,CAMERA_HIGHT) == 0) return 0;
		m_userHeadSprite[i].setAngle(90);
		m_userHeadSprite[i].setSize(0.5f,0.5f);
	}
	m_nowUserHeadOrder = 0;
	m_nowUserHeadData = 0;
	try
	{
		m_gameOrderUserHeadData = new unsigned char[MAX_GAME_ORDER_CAMERA_DATA * USER_HEAD_SUM];
		if(m_gameOrderUserHeadData == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	for(int i = 0;i < USER_HEAD_SUM;++ i)
	{
		glGenTextures(1,&(m_userHeadTex[i]));
		glBindTexture(GL_TEXTURE_2D, m_userHeadTex[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CAMERA_TEX_WIDTH, CAMERA_TEX_HIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}

//	initConfigData();
//	initRecordData();
//	initRecordData();
	if(readRecordData() == 0) 
	{
		initConfigData();
		initRecordData();
		initRecordData();
		if(readRecordData() == 0)  
		{
			return 0;
		}
	}

//	initHeadData();
//	initHeadData();
	if(readHeadData() == 0) 
	{
		initHeadData();
		initHeadData();
		if(readHeadData() == 0) 
		{
			return 0;
		}
	}
	if(m_isCameraSupply == 0)
	{
		initHeadData(1);
	}

	try
	{
		m_cameraTexData = new unsigned char[CAMERA_TEX_WIDTH * CAMERA_TEX_HIGHT * 3];
		if(m_cameraTexData == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	memset(m_cameraTexData,0,CAMERA_TEX_WIDTH * CAMERA_TEX_HIGHT * 3);

	//注意！注意！去掉这个之后会在目标机器上面造成贴图问题，暂时原因尚不明确，不是2的n次方的问题，可能与目标机器的驱动相关
	for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
	{
		updataHeadTex(i);
	}
//	initUserData();
//	initUserData();
	if(readUserData() == 0) 
	{
		initUserData();
		initUserData();
		if(readUserData() == 0) 
		{
			return 0;
		}
	}
	for(int i = 0;i < USER_HEAD_SUM;++ i)
	{
		updataUserHeadTex(i);
	}
	for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
	{
		updataHeadTex(i);
	}
	_XSound::GetInstance().setAllVolume(m_configData.volume * 10.0f / 100.0f * 128);

	m_BGPicOrder = 0;
	//if(m_backGround_00.init("normalResource/BG_00.jpg",resoursePosition) == 0) return 0;
	if(m_backGround_00.init("ResourcePack/pic/BG_00.png",resoursePosition) == 0) return 0;
	m_backGround_00.setPosition(0.0f,0.0f);
	//if(m_backGround_01.init("normalResource/BG_01.jpg",resoursePosition) == 0) return 0;
	if(m_backGround_01.init("ResourcePack/pic/BG_01.png",resoursePosition) == 0) return 0;
	m_backGround_01.setPosition(0.0f,0.0f);
	//if(m_backGround_02.init("normalResource/BG_02.jpg",resoursePosition) == 0) return 0;
	if(m_backGround_02.init("ResourcePack/pic/BG_02.png",resoursePosition) == 0) return 0;
	m_backGround_02.setPosition(0.0f,0.0f);
	//if(m_backGround_03.init("normalResource/BG_03.jpg",resoursePosition) == 0) return 0;
	if(m_backGround_03.init("ResourcePack/pic/BG_03.png",resoursePosition) == 0) return 0;
	m_backGround_03.setPosition(0.0f,0.0f);
	//if(m_backGround_04.init("normalResource/BG_04.jpg",resoursePosition) == 0) return 0;
	if(m_backGround_04.init("ResourcePack/pic/BG_04.png",resoursePosition) == 0) return 0;
	m_backGround_04.setPosition(0.0f,0.0f);

	if(m_star[0].init("ResourcePack/pic/star/Space_M0.png",resoursePosition) == 0) return 0;
	m_starSpeed[0].set(0.001f,0.01f);
	if(m_star[1].init("ResourcePack/pic/star/Space_M5.png",resoursePosition) == 0) return 0;
	m_starSpeed[1].set(0.001f,0.01f);
	if(m_star[2].init("ResourcePack/pic/star/Space_M2.png",resoursePosition) == 0) return 0;
	m_starSpeed[2].set(0.001f,0.01f);
	if(m_star[3].init("ResourcePack/pic/star/Space_M3.png",resoursePosition) == 0) return 0;
	m_starSpeed[3].set(0.001f,0.01f);
	if(m_star[4].init("ResourcePack/pic/star/Space_M6.png",resoursePosition) == 0) return 0;
	m_starSpeed[4].set(0.001f,0.01f);
	if(m_star[5].init("ResourcePack/pic/star/Space_M1.png",resoursePosition) == 0) return 0;
	m_starSpeed[5].set(0.001f,0.01f);
	if(m_star[6].init("ResourcePack/pic/star/Space_M4.png",resoursePosition) == 0) return 0;
	m_starSpeed[6].set(0.001f,0.01f);
	if(m_star[7].init("ResourcePack/pic/star/Space_N0.png",resoursePosition) == 0) return 0;
	m_starSpeed[7].set(0.001f,0.01f);
	if(m_star[8].init("ResourcePack/pic/star/Space_N1.png",resoursePosition) == 0) return 0;
	m_starSpeed[8].set(0.001f,0.01f);
	if(m_star[9].init("ResourcePack/pic/star/Space_N2.png",resoursePosition) == 0) return 0;
	m_starSpeed[9].set(0.001f,0.01f);
	if(m_star[10].init("ResourcePack/pic/star/Space_N3.png",resoursePosition) == 0) return 0;
	m_starSpeed[10].set(0.001f,0.01f);
	if(m_star[11].init("ResourcePack/pic/star/Space_N4.png",resoursePosition) == 0) return 0;
	m_starSpeed[11].set(0.001f,0.01f);
	if(m_star[12].init("ResourcePack/pic/star/Space_N5.png",resoursePosition) == 0) return 0;
	m_starSpeed[12].set(0.001f,0.01f);
	for(int i = 0;i < 13;++ i)
	{
		m_starFlag[i] = 0;
	}

	if(m_gunTex_00.load("ResourcePack/pic/gun/Gun_000.png",resoursePosition) == 0) return 0;
	if(m_gunTex_01.load("ResourcePack/pic/gun/Gun_001.png",resoursePosition) == 0) return 0;
	if(m_gunTex_02.load("ResourcePack/pic/gun/Gun_002.png",resoursePosition) == 0) return 0;
	if(m_gunTex_03.load("ResourcePack/pic/gun/Gun_003.png",resoursePosition) == 0) return 0;
	if(m_gunTex_04.load("ResourcePack/pic/gun/Gun_004.png",resoursePosition) == 0) return 0;
	if(m_gunTex_05.load("ResourcePack/pic/gun/Gun_005.png",resoursePosition) == 0) return 0;
	if(m_gunSprite.init(m_gunTex_00.texture.m_w,m_gunTex_00.texture.m_h) == 0) return 0;
	m_gunSprite.setAngle(m_gunAngle);
	m_gunSprite.setPosition(-64.0f,360.0f - 128.0f);

	if(m_gunFireTex_00.load("ResourcePack/pic/gun/BB_000_00.png",resoursePosition) == 0) return 0;
	if(m_gunFireTex_01.load("ResourcePack/pic/gun/BB_001_00.png",resoursePosition) == 0) return 0;
	if(m_gunFireTex_02.load("ResourcePack/pic/gun/BB_002_00.png",resoursePosition) == 0) return 0;
	if(m_gunFireTex_03.load("ResourcePack/pic/gun/BB_003_00.png",resoursePosition) == 0) return 0;
	if(m_gunFireTex_04.load("ResourcePack/pic/gun/BB_004_00.png",resoursePosition) == 0) return 0;
	if(m_gunFireTex_05.load("ResourcePack/pic/gun/BB_005_00.png",resoursePosition) == 0) return 0;
	if(m_gunFireSprite_00.init(m_gunFireTex_00.texture.m_w,m_gunFireTex_00.texture.m_h) == 0) return 0;

	if(m_BGSider[0].init("ResourcePack/pic/UI/LineSider.png",resoursePosition) == 0) return 0;
	m_BGSider[0].setPosition(0.0f,0.0f);
	m_BGSider[1].setACopy(m_BGSider[0]);
	m_BGSider[1].setPosition(0.0f,720.0f - 16.0f);
	m_BGSider[1].setAngle(180.0f);

	if(UIBoardInit(resoursePosition) == 0) return 0;

	if(goldAddAndFlyInit(resoursePosition) == 0) return 0;
	if(m_bulletTex.init(resoursePosition) == 0) return 0;
	if(m_toolsBoom.init("ResourcePack/pic/UI/Effect_00/Effect_000.png",resoursePosition) == 0) return 0;
//	m_toolsBoom.setACopy(m_gems->m_boomFrame_00);
	m_toolsBoom.setAttribute(_XVector2(-55.0f,360.0f - 512.0f - 5.0f),0,1,0,0.01f,0);
	m_toolsBoom.setAngle(90.0f);
	m_toolsBoomFlag = 0;

	if(m_colorBoomFrame.init("ResourcePack/pic/UI/Boom_03/Boom_000.png",resoursePosition) == 0) return 0;
	m_colorBoomFrame.setAttribute(_XVector2(-256 + 16.0f,104.0f),0,0,0,0.05f,0);

	if(processInit(resoursePosition) == 0) return 0;
	if(toolsInit(resoursePosition) == 0) return 0;

	try
	{
		m_bullet = new _XBullet[MAX_BULLET_SUM];
		if(m_bullet == NULL) return 0;
	}catch(...)
	{
		return 0;
	}

	//下面这些变量适用于测试的
	m_gameStage = GAME_STAGE_GAMELOGO;	//游戏阶段的标志
//	m_gameStage = GAME_STAGE_GAMEALLOVER;	//游戏阶段的标志
	m_gameCounter = 0;		//游戏阶段的辅助性参数
	m_nowLevel = 0;			//当前是第几关

	{
		char temp[32];
		sprintf(temp,"%3d+%d",m_creditSum,m_configData.coinToCredit);
		m_creditNum.setNumber(temp);
	}

	if(gameHeadInit(resoursePosition) == 0) return 0;
	if(gameWinInit(resoursePosition) == 0) return 0;
	if(gameLostInit(resoursePosition) == 0) return 0;
	if(gameDemoInit(resoursePosition) == 0) return 0;

	if(gameOrderInit(resoursePosition) == 0) return 0;
	if(orderShowInit(resoursePosition) == 0) return 0;
	if(gameAllOverInit(resoursePosition) == 0) return 0;

	if(gameTimerInit(resoursePosition) == 0) return 0;
	if(weaponInfoInit(resoursePosition) == 0) return 0;
	if(warnningInit(resoursePosition) == 0) return 0;
	if(helpInit(resoursePosition) == 0) return 0;
	if(ticketInit(resoursePosition) == 0) return 0;
	if(configInit(resoursePosition) == 0) return 0;

	if(instructInit(resoursePosition) == 0) return 0;
	if(goldBoxEffectInit(resoursePosition) == 0) return 0;
	if(gamePlayInit(resoursePosition) == 0) return 0;
	if(smallStarInit(resoursePosition) == 0) return 0;
	if(faceInit(resoursePosition) == 0) return 0;
	if(userBuffInit(resoursePosition) == 0) return 0;
	if(toolsEffectInit(resoursePosition) == 0) return 0;

	if(soundInit(resoursePosition) == 0) return 0;
	m_UIShakeStage = 0;

	//初始化背景音乐
	BGMusicHandle[0] = _XMusic::GetInstance().addMusic("Music/BG_00.mp3");
	if(BGMusicHandle[0] < 0) return 0;
	BGMusicHandle[1] = _XMusic::GetInstance().addMusic("Music/BG_01.mp3");
	if(BGMusicHandle[1] < 0) return 0;
	BGMusicHandle[2] = _XMusic::GetInstance().addMusic("Music/BG_02.mp3");
	if(BGMusicHandle[2] < 0) return 0;
	BGMusicHandle[3] = _XMusic::GetInstance().addMusic("Music/BG_03.mp3");
	if(BGMusicHandle[3] < 0) return 0;
	BGMusicHandle[4] = _XMusic::GetInstance().addMusic("Music/BG_04.mp3");
	if(BGMusicHandle[4] < 0) return 0;
	BGMusicHandle[5] = _XMusic::GetInstance().addMusic("Music/BG_05.mp3");
	if(BGMusicHandle[5] < 0) return 0;
	_XMusic::GetInstance().setMusicVolume(m_configData.volume * 10.0f / 100.0f * 128);

#if USE_FBO
	if(m_fbo.init(2048,1024) == 0) return 0;
	m_fbo.addOneTexture(2048,1024);	//这个是为了实现残影效果而定义的
	m_fbo.addOneTexture(2048,1024);	//这个是为了实现震动效果而定义的
	m_fboSprite.init(2048,1024);
	m_fboSprite.setPosition(0.0f,0.0f);
#if USE_SHADER
	//m_fboSprite.m_pShaderProc = useShader;
	m_fboSprite.setShaderClass(&m_tmpSader);
#else
	m_fboSprite.setAlpha(0.9f);
#endif
	m_fboOrder = 0;

	m_fboShake.init(2048,1024);
#endif

#if USE_FBO
#if USE_SHADER
	initShaderFromText();
#endif
#endif

	m_isInited = 1;
	return 1;
}

int _XGun::toolsEffectInit(_XResourcePosition resoursePosition)
{
	if(m_toolsEffect[0].init("ResourcePack/pic/UI/ToolsEffect02.png",resoursePosition) == 0) return 0;	//分数
	m_toolsEffect[0].setAngle(90.0f);
	m_toolsEffect[0].setPosition(-18.0f + 1087.0f - 4.0f,150.0f + 317.0f - 75.0f);
	if(m_toolsEffect[1].init("ResourcePack/pic/UI/ToolsEffect00.png",resoursePosition) == 0) return 0;	//金币
	m_toolsEffect[1].setAngle(90.0f);
	m_toolsEffect[1].setPosition(-28.0f,150.0f - 32.0f);
	m_toolsEffect[2].setACopy(m_toolsEffect[1]);	//能量
	m_toolsEffect[2].setAngle(90.0f);
	m_toolsEffect[2].setPosition(-97.0f,150.0f - 32.0f);
	if(m_toolsEffect[3].init("ResourcePack/pic/UI/ToolsEffect01.png",resoursePosition) == 0) return 0;	//时间
	m_toolsEffect[3].setAngle(90.0f);
	m_toolsEffect[3].setPosition(819.0f,250.0f);
	m_toolsEffect[4].setACopy(m_toolsEffect[1]);	//减少能量
	m_toolsEffect[4].setAngle(90.0f);
	m_toolsEffect[4].setPosition(-97.0f,150.0f - 32.0f);
	m_toolsEffect[5].setColor(1.0f,1.0f,0.0f,1.0f);
	m_toolsEffect[5].setACopy(m_toolsEffect[1]);	//不需要能量
	m_toolsEffect[5].setAngle(90.0f);
	m_toolsEffect[5].setPosition(-97.0f,150.0f - 32.0f);
	m_toolsEffect[5].setColor(1.0f,0.0f,0.0f,1.0f);

	m_toolsEffectStage[0] = 0;
	m_toolsEffectStage[1] = 0;
	m_toolsEffectStage[2] = 0;
	m_toolsEffectStage[3] = 0;
	m_toolsEffectStage[4] = 0;
	m_toolsEffectStage[5] = 0;
	return 1;
}

void _XGun::toolsEffectMove(int delay)
{
	for(int i = 0;i < 4;++ i)
	{
		switch(m_toolsEffectStage[i])
		{
		case 1:
			m_toolsEffect[i].setAlpha(0.0f);
			m_toolsEffect[i].setSize(10.0f,10.0f);
			m_toolsEffectData[i].set(0.0f,1.0f,0.25f);
			m_toolsEffectStage[i] = 2;
			break;
		case 2://出现
			m_toolsEffectData[i].move(delay);
			m_toolsEffect[i].setAlpha(m_toolsEffectData[i].getNowData());
			m_toolsEffect[i].setSize(10.0f - 9.0f * m_toolsEffectData[i].getNowData(),10.0f - 9.0f * m_toolsEffectData[i].getNowData());
			if(m_toolsEffectData[i].getIsEnd() != 0)
			{
				m_toolsEffectStage[i] = 3;
				m_toolsEffectData[i].set(1.0f,0.0f,0.1f,MOVE_DATA_MODE_COS_MULT);
			}
			break;
		case 3://消失
			m_toolsEffectData[i].move(delay);
			m_toolsEffect[i].setAlpha(m_toolsEffectData[i].getNowData());
			if(m_toolsEffectData[i].getIsEnd() != 0)
			{
				m_toolsEffectStage[i] = 0;
			}
			break;
		}
	}

	for(int i = 4;i < 6;++ i)
	{
		switch(m_toolsEffectStage[i])
		{
		case 1:
			m_toolsEffect[i].setAlpha(0.5f);
			m_toolsEffectData[i].set(0.5f,1.0f,0.25f,MOVE_DATA_MODE_LINE,-1);
			m_toolsEffectStage[i] = 2;
			break;
		case 2://闪烁
			m_toolsEffectData[i].move(delay);
			m_toolsEffect[i].setAlpha(m_toolsEffectData[i].getNowData());
			break;
		case 3://消失
			m_toolsEffect[i].setAlpha(1.0f);
			m_toolsEffectData[i].set(1.0f,0.0f,0.1f);
			m_toolsEffectStage[i] = 4;
			break;
		case 4:
			m_toolsEffectData[i].move(delay);
			m_toolsEffect[i].setAlpha(m_toolsEffectData[i].getNowData());
			if(m_toolsEffectData[i].getIsEnd() != 0)
			{
				m_toolsEffectStage[i] = 0;
			}
			break;
		}
	}
}

void _XGun::toolsEffectDraw()
{
	for(int i = 0;i < 6;++ i)
	{
		if(m_toolsEffectStage[i] != 0) 
		{
			m_toolsEffect[i].draw();
		}
	}
}

int _XGun::userBuffInit(_XResourcePosition resoursePosition)
{
	if(m_userBuffLogo[0].init("ResourcePack/pic/object/Buff_10.png",resoursePosition) == 0) return 0;
	m_userBuffLogo[0].setAngle(90.0f);
	//m_userBuffLogo[0].setPosition(215.0f - 2.0f,640.0f + 32.0f);
	m_userBuffLogo[0].setPosition(2.0f,670.0f);
	m_userBuffLogo[0].setAlpha(0.0f);
	if(m_userBuffLogo[1].init("ResourcePack/pic/object/Buff_11.png",resoursePosition) == 0) return 0;
	m_userBuffLogo[1].setAngle(90.0f);
	//m_userBuffLogo[1].setPosition(215.0f - 2.0f + 37.0f,640.0f + 32.0f);
	m_userBuffLogo[1].setPosition(2.0f + 34.0f,670.0f);
	m_userBuffLogo[1].setAlpha(0.0f);
	if(m_userBuffLogo[2].init("ResourcePack/pic/object/Buff_12.png",resoursePosition) == 0) return 0;
	m_userBuffLogo[2].setAngle(90.0f);
	//m_userBuffLogo[2].setPosition(215.0f - 2.0f + 74.0f,640.0f + 32.0f);
	m_userBuffLogo[2].setPosition(2.0f + 68.0f,670.0f);
	m_userBuffLogo[2].setAlpha(0.0f);
	if(m_userBuffLogo[3].init("ResourcePack/pic/object/Buff_13.png",resoursePosition) == 0) return 0;
	m_userBuffLogo[3].setAngle(90.0f);
	//m_userBuffLogo[3].setPosition(215.0f - 2.0f,640.0f - 39.0f + 32.0f);
	m_userBuffLogo[3].setPosition(2.0f + 102.0f,670.0f);
	m_userBuffLogo[3].setAlpha(0.0f);
	if(m_userBuffLogo[4].init("ResourcePack/pic/object/Buff_14.png",resoursePosition) == 0) return 0;
	m_userBuffLogo[4].setAngle(90.0f);
	//m_userBuffLogo[4].setPosition(215.0f - 2.0f + 37.0f,640.0f - 39.0f + 32.0f);
	m_userBuffLogo[4].setPosition(2.0f + 136.0f,670.0f);
	m_userBuffLogo[4].setAlpha(0.0f);
	if(m_userBuffLogo[5].init("ResourcePack/pic/object/Buff_15.png",resoursePosition) == 0) return 0;
	m_userBuffLogo[5].setAngle(90.0f);
	//m_userBuffLogo[5].setPosition(215.0f - 2.0f + 74.0f,640.0f - 39.0f + 32.0f);
	m_userBuffLogo[5].setPosition(2.0f + 170.0f,670.0f);
	m_userBuffLogo[5].setAlpha(0.0f);
	clearAllBuff();
	return 1;
}

void _XGun::userBuffMove(int delay)
{
	if(m_userBuffFlag[2] > 0 && m_userBuffFlag[2] < 6)
	{//威力加强时的枪闪烁
		m_userBuffTimer += delay;
		if(m_userBuffTimer > 150)
		{
			if(m_gunIsFlash == 0) m_gunIsFlash = 1;
			else m_gunIsFlash = 0;
			m_userBuffTimer = 0;
		}
	}
	for(int i = 0;i < 6;++ i)
	{
		if(m_userBuffFlag[i] != 0)
		{
			if(m_userBuffFlag[i] < 6)
			{
				m_userBuffReleaseTime[i] -= delay;
				if(m_userBuffReleaseTime[i] <= 0)
				{
					m_userBuffReleaseTime[i] = 0;
					m_userBuffFlag[i] = 6;	//进入消失
					if(i == 5)
					{//免费射击
						m_toolsFlag = 0;
						checkGunType();
					}
				}
			}
			switch(m_userBuffFlag[i])
			{
			case 1:	//准备出现
				m_userBuffLogo[i].setAlpha(0.0f);
				m_userBuffData[i].set(0.0f,1.0f,0.1f);
				m_userBuffFlag[i] = 2;
				if(i == 1) _XSound::GetInstance().enableAllSlowDown();	//时间减慢的道具造成声音也减慢
				break;
			case 2:
				m_userBuffData[i].move(delay);
				m_userBuffLogo[i].setAlpha(m_userBuffData[i].getNowData());
				if(m_userBuffData[i].getIsEnd() != 0)
				{
					m_userBuffFlag[i] = 3;
				}
				break;
			case 3://进入持续
				m_userBuffLogo[i].setAlpha(1.0f);
				m_userBuffFlag[i] = 4;
				break;
			case 4:
				if(m_userBuffReleaseTime[i] < 3000)
				{//进入闪烁
					m_userBuffFlag[i] = 5;
					m_userBuffData[i].set(1.0f,0.5f,0.5f,MOVE_DATA_MODE_LINE,-1);
				}
				break;
			case 5:
				m_userBuffData[i].move(delay);
				m_userBuffLogo[i].setAlpha(m_userBuffData[i].getNowData());
				break;
			case 6://进入消失
				if(i == 5) m_toolsEffectStage[5] = 3;	//免费射击的道具
				if(i == 4) m_toolsEffectStage[4] = 3;	//免费射击的道具
				m_userBuffLogo[i].setAlpha(1.0f);
				m_userBuffData[i].set(1.0f,0.0f,0.1f);
				m_userBuffFlag[i] = 7;
				if(i == 2) m_gunIsFlash = 0;	//退出时取消闪烁
				break;
			case 7:
				m_userBuffData[i].move(delay);
				m_userBuffLogo[i].setAlpha(m_userBuffData[i].getNowData());
				if(m_userBuffData[i].getIsEnd() != 0)
				{
					m_userBuffFlag[i] = 0;
					if(i == 1) _XSound::GetInstance().disableAllSlowDown();	//时间减慢的道具造成声音也减慢
				}
				break;
			}
		}
	}
}

void _XGun::userBuffDraw()
{
	for(int i = 0;i < 6;++ i)
	{
		if(m_userBuffFlag[i] != 0) m_userBuffLogo[i].draw();
		//m_userBuffLogo[i].draw();
	}
}

int _XGun::faceInit(_XResourcePosition resoursePosition)
{
	if(m_girlFace[0].init("ResourcePack/pic/UI/face/Face000.png",resoursePosition) == 0) return 0;
	m_girlFace[0].setPosition(200.0f,18.0f);
	m_girlFace[0].setAngle(90.0f);
	if(m_girlFace[1].init("ResourcePack/pic/UI/face/Face001.png",resoursePosition) == 0) return 0;
	m_girlFace[1].setPosition(200.0f,18.0f);
	m_girlFace[1].setAngle(90.0f);
	if(m_girlFace[2].init("ResourcePack/pic/UI/face/Face002.png",resoursePosition) == 0) return 0;
	m_girlFace[2].setPosition(200.0f,18.0f);
	m_girlFace[2].setAngle(90.0f);
	if(m_girlFace[3].init("ResourcePack/pic/UI/face/Face003.png",resoursePosition) == 0) return 0;
	m_girlFace[3].setPosition(200.0f,18.0f);
	m_girlFace[3].setAngle(90.0f);
	if(m_girlFace[4].init("ResourcePack/pic/UI/face/Face004.png",resoursePosition) == 0) return 0;
	m_girlFace[4].setPosition(200.0f,18.0f);
	m_girlFace[4].setAngle(90.0f);
	if(m_girlFace[5].init("ResourcePack/pic/UI/face/Face005.png",resoursePosition) == 0) return 0;
	m_girlFace[5].setPosition(200.0f,18.0f);
	m_girlFace[5].setAngle(90.0f);

	if(m_faceBG.init("ResourcePack/pic/UI/face/Board.png",resoursePosition) == 0) return 0;
	m_faceBG.setPosition(200.0f,134.0f);
	//m_faceBG.setAngle(90.0f);
	if(m_faceText.init("ResourcePack/pic/UI/Font/FontUnicode30_T000.png",_XVector2(30,30),_XVector2(34,34),4,resoursePosition) == 0) return 0;
	m_faceText.setPosition(280.0f,180.0f);
	m_faceText.setAngle(90.0f);
	m_faceText.setString("让我们尽情破坏吧!我期待很久了.");

	m_faceStage = 0;
	m_nowFaceOrder = 0;

	return 1;
}

char faceString[12][64] = {
						"让我们尽情破坏吧!我期待很久了.",
						"摧毁那些宝石,获得更多金币.",
						"这也太简单了吧!",
						"简直就是小菜一碟嘛!",
						"目标已达成,哈哈哈....",
						"达成目标原来这么简单呀!",
						"轻松达成目标,一切尽在掌握之中.",
						"熟能生巧,一切都简单起来了.",
						"得注意技巧,还需要多练习才行呀.",
						"还是没有掌握要领呀,真伤脑筋.",
						"终于达成目标了,哎,不容易呀!",
						"这样下去很快就要Over了呀!"};

void _XGun::faceMove(int delay)
{
	delay = (float)(delay) / buffTimerRate();
	switch(m_faceStage)
	{//初始化
	case 1:
		m_girlFace[0].setAlpha(0.0f);
		m_girlFace[1].setAlpha(0.0f);
		m_girlFace[2].setAlpha(0.0f);
		m_girlFace[3].setAlpha(0.0f);
		m_girlFace[4].setAlpha(0.0f);
		m_girlFace[5].setAlpha(0.0f);
		m_faceTextRandom = random(100) % 2;
		
		m_faceBG.setAlpha(0.0f);
		m_faceText.setColor(-1.0f,-1.0f,-1.0f,0.0f);

		m_faceMoveData.set(0.0f,1.0f,0.2f);
		m_faceStage = 2;
		break;
	case 2://显示背景
		{
			m_faceMoveData.move(delay);
			float temp = m_faceMoveData.getNowData();
			m_girlFace[m_nowFaceOrder].setAlpha(temp);
			m_faceBG.setAlpha(temp);
			m_faceBG.setClipRect(0.0f,0.0f,128.0f,38.0f + (512.0f - 38.0f) * temp);
			if(m_faceMoveData.getIsEnd() != 0)
			{//结束
				m_faceStage = 3;
				m_faceTextSum = 0;
				m_faceTextTimer = 0;
				m_faceText.setString(" ");
				m_faceText.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			}
		}
		break;
	case 3:	//显示文字
		m_faceTextTimer += delay;
		if(m_faceTextTimer >= 100)
		{
			m_faceTextTimer = 0;

			m_faceTextSum ++;
			char tempString[64];
			strcpy(tempString,faceString[2 * m_nowFaceOrder + m_faceTextRandom]);
			if(m_faceTextSum >= strlen(tempString))
			{
				m_faceStage = 4;
				m_faceTextTimer = 0;
			}
			if(tempString[m_faceTextSum - 1] > 0)
			{
				tempString[m_faceTextSum + 0] = '\0';
				tempString[m_faceTextSum + 1] = '\0';
				m_faceText.setString(tempString);
			}else
			{
				m_faceTextSum ++;
				if(m_faceTextSum >= strlen(tempString))
				{
					m_faceStage = 4;
					m_faceTextTimer = 0;
				}
			//	if(tempString[m_faceTextSum - 1] > 0)
				{
					tempString[m_faceTextSum + 0] = '\0';
					tempString[m_faceTextSum + 1] = '\0';
					m_faceText.setString(tempString);
				}
			}
		}
		break;
	case 4:	//等待一段时间
		m_faceTextTimer += delay;
		if((m_faceTextTimer >= 2000 && m_nowFaceOrder == 0) || m_faceTextTimer > 5000)
		{
			m_faceStage = 5;
		}
		break;
	case 5:	//准备消失
		m_faceMoveData.set(0.0f,1.0f,0.5f);
		m_faceStage = 6;
		break;
	case 6:
		{
			m_faceMoveData.move(delay);
			float temp = 1.0f - m_faceMoveData.getNowData();

			m_girlFace[m_nowFaceOrder].setAlpha(temp);
			m_faceBG.setAlpha(temp);
			m_faceText.setColor(-1.0f,-1.0f,-1.0f,temp);

			if(m_faceMoveData.getIsEnd() != 0)
			{
				m_faceStage = 0;
			}
		}
		break;
	}
}

void _XGun::faceDraw()
{
	if(m_faceStage != 0)
	{
		m_girlFace[0].draw();
		m_girlFace[1].draw();
		m_girlFace[2].draw();
		m_girlFace[3].draw();
		m_girlFace[4].draw();
		m_girlFace[5].draw();
		m_faceBG.draw();
		m_faceText.draw();
	}
}

int _XGun::smallStarInit(_XResourcePosition resoursePosition)
{
	if(m_smallStar[0].init("ResourcePack/pic/star/Space_N0.png",resoursePosition) == 0) return 0;
	if(m_smallStar[1].init("ResourcePack/pic/star/Space_N1.png",resoursePosition) == 0) return 0;
	if(m_smallStar[2].init("ResourcePack/pic/star/Space_N2.png",resoursePosition) == 0) return 0;
	if(m_smallStar[3].init("ResourcePack/pic/star/Space_N3.png",resoursePosition) == 0) return 0;
	if(m_smallStar[4].init("ResourcePack/pic/star/Space_N4.png",resoursePosition) == 0) return 0;
	if(m_smallStar[5].init("ResourcePack/pic/star/Space_N5.png",resoursePosition) == 0) return 0;
	for(int i = 0;i < SMALL_STAR_SUM;++ i)
	{
		m_smallSrarStage[i] = 0;
	}
	//m_smallStarM = 2.0f;
	m_smallStarMoveData.set(3.0f,88.0f,0.0001f,MOVE_DATA_MODE_LINE,-1);
	return 1;
}

void _XGun::smallStarMove(int delay)
{
	//下面是小行星的生成
	m_smallStarMoveData.move(delay);
	if(random(10000) < m_smallStarMoveData.getNowData())
	{//符合生成条件产生一颗小行星
		for(int i = 0;i < SMALL_STAR_SUM;++ i)
		{
			if(m_smallSrarStage[i] == 0)
			{
				if(random(100) < 5)
				{//垂直下落的
					m_smallStarPosition[i].set(1280.0f + 128.0f,128.0f + random(720.0f - 256.0f));
					float temp = random(100)/100.0f * 0.8f + 0.2f;
					m_smallStarSize[i].set(temp,temp);
					m_smallStarSpeed[i].set((random(100) / 100.0f * 0.9f + 0.1f ) * 0.1f,(random(100) / 100.0f - 0.5f) * 0.01f);
				}else
				{
					m_smallStarPosition[i].set(900.0f + gaussRand() * 400.0f,720.0f + 128.0f);
					float temp = random(100)/100.0f * 0.8f + 0.2f;
					m_smallStarSize[i].set(temp,temp);
					temp = (random(100) / 100.0f * 0.9f + 0.1f ) * 0.05f;
					m_smallStarSpeed[i].set(temp,temp);
				}
				m_smallStarType[i] = (m_nowLevel + random(5)) % 6;
				m_smallSrarStage[i] = 1;
				break;
			}
		}
	}
	//下面是小行星的运行
	for(int i = 0;i < SMALL_STAR_SUM;++ i)
	{
		if(m_smallSrarStage[i] != 0)
		{
			m_smallStarPosition[i].x -= m_smallStarSpeed[i].x * delay;
			m_smallStarPosition[i].y -= m_smallStarSpeed[i].y * delay;

			if(m_smallStarPosition[i].x < 0.0f - 128.0f || m_smallStarPosition[i].y < 0.0f - 128.0f) m_smallSrarStage[i] = 0;
		}
	}
}

void _XGun::smallStarDraw()
{
	for(int i = 0;i < SMALL_STAR_SUM;++ i)
	{
		if(m_smallSrarStage[i] != 0)
		{
			m_smallStar[m_smallStarType[i]].setPosition(m_smallStarPosition[i]);
			m_smallStar[m_smallStarType[i]].setSize(m_smallStarSize[i]);
			m_smallStar[m_smallStarType[i]].draw();
		}
	}
}

int _XGun::soundInit(_XResourcePosition resoursePosition)
{
	if((m_soundGun_00 = _XSound::GetInstance().addSound("ResourcePack/Sound/gun_00.wav",resoursePosition)) < 0) return 0;
	if((m_soundGun_01 = _XSound::GetInstance().addSound("ResourcePack/Sound/gun_01.wav",resoursePosition)) < 0) return 0;
	if((m_soundGun_02 = _XSound::GetInstance().addSound("ResourcePack/Sound/gun_02.wav",resoursePosition)) < 0) return 0;
	if((m_soundGun_03 = _XSound::GetInstance().addSound("ResourcePack/Sound/gun_03.wav",resoursePosition)) < 0) return 0;
	if((m_soundGun_04 = _XSound::GetInstance().addSound("ResourcePack/Sound/gun_04.wav",resoursePosition)) < 0) return 0;
	if((m_soundGun_05 = _XSound::GetInstance().addSound("ResourcePack/Sound/gun_05.wav",resoursePosition)) < 0) return 0;
	if((m_soundGun_06 = _XSound::GetInstance().addSound("ResourcePack/Sound/gun_06.wav",resoursePosition)) < 0) return 0;
	if((m_soundBoom_00 = _XSound::GetInstance().addSound("ResourcePack/Sound/Boom_00.wav",resoursePosition)) < 0) return 0;
	if((m_soundBoom_01 = _XSound::GetInstance().addSound("ResourcePack/Sound/Boom_01.wav",resoursePosition)) < 0) return 0;
	if((m_soundBoom_02 = _XSound::GetInstance().addSound("ResourcePack/Sound/Boom_02.wav",resoursePosition)) < 0) return 0;
	if((m_soundBoom_03 = _XSound::GetInstance().addSound("ResourcePack/Sound/Boom_03.wav",resoursePosition)) < 0) return 0;
	if((m_soundBoom_04 = _XSound::GetInstance().addSound("ResourcePack/Sound/Boom_04.wav",resoursePosition)) < 0) return 0;
	if((m_soundCoinIn = _XSound::GetInstance().addSound("ResourcePack/Sound/CoinIn.wav",resoursePosition)) < 0) return 0;
	if((m_soundSumAdd_00 = _XSound::GetInstance().addSound("ResourcePack/Sound/SumAdd_00.wav",resoursePosition)) < 0) return 0;
	if((m_soundTimerOver = _XSound::GetInstance().addSound("ResourcePack/Sound/TimeOverSound.wav",resoursePosition)) < 0) return 0;
	if((m_soundStarAdd = _XSound::GetInstance().addSound("ResourcePack/Sound/StarAdd.wav",resoursePosition)) < 0) return 0;
	if((m_soundTextDown = _XSound::GetInstance().addSound("ResourcePack/Sound/TextDown.wav",resoursePosition)) < 0) return 0;
	if((m_soundTimer_00 = _XSound::GetInstance().addSound("ResourcePack/Sound/Timer_00.wav",resoursePosition)) < 0) return 0;
	//if((m_soundTimer_01 = _XSound::GetInstance().addSound("ResourcePack/Sound/Timer_01.wav",resoursePosition)) < 0) return 0;
	if((m_soundAddEnergy = _XSound::GetInstance().addSound("ResourcePack/Sound/AddEnegy.wav",resoursePosition)) < 0) return 0;
	if((m_soundChangeGun = _XSound::GetInstance().addSound("ResourcePack/Sound/changeGun.wav",resoursePosition)) < 0) return 0;
	if((m_soundTextMove_00 = _XSound::GetInstance().addSound("ResourcePack/Sound/TextMove_00.wav",resoursePosition)) < 0) return 0;
	if((m_soundTextMove_01 = _XSound::GetInstance().addSound("ResourcePack/Sound/TextMove_01.wav",resoursePosition)) < 0) return 0;
	if((m_soundTypeIn = _XSound::GetInstance().addSound("ResourcePack/Sound/TypeIn.wav",resoursePosition)) < 0) return 0;
	if((m_soundTypeMove = _XSound::GetInstance().addSound("ResourcePack/Sound/TypeMove.wav",resoursePosition)) < 0) return 0;
	if((m_soundHitGem = _XSound::GetInstance().addSound("ResourcePack/Sound/hitGem.wav",resoursePosition)) < 0) return 0;
	if((m_soundHitShield = _XSound::GetInstance().addSound("ResourcePack/Sound/hotDun.wav",resoursePosition)) < 0) return 0;
	if((m_soundGemBreak = _XSound::GetInstance().addSound("ResourcePack/Sound/gemBreak.wav",resoursePosition)) < 0) return 0;
	if((m_soundEnergyFree = _XSound::GetInstance().addSound("ResourcePack/Sound/energyIn.wav",resoursePosition)) < 0) return 0;
	if((m_soundHitSide = _XSound::GetInstance().addSound("ResourcePack/Sound/hitSide.wav",resoursePosition)) < 0) return 0;
	if((m_soundSJTools = _XSound::GetInstance().addSound("ResourcePack/Sound/SJTools.wav",resoursePosition)) < 0) return 0;
	if((m_soundFSTools = _XSound::GetInstance().addSound("ResourcePack/Sound/FSTools.wav",resoursePosition)) < 0) return 0;
	if((m_soundScreenDown = _XSound::GetInstance().addSound("ResourcePack/Sound/ScreenDown.wav",resoursePosition)) < 0) return 0;
	if((m_soundScreenUp = _XSound::GetInstance().addSound("ResourcePack/Sound/ScreenUp.wav",resoursePosition)) < 0) return 0;
	if((m_soundColorBoom = _XSound::GetInstance().addSound("ResourcePack/Sound/HitColorBoom.wav",resoursePosition)) < 0) return 0;

	_XSound::GetInstance().slowDownAllInit(3);
	return 1;
}

int _XGun::readRecordData()
{
#if DEBUG00
	printf("readRecordData!\n");
#endif	
	unsigned char *data = NULL;
	int size = MAX_GAME_ORDER_SUM * sizeof(int) + MAX_GAME_ORDER_SUM * MAX_GAME_ORDER_NAME_LENGTH + sizeof(_XConfigData);
	try
	{
		data = new unsigned char[size];
		if(data == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	if(m_data00.mergeData(SAVE_FILE_NAME) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	if(m_data00.analyzingData(data,size) == -1) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	memcpy(m_gameOrderScores,data,MAX_GAME_ORDER_SUM * sizeof(int));
	memcpy(m_gameOrderName,data + MAX_GAME_ORDER_SUM * sizeof(int),MAX_GAME_ORDER_SUM * MAX_GAME_ORDER_NAME_LENGTH);
	memcpy(&m_configData,data + MAX_GAME_ORDER_SUM * sizeof(int) + MAX_GAME_ORDER_SUM * MAX_GAME_ORDER_NAME_LENGTH,sizeof(_XConfigData));
	XDELETE_ARRAY(data);
	return 1;
}

int _XGun::readHeadData()
{
#if DEBUG00
	printf("readHeadData!\n");
#endif
	unsigned char *data = NULL;
	int size = MAX_GAME_ORDER_CAMERA_DATA * MAX_GAME_ORDER_SUM;
	try
	{
		data = new unsigned char[size];
		if(data == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	if(m_data01.mergeData(SAVE_FILE_NAME_HEAD) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	if(m_data01.analyzingData(data,size) == -1) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	memcpy(m_gameOrderCameraData,data,MAX_GAME_ORDER_CAMERA_DATA * MAX_GAME_ORDER_SUM);
	XDELETE_ARRAY(data);
	return 1;
}

int _XGun::writeRecordData()
{
#if DEBUG00
	printf("writeRecordData!\n");
#endif
	unsigned char *data = NULL;
	int size = MAX_GAME_ORDER_SUM * sizeof(int) + MAX_GAME_ORDER_SUM * MAX_GAME_ORDER_NAME_LENGTH + sizeof(_XConfigData);
	try
	{
		data = new unsigned char[size];
		if(data == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	memcpy(data,m_gameOrderScores,MAX_GAME_ORDER_SUM * sizeof(int));
	memcpy(data + MAX_GAME_ORDER_SUM * sizeof(int),m_gameOrderName,MAX_GAME_ORDER_SUM * MAX_GAME_ORDER_NAME_LENGTH);
	memcpy(data + MAX_GAME_ORDER_SUM * sizeof(int) + MAX_GAME_ORDER_SUM * MAX_GAME_ORDER_NAME_LENGTH,&m_configData,sizeof(_XConfigData));
	if(m_data00.mergeData(data,size) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	if(m_data00.analyzingDataManager(SAVE_FILE_NAME) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	XDELETE_ARRAY(data);
	return 1;
}

int _XGun::writeHeadData()
{
#if DEBUG00
	printf("writeHeadData!\n");
#endif
	unsigned char *data = NULL;
	int size = MAX_GAME_ORDER_CAMERA_DATA * MAX_GAME_ORDER_SUM;
	try
	{
		data = new unsigned char[size];
		if(data == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	memcpy(data,m_gameOrderCameraData,MAX_GAME_ORDER_CAMERA_DATA * MAX_GAME_ORDER_SUM);
	if(m_data01.mergeData(data,size) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	if(m_data01.analyzingDataManager(SAVE_FILE_NAME_HEAD) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	XDELETE_ARRAY(data);
	return 1;
}

int _XGun::readUserData()
{
#if DEBUG00
	printf("readUserData!\n");
#endif
	unsigned char *data = NULL;
	int size = MAX_GAME_ORDER_CAMERA_DATA * USER_HEAD_SUM + 1 + USER_HEAD_SUM * MAX_GAME_ORDER_NAME_LENGTH;
	try
	{
		data = new unsigned char[size];
		if(data == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	if(m_data02.mergeData(SAVE_FILE_NAME_USER) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	if(m_data02.analyzingData(data,size) == -1) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	m_takePhotoPoint = data[0];
	memcpy(m_gameOrderUserHeadName,data + 1,USER_HEAD_SUM * MAX_GAME_ORDER_NAME_LENGTH);
	memcpy(m_gameOrderUserHeadData,data + 1 + USER_HEAD_SUM * MAX_GAME_ORDER_NAME_LENGTH,MAX_GAME_ORDER_CAMERA_DATA * USER_HEAD_SUM);
	XDELETE_ARRAY(data);
	return 1;
}

int _XGun::writeUserData()
{
#if DEBUG00
	printf("writeUserData!\n");
#endif
	unsigned char *data = NULL;
	int size = MAX_GAME_ORDER_CAMERA_DATA * USER_HEAD_SUM + 1 + USER_HEAD_SUM * MAX_GAME_ORDER_NAME_LENGTH;
	try
	{
		data = new unsigned char[size];
		if(data == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	data[0] = m_takePhotoPoint;
	memcpy(data + 1,m_gameOrderUserHeadName,USER_HEAD_SUM * MAX_GAME_ORDER_NAME_LENGTH);
	memcpy(data + 1 + USER_HEAD_SUM * MAX_GAME_ORDER_NAME_LENGTH,m_gameOrderUserHeadData,MAX_GAME_ORDER_CAMERA_DATA * USER_HEAD_SUM);

	if(m_data02.mergeData(data,size) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	if(m_data02.analyzingDataManager(SAVE_FILE_NAME_USER) == false) 
	{
		XDELETE_ARRAY(data);
		return 0;
	}
	XDELETE_ARRAY(data);
	return 1;
}

int _XGun::initRecordData()
{
#if DEBUG00
	printf("initRecordData!\n");
#endif
	//初始化玩家的排行榜信息，这个正式版本的时候需要从外部文件中读取
	for(int i = 0; i < MAX_GAME_ORDER_SUM;++ i)
	{
		m_gameOrderScores[i] = 5000 * (i + 1);
		strcpy(m_gameOrderName[i],"xiajia");
	}
	if(writeRecordData() == 0) return 0;
	return 1;
}

int _XGun::initHeadData(int flagWrite)
{
#if DEBUG00
	printf("initHeadData!\n");
#endif
	//从默认文件中读取默认数据
	int length = _XResourcePack::GetInstance().getFileLength("defaulthead.dat");
	if(length < 0) return 0;
	unsigned char *p = NULL;
	try
	{
		p = new unsigned char[length + 1];
		if(p == NULL) return NULL;
	}catch(...)
	{
		return 0;
	}
	if(_XResourcePack::GetInstance().unpackResource("defaulthead.dat",p) != 1)
	{
		return 0;
	}
	for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
	{
		memcpy(m_gameOrderCameraData + i * MAX_GAME_ORDER_CAMERA_DATA,p,MAX_GAME_ORDER_CAMERA_DATA);
	}
	XDELETE_ARRAY(p);

	//memset(m_gameOrderCameraData,0,MAX_GAME_ORDER_CAMERA_DATA * MAX_GAME_ORDER_SUM);
	if(flagWrite == 0)
	{
		if(writeHeadData() == 0) return 0;
	}
	return 1;
}

int _XGun::initUserData()
{
#if DEBUG00
	printf("initUserData!\n");
#endif
	//从默认文件中读取默认数据
	int length = _XResourcePack::GetInstance().getFileLength("defaulthead.dat");
	if(length < 0) return 0;
	unsigned char *p = NULL;
	try
	{
		p = new unsigned char[length + 1];
		if(p == NULL) return NULL;
	}catch(...)
	{
		return 0;
	}
	if(_XResourcePack::GetInstance().unpackResource("defaulthead.dat",p) != 1)
	{
		return 0;
	}
	for(int i = 0;i < USER_HEAD_SUM;++ i)
	{
		memcpy(m_gameOrderUserHeadData + i * MAX_GAME_ORDER_CAMERA_DATA,p,MAX_GAME_ORDER_CAMERA_DATA);
		strcpy(m_gameOrderUserHeadName[i],"xiajia");
	}
	XDELETE_ARRAY(p);

	m_takePhotoPoint = 0;
	if(writeUserData() == 0) return 0;
	return 1;
}

int _XGun::gameOrderInit(_XResourcePosition resoursePosition)
{
//	if(m_gameOrderFont.init("ResourcePack/pic/UI/Font/FontUnicode30_T000.png",_XVector2(30,30),_XVector2(34,34),4,resoursePosition) == 0) return 0;
//	m_gameOrderFont.setAngle(90.0f);
//	m_gameOrderFont.setPosition(1070,90);
//	m_gameOrderFont.setString("欢迎进入新的冒险!");
//	m_gameOrderFontInput.setACopy(m_gameOrderFont);
	if(m_gameOrderFontInput.init("ResourcePack/pic/UI/Font/FontUnicode30_T000.png",_XVector2(30,30),_XVector2(34,34),4,resoursePosition) == 0) return 0;
	m_gameOrderFontInput.setAngle(90.0f);
	m_gameOrderFontInput.setPosition(296,234);
	m_gameOrderFontInput.setString("请输入您的称呼:");
//	m_gameOrderNumber_00.setACopy(m_goldAddNum[0]);
//	m_gameOrderNumber_00.setAngle(90.0f);
//	m_gameOrderNumber_00.setNumber(10);
//	m_gameOrderNumber_00.setPosition(1067,120);
//	m_gameOrderNumber_01.setACopy(m_goldAddNum[0]);
//	m_gameOrderNumber_01.setAngle(90.0f);
//	m_gameOrderNumber_01.setNumber(100000);
//	m_gameOrderNumber_01.setPosition(1067,527);
//	if(m_gameOrderText_00.init("ResourcePack/pic/UI/OrderText.png",resoursePosition) == 0) return 0;
//	m_gameOrderText_00.setAngle(90);
//	m_gameOrderText_00.setPosition(540.0f,402.0f);
//	if(m_gameOrderText_01.init("ResourcePack/pic/UI/OrderText_00.png",resoursePosition) == 0) return 0;
//	m_gameOrderText_01.setAngle(90);
//	m_gameOrderText_01.setPosition(540.0f,320.0f);
//	m_showGameOrderFlag = 0;
	m_gameOrderProBG.setACopy(m_nowLevelProcessB);
	m_gameOrderProBG.setPosition(1140.0f,50.0f + 64.0f);
	m_gameOrderProMD.setACopy(m_nowLevelProcess);
	m_gameOrderProMD.setPosition(1140.0f,50.0f + 64.0f);
	m_gameOrderProMD.setClipRect(0,0,32,1);
	if(m_gameOrderProUP.init("ResourcePack/pic/UI/Progress/PU_01.png",resoursePosition) == 0) return 0;
	m_gameOrderProUP.setPosition(1140.0f,50.0f + 64.0f);
	m_gameOrderProNumber.setACopy(m_goldAddNum[0]);
	m_gameOrderProNumber.setAngle(90.0f);
	m_gameOrderProNumber.setPosition(1172.0f,50.0f + 290.0f);
	m_gameOrderProNumber.setNumber(10);
	m_gameOrderProMoveNumber.setArmNumber(0.0f);

	if(m_gameInputNameBG.init("ResourcePack/pic/UI/GameOrderNameBG.png",resoursePosition) == 0) return 0;
	m_gameInputNameBG.setAngle(90);
	m_gameInputNameBG.setPosition(100.0f,320.0f);
	if(m_inputNameFont.init("ResourcePack/pic/UI/GameOrderFont.png",_XVector2(20,24),_XVector2(12,8),resoursePosition) == 0) return 0;
	m_inputNameFont.setAngle(90);
	m_userheadName[0].setACopy(m_inputNameFont);
	m_userheadName[0].setAngle(90);
	m_userheadName[0].setDistance(-4);
	m_userheadName[1].setACopy(m_inputNameFont);
	m_userheadName[1].setAngle(90);
	m_userheadName[1].setDistance(-4);
	m_userheadName[2].setACopy(m_inputNameFont);
	m_userheadName[2].setAngle(90);
	m_userheadName[2].setDistance(-4);
	m_userheadName[3].setACopy(m_inputNameFont);
	m_userheadName[3].setAngle(90);
	m_userheadName[3].setDistance(-4);

	getNowOrder();
	return 1;
}

void _XGun::gameOrderDraw()
{
//	m_gameOrderFont.draw();
	//if(m_showGameOrderFlag == 0)
	//{
	//	m_gameOrderText_00.draw();
	//	m_gameOrderNumber_00.draw();
	//	m_gameOrderNumber_01.draw();
	//}else
	//{
	//	m_gameOrderText_01.draw();
	//}
	m_gameOrderProBG.draw();
	m_gameOrderProMD.draw();
	m_gameOrderProUP.draw();
	m_gameOrderProNumber.draw();
}

void _XGun::gameOrderDrawUp()
{
	m_gameOrderProNumber.draw();
}

void _XGun::updateGameOrder()
{
	getNowOrder();
////	char tempStr[256];
	if(m_gameNowOrder == MAX_GAME_ORDER_SUM)
	{//最后一名之后
////		sprintf(tempStr,"您现在排在第%d名之后,离下一名还差%d分",m_gameNowOrder,m_gameOrderNextNeedScores);
//		m_showGameOrderFlag = 0;
//		if(m_gameNowOrder < 10) m_gameOrderNumber_00.setPosition(1067,132);
//		else m_gameOrderNumber_00.setPosition(1067,120);
//		m_gameOrderNumber_00.setNumber(m_gameNowOrder);
//		if(m_gameOrderNextNeedScores >= 100000) m_gameOrderNumber_01.setPosition(1067,527);
//		else if(m_gameOrderNextNeedScores >= 10000) m_gameOrderNumber_01.setPosition(1067,539);
//		else if(m_gameOrderNextNeedScores >= 1000) m_gameOrderNumber_01.setPosition(1067,551);
//		else if(m_gameOrderNextNeedScores >= 100) m_gameOrderNumber_01.setPosition(1067,563);
//		else if(m_gameOrderNextNeedScores >= 10) m_gameOrderNumber_01.setPosition(1067,575);
//		else m_gameOrderNumber_01.setPosition(1067,587);
//		m_gameOrderNumber_01.setNumber(m_gameOrderNextNeedScores);
		m_gameOrderProNumber.setPosition(1172.0f,50.0f + 290.0f);
		m_gameOrderProNumber.setNumber(10);
		if(m_gameOrderScores[0] > 0)
		{
			m_gameOrderProMoveNumber.setArmNumber(1000.0f * m_scoresArmSum / m_gameOrderScores[0]);
		//	m_gameOrderProMD.setClipRect(0,0,32.0f,512.0f * (float)m_scoresArmSum / (float)m_gameOrderScores[0]);
		}else
		{
			m_gameOrderProMoveNumber.setArmNumber(1000.0f);
		//	m_gameOrderProMD.setClipRect(0,0,32.0f,512.0f);
		}
	}else
	if(m_gameNowOrder == 0)
	{//第一名之前
////		sprintf(tempStr,"您排在第1名,您创造了新的记录,再接再厉！");
//		m_showGameOrderFlag = 1;
		m_gameOrderProNumber.setNumber(1);
		m_gameOrderProMoveNumber.setArmNumber(1000.0f);
	//	m_gameOrderProMD.setClipRect(0,0,32.0f,512.0f);
	}else
	{
////		sprintf(tempStr,"您现在排在第%d名之后,离下一名还差%d分",m_gameNowOrder,m_gameOrderNextNeedScores);
//		m_showGameOrderFlag = 0;
//		if(m_gameNowOrder < 10) m_gameOrderNumber_00.setPosition(1067,132);
//		else m_gameOrderNumber_00.setPosition(1067,120);
//		m_gameOrderNumber_00.setNumber(m_gameNowOrder);
//		if(m_gameOrderNextNeedScores >= 100000) m_gameOrderNumber_01.setPosition(1067,527);
//		else if(m_gameOrderNextNeedScores >= 10000) m_gameOrderNumber_01.setPosition(1067,539);
//		else if(m_gameOrderNextNeedScores >= 1000) m_gameOrderNumber_01.setPosition(1067,551);
//		else if(m_gameOrderNextNeedScores >= 100) m_gameOrderNumber_01.setPosition(1067,563);
//		else if(m_gameOrderNextNeedScores >= 10) m_gameOrderNumber_01.setPosition(1067,575);
//		else m_gameOrderNumber_01.setPosition(1067,587);
//		m_gameOrderNumber_01.setNumber(m_gameOrderNextNeedScores);
		m_gameOrderProNumber.setPosition(1172.0f,50.0f + 290.0f + 16.0f);
		m_gameOrderProNumber.setNumber(m_gameNowOrder + 1);
		float temp = m_gameOrderScores[MAX_GAME_ORDER_SUM - m_gameNowOrder] - m_gameOrderScores[MAX_GAME_ORDER_SUM - m_gameNowOrder - 1];
		if(temp > 0)
		{
			m_gameOrderProMoveNumber.setArmNumber((1.0f - m_gameOrderNextNeedScores / temp) * 1000.0f);
		//	m_gameOrderProMD.setClipRect(0,0,32.0f,512.0f * (1.0f - m_gameOrderNextNeedScores / temp));
		}else
		{
			m_gameOrderProMoveNumber.setArmNumber(1000.0f);
		//	m_gameOrderProMD.setClipRect(0,0,32.0f,512.0f);
		}
	}
////	m_gameOrderFont.setString(tempStr);
}

void _XGun::gameOrderMove(int interval)
{
	inPutNameMove(interval);
	if(m_gameOrderProMoveNumber.move(interval) != 0)
	{
		m_gameOrderProMD.setClipRect(0,0,32.0f,512.0f * m_gameOrderProMoveNumber.getNowNumber() / 1000.0f);
	}
}

void _XGun::drawChar(float alpha)
{
	char temp[5];
	float tempMove = 0;
	if(m_gameCounter >= 9)
	{
		m_inputCharFlag = 0;
		tempMove = 600.0f * sin(m_inputNameY);
	}else
	{
		tempMove = 60.0f * sin(m_inputCharMove) * m_inputCharFlag;
	}
	if(m_inputCharFlag == 0)
	{
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * 0.5f + tempMove / 60.0f);
		if(m_nowChoose > 'A') sprintf(temp,"%c",m_nowChoose - 1);
		else sprintf(temp,"%c",'Z');
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,286 - tempMove);
		m_inputNameFont.draw();
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha + tempMove / 60.0f);
		sprintf(temp,"%c",m_nowChoose);
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,346 - tempMove);
		m_inputNameFont.draw();
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * 0.5f + tempMove / 60.0f);
		if(m_nowChoose < 'Z') sprintf(temp,"%c",m_nowChoose + 1);
		else sprintf(temp,"%c",'A');
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,406 - tempMove);
		m_inputNameFont.draw();
	}else
	if(m_inputCharFlag == -1)
	{//右边移动
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * (0.0f + 0.5f * sin(m_inputCharMove)));	//1 0 - 0.5
		if(m_nowChoose > 'A') sprintf(temp,"%c",m_nowChoose - 1);
		else sprintf(temp,"%c",'Z');
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,226 - tempMove);
		m_inputNameFont.draw();
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * (0.5f + 0.5f * sin(m_inputCharMove)));	//2 0.5 - 1
		sprintf(temp,"%c",m_nowChoose);
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,286 - tempMove);
		m_inputNameFont.draw();
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * (1.0f - 0.5f * sin(m_inputCharMove)));	//3 1 - 0.5
		if(m_nowChoose < 'Z') sprintf(temp,"%c",m_nowChoose + 1);
		else sprintf(temp,"%c",'A');
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,346 - tempMove);
		m_inputNameFont.draw();
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * (0.5f - 0.5f * sin(m_inputCharMove)));	//4 0.5 - 0
		if(m_nowChoose < 'Y') sprintf(temp,"%c",m_nowChoose + 2); else
		if(m_nowChoose < 'Z') sprintf(temp,"%c",'A'); else 
		sprintf(temp,"%c",'B');
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,406 - tempMove);
		m_inputNameFont.draw();
	}else
	if(m_inputCharFlag == 1)
	{//左边移动
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * (0.5f - 0.5f * sin(m_inputCharMove)));	//1 0.5 - 0
		if(m_nowChoose > 'B') sprintf(temp,"%c",m_nowChoose - 2); else
		if(m_nowChoose > 'A') sprintf(temp,"%c",'Z'); else
		sprintf(temp,"%c",'Y');
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,286 - tempMove);
		m_inputNameFont.draw();
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * (1.0f - 0.5f * sin(m_inputCharMove)));	//2 1 - 0.5
		if(m_nowChoose > 'A') sprintf(temp,"%c",m_nowChoose - 1);
		else sprintf(temp,"%c",'Z');
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,346 - tempMove);
		m_inputNameFont.draw();
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * (0.5f + 0.5f * sin(m_inputCharMove)));	//3 0.5 - 1
		sprintf(temp,"%c",m_nowChoose);
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,406 - tempMove);
		m_inputNameFont.draw();
		m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,alpha * (0.0f + 0.5f * sin(m_inputCharMove)));	//4 0 - 0.5
		if(m_nowChoose < 'Z') sprintf(temp,"%c",m_nowChoose + 1);
		else sprintf(temp,"%c",'A');
		m_inputNameFont.setString(temp);
		m_inputNameFont.setPosition(238,466 - tempMove);
		m_inputNameFont.draw();
	}
}

void _XGun::inputNameDraw()
{
	if(m_gameStage == GAME_STAGE_GAMEINPUTNAME && m_gameCounter > 0)
	{
		char temp[20];
		m_gameInputNameBG.draw();
		if(m_gameCounter == 2)
		{
			if(m_inputAlpha <= 1.0f)
			{//显示"请输入"
				m_gameOrderFontInput.setColor(-1.0f,-1.0f,-1.0f,m_inputAlpha);
			}
			if(m_inputAlpha > 1.0f && m_inputAlpha <= 2.0f)
			{//显示三个字母
				drawChar(m_inputAlpha - 1.0f);
			}else
			if(m_inputAlpha > 2.0f && m_inputAlpha <= 3.0f)
			{
				drawChar(1.0f);
				m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,m_inputAlpha - 2.0f);
				m_inputNameFont.setString(m_tempName);
				m_inputNameFont.setPosition(184,300);
				m_inputNameFont.draw();
			}else
			if(m_inputAlpha > 3.0f)
			{
				drawChar(1.0f);
				m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,1.0f);
				m_inputNameFont.setString(m_tempName);
				m_inputNameFont.setPosition(184,300);
				m_inputNameFont.draw();
				m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,m_inputAlpha - 3.0f);
				sprintf(temp,"%2d",m_inputNameTimer/1000);
				m_inputNameFont.setString(temp);
				m_inputNameFont.setPosition(339,336);
				m_inputNameFont.draw();
			}
			m_gameOrderFontInput.draw();
		}else
		if(m_gameCounter == 3)
		{
			drawChar(1.0f);
			m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			if(m_inputGlare < 500)
			{
				strcpy(temp,m_tempName);
				temp[m_inputNamePosition] = '_';
				m_inputNameFont.setString(temp);
			}else
			{
				m_inputNameFont.setString(m_tempName);
			}
			m_inputNameFont.setPosition(184,300);
			m_inputNameFont.draw();
			if(m_inputNameTimer < 10000)
			{//使用红色作为警告
				m_inputNameFont.setColor(1.0f,0.0f,0.0f,1.0f);
			}else
			{
				m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			}
			sprintf(temp,"%2d",m_inputNameTimer/1000);
			m_inputNameFont.setString(temp);
			m_inputNameFont.setPosition(339,336);
			m_inputNameFont.draw();
			m_inputNameFont.setColor(1.0f,1.0f,1.0f,1.0f);
			m_gameOrderFontInput.draw();
		}else
		if(m_gameCounter >= 4 && m_gameCounter < 9)
		{
			drawChar(1.0f);
			m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			m_inputNameFont.setString(m_tempName);
			m_inputNameFont.setPosition(184,300);
			m_inputNameFont.draw();
			if(m_inputNameTimer < 10000)
			{//使用红色作为警告
				m_inputNameFont.setColor(1.0f,0.0f,0.0f,1.0f);
			}else
			{
				m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			}
			sprintf(temp,"%2d",m_inputNameTimer/1000);
			m_inputNameFont.setString(temp);
			m_inputNameFont.setPosition(339,336);
			m_inputNameFont.draw();
			m_inputNameFont.setColor(1.0f,1.0f,1.0f,1.0f);
			m_gameOrderFontInput.draw();
		}else
		if(m_gameCounter >= 9)
		{
			drawChar(1.0f);
			m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			m_inputNameFont.setString(m_tempName);
			m_inputNameFont.setPosition(184,300 - 600 * sin(m_inputNameY));
			m_inputNameFont.draw();
			if(m_inputNameTimer < 10000)
			{//使用红色作为警告
				m_inputNameFont.setColor(1.0f,0.0f,0.0f,1.0f);
			}else
			{
				m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			}
			sprintf(temp,"%2d",m_inputNameTimer/1000);
			m_inputNameFont.setString(temp);
			m_inputNameFont.setPosition(339,336 - 600 * sin(m_inputNameY));
			m_inputNameFont.draw();
			m_inputNameFont.setColor(1.0f,1.0f,1.0f,1.0f);
			m_gameOrderFontInput.draw();
		}
		if(m_isCameraSupply != 0)
		{
			m_camera.draw();
			if(m_gameCounter >= 4)
			{
				m_takePhotoTex00.draw();
				m_takePhotoTex01.draw();
				m_takePhotoTex02.draw();
				m_takePhotoTex03.draw();
			}
			m_cameraBG.draw();
		}
	}
}

void _XGun::inputName(INPUT_NAME_KEY key)
{
	if(m_gameStage == GAME_STAGE_GAMEINPUTNAME && m_gameCounter == 3)
	{
		if(key == INPUT_KEY_ENTER)
		{
			if(m_inputNamePosition < MAX_GAME_ORDER_NAME_LENGTH - 1)
			{//输入
				m_tempName[m_inputNamePosition] = m_nowChoose;
				m_inputNamePosition ++;
				if(m_inputNamePosition >= MAX_GAME_ORDER_NAME_LENGTH - 1)
				{
					m_gameCounter = 4;
				}
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{//输入完成
				m_gameCounter = 4;
			}
		}else
		if(key == INPUT_KEY_LEFT && m_inputCharFlag == 0)
		{
			m_nowChoose ++;
			m_inputCharMove = 0.0f;
			m_inputCharFlag = 1;
			if(m_nowChoose > 'Z') m_nowChoose = 'A';
			_XSound::GetInstance().play(m_soundTypeMove);
		}else
		if(key == INPUT_KEY_RIGHT && m_inputCharFlag == 0)
		{
			m_nowChoose --;
			m_inputCharMove = 0.0f;
			m_inputCharFlag = -1;
			if(m_nowChoose < 'A') m_nowChoose = 'Z';
			_XSound::GetInstance().play(m_soundTypeMove);
		}
	}
}

void _XGun::inPutNameMove(int interval)
{
	if(m_gameStage == GAME_STAGE_GAMEINPUTNAME)
	{
		if(m_isCameraSupply != 0)
		{
			m_camera.upDateFrame();
		}
		if(m_gameCounter == 0)
		{//初始化
			if(m_isCameraSupply != 0)
			{
				m_camera.setWork();
				m_camera.m_cameraSprite.setPosition(388.0f,1056.0f);
				m_cameraBG.setPosition(388.0f - 96.0f,920.0f);
			}

			m_inputNameY = 0.0f;
			m_inputAlpha = 0.0f;
			m_gameInputNameBG.setPosition(100.0f,920.0f);
			m_inputNameTimer = 60000;
			m_gameCounter = 1;
			_XSound::GetInstance().play(m_soundTextMove_01);
			m_inputNameFont.setColor(-1.0f,-1.0f,-1.0f,m_inputAlpha);
			m_gameOrderFontInput.setPosition(296,234);
			m_gameOrderFontInput.setColor(-1.0f,-1.0f,-1.0f,m_inputAlpha);
			m_nowChoose = 'A';
			m_inputNamePosition = 0;
			for(int i = 0;i < MAX_GAME_ORDER_NAME_LENGTH - 1;++ i)
			{
				m_tempName[i] = 'A';
			}
			m_tempName[MAX_GAME_ORDER_NAME_LENGTH - 1] = '\0';
			m_inputCharFlag = 0;
			//插入名次
#if DEBUG00
			printf("Move record data to up!\n");
#endif
			if(m_gameNowOrder < MAX_GAME_ORDER_SUM && m_gameNowOrder >= 0)
			{
				for(int i = 0;i < MAX_GAME_ORDER_SUM - m_gameNowOrder - 1;++ i)
				{
					m_gameOrderScores[i] = m_gameOrderScores[i + 1];
					strcpy(m_gameOrderName[i],m_gameOrderName[i + 1]);
					if(m_isCameraSupply != 0)
					{
						memcpy(m_gameOrderCameraData + i * MAX_GAME_ORDER_CAMERA_DATA,m_gameOrderCameraData + (i + 1) * MAX_GAME_ORDER_CAMERA_DATA,MAX_GAME_ORDER_CAMERA_DATA);
						updataHeadTex(i);
					}
				}
				m_gameOrderScores[MAX_GAME_ORDER_SUM - 1 - m_gameNowOrder] = m_scoresArmSum;
				strcpy(m_gameOrderName[MAX_GAME_ORDER_SUM - 1 - m_gameNowOrder],"AAAAAA");
				if(m_isCameraSupply != 0)
				{
					memset(m_gameOrderCameraData + (MAX_GAME_ORDER_SUM - 1 - m_gameNowOrder) * MAX_GAME_ORDER_CAMERA_DATA,0,MAX_GAME_ORDER_CAMERA_DATA);
					//updataHeadTex(MAX_GAME_ORDER_SUM - 1 - m_gameNowOrder);
				}
			}
		}else
		if(m_gameCounter == 1)
		{//背景出现
			m_inputNameY += 0.001f * interval;
			if(m_inputNameY >= PI * 0.5f)
			{
				m_inputNameY = PI * 0.5f;
				m_gameCounter = 2;
			}
			float tempSin = sin(m_inputNameY);
			m_gameInputNameBG.setPosition(100.0f,1136.0f - (1136.0f - 320.0f) * tempSin);
			if(m_isCameraSupply != 0)
			{
				m_camera.m_cameraSprite.setPosition(388,1056.0f - (1056.0f - 240.0f) * tempSin);
				m_cameraBG.setPosition(388 - 96,920.0f - (920.0f - 104.0f) * tempSin);
			}
		}else
		if(m_gameCounter == 2)
		{//提示文字出现
			m_inputAlpha += 0.005f * interval;
			if(m_inputAlpha > 4.0f)
			{
				m_inputAlpha = 3.0f;
				m_inputGlare = 0;
				m_gameCounter = 3;
			}
		}else
		if(m_gameCounter == 3)
		{//等待输入
			m_inputNameTimer -= interval;
			if(m_inputNameTimer <= 0)
			{
				m_inputNameTimer = 0;
				m_gameCounter = 4;	//时间到
			}
			m_inputGlare += interval;
			if(m_inputGlare > 1000) m_inputGlare -= 1000;

			if(m_inputCharFlag != 0)
			{
				m_inputCharMove += 0.005f * interval;
				if(m_inputCharMove >= PI * 0.5f)
				{
					m_inputCharMove = PI * 0.5f;
					m_inputCharFlag = 0;
				}
			}
		}else
		if(m_gameCounter == 4)
		{//准备显示准备拍照
			if(m_isCameraSupply != 0)
			{
				m_takePhotoChange = 0.0f;
				m_takePhotoTex00.setAlpha(0.0f);
				m_takePhotoTex00.setSize(0.1f,0.1f);
				m_takePhotoTex01.setAlpha(0.0f);
				m_takePhotoTex01.setSize(0.1f,0.1f);
				m_takePhotoTex02.setAlpha(0.0f);
				m_takePhotoTex02.setSize(0.1f,0.1f);
				m_takePhotoTex03.setAlpha(0.0f);
				//m_takePhotoTex03.setSize(0.1f,0.1f);
				m_gameCounter = 5;
			}else
			{//跳过拍照界面
				m_gameCounter = 8;
			}
		}else
		if(m_gameCounter == 5)
		{
			m_takePhotoChange += 0.0005f * interval;
			float temp = 0.0f;
			if(m_takePhotoChange <= 0.5f)
			{
				temp = sin(m_takePhotoChange * PI2) * 2.0f;
				if(temp > 1.0f) temp = 1.0f;
				m_takePhotoTex02.setAlpha(temp);
				temp =  0.5f + sin(m_takePhotoChange * PI2) * 0.5f;
				m_takePhotoTex02.setSize(temp,temp);
			}else
			if(m_takePhotoChange > 1.0f && m_takePhotoChange <= 1.5f)
			{
				m_takePhotoTex02.setAlpha(0.0f);
				temp = sin((m_takePhotoChange - 1.0f) * PI2) * 2.0f;
				if(temp > 1.0f) temp = 1.0f;
				m_takePhotoTex01.setAlpha(temp);
				temp =  0.5f + sin((m_takePhotoChange - 1.0f) * PI2) * 0.5f;
				m_takePhotoTex01.setSize(temp,temp);
			}else
			if(m_takePhotoChange > 2.0f && m_takePhotoChange <= 2.5f)
			{
				m_takePhotoTex01.setAlpha(0.0f);
				temp = sin((m_takePhotoChange - 2.0f) * PI2) * 2.0f;
				if(temp > 1.0f) temp = 1.0f;
				m_takePhotoTex00.setAlpha(temp);
				temp =  0.5f + sin((m_takePhotoChange - 2.0f) * PI2) * 0.5f;
				m_takePhotoTex00.setSize(temp,temp);
			}else
			{
				m_takePhotoTex00.setAlpha(0.0f);
			}

			if(m_takePhotoChange >= 3.0f)
			{
				m_camera.setStop();
				m_takePhotoChange = 0.0f;
				m_gameCounter = 6;
			}
		}else
		if(m_gameCounter == 6)
		{
			m_takePhotoChange += 0.0005f * interval;
			if(m_takePhotoChange <= 1.0f)
			{
				m_takePhotoTex03.setAlpha(cos(m_takePhotoChange * PI_HALF));
			}
			if(m_takePhotoChange >= 1.0f)
			{
				m_takePhotoChange = 0.0f;
				m_gameCounter = 7;
			}
		}else
		if(m_gameCounter == 7)
		{
			m_takePhotoChange += interval;
			if(m_takePhotoChange > 1500)
			{//停顿1.5s
				m_gameCounter = 8;
			}
		}else
		if(m_gameCounter == 8)
		{//输入完成
#if DEBUG00
			printf("Update date after input name！\n");
#endif
			if(m_gameNowOrder < MAX_GAME_ORDER_SUM && m_gameNowOrder >= 0)
			{
				strcpy(m_gameOrderName[MAX_GAME_ORDER_SUM - 1 - m_gameNowOrder],m_tempName);	//将输入的名字放入
				if(writeRecordData() == 0) printf("数据写入失败！\n");
				if(m_isCameraSupply != 0)
				{
					m_camera.getData((unsigned char *)(m_gameOrderCameraData + (MAX_GAME_ORDER_SUM - 1 - m_gameNowOrder) * MAX_GAME_ORDER_CAMERA_DATA));
					updataHeadTex(MAX_GAME_ORDER_SUM - 1 - m_gameNowOrder);
					if(writeHeadData() == 0) printf("数据写入失败！\n");
				}
			}
			if(m_isCameraSupply != 0)
			{
				m_takePhotoPoint ++;
				if(m_takePhotoPoint >= USER_HEAD_SUM) m_takePhotoPoint = 0;
				strcpy(m_gameOrderUserHeadName[m_takePhotoPoint],m_tempName);	//将输入的名字放入
				m_camera.getData((unsigned char *)(m_gameOrderUserHeadData + m_takePhotoPoint * MAX_GAME_ORDER_CAMERA_DATA));
				updataUserHeadTex(m_takePhotoPoint);
				if(writeUserData() == 0) printf("数据写入失败！\n");
			}
			m_gameCounter = 9;
		}else
		if(m_gameCounter == 9)
		{//所有元素退场
			m_gameCounter = 10;
			m_inputNameY = 0.0f;
			_XSound::GetInstance().play(m_soundTextMove_00);
		}else
		if(m_gameCounter == 10)
		{
			m_inputNameY += 0.0025f * interval;
			if(m_inputNameY >= PI * 0.5f)
			{
				m_inputNameY = PI * 0.5f;
				//退场完成
				if(m_configData.isTicketNeed == 0 || m_releaseArmTicket == 0)
				{
					m_gameStage = GAME_STAGE_GAMESHOWORDER;
					m_gameCounter = 0;
					m_orderShowFlag = 0;
				}
			}
			float tempSin = sin(m_inputNameY);
			m_gameOrderFontInput.setPosition(296,234 - 800.0f * tempSin);
			m_gameInputNameBG.setPosition(100.0f,320 - 886.0f * tempSin);
			if(m_isCameraSupply != 0)
			{
				m_camera.m_cameraSprite.setPosition(388,240.0f - 806.0f * tempSin);
				m_cameraBG.setPosition(388 - 96,104.0f - 670.0f * tempSin);
			}
		}
	}
}

int _XGun::getNowOrder()
{
	for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
	{
		if(m_scoresArmSum < m_gameOrderScores[i]) 
		{
			m_gameNowOrder = MAX_GAME_ORDER_SUM - i;	//(1 - MAX_GAME_ORDER_SUM)
			m_gameOrderNextNeedScores = m_gameOrderScores[i] - m_scoresArmSum;
			return 1;
		}
	}
	m_gameNowOrder = 0;	//第一名
	m_gameOrderNextNeedScores = 0;
	return 1;
}

int _XGun::orderShowInit(_XResourcePosition resoursePosition)
{
	//if(m_orderShowBG.init("normalResource/UI/GameOrderBG.jpg",resoursePosition) == 0) return 0;	//大背景，小背景
	if(m_orderShowBG.init("ResourcePack/pic/UIBG/GameOrderBG.png",resoursePosition) == 0) return 0;	//大背景，小背景
	m_orderShowBG.setPosition(0.0f,0.0f);
	if(m_orderShowBG1.init("ResourcePack/pic/UI/GameOrderBG1.png",resoursePosition) == 0) return 0;	//大背景，小背景
	m_orderShowBG1.setAngle(90);
	if(m_orderShowBG2.init("ResourcePack/pic/UI/GameOrderBG2.png",resoursePosition) == 0) return 0;	//大背景，小背景
	m_orderShowBG2.setAngle(90);
	if(m_orderShowText.init("ResourcePack/pic/UI/GameOrder.png",resoursePosition) == 0) return 0;	//大背景，小背景
	m_orderShowText.setAngle(90);
	if(m_orderShowNum.init("ResourcePack/pic/UI/GameOrderNumber.png",_XVector2(45,64),_XVector2(10,1),resoursePosition) == 0) return 0;
	m_orderShowNum.setAngle(90);
	m_orderShowNum.setDistance(-9.0f);
	return 1;
}

void _XGun::orderShowDraw()
{
	if(m_gameStage == GAME_STAGE_GAMESHOWORDER)
	{
		if(m_orderShowFlag != 0)
		{
			m_gameDemoBG.drawWithoutBlend();
			m_gameDemoText1.draw();
			m_gameDemoText2.draw();
		}
		if(m_gameCounter > 0)
		{
			m_orderShowBG.drawWithoutBlend();
			if(m_gameCounter == 3)
			{
				int tempSum = 0;
				for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
				{
					if(m_orderShowBGY > i * PI * 0.25f && m_orderShowBGY <= i * PI * 0.25f + PI * 0.5f)
					{//移动中
						if(m_gameNowOrder != i)
						{
							showOrder(1020.0f - 100.0f * i,720.0f - (720.0f - 10.0f) * sin(m_orderShowBGY - i * PI * 0.25f) ,i,0);
						}else
						{
							showOrder(1020.0f - 100.0f * i,720.0f - (720.0f - 10.0f) * sin(m_orderShowBGY - i * PI * 0.25f) ,i,1);
						}
						tempSum = i;
					}else
					if(m_orderShowBGY >= i * PI * 0.25f + PI * 0.5f)
					{//移动完成
						if(m_gameNowOrder != i)
						{
							showOrder(1020.0f - 100.0f * i,10.0f,i,0);
						}else
						{
							showOrder(1020.0f - 100.0f * i,10.0f,i,1);
						}
					}
				//	else
				//	if(m_orderShowBGY <= i * PI * 0.5f)
				//	{//尚未移动
				//	}
				}
				if(tempSum != m_soundShowOrderTemp)
				{
					m_soundShowOrderTemp = tempSum;
					_XSound::GetInstance().play(m_soundTextMove_01);
				}
			}else
			if(m_gameCounter > 3 && m_gameCounter < 6)
			{
				for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
				{
					if(m_gameNowOrder != i)
					{
						showOrder(1020.0f - 100.0f * i,10.0f,i,0);
					}else
					{
						showOrder(1020.0f - 100.0f * i,10.0f,i,1);
					}
				}
			}
		}
		if(m_releaseTicket > 0) ticketDraw();
	}
	if(m_upGameStage == GAME_STAGE_GAMESHOWORDER && m_gameStage == GAME_STAGE_GAMELOGO && m_gameCounter <= 2)
	{//输入名称之后这个就没有问题，如果是直接over这里就会有问题
		m_orderShowBG.drawWithoutBlend();
		for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
		{
			if(m_gameNowOrder != i)
			{
				showOrder(1020.0f - 100.0f * i,10.0f,i,0);
			}else
			{
				showOrder(1020.0f - 100.0f * i,10.0f,i,1);
			}
		}
	}
}

void _XGun::orderShowMove(int interval)
{
	if(m_gameStage == GAME_STAGE_GAMESHOWORDER)
	{
		if(m_gameCounter == 0)
		{//初始化
			m_orderShowBGY = 0;
			m_orderShowBG.setPosition(1280.0f,0.0f);
			m_inputNameFont.setColor(1.0f,1.0f,1.0f,1.0f);
			m_gameCounter = 1;
			_XSound::GetInstance().play(m_soundScreenDown);
		}else
		if(m_gameCounter == 1)
		{//背景落下
			m_orderShowBGY += 0.002f * interval;
			if(m_orderShowBGY >= PI * 0.5f)
			{
				m_orderShowBGY = PI * 0.5f;
				m_gameCounter = 2;
			}
			m_orderShowBG.setPosition(1280.0f * cos(m_orderShowBGY),0.0f);
		}else
		if(m_gameCounter == 2)
		{//名次依次出现
			m_orderShowBGY = 0.0f;
			m_gameCounter = 3;
			m_soundShowOrderTemp = 0;
		}else
		if(m_gameCounter == 3)
		{//名次依次出现
			m_orderShowBGY += 0.002f * interval;
			if(m_orderShowBGY >= PI * 0.5f * MAX_GAME_ORDER_SUM)
			{
				m_orderShowBGY = PI * 0.5f * MAX_GAME_ORDER_SUM;
				m_gameCounter = 4;
			}
		}else
		if(m_gameCounter == 4)
		{//停顿一段时间
			m_orderShowBGY = 0.0f;
			m_gameCounter = 5;
		}else
		if(m_gameCounter == 5)
		{//停顿一段时间
			m_orderShowBGY += interval;
			if(m_orderShowBGY >= 5000 || ((m_creditSum >= m_configData.coinToCredit || m_energyArmSum > 0.0f) && m_orderShowFlag != 0))
			{//准备谢幕
				m_orderShowBGY = 0.0f;
				m_gameStage = GAME_STAGE_GAMELOGO;
				m_upGameStage = GAME_STAGE_GAMESHOWORDER;
				m_gameCounter = 0;
			}
		}
	}
}

void _XGun::showOrder(int x,int y,int order,int flag)
{
	if(order < 0 || order >= MAX_GAME_ORDER_SUM) return;
	m_cameraHead.setPosition(x + 65.0f - 172.0f,y + 465.0f - 173.0f);
	m_cameraHead.setSize(0.3f,0.3f);
	m_cameraHead.draw(&(m_cameraRecordTex[MAX_GAME_ORDER_SUM - 1 - order]));
	if(flag == 1)
	{
		m_orderShowBG1.setPosition(x - 463.0f,y + 438.0f);
		m_orderShowBG1.draw();
	}else
	{
		m_orderShowBG2.setPosition(x - 463.0f,y + 438.0f);
		m_orderShowBG2.draw();
	}
//	m_orderShowText.setPosition(x - 72.0f,y + 50.0f);
//	m_orderShowText.draw();
	m_orderShowNum.setNumber(order + 1);
	if(order < 9)
	{
		m_orderShowNum.setPosition(x + 85.0f,y + 40.0f);
	}else
	{
		m_orderShowNum.setPosition(x + 85.0f,y + 40.0f - 20.0f);
	}
	m_orderShowNum.draw();
	m_orderShowNum.setNumber(m_gameOrderScores[MAX_GAME_ORDER_SUM - 1 - order]);
	m_orderShowNum.setPosition(x + 85.0f,y + 115.0f);
	m_orderShowNum.draw();
	m_inputNameFont.setString(m_gameOrderName[MAX_GAME_ORDER_SUM - 1 - order]);
	m_inputNameFont.setPosition(x + 65.0f,y + 475.0f);
	m_inputNameFont.draw();
}

int _XGun::weaponInfoInit(_XResourcePosition resoursePosition)
{
	if(m_weaponInfoBG_00.init("ResourcePack/pic/UI/weapon_00.png",resoursePosition) == 0) return 0;
	m_weaponInfoBG_00.setAngle(90);
	m_weaponInfoBG_00.setPosition(874.0f,300.0f);
	m_weaponInfoBG_10.setACopy(m_weaponInfoBG_00);
	m_weaponInfoBG_10.setAngle(90);
	m_weaponInfoBG_10.setPosition(874.0f,300.0f);
	if(m_weaponInfoBG_01.init("ResourcePack/pic/UI/weapon_01.png",resoursePosition) == 0) return 0;
	m_weaponInfoBG_01.setAngle(90);
	m_weaponInfoBG_01.setPosition(0.0f,15.0f);
	m_weaponInfoBG_02.setACopy(m_weaponInfoBG_01);
	m_weaponInfoBG_02.setPosition(70.0f,15.0f);
	m_weaponInfoBG_03.setACopy(m_weaponInfoBG_01);
	m_weaponInfoBG_03.setPosition(140.0f,15.0f);
	m_weaponInfoBG_04.setACopy(m_weaponInfoBG_01);
	m_weaponInfoBG_04.setPosition(210.0f,15.0f);
	m_weaponInfoBG_05.setACopy(m_weaponInfoBG_01);
	m_weaponInfoBG_05.setPosition(280.0f,15.0f);
	m_weaponInfoBG_06.setACopy(m_weaponInfoBG_01);
	m_weaponInfoBG_06.setPosition(350.0f,15.0f);
	if(m_weaponInfoLK_00.init("ResourcePack/pic/UI/weapon_lock.png",resoursePosition) == 0) return 0;
	m_weaponInfoLK_00.setAngle(90);
	m_weaponInfoLK_00.setPosition(0.0f + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_01.setACopy(m_weaponInfoLK_00);
	m_weaponInfoLK_01.setPosition(70.0f + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_02.setACopy(m_weaponInfoLK_00);
	m_weaponInfoLK_02.setPosition(140.0f + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_03.setACopy(m_weaponInfoLK_00);
	m_weaponInfoLK_03.setPosition(210.0f + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_04.setACopy(m_weaponInfoLK_00);
	m_weaponInfoLK_04.setPosition(280.0f + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_05.setACopy(m_weaponInfoLK_00);
	m_weaponInfoLK_05.setPosition(350.0f + 16.0f,15.0f + 16.0f);

	if(m_weaponInfoLG_00.init("ResourcePack/pic/UI/weapon_02_00.png",resoursePosition) == 0) return 0;
	m_weaponInfoLG_00.setAngle(90);
	m_weaponInfoLG_00.setPosition(16.0f,15.0f);
	if(m_weaponInfoLG_01.init("ResourcePack/pic/UI/weapon_02_01.png",resoursePosition) == 0) return 0;
	m_weaponInfoLG_01.setAngle(90);
	m_weaponInfoLG_01.setPosition(86.0f,15.0f);
	if(m_weaponInfoLG_02.init("ResourcePack/pic/UI/weapon_02_02.png",resoursePosition) == 0) return 0;
	m_weaponInfoLG_02.setAngle(90);
	m_weaponInfoLG_02.setPosition(156.0f,15.0f);
	if(m_weaponInfoLG_03.init("ResourcePack/pic/UI/weapon_02_03.png",resoursePosition) == 0) return 0;
	m_weaponInfoLG_03.setAngle(90);
	m_weaponInfoLG_03.setPosition(226.0f,15.0f);
	if(m_weaponInfoLG_04.init("ResourcePack/pic/UI/weapon_02_04.png",resoursePosition) == 0) return 0;
	m_weaponInfoLG_04.setAngle(90);
	m_weaponInfoLG_04.setPosition(296.0f,15.0f);
	if(m_weaponInfoLG_05.init("ResourcePack/pic/UI/weapon_02_05.png",resoursePosition) == 0) return 0;
	m_weaponInfoLG_05.setAngle(90);
	m_weaponInfoLG_05.setPosition(366.0f,15.0f);

	m_weaponInfoMoveData.setArmNumber(0.0f);

	m_upChooseGunType = GUN_TYPE_NULL;
	m_upCanUseGunType = GUN_TYPE_NULL;
	m_chooseStage = 0;
	m_canUseGunChangeStage = 0;
	return 1;
}

void _XGun::weaponInfoDraw()
{
	if(m_chooseStage == 0)
	{
		if(m_upChooseGunType != GUN_TYPE_00) m_weaponInfoBG_01.draw();
		if(m_upChooseGunType != GUN_TYPE_01) m_weaponInfoBG_02.draw();
		if(m_upChooseGunType != GUN_TYPE_02) m_weaponInfoBG_03.draw();
		if(m_upChooseGunType != GUN_TYPE_03) m_weaponInfoBG_04.draw();
		if(m_upChooseGunType != GUN_TYPE_04) m_weaponInfoBG_05.draw();
		if(m_upChooseGunType != GUN_TYPE_05) m_weaponInfoBG_06.draw();
		m_weaponInfoBG_00.draw();
	}else
	{
		m_weaponInfoBG_00.draw();
		m_weaponInfoBG_01.draw();
		m_weaponInfoBG_02.draw();
		m_weaponInfoBG_03.draw();
		m_weaponInfoBG_04.draw();
		m_weaponInfoBG_05.draw();
		m_weaponInfoBG_06.draw();
		m_weaponInfoBG_10.draw();
	}

	m_weaponInfoLG_00.draw();
	m_weaponInfoLG_01.draw();
	m_weaponInfoLG_02.draw();
	m_weaponInfoLG_03.draw();
	m_weaponInfoLG_04.draw();
	m_weaponInfoLG_05.draw();

	//if(m_canUseGunChangeStage == 0)
	//{
	//	if(m_canUserGunType < GUN_TYPE_00) m_weaponInfoLK_00.draw();
	//	if(m_canUserGunType < GUN_TYPE_01) m_weaponInfoLK_01.draw();
	//	if(m_canUserGunType < GUN_TYPE_02) m_weaponInfoLK_02.draw();
	//	if(m_canUserGunType < GUN_TYPE_03) m_weaponInfoLK_03.draw();
	//	if(m_canUserGunType < GUN_TYPE_04) m_weaponInfoLK_04.draw();
	//	if(m_canUserGunType < GUN_TYPE_05) m_weaponInfoLK_05.draw();
	//}else
	{
		if(m_upCanUseGunType < GUN_TYPE_00) 
		{
			m_weaponInfoLK_00.setBlendType(4,5);
			m_weaponInfoLK_00.draw();
			if(m_weaponInfoTimer < 200)
			{
				m_weaponInfoLK_00.setBlendType(4,1);
				m_weaponInfoLK_00.draw();
			}
		}
		if(m_upCanUseGunType < GUN_TYPE_01) 
		{
			m_weaponInfoLK_01.setBlendType(4,5);
			m_weaponInfoLK_01.draw();
			if(m_weaponInfoTimer < 200)
			{
				m_weaponInfoLK_01.setBlendType(4,1);
				m_weaponInfoLK_01.draw();
			}
		}
		if(m_upCanUseGunType < GUN_TYPE_02) 
		{
			m_weaponInfoLK_02.setBlendType(4,5);
			m_weaponInfoLK_02.draw();
			if(m_weaponInfoTimer < 200)
			{
				m_weaponInfoLK_02.setBlendType(4,1);
				m_weaponInfoLK_02.draw();
			}
		}
		if(m_upCanUseGunType < GUN_TYPE_03) 
		{
			m_weaponInfoLK_03.setBlendType(4,5);
			m_weaponInfoLK_03.draw();
			if(m_weaponInfoTimer < 200)
			{
				m_weaponInfoLK_03.setBlendType(4,1);
				m_weaponInfoLK_03.draw();
			}
		}
		if(m_upCanUseGunType < GUN_TYPE_04) 
		{
			m_weaponInfoLK_04.setBlendType(4,5);
			m_weaponInfoLK_04.draw();
			if(m_weaponInfoTimer < 200)
			{
				m_weaponInfoLK_04.setBlendType(4,1);
				m_weaponInfoLK_04.draw();
			}
		}
		if(m_upCanUseGunType < GUN_TYPE_05) 
		{
			m_weaponInfoLK_05.setBlendType(4,5);
			m_weaponInfoLK_05.draw();
			if(m_weaponInfoTimer < 200)
			{
				m_weaponInfoLK_05.setBlendType(4,1);
				m_weaponInfoLK_05.draw();
			}
		}
	}
}

void _XGun::updataWeaponInfo()
{
	m_weaponInfoBG_00.setPosition(m_weaponInfoNowPos - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoBG_10.setPosition(m_weaponInfoUpPos - m_weaponInfoMoveData.getNowNumber(),15.0f);

	m_weaponInfoBG_01.setPosition(0.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoBG_02.setPosition(70.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoBG_03.setPosition(140.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoBG_04.setPosition(210.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoBG_05.setPosition(280.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoBG_06.setPosition(350.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);

	m_weaponInfoLK_00.setPosition(0.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_01.setPosition(70.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_02.setPosition(140.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_03.setPosition(210.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_04.setPosition(280.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f);
	m_weaponInfoLK_05.setPosition(350.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f);

	m_weaponInfoLG_00.setPosition(16.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoLG_01.setPosition(86.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoLG_02.setPosition(156.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoLG_03.setPosition(226.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoLG_04.setPosition(296.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);
	m_weaponInfoLG_05.setPosition(366.0f - m_weaponInfoMoveData.getNowNumber(),15.0f);

	float temp = 1.0f - (m_weaponInfoMoveData.getNowNumber() - 0.0f) / 70.0f * 0.5f;
	if(temp < 0.0f) temp = 0.0f;
	m_weaponInfoBG_01.setAlpha(temp);
	m_weaponInfoLK_00.setAlpha(temp);
	m_weaponInfoLG_00.setAlpha(temp);
	temp = 1.0f - abs(m_weaponInfoMoveData.getNowNumber() - 70.0f) / 70.0f * 0.5f;
	if(temp < 0.0f) temp = 0.0f;
	m_weaponInfoBG_02.setAlpha(temp);
	m_weaponInfoLK_01.setAlpha(temp);
	m_weaponInfoLG_01.setAlpha(temp);
	temp = 1.0f - abs(m_weaponInfoMoveData.getNowNumber() - 140.0f) / 70.0f * 0.5f;
	if(temp < 0.0f) temp = 0.0f;
	m_weaponInfoBG_03.setAlpha(temp);
	m_weaponInfoLK_02.setAlpha(temp);
	m_weaponInfoLG_02.setAlpha(temp);
	temp = 1.0f - abs(m_weaponInfoMoveData.getNowNumber() - 210.0f) / 70.0f * 0.5f;
	if(temp < 0.0f) temp = 0.0f;
	m_weaponInfoBG_04.setAlpha(temp);
	m_weaponInfoLK_03.setAlpha(temp);
	m_weaponInfoLG_03.setAlpha(temp);
	temp = 1.0f - abs(m_weaponInfoMoveData.getNowNumber() - 280.0f) / 70.0f * 0.5f;
	if(temp < 0.0f) temp = 0.0f;
	m_weaponInfoBG_05.setAlpha(temp);
	m_weaponInfoLK_04.setAlpha(temp);
	m_weaponInfoLG_04.setAlpha(temp);
	temp = 1.0f - abs(m_weaponInfoMoveData.getNowNumber() - 350.0f) / 70.0f * 0.5f;
	if(temp < 0.0f) temp = 0.0f;
	m_weaponInfoBG_06.setAlpha(temp);
	m_weaponInfoLK_05.setAlpha(temp);
	m_weaponInfoLG_05.setAlpha(temp);
}

void _XGun::waeponInfoMove(int interval)
{
	if(m_upChooseGunType != m_gunType && m_chooseStage == 0)
	{//改变了武器的选择
		m_nowChooseGunType = m_gunType;
		//m_weaponInfoBG_00.setPosition(0.0f,15.0f);
		//m_weaponInfoBG_10.setPosition(0.0f,15.0f);
		
		if(m_nowChooseGunType == GUN_TYPE_00) m_weaponInfoNowPos = 0.0f; else
		if(m_nowChooseGunType == GUN_TYPE_01) m_weaponInfoNowPos = 70.0f; else
		if(m_nowChooseGunType == GUN_TYPE_02) m_weaponInfoNowPos = 140.0f; else
		if(m_nowChooseGunType == GUN_TYPE_03) m_weaponInfoNowPos = 210.0f; else
		if(m_nowChooseGunType == GUN_TYPE_04) m_weaponInfoNowPos = 280.0f; else
		if(m_nowChooseGunType == GUN_TYPE_05) m_weaponInfoNowPos = 350.0f;
		if(m_upChooseGunType == GUN_TYPE_00) m_weaponInfoUpPos = 0.0f; else
		if(m_upChooseGunType == GUN_TYPE_01) m_weaponInfoUpPos = 70.0f; else
		if(m_upChooseGunType == GUN_TYPE_02) m_weaponInfoUpPos = 140.0f; else
		if(m_upChooseGunType == GUN_TYPE_03) m_weaponInfoUpPos = 210.0f; else
		if(m_upChooseGunType == GUN_TYPE_04) m_weaponInfoUpPos = 280.0f; else
		if(m_upChooseGunType == GUN_TYPE_05) m_weaponInfoUpPos = 350.0f;

		if(m_nowChooseGunType == GUN_TYPE_00) m_weaponInfoMoveData.setArmNumber(0.0f);
		if(m_nowChooseGunType == GUN_TYPE_01) m_weaponInfoMoveData.setArmNumber(70.0f);
		if(m_nowChooseGunType == GUN_TYPE_02) m_weaponInfoMoveData.setArmNumber(140.0f);
		if(m_nowChooseGunType == GUN_TYPE_03) m_weaponInfoMoveData.setArmNumber(210.0f);
		if(m_nowChooseGunType == GUN_TYPE_04) m_weaponInfoMoveData.setArmNumber(280.0f);
		if(m_nowChooseGunType == GUN_TYPE_05) m_weaponInfoMoveData.setArmNumber(350.0f);
		m_chooseSize = 0.0f;
		m_chooseStage = 1;

		updataWeaponInfo();
	}
	m_weaponInfoTimer += interval;
	if(m_weaponInfoTimer >= 400)
	{
		m_weaponInfoTimer = 0;
	}
	if(m_weaponInfoMoveData.move(interval) != 0)
	{
		updataWeaponInfo();
	}
	if(m_chooseStage == 1)
	{
		m_chooseSize += 0.005f * interval;
		if(m_chooseSize >= 1.0f)
		{
			m_chooseSize = 1.0f;
			m_upChooseGunType = m_nowChooseGunType;
			m_chooseStage = 0;
		}
		m_weaponInfoBG_10.setSize(1.0f - m_chooseSize,1.0f - m_chooseSize);
		m_weaponInfoBG_00.setSize(m_chooseSize,m_chooseSize);
		if(m_nowChooseGunType == GUN_TYPE_00) m_weaponInfoBG_01.setSize(1.0f - m_chooseSize,1.0f - m_chooseSize); else
		if(m_nowChooseGunType == GUN_TYPE_01) m_weaponInfoBG_02.setSize(1.0f - m_chooseSize,1.0f - m_chooseSize); else
		if(m_nowChooseGunType == GUN_TYPE_02) m_weaponInfoBG_03.setSize(1.0f - m_chooseSize,1.0f - m_chooseSize); else
		if(m_nowChooseGunType == GUN_TYPE_03) m_weaponInfoBG_04.setSize(1.0f - m_chooseSize,1.0f - m_chooseSize); else
		if(m_nowChooseGunType == GUN_TYPE_04) m_weaponInfoBG_05.setSize(1.0f - m_chooseSize,1.0f - m_chooseSize); else
		if(m_nowChooseGunType == GUN_TYPE_05) m_weaponInfoBG_06.setSize(1.0f - m_chooseSize,1.0f - m_chooseSize);
		if(m_upChooseGunType == GUN_TYPE_00) m_weaponInfoBG_01.setSize(m_chooseSize,m_chooseSize); else
		if(m_upChooseGunType == GUN_TYPE_01) m_weaponInfoBG_02.setSize(m_chooseSize,m_chooseSize); else
		if(m_upChooseGunType == GUN_TYPE_02) m_weaponInfoBG_03.setSize(m_chooseSize,m_chooseSize); else
		if(m_upChooseGunType == GUN_TYPE_03) m_weaponInfoBG_04.setSize(m_chooseSize,m_chooseSize); else
		if(m_upChooseGunType == GUN_TYPE_04) m_weaponInfoBG_05.setSize(m_chooseSize,m_chooseSize); else
		if(m_upChooseGunType == GUN_TYPE_05) m_weaponInfoBG_06.setSize(m_chooseSize,m_chooseSize);
	}
	if(m_upCanUseGunType != m_canUserGunType && m_canUseGunChangeStage == 0)
	{//改变了武器的选择(注意这里一次只允许开启一种武器，不然会出现显示bug)
		//m_weaponInfoBG_01.setAlpha(0.25f);
		//m_weaponInfoBG_02.setAlpha(0.25f);
		//m_weaponInfoBG_03.setAlpha(0.25f);
		//m_weaponInfoBG_04.setAlpha(0.25f);
		//m_weaponInfoBG_05.setAlpha(0.25f);
		//m_weaponInfoBG_06.setAlpha(0.25f);
		//m_weaponInfoLG_00.setAlpha(0.25f);
		//m_weaponInfoLG_01.setAlpha(0.25f);
		//m_weaponInfoLG_02.setAlpha(0.25f);
		//m_weaponInfoLG_03.setAlpha(0.25f);
		//m_weaponInfoLG_04.setAlpha(0.25f);
		//m_weaponInfoLG_05.setAlpha(0.25f);
		//if(m_canUserGunType >= GUN_TYPE_00) {m_weaponInfoBG_01.setAlpha(1.0f);m_weaponInfoLG_00.setAlpha(1.0f);}
		//if(m_canUserGunType >= GUN_TYPE_01) {m_weaponInfoBG_02.setAlpha(1.0f);m_weaponInfoLG_01.setAlpha(1.0f);}
		//if(m_canUserGunType >= GUN_TYPE_02) {m_weaponInfoBG_03.setAlpha(1.0f);m_weaponInfoLG_02.setAlpha(1.0f);}
		//if(m_canUserGunType >= GUN_TYPE_03) {m_weaponInfoBG_04.setAlpha(1.0f);m_weaponInfoLG_03.setAlpha(1.0f);}
		//if(m_canUserGunType >= GUN_TYPE_04) {m_weaponInfoBG_05.setAlpha(1.0f);m_weaponInfoLG_04.setAlpha(1.0f);}
		//if(m_canUserGunType >= GUN_TYPE_05) {m_weaponInfoBG_06.setAlpha(1.0f);m_weaponInfoLG_05.setAlpha(1.0f);}
		m_nowCanUseGunType = m_canUserGunType;
		m_canUserGunY = 0.0f;
		m_canUserGunAlpha = 0.0f;
		m_canUserGunSize = 0.0f;
		m_canUseGunChangeStage = 1;
	}
	if(m_canUseGunChangeStage == 1)
	{
		if(m_canUserGunY < PI * 0.5f)
		{
			m_canUserGunY += 0.005 * interval;
			if(m_canUserGunY > PI * 0.5f) 
			{
				m_canUserGunY = PI * 0.5f;
				m_canUseGunChangeStage = 2;
			}
		}
		if(m_canUserGunAlpha <= 0.75f)
		{
			m_canUserGunAlpha += 0.005 * interval;
			if(m_canUserGunAlpha > 0.75f) m_canUserGunAlpha = 0.75f;
		}
		if(m_nowCanUseGunType == GUN_TYPE_00) 
		{
			m_weaponInfoLK_00.setPosition(0.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f - 100.0f * sin(m_canUserGunY));
		//	m_weaponInfoBG_01.setAlpha(0.25f + m_canUserGunAlpha);
		//	m_weaponInfoLG_00.setAlpha(0.25f + m_canUserGunAlpha);
		}else
		if(m_nowCanUseGunType == GUN_TYPE_01) 
		{
			m_weaponInfoLK_01.setPosition(70.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f - 100.0f * sin(m_canUserGunY));
		//	m_weaponInfoBG_02.setAlpha(0.25f + m_canUserGunAlpha);
		//	m_weaponInfoLG_01.setAlpha(0.25f + m_canUserGunAlpha);
		}
		if(m_nowCanUseGunType == GUN_TYPE_02) 
		{
			m_weaponInfoLK_02.setPosition(140.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f - 100.0f * sin(m_canUserGunY));
		//	m_weaponInfoBG_03.setAlpha(0.25f + m_canUserGunAlpha);
		//	m_weaponInfoLG_02.setAlpha(0.25f + m_canUserGunAlpha);
		}else
		if(m_nowCanUseGunType == GUN_TYPE_03) 
		{
			m_weaponInfoLK_03.setPosition(210.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f - 100.0f * sin(m_canUserGunY));
		//	m_weaponInfoBG_04.setAlpha(0.25f + m_canUserGunAlpha);
		//	m_weaponInfoLG_03.setAlpha(0.25f + m_canUserGunAlpha);
		}else
		if(m_nowCanUseGunType == GUN_TYPE_04) 
		{
			m_weaponInfoLK_04.setPosition(280.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f - 100.0f * sin(m_canUserGunY));
		//	m_weaponInfoBG_05.setAlpha(0.25f + m_canUserGunAlpha);
		//	m_weaponInfoLG_04.setAlpha(0.25f + m_canUserGunAlpha);
		}else
		if(m_nowCanUseGunType == GUN_TYPE_05) 
		{
			m_weaponInfoLK_05.setPosition(350.0f - m_weaponInfoMoveData.getNowNumber() + 16.0f,15.0f + 16.0f - 100.0f * sin(m_canUserGunY));
		//	m_weaponInfoBG_06.setAlpha(0.25f + m_canUserGunAlpha);
		//	m_weaponInfoLG_05.setAlpha(0.25f + m_canUserGunAlpha);
		}
	}else
	if(m_canUseGunChangeStage == 2)
	{
		if(m_canUserGunSize <= PI)
		{
			m_canUserGunSize += 0.01 * interval;
			if(m_canUserGunSize > PI) 
			{
				m_canUserGunSize = PI;
				m_upCanUseGunType = m_nowCanUseGunType;
				m_canUseGunChangeStage = 0;
			}
		}
		if(m_nowCanUseGunType == GUN_TYPE_00) 
		{
			m_weaponInfoBG_01.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
			m_weaponInfoLG_00.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
		}else
		if(m_nowCanUseGunType == GUN_TYPE_01) 
		{
			m_weaponInfoBG_02.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
			m_weaponInfoLG_01.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
		}
		if(m_nowCanUseGunType == GUN_TYPE_02) 
		{
			m_weaponInfoBG_03.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
			m_weaponInfoLG_02.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
		}else
		if(m_nowCanUseGunType == GUN_TYPE_03) 
		{
			m_weaponInfoBG_04.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
			m_weaponInfoLG_03.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
		}else
		if(m_nowCanUseGunType == GUN_TYPE_04) 
		{
			m_weaponInfoBG_05.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
			m_weaponInfoLG_04.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
		}else
		if(m_nowCanUseGunType == GUN_TYPE_05) 
		{
			m_weaponInfoBG_06.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
			m_weaponInfoLG_05.setSize(1.0f + sin(m_canUserGunSize),1.0f + sin(m_canUserGunSize));
		}
	}
}

int _XGun::ticketInit(_XResourcePosition resoursePosition)
{
	if(m_ticketLog.init("ResourcePack/pic/UI/Ticket_logo.png",resoursePosition) == 0) return 0;
	m_ticketLog.setAngle(90);
	m_ticketLog.setPosition(1150.0f,30.0f);
	if(m_ticketLog0.init("ResourcePack/pic/UI/Ticket_logo0.png",resoursePosition) == 0) return 0;
	m_ticketLog0.setAngle(90);
	m_ticketLog0.setPosition(1150.0f,30.0f);
	m_ticketLog0.setAlpha(0.0f);
	if(m_ticketText00.init("ResourcePack/pic/UI/NoTicket_00.png",resoursePosition) == 0) return 0;
	m_ticketText00.setAngle(90);
	m_ticketText00.setPosition(990.0f,320.0f);
	if(m_ticketText01.init("ResourcePack/pic/UI/NoTicket_01.png",resoursePosition) == 0) return 0;
	m_ticketText01.setAngle(90);
	m_ticketText01.setPosition(990.0f,320.0f);
	if(m_ticketNumber.init("ResourcePack/pic/UI/TicketSum.png",_XVector2(64,64),_XVector2(11,1),resoursePosition) == 0) return 0;
	m_ticketNumber.setAngle(90);
	m_ticketNumber.setDistance(-10.0f);
	m_ticketNumber.setPosition(1240.0f,110.0f);
	m_ticketNumber.setNumber("0+0");
	m_ticketAddNumber.setACopy(m_goldAddNum[0]);
	m_ticketAddNumber.setAngle(90.0f);
	m_ticketAddNumber.setPosition(1220.0f,20.0f);
	m_ticketAddNumber.setNumber("+100");
	m_isWrong = 0;
	m_ticketJumpStage = 0;
	return 1;
}

void _XGun::ticketDraw()
{
	if(m_configData.isTicketNeed == 0) return;
	if(m_allArmOutTicket < m_releaseArmTicket) m_allArmOutTicket = m_releaseArmTicket;

	m_ticketBarDown.draw();
	m_ticketBar.draw();
	m_ticketLog.draw();
	m_ticketLog0.draw();
	m_ticketNumber.draw();
	if(m_numberStage > 1)
	{
		m_ticketAddNumber.draw();
	}
	if(m_isWrong == 1)
	{
		if(m_ticketWrongStage == 2 || m_ticketWrongStage == 5)
		{
			m_ticketText00.draw();
		}else
		if(m_ticketWrongStage == 3)
		{
			if(m_ticketWrongY < 500)
			{
				m_ticketText00.draw();
			}else
			{
				m_ticketText01.draw();
			}
		}
	}
}

void _XGun::ticketMove(int interval)
{
	if(m_configData.isTicketNeed == 0) return;
	if(m_isWrong == 1)
	{
		if(m_ticketWrongStage == 1)
		{//出现的初始化
			m_ticketWrongY = 0.0f;
			m_ticketText00.setPosition(990.0f + 80.0f,320.0f);
			m_ticketText01.setPosition(990.0f,320.0f);
			m_ticketWrongStage = 2;
		}else
		if(m_ticketWrongStage == 2)
		{//出现
			m_ticketWrongY += 0.002f * interval;
			if(m_ticketWrongY >= PI * 0.5f)
			{
				m_ticketWrongY = PI * 0.5f;
				m_ticketWrongStage = 3;
			}
			m_ticketText00.setPosition(990.0f + 80.0f * cos(m_ticketWrongY),320.0f);
		}else
		if(m_ticketWrongStage == 3)
		{//持续阶段
			m_ticketWrongY += interval;
			if(m_ticketWrongY > 1000) m_ticketWrongY -= 1000;
		}else
		if(m_ticketWrongStage == 4)
		{//准备消失
			m_ticketWrongY = 0.0f;
			m_ticketText00.setPosition(990.0f,320.0f);
			m_ticketWrongStage = 5;
		}else
		if(m_ticketWrongStage == 5)
		{
			m_ticketWrongY += 0.002f * interval;
			if(m_ticketWrongY >= PI * 0.5f)
			{
				m_ticketWrongY = PI * 0.5f;
				m_ticketWrongStage = 0;
				m_isWrong = 0;
			}
			m_ticketText00.setPosition(990.0f + 80.0f * sin(m_ticketWrongY),320.0f);
		}
	}
	//下面是数字的变化
	float tempSum = 0.0f;
	int  flag = 0;
	if(m_allArmOutTicket != m_allOutTicket)
	{
		tempSum = m_allArmOutTicket - m_allOutTicket;
		if(0.0075f * interval < 1.0f)
		{
			tempSum *= 0.0075f * interval;
		}
		if(tempSum < 1.0f && tempSum > -1.0f)
		{
			if(tempSum < 0) tempSum = -1.0f;
			else tempSum = 1.0f;
		}
		m_allOutTicket += tempSum;
		flag = 1;
	}
	if(m_releaseArmTicket != m_releaseTicket)
	{
		tempSum = m_releaseArmTicket - m_releaseTicket;
		if(0.0075f * interval < 1.0f)
		{
			tempSum *= 0.0075f * interval;
		}

		if(tempSum < 1.0f && tempSum > -1.0f)
		{
			if(tempSum < 0) tempSum = -1.0f;
			else tempSum = 1.0f;
		}
		m_releaseTicket += tempSum;
		flag = 1;
	}
	if(flag != 0)
	{
		char temp[20];
		sprintf(temp,"%d+%d",m_allOutTicket,m_releaseTicket);
		m_ticketNumber.setNumber(temp);
	}
	//下面是彩票跳跃的动画
	//if(m_ticketJumpStage == 1)
	//{//初始化
	//	m_ticketJumpSize = 0.0f;
	//	m_ticketLog.setSize(1.0f,1.0f);
	//	m_ticketJumpStage = 2;
	//}else
	//if(m_ticketJumpStage == 2)
	//{
	//	m_ticketJumpSize += 0.01f * interval;
	//	if(m_ticketJumpSize >= PI)
	//	{
	//		m_ticketJumpSize = PI;
	//		m_ticketJumpStage = 0;
	//		m_ticketLog.setSize(1.0f,1.0f);
	//	}else
	//	{
	//		m_ticketLog.setSize(1.0f + sin(m_ticketJumpSize),1.0f + sin(m_ticketJumpSize));
	//	}
	//}
	if(m_ticketJumpStage == 1)
	{//初始化
		m_ticketJumpSize = 0.0f;
		m_ticketLog0.setSize(10.0f,10.0f);
		m_ticketLog0.setAlpha(0.0f);
		m_ticketJumpStage = 2;
	}else
	if(m_ticketJumpStage == 2)
	{
		m_ticketJumpSize += 0.005f * interval;
		if(m_ticketJumpSize >= PI * 0.5f)
		{
			m_ticketJumpSize = PI * 0.5f;
			m_ticketJumpStage = 0;
			m_ticketLog0.setSize(1.0f,1.0f);
			m_ticketLog0.setAlpha(0.0f);
		}else
		{
			m_ticketLog0.setSize(10.0f * cos(m_ticketJumpSize) + 1.0f,10.0f * cos(m_ticketJumpSize) + 1.0f);
			m_ticketLog0.setAlpha(sin(m_ticketJumpSize * 2.0f) * 0.75f);
		}
	}
	//增加数字的变化
	if(m_numberStage == 1)
	{//初始化
		m_numberAlpha = 0.0f;
		m_numberStage = 2;
		m_ticketAddNumber.setColor(-1.0f,-1.0f,-1.0f,0.0f);
	}else
	if(m_numberStage == 2)
	{//出现
		m_numberAlpha += 0.01f * interval;
		if(m_numberAlpha >= 1.0f)
		{
			m_numberAlpha = 1.0f;
			m_numberStage = 3;
		}
		m_ticketAddNumber.setColor(-1.0f,-1.0f,-1.0f,m_numberAlpha);
	}else
	if(m_numberStage == 3)
	{//持续
		m_numberAlpha += interval;
		if(m_numberAlpha > 200)
		{
			m_numberAlpha = 1.0f;
			m_numberStage = 4;
		}
	}else
	if(m_numberStage == 4)
	{//消失
		m_numberAlpha -= 0.005f * interval;
		if(m_numberAlpha <= 0.0f)
		{
			m_numberAlpha = 0.0f;
			m_numberStage = 0;
		}
		m_ticketAddNumber.setColor(-1.0f,-1.0f,-1.0f,m_numberAlpha);
	}
}

void _XGun::configKeyProc(CONFIG_KEY key)
{
	if(m_isInConfig == 0 || m_configStage != 3) return;
	switch(key)
	{
	case CONFIG_KEY_UP:
		m_configNowChoose --;
		if(m_configNowChoose < -1) 
		{
			m_configNowChoose = -1;
			_XSound::GetInstance().play(m_soundTypeIn);
		}else 
		{
			m_configChooseStage = 1;
			_XSound::GetInstance().play(m_soundTextMove_01);
		}

		if(m_configNowChoose < 6) m_configChoose.setPosition(408.0f - 30.0f * m_configNowChoose,330.0f);
		else m_configChoose.setPosition(408.0f - 30.0f * (m_configNowChoose + 2),330.0f);
		break;
	case CONFIG_KEY_DOWN:
		m_configNowChoose ++;
		if(m_configNowChoose > 8) 
		{
			m_configNowChoose = 8;
			_XSound::GetInstance().play(m_soundTypeIn);
		}else 
		{
			m_configChooseStage = 1;
			_XSound::GetInstance().play(m_soundTextMove_01);
		}
		if(m_configNowChoose < 6) m_configChoose.setPosition(408.0f - 30.0f * m_configNowChoose,330.0f);
		else m_configChoose.setPosition(408.0f - 30.0f * (m_configNowChoose + 2),330.0f);
		break;
	case CONFIG_KEY_LEFT:
		if(m_configNowChoose == -1)
		{
			m_configData.levelSum --;
			if(m_configData.levelSum < 0)
			{
				m_configData.levelSum = 0;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}else
		if(m_configNowChoose == 0)
		{
			if(m_configData.isTicketNeed != 0) m_configData.isTicketNeed = 0;
			else m_configData.isTicketNeed = 1;
			_XSound::GetInstance().play(m_soundSumAdd_00);
		}else
		if(m_configNowChoose == 1)
		{
			m_configData.ticketSum --;
			if(m_configData.ticketSum < 0) 
			{
				m_configData.ticketSum = 0;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}else
		if(m_configNowChoose == 2)
		{
			if(m_configData.isAutoShoot != 0) m_configData.isAutoShoot = 0;
			else m_configData.isAutoShoot = 1;
			_XSound::GetInstance().play(m_soundSumAdd_00);
		}else
		if(m_configNowChoose == 3)
		{
			m_configData.coinToCredit --;
			if(m_configData.coinToCredit < 1) 
			{
				m_configData.coinToCredit = 1;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}else
		if(m_configNowChoose == 4)
		{
			m_configData.oneCreditEnergy --;
			if(m_configData.oneCreditEnergy < 0) 
			{
				m_configData.oneCreditEnergy = 0;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}else
		if(m_configNowChoose == 5)
		{
			m_configData.volume --;
			if(m_configData.volume < 0) 
			{
				m_configData.volume = 0;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}
		break;
	case CONFIG_KEY_RIGHT:
		if(m_configNowChoose == -1)
		{
			m_configData.levelSum ++;
			if(m_configData.levelSum > 4)
			{
				m_configData.levelSum = 4;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}else
		if(m_configNowChoose == 0)
		{
			if(m_configData.isTicketNeed != 0) m_configData.isTicketNeed = 0;
			else m_configData.isTicketNeed = 1;
			_XSound::GetInstance().play(m_soundSumAdd_00);
		}else
		if(m_configNowChoose == 1)
		{
			m_configData.ticketSum ++;
			if(m_configData.ticketSum > 2) 
			{
				m_configData.ticketSum = 2;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}else
		if(m_configNowChoose == 2)
		{
			if(m_configData.isAutoShoot != 0) m_configData.isAutoShoot = 0;
			else m_configData.isAutoShoot = 1;
			_XSound::GetInstance().play(m_soundSumAdd_00);
		}else
		if(m_configNowChoose == 3)
		{
			m_configData.coinToCredit ++;
			if(m_configData.coinToCredit > 5) 
			{
				m_configData.coinToCredit = 5;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}else
		if(m_configNowChoose == 4)
		{
			m_configData.oneCreditEnergy ++;
			if(m_configData.oneCreditEnergy > 4) 
			{
				m_configData.oneCreditEnergy = 4;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}else
		if(m_configNowChoose == 5)
		{
			m_configData.volume ++;
			if(m_configData.volume > 10) 
			{
				m_configData.volume = 10;
				_XSound::GetInstance().play(m_soundTypeIn);
			}else
			{
				_XSound::GetInstance().play(m_soundSumAdd_00);
			}
		}
		break;
	case CONFIG_KEY_SHOOT:
		if(m_configNowChoose == 6)
		{//归零
			m_configData.allInCoin = 0;
			m_configData.allOutTicket = 0;
			m_configBackData.allInCoin = 0;
			m_configBackData.allOutTicket = 0;
			m_configChooseStage = 1;
		}else
		if(m_configNowChoose == 7)
		{
			writeRecordData();
			writeRecordData();
			m_configChooseStage = 1;
			m_configBackData = m_configData;
			_XSound::GetInstance().setAllVolume(m_configData.volume * 10.0f / 100.0f * 128);
			_XMusic::GetInstance().setMusicVolume(m_configData.volume * 10.0f / 100.0f * 128);
			//刷新一下下面显示的值
			char temp[32];
			sprintf(temp,"%3d+%d",m_creditSum,m_configData.coinToCredit);
			m_creditNum.setNumber(temp);
		}else
		if(m_configNowChoose == 8)
		{//退出
			m_configStage = 4;
			m_configChooseStage = 1;
			m_configData = m_configBackData;
		}
		_XSound::GetInstance().play(m_soundStarAdd);
		break;
	}
}

int _XGun::configInit(_XResourcePosition resoursePosition)
{
	if(m_configBG.init("ResourcePack/pic/UI/GameConfig/GameConfigBG.png",resoursePosition) == 0) return 0;
	m_configBG.setAngle(90);
	m_configBG.setPosition(215.0f,100.0f);
	if(m_configText_00.init("ResourcePack/pic/UI/GameConfig/isTicketNeed.png",resoursePosition) == 0) return 0;
	m_configText_00.setAngle(90);
	m_configText_00.setPosition(408.0f,335.0f);
	if(m_configText_01.init("ResourcePack/pic/UI/GameConfig/ticketSum.png",resoursePosition) == 0) return 0;
	m_configText_01.setAngle(90);
	m_configText_01.setPosition(408.0f - 30.0f,335.0f);
	if(m_configText_02.init("ResourcePack/pic/UI/GameConfig/coinToCredit.png",resoursePosition) == 0) return 0;
	m_configText_02.setAngle(90);
	m_configText_02.setPosition(408.0f - 90.0f,335.0f);
	if(m_configText_03.init("ResourcePack/pic/UI/GameConfig/oneCreditEnergy.png",resoursePosition) == 0) return 0;
	m_configText_03.setAngle(90);
	m_configText_03.setPosition(408.0f - 120.0f,335.0f);
	if(m_configText_04.init("ResourcePack/pic/UI/GameConfig/volume.png",resoursePosition) == 0) return 0;
	m_configText_04.setAngle(90);
	m_configText_04.setPosition(408.0f - 150.0f,335.0f);
	if(m_configText_05.init("ResourcePack/pic/UI/GameConfig/AllCoin.png",resoursePosition) == 0) return 0;
	m_configText_05.setAngle(90);
	m_configText_05.setPosition(408.0f - 180.0f,335.0f);
	if(m_configText_06.init("ResourcePack/pic/UI/GameConfig/AllTicket.png",resoursePosition) == 0) return 0;
	m_configText_06.setAngle(90);
	m_configText_06.setPosition(408.0f - 210.0f,335.0f);
	if(m_configText_07.init("ResourcePack/pic/UI/GameConfig/reset.png",resoursePosition) == 0) return 0;
	m_configText_07.setAngle(90);
	m_configText_07.setPosition(408.0f - 240.0f,335.0f);
	if(m_configText_08.init("ResourcePack/pic/UI/GameConfig/Save.png",resoursePosition) == 0) return 0;
	m_configText_08.setAngle(90);
	m_configText_08.setPosition(408.0f - 270.0f,335.0f);
	if(m_configText_09.init("ResourcePack/pic/UI/GameConfig/Exit.png",resoursePosition) == 0) return 0;
	m_configText_09.setAngle(90);
	m_configText_09.setPosition(408.0f - 300.0f,335.0f);
	if(m_configText_10.init("ResourcePack/pic/UI/GameConfig/AutoShoot.png",resoursePosition) == 0) return 0;
	m_configText_10.setAngle(90);
	m_configText_10.setPosition(408.0f - 60.0f,335.0f);
	if(m_configText_11.init("ResourcePack/pic/UI/GameConfig/LevelSum.png",resoursePosition) == 0) return 0;
	m_configText_11.setAngle(90);
	m_configText_11.setPosition(408.0f + 30.0f,335.0f);
	if(m_configChoose.init("ResourcePack/pic/UI/GameConfig/Choose.png",resoursePosition) == 0) return 0;
	m_configChoose.setAngle(90);
	m_configChoose.setPosition(408.0f,330.0f);
	m_configFont.setACopy(m_gameOrderFontInput);
	m_configFont.setAngle(90);
	m_configFont.setPosition(677.0f,405.0f);
//	m_configFont.setColor(1,0,0,1);
//	m_configFont.setString("测试");
	return 1;
}

void _XGun::configDraw()
{
	if(m_isInConfig != 0 && m_configStage > 1)
	{
		m_configBG.draw();
		m_configChoose.draw();
		
		m_configText_00.draw();
		if(m_configNowChoose == 0) m_configFont.setColor(1,0,0,1);
		else m_configFont.setColor(1,1,1,1);
		m_configFont.setPosition(677.0f,405.0f + m_configRealY);
		if(m_configData.isTicketNeed == 0)
		{
			m_configFont.setString("否");
		}else
		{
			m_configFont.setString("是");
		}
		m_configFont.draw();

		m_configText_01.draw();
		if(m_configNowChoose == 1) m_configFont.setColor(1,0,0,1);
		else m_configFont.setColor(1,1,1,1);
		m_configFont.setPosition(677.0f - 30.0f,405.0f + m_configRealY);
		if(m_configData.ticketSum == 0)
		{
			m_configFont.setString("少量");
		}else
		if(m_configData.ticketSum == 1)
		{
			m_configFont.setString("中量");
		}else
		{
			m_configFont.setString("大量");
		}
		m_configFont.draw();

		m_configText_02.draw();
		if(m_configNowChoose == 3) m_configFont.setColor(1,0,0,1);
		else m_configFont.setColor(1,1,1,1);
		m_configFont.setPosition(677.0f - 90.0f,405.0f + m_configRealY);
		char ch[20];
		sprintf(ch,"%d",m_configData.coinToCredit);
		m_configFont.setString(ch);
		m_configFont.draw();

		m_configText_03.draw();
		if(m_configNowChoose == 4) m_configFont.setColor(1,0,0,1);
		else m_configFont.setColor(1,1,1,1);
		m_configFont.setPosition(677.0f - 120.0f,405.0f + m_configRealY);
		if(m_configData.oneCreditEnergy == 0) m_configFont.setString("400"); else
		if(m_configData.oneCreditEnergy == 1) m_configFont.setString("800"); else
		if(m_configData.oneCreditEnergy == 2) m_configFont.setString("1200"); else
		if(m_configData.oneCreditEnergy == 3) m_configFont.setString("1600"); else
		m_configFont.setString("2000");
		m_configFont.draw();

		m_configText_04.draw();
		if(m_configNowChoose == 5) m_configFont.setColor(1,0,0,1);
		else m_configFont.setColor(1,1,1,1);
		m_configFont.setPosition(677.0f - 150.0f,405.0f + m_configRealY);
		sprintf(ch,"%d",m_configData.volume * 10);
		m_configFont.setString(ch);
		m_configFont.draw();

		m_configText_05.draw();
		m_configFont.setColor(1,1,0,1);
		m_configFont.setPosition(677.0f - 180.0f,405.0f + m_configRealY);
		sprintf(ch,"%d",m_configData.allInCoin);
		m_configFont.setString(ch);
		m_configFont.draw();

		m_configText_06.draw();
		m_configFont.setColor(1,1,0,1);
		m_configFont.setPosition(677.0f - 210.0f,405.0f + m_configRealY);
		sprintf(ch,"%d",m_configData.allOutTicket);
		m_configFont.setString(ch);
		m_configFont.draw();

		m_configText_10.draw();
		if(m_configNowChoose == 2) m_configFont.setColor(1,0,0,1);
		else m_configFont.setColor(1,1,1,1);
		m_configFont.setPosition(677.0f - 60.0f,405.0f + m_configRealY);
		if(m_configData.isAutoShoot == 0)
		{
			m_configFont.setString("否");
		}else
		{
			m_configFont.setString("是");
		}
		m_configFont.draw();

		m_configText_11.draw();
		if(m_configNowChoose == -1) m_configFont.setColor(1,0,0,1);
		else m_configFont.setColor(1,1,1,1);
		m_configFont.setPosition(677.0f + 30.0f,405.0f + m_configRealY);
		if(m_configData.levelSum == 0) m_configFont.setString("6关"); else
		if(m_configData.levelSum == 1) m_configFont.setString("7关"); else
		if(m_configData.levelSum == 2) m_configFont.setString("8关"); else
		if(m_configData.levelSum == 3) m_configFont.setString("9关"); else
		if(m_configData.levelSum == 4) m_configFont.setString("不限制"); 
		m_configFont.draw();

		m_configText_07.draw();
		m_configText_08.draw();
		m_configText_09.draw();
	}
}

void _XGun::configMove(int interval)
{
	if(m_isInConfig != 0)
	{
		if(m_configStage == 1)
		{//初始化
			m_configY = 0.0f;
			m_configStage = 2;
			m_configRealY = 900.0f;
			m_configBG.setPosition(215.0f,100.0f + m_configRealY);
			m_configText_00.setPosition(408.0f,335.0f + m_configRealY);
			m_configText_01.setPosition(408.0f - 30.0f,335.0f + m_configRealY);
			m_configText_02.setPosition(408.0f - 90.0f,335.0f + m_configRealY);
			m_configText_03.setPosition(408.0f - 120.0f,335.0f + m_configRealY);
			m_configText_04.setPosition(408.0f - 150.0f,335.0f + m_configRealY);
			m_configText_05.setPosition(408.0f - 180.0f,335.0f + m_configRealY);
			m_configText_06.setPosition(408.0f - 210.0f,335.0f + m_configRealY);
			m_configText_07.setPosition(408.0f - 240.0f,335.0f + m_configRealY);
			m_configText_08.setPosition(408.0f - 270.0f,335.0f + m_configRealY);
			m_configText_09.setPosition(408.0f - 300.0f,335.0f + m_configRealY);
			m_configText_10.setPosition(408.0f - 60.0f,335.0f + m_configRealY);
			m_configText_11.setPosition(408.0f + 30.0f,335.0f + m_configRealY);
			m_configChoose.setPosition(408.0f,330.0f + m_configRealY);
			_XSound::GetInstance().play(m_soundTextMove_01);
		}else
		if(m_configStage == 2)
		{//出现
			m_configY += 0.002f * interval;
			if(m_configY >= PI * 0.5f)
			{
				m_configY = PI * 0.5f;
				m_configStage = 3;
			}
			m_configRealY = 900.0f * cos(m_configY);
			m_configBG.setPosition(215.0f,100.0f + m_configRealY);
			m_configText_00.setPosition(408.0f,335.0f + m_configRealY);
			m_configText_01.setPosition(408.0f - 30.0f,335.0f + m_configRealY);
			m_configText_02.setPosition(408.0f - 90.0f,335.0f + m_configRealY);
			m_configText_03.setPosition(408.0f - 120.0f,335.0f + m_configRealY);
			m_configText_04.setPosition(408.0f - 150.0f,335.0f + m_configRealY);
			m_configText_05.setPosition(408.0f - 180.0f,335.0f + m_configRealY);
			m_configText_06.setPosition(408.0f - 210.0f,335.0f + m_configRealY);
			m_configText_07.setPosition(408.0f - 240.0f,335.0f + m_configRealY);
			m_configText_08.setPosition(408.0f - 270.0f,335.0f + m_configRealY);
			m_configText_09.setPosition(408.0f - 300.0f,335.0f + m_configRealY);
			m_configText_10.setPosition(408.0f - 60.0f,335.0f + m_configRealY);
			m_configText_11.setPosition(408.0f + 30.0f,335.0f + m_configRealY);
			m_configChoose.setPosition(408.0f,330.0f + m_configRealY);
		}else
		if(m_configStage == 3)
		{//持续
		}else
		if(m_configStage == 4)
		{//准备退出
			m_configY = 0.0f;
			m_configStage = 5;
			m_configRealY = 0.0f;
			m_configBG.setPosition(215.0f,100.0f + m_configRealY);
			m_configText_00.setPosition(408.0f,335.0f + m_configRealY);
			m_configText_01.setPosition(408.0f - 30.0f,335.0f + m_configRealY);
			m_configText_02.setPosition(408.0f - 90.0f,335.0f + m_configRealY);
			m_configText_03.setPosition(408.0f - 120.0f,335.0f + m_configRealY);
			m_configText_04.setPosition(408.0f - 150.0f,335.0f + m_configRealY);
			m_configText_05.setPosition(408.0f - 180.0f,335.0f + m_configRealY);
			m_configText_06.setPosition(408.0f - 210.0f,335.0f + m_configRealY);
			m_configText_07.setPosition(408.0f - 240.0f,335.0f + m_configRealY);
			m_configText_08.setPosition(408.0f - 270.0f,335.0f + m_configRealY);
			m_configText_09.setPosition(408.0f - 300.0f,335.0f + m_configRealY);
			m_configText_10.setPosition(408.0f - 60.0f,335.0f + m_configRealY);
			m_configText_11.setPosition(408.0f + 30.0f,335.0f + m_configRealY);
			if(m_configNowChoose < 5) m_configChoose.setPosition(408.0f - 30.0f * m_configNowChoose,330.0f + m_configRealY);
			else m_configChoose.setPosition(408.0f - 30.0f * (m_configNowChoose + 2),330.0f + m_configRealY);
			_XSound::GetInstance().play(m_soundTextMove_00);
		}else
		if(m_configStage == 5)
		{//退出
			m_configY += 0.004f * interval;
			if(m_configY >= PI * 0.5f)
			{
				m_configY = PI * 0.5f;
				m_configStage = 0;
				m_isInConfig = 0;
			}
			m_configRealY = -900.0f * sin(m_configY);
			m_configBG.setPosition(215.0f,100.0f + m_configRealY);
			m_configText_00.setPosition(408.0f,335.0f + m_configRealY);
			m_configText_01.setPosition(408.0f - 30.0f,335.0f + m_configRealY);
			m_configText_02.setPosition(408.0f - 90.0f,335.0f + m_configRealY);
			m_configText_03.setPosition(408.0f - 120.0f,335.0f + m_configRealY);
			m_configText_04.setPosition(408.0f - 150.0f,335.0f + m_configRealY);
			m_configText_05.setPosition(408.0f - 180.0f,335.0f + m_configRealY);
			m_configText_06.setPosition(408.0f - 210.0f,335.0f + m_configRealY);
			m_configText_07.setPosition(408.0f - 240.0f,335.0f + m_configRealY);
			m_configText_08.setPosition(408.0f - 270.0f,335.0f + m_configRealY);
			m_configText_09.setPosition(408.0f - 300.0f,335.0f + m_configRealY);
			m_configText_10.setPosition(408.0f - 60.0f,335.0f + m_configRealY);
			m_configText_11.setPosition(408.0f + 30.0f,335.0f + m_configRealY);
			m_configChoose.setPosition(408.0f,330.0f + m_configRealY);
			if(m_configNowChoose < 5) m_configChoose.setPosition(408.0f - 30.0f * m_configNowChoose,330.0f + m_configRealY);
			else m_configChoose.setPosition(408.0f - 30.0f * (m_configNowChoose + 2),330.0f + m_configRealY);
		}
		if(m_configChooseStage == 1)
		{//初始化
			m_configChooseSize = 0.0f;
			m_configChooseStage = 2;
			m_configChoose.setSize(0.5f,1.0f);
		}else
		if(m_configChooseStage == 2)
		{//变大
			m_configChooseSize += 0.01f * interval;
			if(m_configChooseSize >= PI * 0.5f)
			{
				m_configChooseSize = PI * 0.5f;
				m_configChooseStage = 0;
				m_configChoose.setSize(1.0f,1.0f);
			}else
			{
				m_configChoose.setSize(0.5f + sin(m_configChooseSize) * 0.5f,1.0f);
			}
		}
	}
}

int _XGun::helpInit(_XResourcePosition resoursePosition)
{
	if(m_helpText.init("ResourcePack/pic/UI/Help_Text.png",resoursePosition) == 0) return 0;
	m_helpText.setAngle(90);
	//m_helpText.setPosition(270.0f,43.0f);
	if(m_helpPic.init("ResourcePack/pic/UI/Help.png",resoursePosition) == 0) return 0;
	m_helpPic.setAngle(0);
	//m_helpPic.setPosition(270.0f,43.0f);
	m_helpAngle = 0.0f;
	//setHelpPosition(300.0f,100.0f);
	m_helpPosition.set(300.0f,100.0f);
	m_helpText.setPosition(300.0f + 64.0f,100.0f + 32.0f);
	m_helpPic.setPosition(300.0f + 32,100.0f + 32.0f);

	m_helpStage = 0;
	m_helpMoveStage = 0;
	return 1;
}

void _XGun::setHelpPosition(float x,float y)
{
	m_helpMoveStage = 1;
	m_helpOldPosition = m_helpPosition;
	m_helpArmPosition.set(x,y);
	m_helpMoveX = 0;
}

void _XGun::helpDraw()
{
	if(m_helpStage != 0 && m_nowLevel == 0)		//只有第一关才有提示
	{
		m_helpText.draw();
		m_helpPic.draw();
	}
}

void _XGun::helpMove(int interval)
{
	if(m_helpStage != 0)
	{
		m_helpAngle += 0.02f * interval;
		if(m_helpAngle >= 24) m_helpAngle = 0.0f;
		m_helpPic.setAngle((int)(m_helpAngle) * 15.0f);
		if(m_helpMoveStage == 1)
		{
			m_helpMoveX += 0.005f * interval;
			if(m_helpMoveX >= PI * 0.5f)
			{
				m_helpMoveX = PI * 0.5f;
				m_helpMoveStage = 0;
			}
			m_helpPosition.set(m_helpOldPosition.x + (m_helpArmPosition.x - m_helpOldPosition.x) * sin(m_helpMoveX),
				m_helpOldPosition.y + (m_helpArmPosition.y - m_helpOldPosition.y) * sin(m_helpMoveX));
			m_helpText.setPosition(m_helpPosition.x + 64.0f,m_helpPosition.y + 32.0f);
			m_helpPic.setPosition(m_helpPosition.x + 32,m_helpPosition.y + 32.0f);
		}
	}
	if(m_helpStage == 1)
	{//初始化
		m_helpAlpha = 0;
		m_helpText.setAlpha(m_helpAlpha);
		m_helpPic.setAlpha(m_helpAlpha);
		m_helpStage = 2;
	}else
	if(m_helpStage == 2)
	{//出现
		m_helpAlpha += 0.001f * interval;
		if(m_helpAlpha >= 1.0f) 
		{
			m_helpAlpha = 1.0f;
			m_helpStage = 3;
		}
		m_helpText.setAlpha(m_helpAlpha);
		m_helpPic.setAlpha(m_helpAlpha);
	}else
	if(m_helpStage == 3)
	{//持续
		m_helpAlpha += 0.002f * interval;
		if(m_helpAlpha >= PI)
		{
			m_helpAlpha -= PI;
		}
		m_helpText.setAlpha(1.0f - sin(m_helpAlpha) * 0.25f);
	}else
	if(m_helpStage == 4)
	{//初始化
		m_helpAlpha = 1.0f;
		m_helpText.setAlpha(m_helpAlpha);
		m_helpPic.setAlpha(m_helpAlpha);
		m_helpStage = 5;
	}else
	if(m_helpStage == 5)
	{//消失
		m_helpAlpha -= 0.001f * interval;
		if(m_helpAlpha <= 0.0f) 
		{
			m_helpAlpha = 0.0f;
			m_helpStage = 0;
		}
		m_helpText.setAlpha(m_helpAlpha);
		m_helpPic.setAlpha(m_helpAlpha);
	}
}

int _XGun::warnningInit(_XResourcePosition resoursePosition)
{
	if(m_energyLowFace.init("ResourcePack/pic/UI/EnergyLow.png",resoursePosition) == 0) return 0;
	m_energyLowFace.setAngle(90);
	m_energyLowFace.setPosition(290.0f,63.0f);
	if(m_energyLowText.init("ResourcePack/pic/UI/EnergyLowtext.png",resoursePosition) == 0) return 0;
	m_energyLowText.setAngle(90);
	m_energyLowText.setPosition(70.0f,333.0f);
	if(m_timerLowFace.init("ResourcePack/pic/UI/TimerLow.png",resoursePosition) == 0) return 0;
	m_timerLowFace.setAngle(90);
	m_timerLowFace.setPosition(400.0f,87.0f);
	if(m_timerLowText.init("ResourcePack/pic/UI/TimerLowText.png",resoursePosition) == 0) return 0;
	m_timerLowText.setAngle(90);
	m_timerLowText.setPosition(188.0f,401.0f);
	m_energyLowStage = 0;
	m_timerLowStage = 0;
	return 1;
}

void _XGun::warnningDraw()
{
	if(m_energyLowStage != 0)
	{
		m_energyLowFace.draw();
		m_energyLowText.draw();
	}
	if(m_timerLowStage != 0)
	{
		m_timerLowFace.draw();
		m_timerLowText.draw();
	}
}

void _XGun::warnningMove(int interval)
{
	if(m_upEnergySum != m_energyArmSum)
	{
		if(m_energyLowStage == 0 && m_energyArmSum < ENERGY_WARNNING && m_upEnergySum >= ENERGY_WARNNING
			&& m_gameStage != GAME_STAGE_GAMEALLOVER)
		{
			m_energyLowFace.setPosition(290.0f,63.0f);
			m_energyLowText.setPosition(70.0f,333.0f);
			m_energyLowY = 0.0f;
			m_energyLowStage = 1;
			_XSound::GetInstance().play(m_soundTextMove_01);
		}
		m_upEnergySum = m_energyArmSum;
	}
	if(m_energyLowStage == 1)
	{//出现
		m_energyLowY += 0.01f * interval;
		if(m_energyLowY >= PI * 0.5f)
		{
			m_energyLowY = PI * 0.5f;
			m_energyLowStage = 2;
		}
		m_energyLowFace.setPosition(290.0f,920.0f - (920.0f - 63.0f) * sin(m_energyLowY));
		m_energyLowText.setPosition(70.0f,920.0f - (920.0f - 333.0f) * sin(m_energyLowY));
	}else
	if(m_energyLowStage == 2)
	{//持续
		m_energyLowY += interval;
		if(m_energyLowY > 3000)
		{
			m_energyLowY = 0.0f;
			m_energyLowStage = 3;
			_XSound::GetInstance().play(m_soundTextMove_00);
		}
	}else
	if(m_energyLowStage == 3)
	{//消失
		m_energyLowY += 0.01f * interval;
		if(m_energyLowY >= PI * 0.5f)
		{
			m_energyLowY = PI * 0.5f;
			m_energyLowStage = 0;
		}
		m_energyLowFace.setPosition(290.0f,63.0f - 600.0f * sin(m_energyLowY));
		m_energyLowText.setPosition(70.0f,333.0f - 600.0f  * sin(m_energyLowY));
	}

	if(m_isNeedTimer != 0 && m_upTimerSum != m_gamePlayTimer)
	{
		if(m_timerLowStage == 0 && m_gamePlayTimer < 10000 && m_upTimerSum >= 10000)
		{
			m_timerLowFace.setPosition(400.0f,87.0f);
			m_timerLowText.setPosition(70.0f,293.0f);
			m_timerLowY = 0.0f;
			m_timerLowStage = 1;
			_XSound::GetInstance().play(m_soundTextMove_01);
		}
		m_upTimerSum = m_gamePlayTimer;
	}
	if(m_timerLowStage == 1)
	{//出现
		m_timerLowY += 0.01f * interval;
		if(m_timerLowY >= PI * 0.5f)
		{
			m_timerLowY = PI * 0.5f;
			m_timerLowStage = 2;
		}
		m_timerLowFace.setPosition(400.0f,920.0f - (920.0f - 87.0f) * sin(m_timerLowY));
		m_timerLowText.setPosition(188.0f,920.0f - (920.0f - 401.0f) * sin(m_timerLowY));
	}else
	if(m_timerLowStage == 2)
	{//持续
		m_timerLowY += interval;
		if(m_timerLowY > 3000)
		{
			m_timerLowY = 0.0f;
			m_timerLowStage = 3;
			_XSound::GetInstance().play(m_soundTextMove_00);
		}
	}else
	if(m_timerLowStage == 3)
	{//消失
		m_timerLowY += 0.01f * interval;
		if(m_timerLowY >= PI * 0.5f)
		{
			m_timerLowY = PI * 0.5f;
			m_timerLowStage = 0;
		}
		m_timerLowFace.setPosition(400.0f,87.0f - 600.0f * sin(m_timerLowY));
		m_timerLowText.setPosition(188.0f,401.0f - 600.0f  * sin(m_timerLowY));
	}
}

int _XGun::gameTimerInit(_XResourcePosition resoursePosition)
{
	if(m_gameTimerBG.init("ResourcePack/pic/UI/GameTimerBG.png",resoursePosition) == 0) return 0;//用于测试
	m_gameTimerBG.setAngle(90);
	m_gameTimerBG.setPosition(819.0f,250.0f);
	if(m_gameTimerNum.init("ResourcePack/pic/UI/GameTimerNum.png",_XVector2(45,64),_XVector2(11,1),resoursePosition) == 0) return 0;
	m_gameTimerNum.setAngle(90);
	m_gameTimerNum.setPosition(1109.0f,274.0f);
	m_gameTimerNum.setNumber("12+56");
	m_gameTimerNum.setDistance(-4.0f);
	if(m_gameTimerAutoShoot.init("ResourcePack/pic/UI/GameTimerAutoShoot.png",resoursePosition) == 0) return 0;//用于测试
	m_gameTimerAutoShoot.setAngle(90);
	m_gameTimerAutoShoot.setPosition(100.0f,330.0f);

	m_gameTimerStage = 0;	
	//用于测试
//	m_isNeedTimer = 1;
//	m_gamePlayTimer = 60000;
//	m_gameSetPlayerTimer = 60000;
	return 1;	
}

void _XGun::gameTimerDraw()
{
	if(m_isNeedTimer != 0 && m_gameTimerStage > 1)
	{
		m_gameTimerBG.draw();
		m_gameTimerNum.draw();
	}else
	if(m_isNeedTimer == 0)
	{
		if(m_gameTimerStage >= 3)
		{
			m_gameTimerAutoShoot.draw();
			m_gameTimerNum.draw();
		}
	}
}

void _XGun::setGameTimerReset(int mode)
{
	if(mode == 0)
	{
		if(m_isNeedTimer != 0)
		{
			if(m_gameTimerStage != 0)
			{		
				m_gamePlayTimer = m_gameSetPlayerTimer;	//重新开始计时
				m_gameTimerStage = 4;
				m_soundTimerOld = 0;
			}
		}else
		{
			if(m_gameTimerStage != 0)
			{
				if(m_gameTimerStage >= 3) m_gameTimerStage = 5;
				else m_gameTimerStage = 1;
			}
		}
	}else
	{
		if(m_isNeedTimer == 0)
		{
			if(m_gameTimerStage != 0)
			{
				if(m_gameTimerStage >= 3) m_gameTimerStage = 5;
				else m_gameTimerStage = 1;
			}
		}
	}
}

void _XGun::setGameTimerStart()
{
	if(m_isNeedTimer != 0) 
	{
		m_gamePlayTimer = m_gameSetPlayerTimer;
		m_gameTimerStage = 1;
	}
	else m_gameTimerStage = 1;
}

void _XGun::setGameTimerEnd(int mode)	//mode:0 停止 1：结束
{
	if(m_isNeedTimer != 0) 
	{
		if(mode == 0) m_gameTimerStage = 5;
		else m_gameTimerStage = 6;
	}else
	{
		if(mode == 0)
		{
			if(m_gameTimerStage >= 3) m_gameTimerStage = 6;
			else m_gameTimerStage = 0;
		}
	}
}

void _XGun::checkGunType()
{
	if(m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0) return;
	if(m_gunType == GUN_TYPE_05)
	{
		if(m_energyArmSum < GUN_SHOOT_ENERGY_05) setGunType(GUN_TYPE_04); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_04) setGunType(GUN_TYPE_03); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_03) setGunType(GUN_TYPE_02); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_02) setGunType(GUN_TYPE_01); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_01) setGunType(GUN_TYPE_00); else return;
	}else
	if(m_gunType == GUN_TYPE_04)
	{
		if(m_energyArmSum < GUN_SHOOT_ENERGY_04) setGunType(GUN_TYPE_03); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_03) setGunType(GUN_TYPE_02); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_02) setGunType(GUN_TYPE_01); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_01) setGunType(GUN_TYPE_00); else return;
	}else
	if(m_gunType == GUN_TYPE_03)
	{
		if(m_energyArmSum < GUN_SHOOT_ENERGY_03) setGunType(GUN_TYPE_02); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_02) setGunType(GUN_TYPE_01); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_01) setGunType(GUN_TYPE_00); else return;
	}else
	if(m_gunType == GUN_TYPE_02)
	{
		if(m_energyArmSum < GUN_SHOOT_ENERGY_02) setGunType(GUN_TYPE_01); else return;
		if(m_energyArmSum < GUN_SHOOT_ENERGY_01) setGunType(GUN_TYPE_00); else return;
	}else
	if(m_gunType == GUN_TYPE_01)
	{
		if(m_energyArmSum < GUN_SHOOT_ENERGY_01) setGunType(GUN_TYPE_00); else return;
	}
}

void _XGun::gameTimerMove(int interval)
{
	m_faceGameTimer += interval;
	if(m_isNeedTimer != 0)
	{//需要计时
		if(m_gameTimerStage == 1)
		{//初始化
			m_gameTiemrX = 0.0f;
			m_gameTimerAlpha = 0.0f;
			m_gameTimerBG.setPosition(819.0f,920.0f);
			m_gameTimerNum.setColor(-1.0f,-1.0f,-1.0f,m_gameTimerAlpha);
			m_gameTimerNum.setPosition(1109.0f,274.0f);
			char temp[20] = "00+00";
			sprintf(temp,"%2d+%2d",m_gamePlayTimer/60000,(m_gamePlayTimer/1000)%60);
			if(temp[3] == ' ') temp[3] = '0';
			m_gameTimerNum.setNumber(temp);
			m_gameTimerStage = 2;
			_XSound::GetInstance().play(m_soundTextMove_01);
		}else
		if(m_gameTimerStage == 2)
		{//背景出现
			m_gameTiemrX += 0.01f * interval;
			if(m_gameTiemrX >= PI * 0.5f)
			{
				m_gameTiemrX = PI * 0.5f;
				m_gameTimerStage = 3;
			}
			m_gameTimerBG.setPosition(819.0f,920.0f - (920.0f - 250.0f) * sin(m_gameTiemrX));
		}else
		if(m_gameTimerStage == 3)
		{//数字出现
			m_gameTimerAlpha += 0.001f * interval;
			if(m_gameTimerAlpha >= 1.0f)
			{
				m_gameTimerAlpha = 1.0f;
				m_gameTimerStage = 4;
				m_soundTimerOld = 10;
			}
			m_gameTimerNum.setColor(-1.0f,-1.0f,-1.0f,m_gameTimerAlpha);
		}else
		if(m_gameTimerStage == 4)
		{//计时阶段
			m_gamePlayTimer -= interval;
			if(m_gamePlayTimer < 0) m_gamePlayTimer = 0;
			char temp[20] = "00+00";
			sprintf(temp,"%2d+%2d",m_gamePlayTimer/60000,(m_gamePlayTimer/1000)%60);
			if(temp[3] == ' ') temp[3] = '0';
			m_gameTimerNum.setNumber(temp);
			if(m_gamePlayTimer <= 0)
			{
				if(m_energyArmSum < (m_configData.oneCreditEnergy + 1) * 200)
				{
					energyAdd(- m_energyArmSum);
				}else
				{
					energyAdd(-(m_configData.oneCreditEnergy + 1) * 200);
					m_gamePlayTimer = m_gameSetPlayerTimer;	//重新开始计时
				}
			//	m_energyArmSum -= 1000;//减去一个币的能量
			//	if(m_energyArmSum < 0) m_energyArmSum = 0;
				//这里需要检查炮管
				checkGunType();
			}
			if(m_soundTimerOld != (int)(m_gamePlayTimer/1000) && m_gamePlayTimer < 10000)
			{
				m_soundTimerOld = (int)(m_gamePlayTimer/1000);
				_XSound::GetInstance().play(m_soundTimer_00);
			}
		}else
		if(m_gameTimerStage == 5)
		{//时间停止阶段
		}else
		if(m_gameTimerStage == 6)
		{//准备退出阶段
			m_gameTiemrX = 0.0f;
			m_gameTimerStage = 7;
			_XSound::GetInstance().play(m_soundTextMove_00);
		}else
		if(m_gameTimerStage == 7)
		{//退出阶段
			m_gameTiemrX += 0.01f * interval;
			if(m_gameTiemrX >= PI * 0.5f)
			{//退出完成
				m_gameTiemrX = PI * 0.5f;
				m_gameTimerStage = 0;
			}
			m_gameTimerBG.setPosition(819.0f,250.0f - 600.0f * sin(m_gameTiemrX));
			m_gameTimerNum.setPosition(1109.0f,274.0f - 600.0f * sin(m_gameTiemrX));
		}
	}else
	{
		if(m_gameTimerStage == 1)
		{//初始化
			m_gamePlayTimer = 10000;	//10s自动射击依次
			m_gameTimerStage = 2;
		}else
		if(m_gameTimerStage == 2)
		{//计时
			m_gamePlayTimer -= interval;
			if(m_gamePlayTimer <= 0) m_gamePlayTimer = 0;
			if(m_gamePlayTimer <= 5000)
			{
				m_gameTimerAlpha = 0.0f;
				m_gameTimerNum.setPosition(196.0f,330.0f);
				m_gameTimerNum.setNumber(m_gamePlayTimer / 1000);
				m_gameTimerAutoShoot.setPosition(100.0f,330.0f);
				m_gameTimerNum.setColor(-1.0f,-1.0f,-1.0f,m_gameTimerAlpha);
				m_gameTimerAutoShoot.setAlpha(m_gameTimerAlpha);
				m_gameTimerStage = 3;
			}
		}else
		if(m_gameTimerStage == 3)
		{//计时继续，并显示
			m_gamePlayTimer -= interval;
			if(m_gamePlayTimer <= 0) m_gamePlayTimer = 0;
			m_gameTimerNum.setNumber(m_gamePlayTimer / 1000);
			m_gameTimerAlpha += 0.001f * interval;
			if(m_gameTimerAlpha >= 1.0f)
			{
				m_gameTimerAlpha = 1.0f;
				m_gameTimerStage = 4;
				m_soundTimerOld = 5;
			}
			m_gameTimerNum.setColor(-1.0f,-1.0f,-1.0f,m_gameTimerAlpha);
			m_gameTimerAutoShoot.setAlpha(m_gameTimerAlpha);
		}else
		if(m_gameTimerStage == 4)
		{//计时继续，并显示
			m_gamePlayTimer -= interval;
			if(m_gamePlayTimer <= 0) 
			{
				shoot();	//时间到则射击
				m_gameTimerAlpha = 1.0f;
				m_gamePlayTimer = 0;
				m_gameTimerStage = 5;
			}
			m_gameTimerNum.setNumber(m_gamePlayTimer / 1000);
			if(m_soundTimerOld != (int)(m_gamePlayTimer/1000) && m_gamePlayTimer < 10000)
			{
				m_soundTimerOld = (int)(m_gamePlayTimer/1000);
				_XSound::GetInstance().play(m_soundTimer_00);
			}
		}else
		if(m_gameTimerStage == 5)
		{//消失阶段
			m_gameTimerAlpha -= 0.005f * interval;
			if(m_gameTimerAlpha <= 0.0f)
			{
				m_gameTimerAlpha = 0.0f;
				m_gameTimerStage = 1;
			}
			m_gameTimerNum.setColor(-1.0f,-1.0f,-1.0f,m_gameTimerAlpha);
			m_gameTimerAutoShoot.setAlpha(m_gameTimerAlpha);
			m_gameTimerNum.setNumber(m_gamePlayTimer / 1000);
		}else
		if(m_gameTimerStage == 6)
		{//消失并停止计时
			m_gameTimerAlpha -= 0.005f * interval;
			if(m_gameTimerAlpha <= 0.0f)
			{
				m_gameTimerAlpha = 0.0f;
				m_gameTimerStage = 0;
			}
			m_gameTimerNum.setColor(-1.0f,-1.0f,-1.0f,m_gameTimerAlpha);
			m_gameTimerAutoShoot.setAlpha(m_gameTimerAlpha);
			m_gameTimerNum.setNumber(m_gamePlayTimer / 1000);
		}
	}
}

int _XGun::gameAllOverInit(_XResourcePosition resoursePosition)
{
	if(m_gameAllOverText.init("ResourcePack/pic/UI/GameAllOverText.png",resoursePosition) == 0) return 0;
	m_gameAllOverText.setAngle(90);
	m_gameAllOverText.setPosition(400.0f,320.0f);

	return 1;
}

void _XGun::gameAllOverDraw()
{
	if(m_gameStage == GAME_STAGE_GAMEALLOVER && m_gameCounter > 0)
	{
		m_gameAllOverText.draw();
	}
}

void _XGun::gameAllOverMove(int interval)
{
	if(m_gameStage == GAME_STAGE_GAMEALLOVER)
	{
		if(m_gameCounter == 0)
		{//初始化
			m_gameAllOverText.setPosition(400.0f,720.0f + 320.0f);
			m_gameAllOverY = 0.0f;
			m_gameCounter = 1;
			_XSound::GetInstance().play(m_soundTextMove_01);
		}else
		if(m_gameCounter == 1)
		{//进场
			m_gameAllOverY += 0.001f * interval;
			if(m_gameAllOverY >= PI * 0.5f)
			{
				m_gameAllOverTimer = 0.0f;
				m_gameAllOverY = PI * 0.5f;
				m_gameCounter = 2;
				//将能量清空
				m_scoresArmSum += m_energyArmSum * 10;
				energyAdd(- m_energyArmSum);
				//m_energyArmSum = 0;
				getNowOrder();
				updateGameOrder();
			}
			m_gameAllOverText.setPosition(400.0f,1040.0f - (1040.0f - 320.0f) * sin(m_gameAllOverY));
		}else
		if(m_gameCounter == 2)
		{//持续
			m_gameAllOverTimer += interval;
			if(m_gameAllOverTimer > 10000)
			{
				m_gameAllOverY = 0.0f;
				m_gameCounter = 3;
			}
		}else
		if(m_gameCounter == 3)
		{//退场
			m_gameAllOverY += 0.001f * interval;
			if(m_gameAllOverY >= PI * 0.5f)
			{//退场结束
				m_gameAllOverY = PI * 0.5f;
				if(m_gameNowOrder < MAX_GAME_ORDER_SUM && m_gameNowOrder >= 0)
				{
					m_gameStage = GAME_STAGE_GAMEINPUTNAME;
					m_gameCounter = 0;
					m_takePhotoKeyFlag = 0;
				}else
				{
					if(m_isCameraSupply != 0)
					{
						m_gameCounter = 4;	//进入提示拍照流程
					}else
					{
						if(m_configData.isTicketNeed == 0 || m_releaseArmTicket == 0)
						{
							m_gameStage = GAME_STAGE_GAMELOGO;
							m_upGameStage = GAME_STAGE_GAMEALLOVER;
							m_gameCounter = 0;
						}
					}
				}
			}
			m_gameAllOverText.setPosition(400.0f,320.0f - 900.0f* sin(m_gameAllOverY));
		}else
		if(m_gameCounter == 4)
		{
			m_takePhotoAttention.setPosition(290.f,290.f + 720.0f);
			m_gameLostNum.setPosition(700.0f,330.0f + 720.0f);
			m_gameLostNum.setNumber(9);
			m_gameLostNum.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			m_gameAllOverY = 0.0f;
			m_gameCounter = 5;
			_XSound::GetInstance().play(m_soundTextMove_01);
			m_takePhotoNeedShow = 1;
		}else
		if(m_gameCounter == 5)
		{//物件出场
			m_gameAllOverY += 0.001f * interval;
			if(m_gameAllOverY >= PI * 0.5f)
			{
				m_gameAllOverY = PI * 0.5f;
				m_gameCounter = 6;	//进入持续阶段
				m_takePhotoKeyFlag = 2;
				m_takePhotoCanPressKey = 1;
			}
			m_takePhotoAttention.setPosition(290.f,290.f + 720.0f * cos(m_gameAllOverY));
			m_gameLostNum.setPosition(700.0f,330.0f + 720.0f * cos(m_gameAllOverY));
		}else
		if(m_gameCounter == 6)
		{
			m_gameAllOverY += interval;
			if(m_gameAllOverY >= 10000 || m_takePhotoCanPressKey == 2)
			{//进入退场阶段
				m_gameAllOverY = 0.0f;
				m_gameCounter = 7;
				m_takePhotoCanPressKey = 0;
			}else
			{
				m_gameLostNum.setNumber((int)(9.0f - m_gameAllOverY/1000.0f));
			}
		}else
		if(m_gameCounter == 7)
		{
			m_gameAllOverY += 0.001f * interval;
			if(m_gameAllOverY >= PI * 0.5f)
			{
				m_gameAllOverY = PI * 0.5f;
				//退场完毕
				if(m_takePhotoKeyFlag == 1)
				{//进入输入名字界面
					m_gameStage = GAME_STAGE_GAMEINPUTNAME;
					m_gameCounter = 0;
					m_takePhotoNeedShow = 0;
				}else
				{//直接退出
					if(m_configData.isTicketNeed == 0 || m_releaseArmTicket == 0)
					{
						m_gameStage = GAME_STAGE_GAMELOGO;
						m_upGameStage = GAME_STAGE_GAMEALLOVER;
						m_gameCounter = 0;
						m_takePhotoNeedShow = 0;
					}
				}
			}
			m_takePhotoAttention.setPosition(290.0f,290.0f + 720.0f * sin(m_gameAllOverY));
			m_gameLostNum.setPosition(700.0f,330.0f + 720.0f * sin(m_gameAllOverY));
		}
	}
}

int _XGun::UIBoardInit(_XResourcePosition resoursePosition)
{//初始化分数面板的一些东西
//	if(m_creditSprite.init("ResourcePack/pic/UI/credit.png",resoursePosition) == 0) return 0;
//	m_creditSprite.setAngle(90.0f);
//	m_creditSprite.setPosition(-88.0f,150.0f);

	if(m_creditText.init("ResourcePack/pic/UI/creditText.png",resoursePosition) == 0) return 0;
	m_creditText.setAngle(90.0f);
	m_creditText.setPosition(-103.0f,560.0f);

	//if(m_scoresSprite.init("ResourcePack/pic/UI/scores.png",resoursePosition) == 0) return 0;
	//m_scoresSprite.setAngle(90.0f);
	//m_scoresSprite.setPosition(-18.0f,150.0f);

	if(m_scoresText.init("ResourcePack/pic/UI/scoresText.png",resoursePosition) == 0) return 0;
	m_scoresText.setAngle(90.0f);
	m_scoresText.setPosition(-18.0f + 1087.0f,150.0f + 317.0f);

	if(m_goldSprite.init("ResourcePack/pic/UI/gold.png",resoursePosition) == 0) return 0;
	m_goldSprite.setAngle(90.0f);
	m_goldSprite.setPosition(-28.0f,150.0f);
	if(m_energySprite.init("ResourcePack/pic/UI/energy.png",resoursePosition) == 0) return 0;
	m_energySprite.setAngle(90.0f);
	m_energySprite.setPosition(-98.0f,150.0f);
	if(m_lowEnergy.init("ResourcePack/pic/object/LowZEnergy.png",resoursePosition) == 0) return 0;
	m_lowEnergy.setAngle(90.0f);
	m_lowEnergy.setPosition(8.0f,388.0f - 352.0f + 42.0f);
	if(m_energyAddNum.init("ResourcePack/pic/UI/number_02.png",_XVector2(20,32),_XVector2(12,1),resoursePosition) == 0) return 0;
	m_energyAddNum.setAngle(90.0f);
	m_energyAddNum.setPosition(40.0f,269.0f);
	m_energyAddNum.setNumber("+100");
	m_energyAddStage = 0;
	//能量进度条
	if(m_energyBar.init("ResourcePack/pic/UI/EnergyProcess.png",resoursePosition) == 0) return 0;
	m_energyBar.setPosition(-19.0f,128.0f);
	m_energyBarDown.setACopy(m_energyBar);
	m_energyBarDown.setPosition(-19.0f,128.0f);
	//彩票进度条
	if(m_ticketBar.init("ResourcePack/pic/UI/TicketProcess.png",resoursePosition) == 0) return 0;
	//m_ticketBar.setAlpha(0.75f);
	m_ticketBar.setPosition(1172.0f,116.0f);
	m_ticketBarDown.setACopy(m_ticketBar);
	m_ticketBarDown.setPosition(1172.0f,116.0f);
	m_ticketBarDown.setColor(1.0f,0.0f,0.0f,1.0f);
	m_ticketBar.setClipRect(0.0f,0.0f,64.0f,0.0f);
	//下面需要从记录文件中读取相关数据
	if(m_creditNum.init("ResourcePack/pic/UI/number_03.png",_XVector2(20,32),_XVector2(11,1),resoursePosition) == 0) return 0;
	if(m_scoresNum.init("ResourcePack/pic/UI/number_04.png",_XVector2(20,32),_XVector2(10,1),resoursePosition) == 0) return 0;
	if(m_energyNum.init("ResourcePack/pic/UI/number_00.png",_XVector2(25,32),_XVector2(10,1),resoursePosition) == 0) return 0;
	m_goldNum.setACopy(m_energyNum);

	m_creditNum.setAngle(90.0f);
	m_creditNum.setPosition(37.0f,568.0f);
	m_scoresNum.setAngle(90.0f);
	m_scoresNum.setPosition(122.0f + 1087.0f,160.0f + 317.0f);
	m_scoresNum.setNumber("0");
	m_goldNum.setAngle(90.0f);
	m_goldNum.setPosition(112.0f,138.0f);
	m_goldNum.setNumber("0");
	m_energyNum.setAngle(90.0f);
	m_energyNum.setPosition(42.0f,138.0f);
	m_energyNum.setNumber("0");
	m_energyNum.setDistance(-2.0f);

	return 1;
}

float gunShakeData[7] = {1.0f,2.0f,3.0f,4.0f,5.0f,3.0f,6.0f};
void _XGun::UIShakeMove(int interval)
{
	if(m_UIShakeStage == 1)
	{
		m_UIShakeData = 0.0f;
		m_UIShakeTempData = 0.0f;
		m_UIShakeStage = 2;
		m_UIShakeTempData1 = gunShakeData[m_gunType];
	}else
	if(m_UIShakeStage == 2)
	{
		m_UIShakeData += 0.01f * (7 - m_gunShakeTempData1) * interval;
		if(m_UIShakeData >= PI * 1.5f)
		{
			m_UIShakeData = PI * 1.5f;
			m_UIShakeStage = 0;
		}
		if(m_UIShakeData < PI)
		{
			m_UIShakeTempData = sin(m_UIShakeData) * m_UIShakeTempData1;
		//	m_creditSprite.setPosition(-88.0f + m_UIShakeTempData,150.0f);
			m_energySprite.setPosition(-98.0f + m_UIShakeTempData,150.0f);
		//	m_creditNum.setPosition(52.0f + m_UIShakeTempData,138.0f);
			m_energyNum.setPosition(42.0f + m_UIShakeTempData,138.0f);
			m_energyBar.setPosition(-19.0f + m_UIShakeTempData,128.0f);
			m_energyBarDown.setPosition(-19.0f + m_UIShakeTempData,128.0f);
		//	m_UIShakeTempData = sin(m_UIShakeData) * m_UIShakeTempData1 * 5;
		//	m_gunSprite.setPosition(-64.0f - m_UIShakeTempData * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),360.0f - 128.0f - m_UIShakeTempData * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN));
		}
		if(m_UIShakeData > PI * 0.5f)
		{
			m_UIShakeTempData = sin(m_UIShakeData - PI * 0.5f) * m_UIShakeTempData1;
		//	m_scoresSprite.setPosition(-18.0f + m_UIShakeTempData,150.0f);
			m_goldSprite.setPosition(-28.0f + m_UIShakeTempData,150.0f);
		//	m_scoresNum.setPosition(122.0f + m_UIShakeTempData,150.0f);
			m_goldNum.setPosition(112.0f + m_UIShakeTempData,138.0f);
		}
	}
}

void _XGun::gunShakeMove(int interval)
{
	if(m_gunShakeStage == 1)
	{
		m_gunShakeData = 0.0f;
		m_gunShakeTempData = 0.0f;
		m_gunShakeStage = 2;
		m_gunShakeTempData1 = gunShakeData[m_gunType];
	}else
	if(m_gunShakeStage == 2)
	{
		m_gunShakeData += 0.02f * (7 - m_gunShakeTempData1) * interval;
		if(m_gunShakeData >= PI)
		{
			m_gunShakeData = PI;
			m_gunShakeStage = 0;
		}
		m_gunShakeTempData = sin(m_gunShakeData) * m_gunShakeTempData1 * 2.0f;
		m_gunSprite.setPosition(-64.0f - m_gunShakeTempData * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),360.0f - 128.0f - m_gunShakeTempData * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN));
	}
}

int _XGun::goldAddAndFlyInit(_XResourcePosition resoursePosition)
{
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		if(i == 0)
		{
			if(m_goldAddNum[i].init("ResourcePack/pic/UI/number_01.png",_XVector2(24,32),_XVector2(12,1),resoursePosition) == 0) return 0;
			m_goldAddNum[i].setDistance(-1.0f);
		}else
		{
			m_goldAddNum[i].setACopy(m_goldAddNum[0]);
		}
		m_goldAddNum[i].setAngle(90.0f);
		m_goldAddNum[i].setPosition(300,550);
		m_goldAddNum[i].setNumber(0);
		m_goldAddAlpha[i] = 0;
		m_goldAddX[i] = 0;
		m_goldAddFlag[i] = 0;
		m_goldAddBasicX[i] = 150;
		m_goldAddBasicY[i] = -10;
	}

	for(int i = 0;i < MAX_BULLET_SUM * 10;++ i)
	{
		if(i == 0)
		{
			if(m_goldFly[i].init("ResourcePack/pic/UI/Gold/GCA_000.png",resoursePosition) == 0) return 0;
		}else
		{
			m_goldFly[i].setACopy(m_goldFly[0]);
		}
		m_goldFly[i].setAttribute(_XVector2(0.0f,0.0f),1,0,0,0.012f,0);
		m_goldFly[i].setAngle(90.0f);
		m_goldFly[i].setPosition(300,550);

		m_goldFlyAlpha[i] = 0;
		m_goldFlyPosition[i].set(0.0f,0.0f);
		m_goldFlyFlag[i] = 0;
	}
	return 1;
}

int _XGun::processInit(_XResourcePosition resoursePosition)
{//初始化一些游戏相关的参数
	m_levelFiniedSum = 0.0f;
	m_levelFiniedArmSum = 0.0f;
	if(m_nowLevelProcessB.init("ResourcePack/pic/UI/Progress/PB_00.png",resoursePosition) == 0) return 0;
	m_nowLevelProcessB.setPosition(1115.0f,50.0f + 64);
	if(m_nowLevelProcessU.init("ResourcePack/pic/UI/Progress/PU_00.png",resoursePosition) == 0) return 0;
	m_nowLevelProcessU.setPosition(1115.0f,50.0f + 64);
	if(m_nowLevelProcess.init("ResourcePack/pic/UI/Progress/PM_00.png",resoursePosition) == 0) return 0;
	m_nowLevelProcess.setPosition(1115.0f,50.0f + 64);
	m_nowLevelProcess.setClipRect(0,0,32,0);
//	if(m_nowLevelProcessN.init("normalResource/Number.png",_XVector2(15,30),_XVector2(8,2),resoursePosition) == 0) return 0;
//	m_nowLevelProcessN.setPosition(1172.0f,50.0f + 290.0f);
//	m_nowLevelProcessN.setAngle(90);
//	m_nowLevelProcessN.setNumber("0 %");
	if(m_nowLevelProcessLight.init("ResourcePack/pic/UI/ProgressLight.png",resoursePosition) == 0) return 0;
	m_nowLevelProcessLight.setPosition(1115.0f - 47,- 142);
	if(m_nowLevelProcessLage.init("ResourcePack/pic/UI/Progresslage.png",resoursePosition) == 0) return 0;
	m_nowLevelProcessLage.setPosition(1115.0f,562.0f);
	m_nowLevelProcessLage.setAngle(90);
	m_levelProcessBoom.setACopy(m_gems->m_boomFrame);
	m_levelProcessBoom.setAttribute(_XVector2(1115.0f + 16.0f - 128.0f,562.0f + 16.0f - 128.0f),
		0,0,0,0.05f,0);

	m_levelProcessBoom.setSize(0.5f,0.5f);
	m_PrograssAlpha = 0.0f;
	m_levelFlagStage = 0;
	return 1;
}

void _XGun::processMove(int interval)
{
	if(m_PrograssAlpha > 0.85)
	{
		m_PrograssAlphaTemp += 0.001f * interval;
		if(m_PrograssAlphaTemp >= m_PrograssAlpha * 2.0f)
		{
			m_PrograssAlphaTemp = 0.0f;
		}
		if(m_PrograssAlphaTemp < 0.0f) m_PrograssAlphaTemp = 0.0f;
		if(m_PrograssAlpha - m_PrograssAlphaTemp > 0)
		{
			m_nowLevelProcessLight.setAlpha(m_PrograssAlpha - m_PrograssAlphaTemp);
		}else
		{
			m_nowLevelProcessLight.setAlpha(m_PrograssAlphaTemp - m_PrograssAlpha);
		}
	}
	if(m_levelFlagStage == 1)
	{
		m_levelFlagSize += 0.01f * interval;
		if(m_levelFlagSize >= PI * 10.0f)
		{ 
			m_levelFlagSize = 0.0f;
			m_levelFlagStage = 4;
			m_nowLevelProcessLage.setSize(1.0f,1.0f);
			m_levelProcessBoom.reset();
		}else
		{
			m_nowLevelProcessLage.setSize(1.0f + sin(m_levelFlagSize) * 0.5f,1.0f + sin(m_levelFlagSize) * 0.5f);
		}
	}
	if(m_levelFlagStage == 4)
	{
		m_levelProcessBoom.move(interval);
	}
	if(m_levelFlagStage == 2)
	{
		m_levelFlagSize = 0.0f;
		m_nowLevelProcessLage.setPosition(1115.0f,100.0f);
		m_levelFlagStage = 3;
	}else
	if(m_levelFlagStage == 3)
	{
		m_levelFlagSize += 0.001f * interval;
		if(m_levelFlagSize >= PI * 0.5f)
		{ 
			m_levelFlagSize = 0.0f;
			m_levelFlagStage = 0;
			m_nowLevelProcessLage.setPosition(1115.0f,562.0f);
			m_levelProcessBoom.setPosition(1115.0f + 16.0f - 128.0f,562.0f + 16.0f - 128.0f);
		}else
		{
			m_nowLevelProcessLage.setPosition(1115.0f,100.0f + 462.0f * sin(m_levelFlagSize));
		}
	}
}

int _XGun::gameWinInit(_XResourcePosition resoursePosition)
{
	if(m_gameWinBG.init("ResourcePack/pic/UI/GameOverBGBoard.png",resoursePosition) == 0) return 0;
	m_gameWinBG.setAngle(90);
	m_gameWinBG.setPosition(400.0f,100.0f);
	if(m_gameWinBGUp.init("ResourcePack/pic/UI/GameOverBGBoardUp.png",resoursePosition) == 0) return 0;
	m_gameWinBGUp.setAngle(90);
	m_gameWinBGUp.setPosition(400.0f,100.0f);
	if(m_gameWinText00.init("ResourcePack/pic/UI/LevelOverText_01.png",resoursePosition) == 0) return 0;
	m_gameWinText00.setAngle(90);
	m_gameWinText00.setPosition(550.0f - 20.0f,200.0f - 30.0f);
	if(m_gameWinText01.init("ResourcePack/pic/UI/LevelOverText_03.png",resoursePosition) == 0) return 0;
	m_gameWinText01.setAngle(90);
	m_gameWinText01.setPosition(550.0f - 85.0f,200.0f - 30.0f);
	if(m_gameWinText02.init("ResourcePack/pic/UI/LevelOverText_04.png",resoursePosition) == 0) return 0;
	m_gameWinText02.setAngle(90);
	m_gameWinText02.setPosition(550.0f - 150.0f,200.0f);
	if(m_gameWinText03.init("ResourcePack/pic/UI/LevelOverText_05.png",resoursePosition) == 0) return 0;
	m_gameWinText03.setAngle(90);
	m_gameWinText03.setPosition(550.0f - 215.0f,200.0f);
	if(m_gameWinText04.init("ResourcePack/pic/UI/LevelOverText_06.png",resoursePosition) == 0) return 0;
	m_gameWinText04.setAngle(90);
	m_gameWinText04.setPosition(550.0f - 280.0f,200.0f);
	if(m_gameWinText05.init("ResourcePack/pic/UI/LevelOverText_07.png",resoursePosition) == 0) return 0;
	m_gameWinText05.setAngle(90);
	m_gameWinText05.setPosition(550.0f - 315.0f,200.0f);
	if(m_gameWinText06.init("ResourcePack/pic/UI/LevelOverText_08.png",resoursePosition) == 0) return 0;
	m_gameWinText06.setAngle(90);
	m_gameWinText06.setPosition(550.0f - 315.0f,200.0f);
	if(m_gameWinText07.init("ResourcePack/pic/UI/LevelOverText_09.png",resoursePosition) == 0) return 0;
	m_gameWinText07.setAngle(90);
	m_gameWinText07.setPosition(550.0f - 315.0f,200.0f);
	if(m_gameWinText08.init("ResourcePack/pic/UI/LevelOverText_10.png",resoursePosition) == 0) return 0;
	m_gameWinText08.setAngle(90);
	m_gameWinText08.setPosition(550.0f - 315.0f,200.0f);
	if(m_gameWinText09.init("ResourcePack/pic/UI/LevelOverText_11.png",resoursePosition) == 0) return 0;
	m_gameWinText09.setAngle(90);
	m_gameWinText09.setPosition(550.0f - 315.0f,200.0f);

	if(m_gameWinNum.init("ResourcePack/pic/UI/LevelOverNumber_00.png",_XVector2(30,64),_XVector2(16,1),resoursePosition) == 0) return 0;
	m_gameWinNum.setAngle(90);
	m_gameWinNum.setPosition(810.0f,380.0f);
	m_gameWinNum.setNumber("95.0%");

	if(m_gameWinStar_00.load("ResourcePack/pic/UI/Starr_00.png",resoursePosition) == 0) return 0;
	if(m_gameWinStar_01.load("ResourcePack/pic/UI/Starr_01.png",resoursePosition) == 0) return 0;
	m_gameWinStar.init(m_gameWinStar_00.texture.m_w,m_gameWinStar_00.texture.m_h);
	m_gameWinStar.setAngle(90);
	m_gameWinStar.setPosition(695.0f + 20.0f,380.0f);
	return 1;
}

int _XGun::gameLostInit(_XResourcePosition resoursePosition)
{
	if(m_gameLostBG.init("ResourcePack/pic/UI/GameLostBG.png",resoursePosition) == 0) return 0;
	m_gameLostBG.setAngle(90);
	m_gameLostBG.setPosition(550.0f,230.0f);
	if(m_gameLostText.init("ResourcePack/pic/UI/GameLostText.png",resoursePosition) == 0) return 0;
	m_gameLostText.setAngle(90);
	m_gameLostText.setPosition(613.0f,290.0f);
	if(m_gameLostNum.init("ResourcePack/pic/UI/GameLostNum.png",_XVector2(80,128),_XVector2(10,1),resoursePosition) == 0) return 0;
	m_gameLostNum.setAngle(90);
	m_gameLostNum.setPosition(820.0f,320.0f);
	m_gameLostNum.setNumber("5");
	return 1;
}

void _XGun::gameLostDraw()
{
	if(m_gameStage == GAME_STAGE_GAMELOST && m_gameCounter > 0)
	{
		m_gameLostBG.draw();
		m_gameLostText.draw();
		//由于公用资源，这里需要防止重复显示
		if(m_takePhotoNeedShow == 0) m_gameLostNum.draw();
	}
}

void _XGun::gameLostMove(int interval)
{
	interval = (float)(interval) / buffTimerRate();
	if(m_gameStage == GAME_STAGE_GAMEPLAY && m_gameCounter == 0)
	{//下面判断是否进入倒数计时阶段
		if(m_energyArmSum <= 0 && (m_toolsType != TOOLS_TYPE_03 || m_toolsFlag == 0))
		{//没有能量，也不再免费射击状态
			int flag = 0;
			for(int i = 0;i < MAX_BULLET_SUM;++ i)
			{
				if(m_bullet[i].getCanCrash() == 1)
				{
					flag = 1;
				}
			}
			if(flag == 0)
			{
				m_gameStage = GAME_STAGE_GAMELOST;
				m_gameCounter = 0;
				m_gameLostTimer = 10000;
			}
		}
	}
	if(m_gameStage == GAME_STAGE_GAMELOST)
	{
		if(m_gameCounter == 0)
		{//初始化
			m_gameLostBG.setAlpha(0);
			m_gameLostBG.setPosition(550.0f,230.0f);
			m_gameLostText.setPosition(613.0f,920.0f);
			m_gameLostNum.setPosition(820.0f,920.0f);
			m_gameLostAlpha = 0.0f;
			m_gameLostY = 0.0f;
			m_gameCounter = 1;
			setGameTimerEnd(0);
		}else
		if(m_gameCounter == 1)
		{//背景出现
			m_gameLostAlpha += 0.001f * interval;
			if(m_gameLostAlpha >= 1.0f)
			{
				m_gameLostAlpha = 1.0f;
				m_gameCounter = 2;
				_XSound::GetInstance().play(m_soundTextMove_01);
			}
			m_gameLostBG.setAlpha(m_gameLostAlpha);
		}else
		if(m_gameCounter == 2)
		{//文字出现
			m_gameLostY += 0.01f * interval;
			if(m_gameLostY >= PI * 0.75f)
			{
				m_gameLostY = PI * 0.75f;
				m_gameCounter = 3;
				m_soundTimerOverOld = 10;
			}
			if(m_gameLostY < PI * 0.5f)
			{
				m_gameLostText.setPosition(613.0f,920.0f - (920.0f - 290.0f) * sin(m_gameLostY));
			}
			if(m_gameLostY >= PI * 0.25f)
			{
				m_gameLostNum.setPosition(820.0f,920.0f - (920.0f - 330.0f) * sin(m_gameLostY - PI * 0.25f));
			}
		}else
		if(m_gameCounter == 3)
		{//持续阶段
			if(int(m_gameLostTimer / 1000) != m_soundTimerOverOld)
			{
				m_soundTimerOverOld = int(m_gameLostTimer / 1000);
				_XSound::GetInstance().play(m_soundTimerOver);
			}
		}else
		if(m_gameCounter == 4)
		{//进入退出阶段
			m_gameLostY = 0.0f;
			m_gameCounter = 5;
			_XSound::GetInstance().play(m_soundTextMove_00);
		}else
		if(m_gameCounter == 5)
		{//退出阶段
			m_gameLostY += 0.002f * interval;
			if(m_gameLostY >= PI * 0.5f)
			{//退出完成
				m_gameLostY = PI * 0.5f;
				setGameTimerReset();
				m_gameStage = GAME_STAGE_GAMEPLAY;
				m_gameCounter = 0;
			}
			m_gameLostBG.setPosition(550.0f,230.0f - (230.0f + 830.0f) * sin(m_gameLostY));
			m_gameLostText.setPosition(613.0f,290.0f - (290.0f + 890.0f) * sin(m_gameLostY));
			m_gameLostNum.setPosition(820.0f,330.0f - (330.0f + 930.0f) * sin(m_gameLostY));
		}else
		if(m_gameCounter == 6)
		{//进入落幕
			m_gameLostY = 0.0f;
			m_gameCounter = 7;
			_XSound::GetInstance().play(m_soundTextMove_00);
			_XMusic::GetInstance().fadeOutMusic(800);
		}else
		if(m_gameCounter == 7)
		{//进入名字输入界面
			m_gameLostY += 0.002f * interval;
			if(m_gameLostY >= PI * 0.5f)
			{//退出完成
				m_gameLostY = PI * 0.5f;
				if(m_gameNowOrder < MAX_GAME_ORDER_SUM && m_gameNowOrder >= 0)
				{
					setGameTimerEnd(1);
					m_gameStage = GAME_STAGE_GAMEINPUTNAME;
					m_takePhotoKeyFlag = 0;
					m_gameCounter = 0;
				}else
				{
					if(m_isCameraSupply != 0)
					{
						m_gameCounter = 8;	//进入拍照提示界面
					}else
					{
						if(m_configData.isTicketNeed == 0 || m_releaseArmTicket == 0)
						{
							setGameTimerEnd(1);
							m_gameStage = GAME_STAGE_GAMELOGO;
							m_upGameStage = GAME_STAGE_GAMELOST;
							m_gameCounter = 0;
						}
					}
				}
			}
			m_gameLostBG.setPosition(550.0f,230.0f - (230.0f + 830.0f) * sin(m_gameLostY));
			m_gameLostText.setPosition(613.0f,290.0f - (290.0f + 890.0f) * sin(m_gameLostY));
			m_gameLostNum.setPosition(820.0f,330.0f - (330.0f + 930.0f) * sin(m_gameLostY));
		}else
		if(m_gameCounter == 8)
		{
			m_takePhotoAttention.setPosition(290.f,290.f + 720.0f);
			m_gameLostNum.setPosition(700.0f,330.0f + 720.0f);
			m_gameLostNum.setNumber(9);
			m_gameLostNum.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			m_gameLostY = 0.0f;
			m_gameCounter = 9;
			_XSound::GetInstance().play(m_soundTextMove_01);
			m_takePhotoNeedShow = 1;
		}else
		if(m_gameCounter == 9)
		{
			m_gameLostY += 0.001f * interval;
			if(m_gameLostY >= PI * 0.5f)
			{
				m_gameLostY = PI * 0.5f;
				m_gameCounter = 10;	//进入持续阶段
				m_takePhotoKeyFlag = 2;
				m_takePhotoCanPressKey = 1;
			}
			m_takePhotoAttention.setPosition(290.f,290.f + 720.0f * cos(m_gameLostY));
			m_gameLostNum.setPosition(700.0f,330.f + 720.0f * cos(m_gameLostY));
		}else
		if(m_gameCounter == 10)
		{
			m_gameLostY += interval;
			if(m_gameLostY >= 10000 || m_takePhotoCanPressKey == 2)
			{//进入退场阶段
				m_gameLostY = 0.0f;
				m_gameCounter = 11;
				m_takePhotoCanPressKey = 0;
				clearAllBuff();
			}else
			{
				m_gameLostNum.setNumber((int)(9.0f - m_gameLostY / 1000.0f));
			}
		}else
		if(m_gameCounter == 11)
		{
			m_gameLostY += 0.001f * interval;
			if(m_gameLostY >= PI * 0.5f)
			{
				m_gameLostY = PI * 0.5f;
				//退场完毕
				if(m_takePhotoKeyFlag == 1)
				{//进入输入名字界面
					setGameTimerEnd(1);
					m_gameStage = GAME_STAGE_GAMEINPUTNAME;
					m_gameCounter = 0;
					m_takePhotoNeedShow = 0;
				}else
				{//直接退出
					if(m_configData.isTicketNeed == 0 || m_releaseArmTicket == 0)
					{
						setGameTimerEnd(1);
						m_gameStage = GAME_STAGE_GAMELOGO;
						m_upGameStage = GAME_STAGE_GAMELOST;
						m_gameCounter = 0;
						m_takePhotoNeedShow = 0;
					}
				}
			}
			m_takePhotoAttention.setPosition(290.0f,290.0f + 720.0f * sin(m_gameLostY));
			m_gameLostNum.setPosition(700.0f,330.0f + 720.0f * sin(m_gameLostY));
		}

		if(m_energyArmSum > 0 && m_gameCounter == 3)
		{
			m_gameCounter = 4;
		}
		if(m_gameCounter < 6)
		{
			m_gameLostTimer -= interval;
			if(m_gameLostTimer <= 0)
			{//倒数计时结束
				if(m_gameCounter == 3)
				{
					m_gameCounter = 6;
				}
				m_gameLostTimer = 0;
			}
			m_gameLostNum.setNumber(m_gameLostTimer / 1000);
		}
	}
}

int _XGun::gameDemoInit(_XResourcePosition resoursePosition)
{
	//if(m_gameDemoBG.init("normalResource/UI/GameLogoBG.jpg",resoursePosition) == 0) return 0;
	if(m_gameDemoBG.init("ResourcePack/pic/UIBG/GameLogoBG.png",resoursePosition) == 0) return 0;
	m_gameDemoBG.setPosition(0.0f,0.0f);
	if(m_gameDemoText1.init("ResourcePack/pic/UI/GameLogoText.png",resoursePosition) == 0) return 0;
	m_gameDemoText1.setAngle(90);
	m_gameDemoText1.setPosition(550.0f,262.0f);
	if(m_gameDemoText2.init("ResourcePack/pic/UI/QTB.png",resoursePosition) == 0) return 0;
	m_gameDemoText2.setAngle(90);
	m_gameDemoText2.setPosition(200.0f,294.0f);
	if(m_gameDemoText3.init("ResourcePack/pic/UI/QKSYX.png",resoursePosition) == 0) return 0;
	m_gameDemoText3.setAngle(90);
	m_gameDemoText3.setPosition(200.0f - 128.0f,294.0f);
	return 1;
}

void _XGun::gameDemoDraw()
{
	if(m_gameStage == GAME_STAGE_GAMELOGO && m_gameCounter > 0)
	{
		if(m_gameCounter >= 5) m_gameHeadBG.drawWithoutBlend();
		m_gameDemoBG.drawWithoutBlend();
		if(m_gameCounter >= 2) m_gameDemoText1.draw();
		if(m_gameCounter == 3) m_gameDemoText2.draw();
		if(m_gameCounter >= 4) m_gameDemoText3.draw();
		if(m_releaseTicket > 0) ticketDraw();
	}
}

void _XGun::gameDemoMove(int interval)
{
	if(m_gameStage == GAME_STAGE_GAMELOGO)
	{
		if(m_gameCounter == 0)
		{//初始化
			m_gameDemoText1Size = 10.0f;
			m_gameDemoText2Alpha = 0.5f;
			m_gameDemoBGX = 0.0f;
			m_gameDemoBG.setPosition(1280.0f,0.0f);
			m_gameDemoText1.setPosition(550.0f,262.0f);
			m_gameDemoText1.setSize(m_gameDemoText1Size,m_gameDemoText1Size);
			m_gameDemoText2.setAlpha(m_gameDemoText2Alpha);
			m_gameDemoText2.setPosition(200.0f,294.0f);
			m_gameDemoText3.setPosition(200.0f - 128.0f,294.0f);
			m_gameCounter = 1;
			//进入demo界面的时候保存一次数据(为什么要这么做，用于测试吗？)
		//	writeRecordData();
		//	writeRecordData();
			_XSound::GetInstance().play(m_soundScreenDown);
			m_instructionTimer = 0;
			_XMusic::GetInstance().fadeInMusic(BGMusicHandle[0],-1,200);
		}else
		if(m_gameCounter == 1)
		{//背景落下
			m_gameDemoBGX += 0.002f * interval;
			if(m_gameDemoBGX >= PI * 0.5f)
			{
				m_gameDemoBGX = PI * 0.5f;
				m_gameCounter = 2;
			}
			m_gameDemoBG.setPosition(1280.0f - (1280.0f - 0.0f) * sin(m_gameDemoBGX),0.0f);
		}else
		if(m_gameCounter == 2)
		{//标题出现
			m_gameDemoText1Size *= pow(0.98f,0.3f * interval);
			if(m_gameDemoText1Size <= 1.0f)
			{
				m_gameDemoText1Size = 1.0f;
				m_gameCounter = 3;
				//下面初始化所有变量
				m_allArmOutTicket = 0;	//初始化出彩票的值为0
				m_nowLevel = 0;
				m_levelFiniedArmSum = 0.0f;
				m_goldArmSum = 0;
				m_scoresArmSum = 0;
				m_isGunColorBoom = 0;	//彩蛋效果死亡之后消失，过关不会消失
				setGunType(GUN_TYPE_00);
				m_isLightShoot = 0;		//取消激光炮发射的属性
				m_gems->resetMatrix();
				setHelpEnd();
				m_canUserGunType = GUN_TYPE_00;
				m_upChooseGunType = GUN_TYPE_NULL;
				m_upCanUseGunType = GUN_TYPE_NULL;
				m_weaponInfoLK_00.setPosition(0.0f + 16.0f,15.0f + 16.0f);
				m_weaponInfoLK_01.setPosition(70.0f + 16.0f,15.0f + 16.0f);
				m_weaponInfoLK_02.setPosition(140.0f + 16.0f,15.0f + 16.0f);
				m_weaponInfoLK_03.setPosition(210.0f + 16.0f,15.0f + 16.0f);
				m_weaponInfoLK_04.setPosition(280.0f + 16.0f,15.0f + 16.0f);
				m_weaponInfoLK_05.setPosition(350.0f + 16.0f,15.0f + 16.0f);
				updateGameOrder();
			}
			m_gameDemoText1.setSize(m_gameDemoText1Size,m_gameDemoText1Size);
		}else
		if(m_gameCounter == 3)
		{//等待投币
			if(m_gameDemoText2Alpha < 1.0f)
			{
				m_gameDemoText2Alpha += 0.001f * interval;
				m_gameDemoText2.setAlpha(m_gameDemoText2Alpha);
			}else
			{
				m_gameDemoText2Alpha += 0.0005f * interval;
				if(m_gameDemoText2Alpha < 1.5f)
				{
					m_gameDemoText2.setAlpha(1.0f);
				}else
				{
					if(m_gameDemoText2Alpha >= 2.0f)
					{
						m_gameDemoText2Alpha = 0.5f;
					}else
					{
						m_gameDemoText2.setAlpha(2.5f - m_gameDemoText2Alpha);
					}
				}
			}
			if((m_creditSum >= m_configData.coinToCredit || m_energyArmSum > 0.0f) && m_isInConfig == 0)
			{//这里可以提示开始游戏
				m_gameCounter = 4;
			}else
			{
				m_instructionTimer += interval;
				if(m_instructionTimer > 10000 && m_isInConfig == 0)
				{
					m_instructionMode ++;
					if(m_instructionMode < 0 || m_instructionMode > 2) m_instructionMode = 0;
					if(m_instructionMode == 2)
					{
						m_gameStage = GAME_STAGE_GAMESHOWORDER;
						m_orderShowFlag = 1;
						m_gameCounter = 0;
					}else
					{
						m_gameStage = GAME_STAGE_INSTRUCTION;
						m_gameCounter = 0;
						m_instructionTimer = 0;
					}
				}
			}
		}else
		if(m_gameCounter == 4)
		{//等待开始
			if(m_gameDemoText2Alpha < 1.0f)
			{
				m_gameDemoText2Alpha += 0.001f * interval;
				m_gameDemoText3.setAlpha(m_gameDemoText2Alpha);
			}else
			{
				m_gameDemoText2Alpha += 0.0005f * interval;
				if(m_gameDemoText2Alpha >= 1.5f)
				{
					m_gameDemoText2Alpha = 0.5f;
				}else
				{
					m_gameDemoText3.setAlpha(2.0f - m_gameDemoText2Alpha);
				}
			}
			if(m_energyArmSum > 0 && m_releaseTicket <= 0)
			{//退场
			//	m_gameStage = GAME_STAGE_GAMEHEAD;
			//	m_gameCounter = 0;
				m_gameDemoBGX = 0.0f;
				m_gameCounter = 5;
				m_gameHeadBG.setPosition(0.0f,0.0f);
				_XSound::GetInstance().play(m_soundScreenUp);
				_XMusic::GetInstance().fadeOutMusic(800);
			}
		}else
		if(m_gameCounter == 5)
		{//资源退场
			m_gameDemoBGX += 0.002f * interval;
			if(m_gameDemoBGX >= PI * 0.5f)
			{
				m_gameDemoBGX = PI * 0.5f;
				m_gameStage = GAME_STAGE_GAMEHEAD;
				m_gameCounter = 0;
			}
			m_gameDemoBG.setPosition(0.0f,-720.0f * sin(m_gameDemoBGX));
			m_gameDemoText1.setPosition(550.0f,262.0f - 720.0f * sin(m_gameDemoBGX));
			m_gameDemoText2.setPosition(200.0f,294.0f - 720.0f * sin(m_gameDemoBGX));
			m_gameDemoText3.setPosition(200.0f - 128.0f,294.0f - 720.0f * sin(m_gameDemoBGX));
		}
	}
}

int _XGun::toolsInit(_XResourcePosition resoursePosition)
{
//	if(m_tools_00.load("ResourcePack/pic/object/AddEnergy.png",resoursePosition) == 0) return 0;
//	if(m_tools_01.load("ResourcePack/pic/object/AddGold.png",resoursePosition) == 0) return 0;
//	if(m_tools_02.load("ResourcePack/pic/object/GoldToEnergy.png",resoursePosition) == 0) return 0;
//	if(m_tools_03.load("ResourcePack/pic/object/10sFree.png",resoursePosition) == 0) return 0;
//	if(m_tools_04.load("ResourcePack/pic/object/Timer.png",resoursePosition) == 0) return 0;
	if(m_tools_00.load("ResourcePack/pic/object/Tool_energy.png",resoursePosition) == 0) return 0;
	if(m_tools_01.load("ResourcePack/pic/object/Tool_gold.png",resoursePosition) == 0) return 0;
	if(m_tools_02.load("ResourcePack/pic/object/Tool_energyChange.png",resoursePosition) == 0) return 0;
	if(m_tools_03.load("ResourcePack/pic/object/Tool_10Free.png",resoursePosition) == 0) return 0;
	if(m_tools_04.load("ResourcePack/pic/object/Tool_time.png",resoursePosition) == 0) return 0;
	if(m_tools_05.load("ResourcePack/pic/object/Buff_00.png",resoursePosition) == 0) return 0;
	if(m_tools_06.load("ResourcePack/pic/object/Buff_01.png",resoursePosition) == 0) return 0;
	if(m_tools_07.load("ResourcePack/pic/object/Buff_02.png",resoursePosition) == 0) return 0;
	if(m_tools_08.load("ResourcePack/pic/object/Buff_03.png",resoursePosition) == 0) return 0;
	if(m_tools_09.load("ResourcePack/pic/object/Buff_04.png",resoursePosition) == 0) return 0;
	m_toolsSprite.init(m_tools_00.texture.m_w,m_tools_00.texture.m_h);
	m_toolsSprite.setAngle(90);
	m_toolsSprite.setAlpha(0);
	m_toolsSize = 0.0f;
	m_toolsFlag = 0;
	m_toolsAlpha = 0.0f;
//	if(m_toolsTimerText.init("ResourcePack/pic/UI/BigNumber.png",_XVector2(64,128),_XVector2(10,1),resoursePosition) == 0) return 0;
//	m_toolsTimerText.setAngle(90);
//	m_toolsTimerText.setPosition(250,100);
//	if(m_toolsTextSprite.init("ResourcePack/pic/UI/FreeShootText.png",resoursePosition)== 0) return 0;
//	m_toolsTextSprite.setAngle(90);
//	m_toolsTextSprite.setPosition(150,100);
	return 1;
}

void _XGun::toolsMove(int interval)
{
	if(m_isInited == 0) return;
	if(m_toolsFlag == 1)
	{//初始化所有数据
		m_toolsSize = 0.01f;
		m_toolsAlpha = 0.0f;
		m_toolsFlag = 2;
//		m_toolsTextAlpha = 0;
	}else
	if(m_toolsFlag == 2)
	{//淡入阶段
		int flag = 1;
		if(m_toolsSize < 1.0f)
		{
			m_toolsSize *= pow(1.03f,0.3f * interval);
			if(m_toolsSize >= 1.0f) m_toolsSize = 1.0f;
			flag = 0;
			m_toolsSprite.setSize(m_toolsSize,m_toolsSize);
		}
		if(m_toolsAlpha < 1.0f)
		{
			m_toolsAlpha += 0.002f * interval;
			if(m_toolsAlpha >= 1.0f) m_toolsAlpha = 1.0f;
			flag = 0;
			m_toolsSprite.setAlpha(m_toolsAlpha);
		}
		if(flag == 1) 
		{
			m_toolsSize = 0.0f;
			m_toolsFlag = 3;
		}
	}else
	if(m_toolsFlag == 3)
	{//持续阶段
		m_toolsSize += 0.001f * interval;
		if(m_toolsSize >= 1.0f) m_toolsFlag = 4;
	}else
	if(m_toolsFlag == 4)
	{//淡出阶段
		m_toolsAlpha -= 0.0005f * interval;
		if(m_toolsAlpha <= 0.0f)
		{//结束
			m_toolsAlpha = 0.0f;
			if(m_toolsType == TOOLS_TYPE_03)
			{
				m_toolsFlag = 5;
			}else
			{
				m_toolsFlag = 0;
			}
		}
		m_toolsSprite.setAlpha(m_toolsAlpha);
	}
	if(m_toolsType == TOOLS_TYPE_03 && m_toolsFlag >= 1) 
	{
	//	m_toolsTimer -= interval;
	//	if(m_toolsTimer <= 0) 
	//	{
	//		m_toolsTimer = 0;
	//
	//		m_toolsFlag = 0;
	//		checkGunType();
	//	}
	//	m_toolsTextAlpha += 0.002f * interval;
	//	if(m_toolsTextAlpha >= PI)
	//	{
	//		m_toolsTextAlpha = 0.0f;
	//		if(m_toolsTimer <= 0)
	//		{
	//			m_toolsFlag = 0;
	//			checkGunType();
	//		}
	//	}
	//	m_toolsTextSprite.setAlpha(0.4f + 0.6 * sin(m_toolsTextAlpha));

	//	m_toolsTimerText.setNumber(m_toolsTimer/1000);
	//	if(m_soundTimerOld_01 != (int)(m_toolsTimer/1000))
	//	{
	//		m_soundTimerOld_01 = (int)(m_toolsTimer/1000);
	//		_XSound::GetInstance().play(m_soundTimer_01);
	//	}
	}
}

void _XGun::toolsDoProc(_XVector2 position,_XGEM_TYPE type)
{
	if(type == GEM_TYPE_16)
	{//能量
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_00;
		energyAdd(TOOLS_ADD_ENERGY);
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		m_toolsEffectStage[2] = 1;
		return ;
	}else
	if(type == GEM_TYPE_17)
	{//金币
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_01;
		m_goldArmSum += TOOLS_ADD_GOLD;
		m_scoresArmSum += TOOLS_ADD_GOLD;
		for(int i = 0;i < MAX_BULLET_SUM;++ i)
		{
			if(m_goldAddFlag[i] == 0)
			{
				char chTemp[100];
				sprintf(chTemp,"+%d",TOOLS_ADD_GOLD);
				m_goldAddNum[i].setNumber(chTemp);
				m_goldAddFlag[i] = 1;
				m_goldAddBasicX[i] = 150;
				m_goldAddBasicY[i] = -10;
				break;
			}
		}
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		m_toolsEffectStage[1] = 1;
		return ;
	}else
	if(type == GEM_TYPE_18)
	{//金币转化成能量
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_02;
		energyAdd(m_goldArmSum * TOOLS_ENERGY_CHANGE);
		m_goldArmSum = m_goldArmSum % 3;
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		m_toolsEffectStage[1] = 1;
		m_toolsEffectStage[2] = 1;
		return ;
	}else
	if(type == GEM_TYPE_19)
	{//10s免费射击
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_03;
		//m_toolsTimer = TOOLS_FREE_TIMER;
		//m_soundTimerOld_01 = 10;
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);

		if(m_userBuffFlag[5] == 0) m_userBuffFlag[5] = 1; else	//消失
		if(m_userBuffFlag[5] == 5) m_userBuffFlag[5] = 3; else	//闪烁
		if(m_userBuffFlag[5] >= 6) m_userBuffFlag[5] = 1; //消失
		m_userBuffReleaseTime[5] = TOOLS_FREE_TIMER;	//持续时间为10s

		m_toolsEffectStage[5] = 1;
		m_toolsEffectStage[2] = 1;

		return ;
	}else
	if(type == GEM_TYPE_20)
	{//时间
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_04;
		m_gamePlayTimer += TOOLS_ADD_TIMER;
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		m_toolsEffectStage[3] = 1;
		return ;
	}else
	if(type == GEM_TYPE_23)
	{//金币加倍
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_05;
		if(m_userBuffFlag[0] == 0) m_userBuffFlag[0] = 1; else	//消失
		if(m_userBuffFlag[0] == 5) m_userBuffFlag[0] = 3; else	//闪烁
		if(m_userBuffFlag[0] >= 6) m_userBuffFlag[0] = 1; //消失
		m_userBuffReleaseTime[0] = 10000;	//持续时间为10s
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		return ;
	}else
	if(type == GEM_TYPE_24)
	{//时间变慢
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_06;
		if(m_userBuffFlag[1] == 0) m_userBuffFlag[1] = 1; else	//消失
		if(m_userBuffFlag[1] == 5) m_userBuffFlag[1] = 3; else	//闪烁
		if(m_userBuffFlag[1] >= 6) m_userBuffFlag[1] = 1; //消失
		m_userBuffReleaseTime[1] = 5000;	//持续时间为5s
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		return ;
	}else
	if(type == GEM_TYPE_25)
	{//威力增大
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_07;
		if(m_userBuffFlag[2] == 0) m_userBuffFlag[2] = 1; else	//消失
		if(m_userBuffFlag[2] == 5) m_userBuffFlag[2] = 3; else	//闪烁
		if(m_userBuffFlag[2] >= 6) m_userBuffFlag[2] = 1; //消失
		m_userBuffReleaseTime[2] = 10000;	//持续时间为10s
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		return ;
	}else
	if(type == GEM_TYPE_26)
	{//射速加快
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_08;
		if(m_userBuffFlag[3] == 0) m_userBuffFlag[3] = 1; else	//消失
		if(m_userBuffFlag[3] == 5) m_userBuffFlag[3] = 3; else	//闪烁
		if(m_userBuffFlag[3] >= 6) m_userBuffFlag[3] = 1; //消失
		m_userBuffReleaseTime[3] = 10000;	//持续时间为10s
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		return ;
	}else
	if(type == GEM_TYPE_27)
	{//消耗减半
		m_toolsFlag = 1;
		m_toolsType = TOOLS_TYPE_09;
		if(m_userBuffFlag[4] == 0) m_userBuffFlag[4] = 1; else	//消失
		if(m_userBuffFlag[4] == 5) m_userBuffFlag[4] = 3; else	//闪烁
		if(m_userBuffFlag[4] >= 6) m_userBuffFlag[4] = 1; //消失
		m_userBuffReleaseTime[4] = 10000;	//持续时间为10s
		m_toolsSprite.setPosition(position.x - 64.0f,position.y - 64.0f);
		m_toolsEffectStage[4] = 1;
		m_toolsEffectStage[2] = 1;
		return ;
	}
}

int _XGun::canGetTools(_XGemMatrix *gems,_XVector2 position)	//判断是否可以获得道具
{
	if(m_isInited == 0) return 0;
	if(m_nowLevel == 0) return 0;	//第一关不会产生这种道具
	if(m_toolsFlag != 0) return 0;
	int temp = random(1000);
	if(m_isNeedTimer != 0 && m_gamePlayTimer < 10000 && temp < 200)
	{//时间
		if(gems->addToolsGem(position,GEM_TYPE_20) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(temp < 1 || (m_energyArmSum < 300 && temp < 100))
	{//能量：玩家的能量较少时一定几率产生
		if(gems->addToolsGem(position,GEM_TYPE_16) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(temp < 2 || (m_energyArmSum < 300 && temp < 170))
	{//10s免费射击：玩家的能量较少
		if(gems->addToolsGem(position,GEM_TYPE_19) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(m_goldArmSum > 1500 && (temp < 3 || (m_energyArmSum < 300 && temp < 300)))
	{//金币转换成能量：玩家的金币足够多，并且能量较少时
		if(gems->addToolsGem(position,GEM_TYPE_18) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(temp < 10)
	{//金币：随机产生
		if(gems->addToolsGem(position,GEM_TYPE_17) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(temp < 20 && m_userBuffFlag[0] == 0)
	{//金币增强道具
		if(gems->addToolsGem(position,GEM_TYPE_23) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(temp < 30)
	{//时间减慢道具
		if(gems->addToolsGem(position,GEM_TYPE_24) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(temp < 40)
	{//威力加强道具
		if(gems->addToolsGem(position,GEM_TYPE_25) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(temp < 50 && m_configData.isAutoShoot != 0)
	{//射速加快道具
		if(gems->addToolsGem(position,GEM_TYPE_26) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}else
	if(temp < 60)
	{//消耗减半道具
		if(gems->addToolsGem(position,GEM_TYPE_27) != 0)
		{
			m_toolsFlag = -1;	//有道具产生，每次只能产生一个道具
		}
		return 1;
	}
	return 0;
}

void _XGun::toolsDraw()
{
	if(m_toolsFlag > 1)
	{
		if(m_toolsType == TOOLS_TYPE_00) m_toolsSprite.draw(&(m_tools_00));else
		if(m_toolsType == TOOLS_TYPE_01) m_toolsSprite.draw(&(m_tools_01));else
		if(m_toolsType == TOOLS_TYPE_02) m_toolsSprite.draw(&(m_tools_02));else
		if(m_toolsType == TOOLS_TYPE_04) m_toolsSprite.draw(&(m_tools_04));else
		if(m_toolsType == TOOLS_TYPE_05) m_toolsSprite.draw(&(m_tools_05));else
		if(m_toolsType == TOOLS_TYPE_06) m_toolsSprite.draw(&(m_tools_06));else
		if(m_toolsType == TOOLS_TYPE_07) m_toolsSprite.draw(&(m_tools_07));else
		if(m_toolsType == TOOLS_TYPE_08) m_toolsSprite.draw(&(m_tools_08));else
		if(m_toolsType == TOOLS_TYPE_09) m_toolsSprite.draw(&(m_tools_09));else
		if(m_toolsType == TOOLS_TYPE_03) 
		{
//			m_toolsTextSprite.draw();
			m_toolsSprite.draw(&(m_tools_03));
//			m_toolsTimerText.draw();
		}
	}
}

int _XGun::gameHeadInit(_XResourcePosition resoursePosition)
{//下面是游戏头的初始化
	if(m_gameHeadNum.init("ResourcePack/pic/UI/LevelSum.png",_XVector2(70,128),_XVector2(10,1),resoursePosition) == 0) return 0;
	m_gameHeadNum.setAngle(90);
	m_gameHeadNum.setNumber(m_nowLevel + 1);
	m_gameHeadNum.setPosition(820.0f,310.0f);
	m_gameHeadNum.setDistance(-15.0f);
	if(m_gameHeadText.init("ResourcePack/pic/UI/LevelText.png",resoursePosition) == 0) return 0;
	m_gameHeadText.setAngle(90);
	m_gameHeadText.setPosition(500.0f,280.0f);
	if(m_gameHeadBGLevelInfo.init("ResourcePack/pic/UI/LevelInfoText.png",resoursePosition) == 0) return 0;
	m_gameHeadBGLevelInfo.setAngle(90);
	m_gameHeadBGLevelInfo.setPosition(400.0f,312.0f);
	//if(m_gameHeadBG.init("normalResource/UI/GameHeadBG.jpg",resoursePosition) == 0) return 0;
	if(m_gameHeadBG.init("ResourcePack/pic/UIBG/GameHeadBG.png",resoursePosition) == 0) return 0;
	m_gameHeadBG.setPosition(0,0);
	return 1;
}

void _XGun::gameHeadDraw()
{
	if(m_gameStage == GAME_STAGE_GAMEHEAD)
	{
		m_gameHeadBG.drawWithoutBlend();
		if(m_gameCounter > 0)
		{
			m_gameHeadText.draw();
			if(m_nowLevel == 0)
			{
				m_gameHeadBGLevelInfo.draw();
			}
			m_gameHeadNum.draw();
		}
	}
}

void _XGun::gameHeadMove(int interval)
{
	if(m_gameStage == GAME_STAGE_GAMEHEAD)
	{
		if(m_gameCounter == 0)
		{//初始化
			m_gameHeadTextY = 0.0f;
			m_gameHeadText.setPosition(500.0f,920.0f);
			m_gameHeadBGLevelInfo.setPosition(400.0f,920.0f);
			m_gameHeadSize = 0.0f;
			m_gameHeadAlpha = 0.0f;
			m_gameHeadNum.setColor(-1,-1,-1,m_gameHeadAlpha);
			m_gameHeadBG.setPosition(0.0f,0.0f);
			//m_nowLevel = 21;
			if(m_nowLevel + 1 >= 10)
			{
				m_gameHeadNum.setPosition(820.0f,260.0f + 27.0f);
			}else
			{
				m_gameHeadNum.setPosition(820.0f,310.0f);
			}
			m_gameHeadNum.setNumber(m_nowLevel + 1);

			m_isGunColorBoom = 0;	//彩色弹标志初始化

			m_gameCounter = 1;
			_XSound::GetInstance().play(m_soundTextMove_01);
		}else
		if(m_gameCounter == 1)
		{//文字进入
			m_gameHeadTextY += 0.001f * interval;
			if(m_gameHeadTextY >= PI_HALF)
			{
				m_gameHeadTextY = PI_HALF;
				m_gameCounter = 2;
			}
			m_gameHeadText.setPosition(500.0f,920.0f - (920.0f - 280.0f) * sin(m_gameHeadTextY));
			m_gameHeadBGLevelInfo.setPosition(400.0f,920.0f - (920.0f - 312.0f) * sin(m_gameHeadTextY));
		}else
		if(m_gameCounter == 2)
		{//数字出现
			if(m_gameHeadAlpha < 1.0f)
			{//颜色变化
				m_gameHeadAlpha += 0.01f * interval;
				if(m_gameHeadAlpha > 1.0f) m_gameHeadAlpha = 1.0f;
				m_gameHeadNum.setColor(-1,-1,-1,m_gameHeadAlpha);
			}
			m_gameHeadSize += 0.005f * interval;
			float tempSize = 1.0f;
			if(m_gameHeadSize >= PI * 5.0f / 6.0f)
			{
				m_gameHeadTextY = 0.0f;
				m_gameHeadSize = PI * 5.0f / 6.0f;
				m_gameCounter = 3;
			//	//这里将游戏背景换掉
			//	m_backGround.release();
			//	char tempFileName[] = "normalResource/BG_00.jpg";
			//	tempFileName[7] = (m_nowLevel % 5) / 10 + '0';
			//	tempFileName[8] = ((m_nowLevel % 5) % 10) + '0';
			//	if(m_backGround.init(tempFileName,NULL,0,m_resoursePosition) == 0) exit(0);
				m_BGPicOrder = m_nowLevel % 5;

				m_gameHeadNum.setSize(1.0f,1.0f);
			}else
			{
				tempSize = sin(m_gameHeadSize) * 2.0f;
				m_gameHeadNum.setSize(tempSize,tempSize);
			}
		//	}
		//这里为了减少结余误差儿做修改
		//	tempSize = sin(m_gameHeadSize) * 2.0f;
		//	m_gameHeadNum.setSize(tempSize,tempSize);
			if(m_nowLevel + 1 >= 10)
			{
				m_gameHeadNum.setPosition(820.0f - 64.0f * (1.0f - tempSize),260.0f + 27.0f + 70.0f * (1.0f - tempSize));
			}else
			{
				m_gameHeadNum.setPosition(820.0f - 64.0f * (1.0f - tempSize),310.0f + 35.0f * (1.0f - tempSize));
			}
		}else
		if(m_gameCounter == 3)
		{//持续阶段
			m_gameHeadTextY += interval;
			if(m_gameHeadTextY > 2000)
			{
				m_gameHeadTextY = 0.0f;
				m_gameCounter = 4;
				_XSound::GetInstance().play(m_soundTextMove_00);
			}
		}else
		if(m_gameCounter == 4)
		{//消失阶段
			m_gameHeadAlpha -= 0.01f * interval;
			if(m_gameHeadAlpha < 0.0f) 
			{
				m_gameHeadAlpha = 0.0f;
			}
			m_gameHeadNum.setColor(-1,-1,-1,m_gameHeadAlpha);
			m_gameHeadTextY += 0.005f * interval;
			if(m_gameHeadTextY >= PI_HALF)
			{
				m_gameHeadTextY = PI_HALF;
				m_gameHeadSize = 0.0f;
				//下面是游戏进行的初始化
				if(m_gems->readMatrix(m_nowLevel) == 0) 
				{
					printf("数据读取错误！\n");
				}
				m_toolsFlag = 0;	//初始化道具
				m_isNeedTimer = m_gems->m_timerMode;
				m_gamePlayTimer = m_gems->m_timerSum;
				m_gameSetPlayerTimer = m_gems->m_timerSum;

				if(m_gems->m_helpSum > 0) 
				{
					m_helpStage = 1;	//根据需要显示提示内容
					setHelpPosition(m_gems->m_gem[m_gems->m_helpData[m_gems->m_nowHelpPoint].gemOrder].m_position.x,
						m_gems->m_gem[m_gems->m_helpData[m_gems->m_nowHelpPoint].gemOrder].m_position.y);
				}
				for(int i = 0;i < 13;++ i)
				{
					m_starFlag[i] = 0;
				}
				if(m_nowLevel % 5 == 0)
				{
					m_starFlag[0] = 1;
					m_starSpeed[0].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
					m_starPosition[0].set(600 + random(200),random(300));
					m_star[0].setPosition(m_starPosition[0]);
					int temp = random(100);
					if(temp < 30)
					{
						m_starFlag[5] = 1;
						m_starSpeed[5].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[5].set(100 + random(200),random(300));
						m_star[5].setPosition(m_starPosition[5]);
					}else
					if(temp < 60)
					{
						m_starFlag[6] = 1;
						m_starSpeed[6].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[6].set(100 + random(200),random(300));
						m_star[6].setPosition(m_starPosition[6]);
					}
				}else
				if(m_nowLevel % 5 == 1)
				{
					m_starFlag[1] = 1;
					m_starSpeed[1].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
					m_starPosition[1].set(600 + random(200),random(300));
					m_star[1].setPosition(m_starPosition[1]);
					int temp = random(100);
					if(temp < 30)
					{
						m_starFlag[5] = 1;
						m_starSpeed[5].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[5].set(100 + random(200),random(300));
						m_star[5].setPosition(m_starPosition[5]);
					}else
					if(temp < 60)
					{
						m_starFlag[6] = 1;
						m_starSpeed[6].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[6].set(100 + random(200),random(300));
						m_star[6].setPosition(m_starPosition[6]);
					}
				}else
				if(m_nowLevel % 5 == 2)
				{
					m_starFlag[2] = 1;
					m_starSpeed[2].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
					m_starPosition[2].set(600 + random(200),random(300));
					m_star[2].setPosition(m_starPosition[2]);
					int temp = random(100);
					if(temp < 30)
					{
						m_starFlag[5] = 1;
						m_starSpeed[5].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[5].set(100 + random(200),random(300));
						m_star[5].setPosition(m_starPosition[5]);
					}else
					if(temp < 60)
					{
						m_starFlag[6] = 1;
						m_starSpeed[6].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[6].set(100 + random(200),random(300));
						m_star[6].setPosition(m_starPosition[6]);
					}
				}else
				if(m_nowLevel % 5 == 3)
				{
					m_starFlag[3] = 1;
					m_starSpeed[3].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
					m_starPosition[3].set(600 + random(200),random(300));
					m_star[3].setPosition(m_starPosition[3]);
					int temp = random(100);
					if(temp < 30)
					{
						m_starFlag[5] = 1;
						m_starSpeed[5].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[5].set(100 + random(200),random(300));
						m_star[5].setPosition(m_starPosition[5]);
					}else
					if(temp < 60)
					{
						m_starFlag[6] = 1;
						m_starSpeed[6].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[6].set(100 + random(200),random(300));
						m_star[6].setPosition(m_starPosition[6]);
					}
				}else
				if(m_nowLevel % 5 == 4)
				{
					m_starFlag[4] = 1;
					m_starSpeed[4].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
					m_starPosition[4].set(600 + random(200),random(300));
					m_star[4].setPosition(m_starPosition[4]);
					int temp = random(100);
					if(temp < 30)
					{
						m_starFlag[5] = 1;
						m_starSpeed[5].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[5].set(100 + random(200),random(300));
						m_star[5].setPosition(m_starPosition[5]);
					}else
					if(temp < 60)
					{
						m_starFlag[6] = 1;
						m_starSpeed[6].set(-0.002f - random(3) * 0.001,-0.001f - random(2) * 0.001);
						m_starPosition[6].set(100 + random(200),random(300));
						m_star[6].setPosition(m_starPosition[6]);
					}
				}

				m_backGroungPosition = 0.0f;
				m_gameCounter = 5;
				_XSound::GetInstance().play(m_soundScreenUp);
			}
			m_gameHeadText.setPosition(500.0f,280.0f - (280.0f + 512.0f) * sin(m_gameHeadTextY));
			m_gameHeadBGLevelInfo.setPosition(400.0f,312.0f - (312.0f + 512.0f) * sin(m_gameHeadTextY));
		}else
		if(m_gameCounter == 5)
		{//准备开始游戏
			m_gameHeadSize += 0.001f * interval;
			if(m_gameHeadSize >= PI_HALF)
			{
				setGameTimerStart();	//开启计时
				m_gameHeadSize = PI_HALF;
				m_gameStage = GAME_STAGE_GAMEPLAY;
				m_gameCounter = 0;
				m_levelFlagStage = 2;
				if(_XMusic::GetInstance().fadeInMusic(BGMusicHandle[1 + (m_nowLevel % 5)],-1,200) == 0)
				{
				//	Sleep(100);
				//	_XMusic::GetInstance().fadeInMusic(BGMusicHandle[1 + (m_nowLevel % 5)],-1,200);
				}
				printf("Play music %d:\n",1 + (m_nowLevel % 5));
				if(m_nowLevel == 0)
				{//显示头像提示
					m_nowFaceOrder = 0;
					setFaceStart();
				}
				m_faceGameTimer = 0;
			}
			m_gameHeadBG.setPosition(1280.0f * sin(m_gameHeadSize),0);
		}
	}
}

int _XGun::gamePlayInit(_XResourcePosition resoursePosition)
{//下面是游戏过程中的一些文字的初始化
	if(m_gamePlayText00.init("ResourcePack/pic/UI/LevelOverText.png",resoursePosition) == 0) return 0;
	m_gamePlayText00.setAngle(90);
	m_gamePlayText00.setPosition(600.0f,300.0f);
	if(m_gamePlayText01.init("ResourcePack/pic/UI/LevelOverText_00.png",resoursePosition) == 0) return 0;
	m_gamePlayText01.setAngle(90);
	m_gamePlayText01.setPosition(500.0f,300.0f);
	if(m_gamePlayText02.init("ResourcePack/pic/UI/LevelOverText_12.png",resoursePosition) == 0) return 0;
	m_gamePlayText02.setAngle(90);
	m_gamePlayText02.setPosition(400.0f,100.0f);

	if(m_gamePlayNum.init("ResourcePack/pic/UI/BigNumber.png",_XVector2(64,128),_XVector2(10,1),resoursePosition) == 0) return 0;
	//m_gamePlayNum.setACopy(m_toolsTimerText);			//数字
	m_gamePlayNum.setAngle(90);
	m_gamePlayNum.setPosition(720.0f,280.0f);
	m_gamePlayTicketSum = 0;
	m_gamePlayNum.setNumber(m_gamePlayTicketSum);
	if(m_gamePlayLogo00.init("ResourcePack/pic/UI/scoreLogo.png",resoursePosition) == 0) return 0;
	m_gamePlayLogo00.setAngle(90);
	m_gamePlayLogo00.setPosition(400.0f,550.0f);
	m_gamePlayLogo01.setACopy(m_goldBoxTicketLogo);		//图标01
	m_gamePlayLogo01.setAngle(90.0f);
	m_gamePlayLogo01.setPosition(400.0f,550.0f);

	return 1;
}

void _XGun::gamePlayDraw()
{
	if(m_gameStage == GAME_STAGE_GAMEPLAY && m_gameCounter > 0)
	{
		m_gamePlayText00.draw();
		m_gamePlayText01.draw();
		m_gamePlayText02.draw();
		m_gamePlayNum.draw();
		if(m_configData.isTicketNeed != 0)
		{
			m_gamePlayLogo01.draw();
		}else
		{
			m_gamePlayLogo00.draw();
		}
	}
}

void _XGun::gamePlayMove(int interval)
{
	interval = (float)interval / buffTimerRate();
	if(m_gameStage == GAME_STAGE_GAMEPLAY)
	{
		if(m_gameCounter == 1)
		{//初始化
			m_gamePlayAlpha = 0.0f;
			m_gamePlaySize = 10.0f;
			m_gamePlayTextY = 0.0f;
			m_gamePlayText00.setAlpha(m_gamePlayAlpha);
			m_gamePlayText00.setSize(m_gamePlaySize,m_gamePlaySize);
			m_gamePlayText00.setPosition(600.0f,300.0f);
			m_gamePlayText01.setPosition(500.0f,920.0f);
			m_gamePlayText02.setPosition(400.0f,920.0f);

			m_gamePlayNum.setPosition(715.0f,920.0f);
			m_gamePlayLogo00.setPosition(600.0f,920.0f);
			m_gamePlayLogo01.setPosition(606.0f,920.0f);
			m_gameCounter = 2;
			setGameTimerEnd(0);
			_XSound::GetInstance().play(m_soundTextMove_01);
		}else
		if(m_gameCounter == 2)
		{//0文字出现
			if(m_gamePlayAlpha < 1.0f)
			{
				m_gamePlayAlpha += 0.01f * interval;
				if(m_gamePlayAlpha > 1.0f) m_gamePlayAlpha = 1.0f;
			}
			m_gamePlaySize *= pow(0.98f,0.3f * interval);
			if(m_gamePlaySize <= 1.0f)
			{
				m_gamePlayAlpha = 1.0f;
				m_gamePlaySize = 1.0f;
				m_gameCounter = 3;
				_XSound::GetInstance().play(m_soundTextMove_01);
			}
			m_gamePlayText00.setAlpha(m_gamePlayAlpha);
			m_gamePlayText00.setSize(m_gamePlaySize,m_gamePlaySize);
		}else
		if(m_gameCounter == 3)
		{//1文字出现
			m_gamePlayTextY += 0.002f * interval;
			if(m_gamePlayTextY >= PI * 0.5f)
			{
				m_gamePlayTextY = PI * 0.5f;
				m_gameCounter = 4;
				
				if(m_configData.isTicketNeed != 0)
				{//增加彩票
					if(m_configData.ticketSum == 0) m_gamePlayTicketSum = (m_nowLevel*4.0f / 10.0f + 4) * 0.25f;
					else if(m_configData.ticketSum == 1) m_gamePlayTicketSum = (m_nowLevel*4.0f / 10.0f + 4) * 0.5f;
					else m_gamePlayTicketSum = (m_nowLevel*4.0f / 10.0f + 4) * 1.0f;
					m_gamePlayNum.setNumber(m_gamePlayTicketSum);
				}else
				{//增加分数
					m_gamePlayNum.setNumber(LEVEL_OVER_ADD_SCORE);
				}
			}
			m_gamePlayText01.setPosition(500.0f,920.0f - (920.0f - 300.0f) * sin(m_gamePlayTextY));
		}else
		if(m_gameCounter == 4)
		{//2文字出现
			m_gamePlayTextY += 0.002f * interval;
			if(m_gamePlayTextY >= PI)
			{
				m_gamePlayTextY = PI;
				m_gamePlayAlpha = 0.0f;
				m_gameCounter = 5;
				if(m_configData.isTicketNeed != 0)
				{//增加彩票
					addTicket((int)(m_nowLevel*4.0f / 10.0f + 4)); //这个数值需要调整
				}else
				{//增加分数
					for(int i = 0;i < MAX_BULLET_SUM;++ i)
					{
						if(m_goldAddFlag[i] == 0)
						{
							char ch[20];
							sprintf(ch,"+%d",LEVEL_OVER_ADD_SCORE);
							m_goldAddNum[i].setNumber(ch);
							m_goldAddFlag[i] = 1;
							m_goldAddBasicX[i] = 150;
							m_goldAddBasicY[i] = -10;
							break;
						}
					}
					m_scoresArmSum += LEVEL_OVER_ADD_SCORE;
					getNowOrder();
					updateGameOrder();
				}
			}
			if(m_configData.isTicketNeed != 0)
			{//增加彩票
				m_gamePlayText02.setPosition(400.0f,920.0f - (920.0f - 200.0f) * sin(m_gamePlayTextY - PI * 0.5f));
				if(m_gamePlayTicketSum >= 10) m_gamePlayNum.setPosition(715.0f,920.0f - (920.0f - 345.0f) * sin(m_gamePlayTextY - PI * 0.5f));
				else m_gamePlayNum.setPosition(715.0f,920.0f - (920.0f - 385.0f) * sin(m_gamePlayTextY - PI * 0.5f));
				m_gamePlayLogo01.setPosition(606.0f,920.0f - (920.0f - 464.0f) * sin(m_gamePlayTextY - PI * 0.5f));
			}else
			{//增加分数
				m_gamePlayText02.setPosition(400.0f,920.0f - (920.0f - 100.0f) * sin(m_gamePlayTextY - PI * 0.5f));
				m_gamePlayNum.setPosition(720.0f,920.0f - (920.0f - 280.0f) * sin(m_gamePlayTextY - PI * 0.5f));
				m_gamePlayLogo00.setPosition(600.0f,920.0f - (920.0f - 550.0f) * sin(m_gamePlayTextY - PI * 0.5f));
			}
		}else
		if(m_gameCounter == 5)
		{//持续
			m_gamePlayAlpha += interval;
			if(m_gamePlayAlpha >= 2000)
			{
				m_gamePlayTextY = 0;
				m_gameCounter = 6;
				_XSound::GetInstance().play(m_soundTextMove_00);
			}
		}else
		if(m_gameCounter == 6)
		{//消失
			m_gamePlayTextY += 0.004f * interval;
			if(m_gamePlayTextY >= PI)
			{
				m_gamePlayTextY = PI;
				m_gameCounter = 7;
			}
			if(m_gamePlayTextY > 0.0f && m_gamePlayTextY < PI * 0.5f)
			{
				m_gamePlayText00.setPosition(600.0f,300.0f - (300.0f + 512.0f) * sin(m_gamePlayTextY));
			}
			if(m_gamePlayTextY > PI * 0.25f && m_gamePlayTextY < PI * 0.75f)
			{
				m_gamePlayText01.setPosition(500.0f,300.0f - (300.0f + 512.0f) * sin(m_gamePlayTextY - PI * 0.25f));
			}
			if(m_gamePlayTextY > PI * 0.5f && m_gamePlayTextY < PI)
			{
				if(m_configData.isTicketNeed != 0)
				{//增加彩票
					m_gamePlayText02.setPosition(400.0f,100.0f - (100.0f + 512.0f) * sin(m_gamePlayTextY - PI * 0.5f));
					if(m_gamePlayTicketSum >= 10) m_gamePlayNum.setPosition(715.0f,345.0f - (345.0f + 512.0f) * sin(m_gamePlayTextY - PI * 0.5f));
					else m_gamePlayNum.setPosition(715.0f,385.0f - (385.0f + 512.0f) * sin(m_gamePlayTextY - PI * 0.5f));
					m_gamePlayLogo01.setPosition(606.0f,464.0f - (464.0f + 512.0f) * sin(m_gamePlayTextY - PI * 0.5f));
				}else
				{//增加分数
					m_gamePlayText02.setPosition(400.0f,100.0f - (100.0f + 512.0f) * sin(m_gamePlayTextY - PI * 0.5f));
					m_gamePlayNum.setPosition(720.0f,280.0f - (280.0f + 512.0f) * sin(m_gamePlayTextY - PI * 0.5f));
					m_gamePlayLogo00.setPosition(600.0f,550.0f - (550.0f + 512.0f) * sin(m_gamePlayTextY - PI * 0.5f));
				}
			}
		}
		if(m_gameCounter > 1)
		{
			m_gamePlayOverTimer -= interval;
			if(m_gamePlayOverTimer <= 0)
			{
				m_gameStage = GAME_STAGE_GAMEWIN;
				m_gameCounter = 0;
			}
		}
	}
}

void _XGun::gameWinDraw()
{
	if(m_gameStage == GAME_STAGE_GAMEWIN)
	{
		char temp[128];
		m_gameWinBG.draw();
		m_gameWinText00.draw();
		if(m_gameCounter >= 3)
		{
			sprintf(temp,"%.1f%%",m_gameWinNum_00_00 * 100.0f);
			m_gameWinNum.setNumber(temp);
			m_gameWinNum.setPosition(810.0f + 25.0f,380.0f - 9.0f);
			m_gameWinNum.draw();
		}
		m_gameWinText01.draw();
		if(m_gameCounter >= 5)
		{
			int tempSum = 0;
			for(int i = 0;i < m_gameWinNum_01_01 * 5;++ i)
			{
				m_gameWinStar.setPosition(695.0f + 20.0f,290.0f + 58.0f * i);
				if(m_gameWinNum_01_00 - 0.90f >= 0.025f * i) 
				{
					m_gameWinStar.draw(&(m_gameWinStar_00));
					tempSum ++;
				}
				else m_gameWinStar.draw(&(m_gameWinStar_01));
			}
			if(tempSum != m_soundStarSumOld)
			{
				m_soundStarSumOld = tempSum;
				_XSound::GetInstance().play(m_soundStarAdd);
			}
		}
		m_gameWinText02.draw();
		if(m_gameCounter >= 7)
		{
			sprintf(temp,"%.0f",m_gameWinNum_02_00);
			m_gameWinNum.setNumber(temp);
			m_gameWinNum.setPosition(810.0f - 120.0f + 15.0f,460.0f - 17.0f);
			m_gameWinNum.draw();
		}
		m_gameWinText03.draw();
		if(m_gameCounter >= 9)
		{
			sprintf(temp,"%.0f",m_gameWinNum_03_00);
			m_gameWinNum.setNumber(temp);
			m_gameWinNum.setPosition(810.0f - 180.0f + 10.0f,460.0f - 17.0f);
			m_gameWinNum.draw();
		}
		m_gameWinText04.draw();
		m_gameWinBGUp.draw();
		if(m_gameCounter >= 11)
		{
		//	if(m_nowLevel == 0) m_gameWinText05.draw(); else
		//	if(m_nowLevel == 1) m_gameWinText06.draw(); else
		//	if(m_nowLevel == 2) m_gameWinText07.draw();
			m_gameWinText05.draw();
			m_gameWinText06.draw();
			m_gameWinText07.draw();
			m_gameWinText08.draw();
			m_gameWinText09.draw();
		}
		if(m_gameCounter >= 13)
		{
			m_gameHeadBG.drawWithoutBlend();
		}
	}
}

void _XGun::gameWinMove(int interval)
{
	interval = (float)(interval) / buffTimerRate();
	if(m_gameStage == GAME_STAGE_GAMEWIN)
	{
		if(m_gameCounter == 0)
		{//初始化
			m_gameWinBGAlpha = 0.0f;
			m_gameWinTextY = 0.0f;
			m_gameWinBG.setAlpha(m_gameWinBGAlpha);
			m_gameWinBGUp.setAlpha(m_gameWinBGAlpha);
			m_gameWinText00.setPosition(530.0f + 25.0f,920.0f - 9.0f);
			m_gameWinText01.setPosition(470.0f + 20.0f,920.0f);
			m_gameWinText02.setPosition(410.0f + 15.0f,920.0f - 17.0f);
			m_gameWinText03.setPosition(350.0f + 10.0f,920.0f - 17.0f);
			m_gameWinText04.setPosition(290.0f + 5.0f,920.0f - 17.0f);
			m_gameWinText05.setPosition(235.0f,920.0f);
			m_gameWinText06.setPosition(235.0f,920.0f);
			m_gameWinText07.setPosition(235.0f,920.0f);
			m_gameWinText08.setPosition(235.0f,920.0f);
			m_gameWinText09.setPosition(235.0f,920.0f);
			m_gameCounter = -1;
			_XMusic::GetInstance().fadeOutMusic(800);
			//显示头像
			if(m_faceGameTimer < 10000) m_nowFaceOrder = 1;
			else if(m_faceGameTimer < 30000) m_nowFaceOrder = 2;
			else if(m_faceGameTimer < 60000) m_nowFaceOrder = 3;
			else if(m_faceGameTimer < 90000) m_nowFaceOrder = 4;
			else m_nowFaceOrder = 5;

			printf("GameTiemr:%d\n",m_faceGameTimer/1000);
			setFaceStart();
		}else
		if(m_gameCounter == -1)
		{//等待所有子弹消失
			int flag = 0;
			for(int i = 0;i < MAX_BULLET_SUM;++ i)
			{
				if(m_bullet[i].getCanCrash() == 1)
				{
					flag = 1;
				}
			}
			if(flag == 0)
			{
				m_gameCounter = 1;
				clearAllBuff();
			}
		}else
		if(m_gameCounter == 1)
		{//背景出现
			m_gameWinBGAlpha += 0.01f * interval;
			if(m_gameWinBGAlpha >= 1.0f)
			{
				m_gameWinBGAlpha = 1.0f;
				m_gameCounter = 2;
				_XSound::GetInstance().play(m_soundTextMove_01);
			}
			m_gameWinBG.setAlpha(m_gameWinBGAlpha);
			m_gameWinBGUp.setAlpha(m_gameWinBGAlpha);
		}else
		if(m_gameCounter == 2)
		{//第一行出现
			m_gameWinTextY += 0.01f * interval;
			if(m_gameWinTextY >= PI * 0.5f)
			{
				m_gameWinTextY = PI * 0.5f;
				m_gameCounter = 3;
				m_soundSumAddTimer = 0;
				m_gameWinNum_00_00 = 0;
				m_gameWinNum_00_01 = m_levelFiniedArmSum;
			}
			m_gameWinText00.setPosition(530.0f + 25.0f,920.0f - (920.0f - 170.0f) * sin(m_gameWinTextY) - 9.0f);
		}else
		if(m_gameCounter == 3)
		{//显示第一行的文字
			float tempSum;
			//if(m_gameWinNum_00_01 >= m_gameWinNum_00_00)
			if(abs(m_gameWinNum_00_01 - m_gameWinNum_00_00) > 0.0001f)
			{
				tempSum = m_gameWinNum_00_01 - m_gameWinNum_00_00;
				if(0.0015f * interval < 1.0f)
				{
					tempSum *= 0.0015f * interval;
				}
				if(tempSum < 0.0001f && tempSum > -0.0001f)
				{
					if(tempSum < 0) tempSum = -0.0001f;
					else tempSum = 0.0001f;
				}
				m_gameWinNum_00_00 += tempSum;
				if(m_soundSumAddTimer == 0)
				{
					_XSound::GetInstance().play(m_soundSumAdd_00);
				}
				m_soundSumAddTimer += interval;
				if(m_soundSumAddTimer > 80) m_soundSumAddTimer = 0;
			}else
			{//如果这里达到了100%的完成量的话，就有一个奖励
				if(m_gameWinNum_00_01 >= 1.0f)
				{
					for(int i = 0;i < MAX_BULLET_SUM;++ i)
					{
						if(m_goldAddFlag[i] == 0)
						{
							m_goldAddNum[i].setNumber("+10000");
							m_goldAddFlag[i] = 1;
							m_goldAddBasicX[i] = 100 + 317;
							m_goldAddBasicY[i] = 1070;
							break;
						}
					}
					m_scoresArmSum += 10000;
					getNowOrder();
					updateGameOrder();
					m_toolsEffectStage[0] = 1;
				}
				m_gameWinTextY = 0;
				m_gameCounter = 4;
				_XSound::GetInstance().play(m_soundTextMove_01);
			}
		}else
		if(m_gameCounter == 4)
		{//第二行出现
			m_gameWinTextY += 0.01f * interval;
			if(m_gameWinTextY >= PI * 0.5f)
			{
				m_gameWinTextY = PI * 0.5f;
				m_gameCounter = 5;
				m_soundStarSumOld = 0;
				m_gameWinNum_01_00 = m_levelFiniedArmSum;	//评分的计算公式
				m_gameWinNum_01_01 = 0.0f;
			}
			m_gameWinText01.setPosition(470.0f + 20.0f,920.0f - (920.0f - 170.0f) * sin(m_gameWinTextY));
		}else
		if(m_gameCounter == 5)
		{//显示第二行的内容
			m_gameWinNum_01_01 += 0.0005f * interval;
			if(m_gameWinNum_01_01 >= 1.0f)
			{
				m_gameWinNum_01_01 = 1.0f;
				m_gameWinTextY = 0.0f;
				m_gameCounter = 6;
				_XSound::GetInstance().play(m_soundTextMove_01);
			}
		}else
		if(m_gameCounter == 6)
		{//第三行出现
			m_gameWinTextY += 0.01f * interval;
			if(m_gameWinTextY >= PI * 0.5f)
			{
				m_gameWinTextY = PI * 0.5f;
				m_gameCounter = 7;
				m_soundSumAddTimer = 0;
				m_gameWinNum_02_00 = 0;
				m_gameWinNum_02_01 = m_goldArmSum;
			}
			m_gameWinText02.setPosition(410.0f + 15.0f,920.0f - (920.0f - 200.0f) * sin(m_gameWinTextY) - 17.0f);
		}else
		if(m_gameCounter == 7)
		{//显示第三行的文字
			float tempSum;
			if(m_gameWinNum_02_01 > m_gameWinNum_02_00)
			{
				tempSum = m_gameWinNum_02_01 - m_gameWinNum_02_00;
				if(0.0015f * interval < 1.0f)
				{
					tempSum *= 0.0015f * interval;
				}

				if(tempSum < 1.0f && tempSum > -1.0f)
				{
					if(tempSum < 0) tempSum = -1.0f;
					else tempSum = 1.0f;
				}
				m_gameWinNum_02_00 += tempSum;
				if(m_soundSumAddTimer == 0)
				{
					_XSound::GetInstance().play(m_soundSumAdd_00);
				}
				m_soundSumAddTimer += interval;
				if(m_soundSumAddTimer > 80) m_soundSumAddTimer = 0;
			}else
			{
				m_gameWinTextY = 0;
				m_gameCounter = 8;
				_XSound::GetInstance().play(m_soundTextMove_01);
			}
		}else
		if(m_gameCounter == 8)
		{//第四行出现
			m_gameWinTextY += 0.01f * interval;
			if(m_gameWinTextY >= PI * 0.5f)
			{
				m_gameWinTextY = PI * 0.5f;
				m_gameCounter = 9;
				m_soundSumAddTimer = 0;
				m_gameWinNum_03_00 = 0;
				m_gameWinNum_03_01 = (int)(m_goldArmSum / (9 - ((m_gameWinNum_01_00 - 0.9f) / 0.025f)));
			}
			m_gameWinText03.setPosition(350.0f + 10.0f,920.0f - (920.0f - 200.0f) * sin(m_gameWinTextY) - 17.0f);
		}else
		if(m_gameCounter == 9)
		{//显示第四行的文字
			float tempSum;
			if(m_gameWinNum_03_01 > m_gameWinNum_03_00)
			{
				tempSum = m_gameWinNum_03_01 - m_gameWinNum_03_00;
				if(0.0075f * interval < 1.0f)
				{
					tempSum *= 0.0075f * interval;
				}
				if(tempSum < 1.0f && tempSum > -1.0f)
				{
					if(tempSum < 0) tempSum = -1.0f;
					else tempSum = 1.0f;
				}
				m_gameWinNum_03_00 += tempSum;
				if(m_soundSumAddTimer == 0)
				{
					_XSound::GetInstance().play(m_soundSumAdd_00);
				}
				m_soundSumAddTimer += interval;
				if(m_soundSumAddTimer > 80) m_soundSumAddTimer = 0;

			}else
			{
				m_gameWinTextY = 0.0f;
				energyAdd(m_gameWinNum_03_01);
				//m_energyArmSum += m_goldArmSum / (9 - ((m_gameWinNum_01_00 - 0.9f) / 0.025f));
				m_goldArmSum = 0;
				if(m_nowLevel == 0
					|| m_nowLevel == 1
					|| m_nowLevel == 2
					|| m_nowLevel == 3
					|| m_nowLevel == 4)
				{//只有第一关才会有额外奖励
					m_gameCounter = 10;
					_XSound::GetInstance().play(m_soundTextMove_01);
				}else
				{
					m_gameCounter = 12;
				}
			}
		}else
		if(m_gameCounter == 10)
		{//第五行出现
			m_gameWinTextY += 0.002f * interval;
			if(m_gameWinTextY >= PI * 0.75f)
			{
				m_gameWinTextY = PI * 0.75f;
				m_gameCounter = 11;
				m_gameWinNum_04_00 = 10.0f;
				m_gameWinNum_04_01 = 0;
				if(m_nowLevel == 0)
				{
					m_gameWinText05.setPosition(215.0f,310.0f);
					m_gameWinText05.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
					m_gameWinText05.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
				}else
				if(m_nowLevel == 1)
				{
					m_gameWinText06.setPosition(215.0f,310.0f);
					m_gameWinText06.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
					m_gameWinText06.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
				}else
				if(m_nowLevel == 2)
				{
					m_gameWinText07.setPosition(215.0f,310.0f);
					m_gameWinText07.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
					m_gameWinText07.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
				}else
				if(m_nowLevel == 3)
				{
					m_gameWinText08.setPosition(215.0f,310.0f);
					m_gameWinText08.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
					m_gameWinText08.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
				}else
				if(m_nowLevel == 4)
				{
					m_gameWinText09.setPosition(215.0f,310.0f);
					m_gameWinText09.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
					m_gameWinText09.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
				}
			}
			if(m_gameWinTextY <= PI * 0.5f)
			{
				m_gameWinText04.setPosition(290.0f + 5.0f,920.0f - (920.0f - 200.0f) * sin(m_gameWinTextY) - 17.0f);
			}
		}else
		if(m_gameCounter == 11)
		{//第六行出现
			if(m_gameWinNum_04_00 >= 1.0f)
			{
				m_gameWinNum_04_00 *= pow(0.98f,interval * 0.3f);
				if(m_gameWinNum_04_00 < 1.0f) 
				{
					m_gameWinNum_04_00 = 1.0f;
					m_gameWinTextY = 0.0f;
					m_gameCounter = 12;
					if(m_nowLevel >= 4) m_canUserGunType = GUN_TYPE_05;else 
					if(m_nowLevel >= 3) m_canUserGunType = GUN_TYPE_04;else 
					if(m_nowLevel >= 2) m_canUserGunType = GUN_TYPE_03;else 
					if(m_nowLevel >= 1) m_canUserGunType = GUN_TYPE_02;else 
					if(m_nowLevel >= 0) m_canUserGunType = GUN_TYPE_01; 
					else m_canUserGunType = GUN_TYPE_00; 
					_XSound::GetInstance().play(m_soundTextDown);
				}
			}
			if(m_nowLevel == 0)
			{
				m_gameWinText05.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
				m_gameWinText05.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
			}else
			if(m_nowLevel == 1)
			{
				m_gameWinText06.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
				m_gameWinText06.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
			}else
			if(m_nowLevel == 2)
			{
				m_gameWinText07.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
				m_gameWinText07.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
			}else
			if(m_nowLevel == 3)
			{
				m_gameWinText08.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
				m_gameWinText08.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
			}else
			if(m_nowLevel == 4)
			{
				m_gameWinText09.setSize(m_gameWinNum_04_00,m_gameWinNum_04_00);
				m_gameWinText09.setAngle(85 + m_gameWinNum_04_00 * 10.0f);
			}
		}else
		if(m_gameCounter == 12)
		{//持续一段时间
			m_gameWinTextY += interval;
			if(m_gameWinTextY > 4000)
			{
				m_gameWinTextY = 0.0f;
				m_nowLevel ++;
				//if(m_nowLevel >= m_gems->m_matrixSum)
				if(m_configData.levelSum < 4)
				{
					if(m_nowLevel >= m_configData.levelSum + 6)
					{//游戏结束，全部完成
						setGameTimerEnd(1);
						m_gameStage = GAME_STAGE_GAMEALLOVER;
						m_gameCounter = 0;
					}else
					{
						m_gameCounter = 13;
						_XSound::GetInstance().play(m_soundScreenDown);
					}
				}else
				{
					if(m_nowLevel >= m_gems->m_matrixSum - 10 + 1)
					{//游戏结束，全部完成
						setGameTimerEnd(1);
						m_gameStage = GAME_STAGE_GAMEALLOVER;
						m_gameCounter = 0;
					}else
					{
						m_gameCounter = 13;
						_XSound::GetInstance().play(m_soundScreenDown);
					}
				}
			}
		}else
		if(m_gameCounter == 13)
		{//落幕
			m_gameWinTextY += 0.002f * interval;
			if(m_gameWinTextY >= PI * 0.5f)
			{
				setHelpEnd();
				setGameTimerEnd(1);
				m_gameWinTextY = PI * 0.5f;
				m_gameStage = GAME_STAGE_GAMEHEAD;
				m_gameCounter = 0;
				m_levelFiniedArmSum = 0.0f;
			}
			m_gameHeadBG.setPosition(1280.0f * cos(m_gameWinTextY),0.0f);
		}
	}
}

void _XGun::drawDown()
{
	if(m_isInited == 0) return;
	if(m_BGPicOrder == 0) 
	{
		m_backGround_00.setPosition(1280.0f - m_backGroungPosition,0.0f);
		m_backGround_00.drawWithoutBlend();
		m_backGround_00.setPosition(0.0f  - m_backGroungPosition,0.0f);
		m_backGround_00.drawWithoutBlend();
	}else
	if(m_BGPicOrder == 1) 
	{
		m_backGround_01.setPosition(1280.0f - m_backGroungPosition,0.0f);
		m_backGround_01.drawWithoutBlend();
		m_backGround_01.setPosition(0.0f  - m_backGroungPosition,0.0f);
		m_backGround_01.drawWithoutBlend();
	}else
	if(m_BGPicOrder == 2) 
	{
		m_backGround_02.setPosition(1280.0f - m_backGroungPosition,0.0f);
		m_backGround_02.drawWithoutBlend();
		m_backGround_02.setPosition(0.0f - m_backGroungPosition,0.0f);
		m_backGround_02.drawWithoutBlend();
	}else
	if(m_BGPicOrder == 3) 
	{
		m_backGround_03.setPosition(1280.0f - m_backGroungPosition,0.0f);
		m_backGround_03.drawWithoutBlend();
		m_backGround_03.setPosition(0.0f  - m_backGroungPosition,0.0f);
		m_backGround_03.drawWithoutBlend();
	}else
	{
		m_backGround_04.setPosition(1280.0f - m_backGroungPosition,0.0f);
		m_backGround_04.drawWithoutBlend();
		m_backGround_04.setPosition(0.0f  - m_backGroungPosition,0.0f);
		m_backGround_04.drawWithoutBlend();
	}
	for(int i = 0;i < 13;++ i)
	{
		if(m_starFlag[i] != 0)
		{
			m_star[i].draw();
		}
	}
	smallStarDraw();	//描绘小行星

	//int temp = 256;
	//int tempColor;
	//for(int i = 0;i < 10;i++)
	//{
	//	tempColor = (m_backGroundColor + i) % 24;
	//	if(tempColor <= 16)
	//	{
	//		tempColor = temp << tempColor;
	//	}else
	//	{
	//		tempColor = (temp << tempColor) + (temp >> (24 - tempColor));
	//	}
	//	drawLine(0,13 - i,1280,13 - i,1,1.0f - ((tempColor >> 16) % 256) / 255.0f,1.0f - ((tempColor >> 8) % 256)/ 255.0f,1.0f - (tempColor % 256) / 255.0f,1.0f - 0.1 * i);
	//	drawLine(0,720 - 13 + i,1280,720 - 13 + i,1,1.0f - ((tempColor >> 16) % 256) / 255.0f,1.0f - ((tempColor >> 8) % 256) / 255.0f,1.0f - (tempColor % 256) / 255.0f,1.0f - 0.1 * i);
	//}
	m_BGSider[0].draw();
	m_BGSider[1].draw();
}

void _XGun::draw()
{
	if(m_isInited == 0) return;
#if USE_FBO
	GLuint tempTex;
	static int tempUpFlag = 0;
	if(m_fboShakeData.getIsEnd() != 0)
	{
	//	m_fbo.useFBO(1280,720);
		m_fbo.useFBO();
		m_fbo.attachTex(m_fboOrder);
		if(tempUpFlag == 0)
		{
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);
		}else
		{
			if(m_fboOrder == 0) tempTex = m_fbo.getTexture(1);
			else tempTex = m_fbo.getTexture(0);
#if USE_SHADER
			m_fboSprite.setBlendType(1,0);
			m_fboSprite.setShaderClass(&m_tmpSader);
			m_tmpSader.connectTexture("Texture",&tempTex);
			//m_fboSprite.m_pShaderProc = useShader;	//设置shader效果
#else
			m_fboSprite.setBlendType(1,0);
			m_fboSprite.setAlpha(0.9f);
#endif
			m_fboSprite.draw(&tempTex);
		}
	}
	tempUpFlag = m_fboShakeData.getIsEnd();
#endif

	if(m_nowLevel == 0)
	{
		drawLine(0.0f + m_lineStartX * cos((m_gunAngle - 90.0f) * ANGLE_TO_RADIAN),360.0f + m_lineStartX * sin((m_gunAngle - 90.0f) * ANGLE_TO_RADIAN),
			0.0f + (1280.0f + m_lineStartX) * cos((m_gunAngle - 90.0f) * ANGLE_TO_RADIAN),
			360.0f + (1280.0f + m_lineStartX) * sin((m_gunAngle - 90.0f) * ANGLE_TO_RADIAN),
			3,1.0f,1.0f,1.0f,0.5f,1);
	}

	toolsEffectDraw();

	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
	//	m_bullet[i].m_bulletSprite_00.setBlendType(4,0);
		m_bullet[i].draw();
	}
	gameTimerDraw();
	if(m_PrograssAlpha > 0.85)
	{
		m_nowLevelProcessLight.draw();
	}

	gameOrderDraw();	//显示排行榜的信息

	m_nowLevelProcessB.draw();
	m_nowLevelProcess.draw();
	m_nowLevelProcessU.draw();

	gameOrderDrawUp();	//显示排行榜的信息

//	m_nowLevelProcessN.draw();
	if(m_levelFlagStage != 4)
	{
		m_nowLevelProcessLage.draw();
	}else
	{
		if(m_levelProcessBoom.getIsEnd() == 0) 
		{
			m_levelProcessBoom.draw();
			//printf("haha\n");
		}
	}
	helpDraw();

	//显示分数
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		if(m_goldAddFlag[i] > 1)
		{
			m_goldAddNum[i].draw();
		}
	}
	//显示金币在飞
	for(int i = 0;i < MAX_BULLET_SUM * 10;++ i)
	{
		if(m_goldFlyFlag[i] > 1)
		{
			m_goldFly[i].draw();
		}
	}
	
	//显示分数栏
	m_goldSprite.draw();
//	m_scoresSprite.draw();
	m_scoresText.draw();
	m_scoresNum.draw();
	
	m_energyBarDown.draw();
	m_energyBar.draw();
	m_energySprite.draw();
	m_energyNum.draw();

	if(m_isEnergyGlare != 0) m_lowEnergy.draw();
	m_goldNum.draw();

	if(m_gunIsFlash != 0)
	{
		m_gunSprite.setBlendType(4,5);
		if(m_gunType == GUN_TYPE_00) m_gunSprite.draw(&(m_gunTex_00)); else 
		if(m_gunType == GUN_TYPE_01) m_gunSprite.draw(&(m_gunTex_01)); else 
		if(m_gunType == GUN_TYPE_02) m_gunSprite.draw(&(m_gunTex_02)); else 
		if(m_gunType == GUN_TYPE_03) m_gunSprite.draw(&(m_gunTex_03)); else 
		if(m_gunType == GUN_TYPE_04) m_gunSprite.draw(&(m_gunTex_04)); else 
		if(m_gunType == GUN_TYPE_05) m_gunSprite.draw(&(m_gunTex_05)); 
		m_gunSprite.setBlendType(4,1);
		if(m_gunType == GUN_TYPE_00) m_gunSprite.draw(&(m_gunTex_00)); else 
		if(m_gunType == GUN_TYPE_01) m_gunSprite.draw(&(m_gunTex_01)); else 
		if(m_gunType == GUN_TYPE_02) m_gunSprite.draw(&(m_gunTex_02)); else 
		if(m_gunType == GUN_TYPE_03) m_gunSprite.draw(&(m_gunTex_03)); else 
		if(m_gunType == GUN_TYPE_04) m_gunSprite.draw(&(m_gunTex_04)); else 
		if(m_gunType == GUN_TYPE_05) m_gunSprite.draw(&(m_gunTex_05)); 
	}else
	{
		m_gunSprite.setBlendType(4,5);
		if(m_gunType == GUN_TYPE_00) m_gunSprite.draw(&(m_gunTex_00)); else 
		if(m_gunType == GUN_TYPE_01) m_gunSprite.draw(&(m_gunTex_01)); else 
		if(m_gunType == GUN_TYPE_02) m_gunSprite.draw(&(m_gunTex_02)); else 
		if(m_gunType == GUN_TYPE_03) m_gunSprite.draw(&(m_gunTex_03)); else 
		if(m_gunType == GUN_TYPE_04) m_gunSprite.draw(&(m_gunTex_04)); else 
		if(m_gunType == GUN_TYPE_05) m_gunSprite.draw(&(m_gunTex_05)); 
	}
	if(m_toolsBoomFlag != 0) m_toolsBoom.draw();

	energyAddDraw();

	if(m_gameStage == GAME_STAGE_GAMEPLAY)
	{
		if(m_gunFireAlphaStage != 0)
		{
			if(m_gunFireType == GUN_TYPE_00)
			{
				m_gunFireSprite_00.draw(&(m_gunFireTex_00));
			}else
			if(m_gunFireType == GUN_TYPE_01)
			{
				m_gunFireSprite_00.draw(&(m_gunFireTex_01));
			}else
			if(m_gunFireType == GUN_TYPE_02)
			{
				m_gunFireSprite_00.draw(&(m_gunFireTex_02));
			}else
			if(m_gunFireType == GUN_TYPE_03)
			{
				m_gunFireSprite_00.draw(&(m_gunFireTex_03));
			}else
			if(m_gunFireType == GUN_TYPE_04)
			{
				m_gunFireSprite_00.draw(&(m_gunFireTex_04));
			}else
			if(m_gunFireType == GUN_TYPE_05)
			{
				m_gunFireSprite_00.draw(&(m_gunFireTex_05));
			}
		}
	}
	if(m_isGunColorBoom != 0)
	{
		m_colorBoomFrame.draw();
	}
	toolsDraw();
	ticketDraw();
	goldBoxEffectDraw();

	faceDraw();	//描绘人物
	userBuffDraw();
	warnningDraw();

#if USE_FBO
	if(m_fboShakeData.getIsEnd() != 0)
	{
		m_fbo.removeFBO();
		if(m_fboOrder == 0) tempTex = m_fbo.getTexture(0);
		else tempTex = m_fbo.getTexture(1);
#if USE_SHADER
		m_fboSprite.setShaderClass(NULL);
		//m_fboSprite.m_pShaderProc = NULL;	//取消shader效果
		m_fboSprite.setBlendType(4,5);
		m_fboSprite.setAlpha(1.0f);
#else
		m_fboSprite.setBlendType(4,5);
		m_fboSprite.setAlpha(1.0f);
#endif
		m_fboSprite.draw(&tempTex);
		m_fboOrder = (m_fboOrder + 1) % 2;
	}
#endif

	inputNameDraw();
	weaponInfoDraw();
}

void _XGun::drawUp()
{
	if(m_isInited == 0) return;
	gameHeadDraw();
	gamePlayDraw();
	gameWinDraw();
	gameLostDraw();
	gameAllOverDraw();
	orderShowDraw();
	gameDemoDraw();
	configDraw();

	instructDraw();
	//显示投币值
//	m_creditSprite.draw();
	m_creditText.draw();
	m_creditNum.draw();

	if(m_takePhotoNeedShow != 0)
	{
		m_takePhotoAttention.draw();
		m_gameLostNum.draw();
	}
	m_cameraLogo.draw();
	if(m_isCameraSupply == 0)
	{
		m_cameraNotSupply.draw();
	}
//	for(int i = 0;i < MAX_GAME_ORDER_SUM;++ i)
//	{
//		m_cameraHead.setPosition(100.0f + 100.0f * i,100.0f);
//		m_cameraHead.setSize(0.3f,0.3f);
//		m_cameraHead.draw(&(m_cameraRecordTex[MAX_GAME_ORDER_SUM - 1 - i]));
//	}
}

void _XGun::move(int interval)
{
	if(m_isInited == 0) return;
	if(m_nowLevel == 0)
	{
		m_lineStartTime += interval;
		if(m_lineStartTime > 50)
		{
			m_lineStartTime = 0;
			m_lineStartX += 1.0f;
			if(m_lineStartX > 16.0f) m_lineStartX = 0.0f;
		}
	}
	if(m_BGSiderMoveData[0].getIsEnd() == 0)
	{
		m_BGSiderMoveData[0].move(interval);
		float temp = 1.0f - sin(m_BGSiderMoveData[0].getNowData() * PI);
		m_BGSider[0].setColor(-1.0f,temp,temp,-1.0f);
	}
	if(m_BGSiderMoveData[1].getIsEnd() == 0)
	{
		m_BGSiderMoveData[1].move(interval);
		float temp = 1.0f - sin(m_BGSiderMoveData[1].getNowData() * PI);
		m_BGSider[1].setColor(-1.0f,temp,temp,-1.0f);
	}
//	m_backGroundTimer += interval;
//	if(m_backGroundTimer > 100)
//	{
//		m_backGroundTimer = 0;
//		++m_backGroundColor;
//		if(m_backGroundColor >= 24) m_backGroundColor = 0;
//	}

	m_backGroungPosition += 0.001f * interval;
	if(m_backGroungPosition >= 1280.0f) m_backGroungPosition -= 1280.0f;
	for(int i = 0;i < 13;++ i)
	{
		if(m_starFlag[i] != 0)
		{
			m_starPosition[i].x += m_starSpeed[i].x * interval;
			m_starPosition[i].y += m_starSpeed[i].y * interval;
			m_star[i].setPosition(m_starPosition[i]);
			//if(i < 7)
			//{
			//	if(m_starPosition[i].x < -512.0f || m_starPosition[i].x > 1280.0f + 512.0f) m_starFlag[i] = 0;
			//	if(m_starPosition[i].y < -512.0f || m_starPosition[i].y > 720.0f + 512.0f) m_starFlag[i] = 0;
			//}else
			//{
			//	if(m_starPosition[i].x < -256.0f || m_starPosition[i].x > 1280.0f + 256.0f) m_starFlag[i] = 0;
			//	if(m_starPosition[i].y < -256.0f || m_starPosition[i].y > 720.0f + 256.0f) m_starFlag[i] = 0;
			//}
			if(i < 7)
			{
				if(m_starPosition[i].x < -512.0f) m_starFlag[i] = 0;
				if(m_starPosition[i].y < -512.0f) m_starFlag[i] = 0;
			}else
			{
				if(m_starPosition[i].x < -256.0f) m_starFlag[i] = 0;
				if(m_starPosition[i].y < -256.0f) m_starFlag[i] = 0;
			}
		}
	}
	//for(int i = 7;i < 13;++ i)
	//{
	//	if(m_starFlag[i] == 0)
	//	{
	//		m_starFlag[i] = 1;
	//		m_starPosition[i].x = 
	//	}
	//}

	if(m_gunIsFlash != 0 && m_gunFlashTime > 0)
	{
		m_gunFlashTime -= interval;
		if(m_gunFlashTime <= 0) 
		{
			m_gunFlashTime = 0;
			m_gunIsFlash = 0;
		}
	}
	if(m_toolsBoomFlag != 0)
	{
		m_toolsBoom.move(interval);
		if(m_toolsBoom.getIsEnd() != 0) m_toolsBoomFlag = 0;
	}

	gameHeadMove(interval);
	gamePlayMove(interval);
	gameWinMove(interval);
	gameLostMove(interval);
	gameDemoMove(interval);
	gameAllOverMove(interval);

	gameOrderMove(interval);
	orderShowMove(interval);
	gameTimerMove(interval);

	int flag = 0;
	int retFlag = 0;
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		retFlag = m_bullet[i].move(interval);
		if(retFlag != 0 && flag == 0)  flag = retFlag;
		else if(retFlag != 0 && retFlag != flag) flag = 3;
	}
	if(flag != 0) _XSound::GetInstance().play(m_soundHitSide);
	if(flag == 1) m_BGSiderMoveData[0].set(0.0f,1.0f,0.5f); else
	if(flag == 2) m_BGSiderMoveData[1].set(0.0f,1.0f,0.5f); else
	if(flag == 3) 
	{
		m_BGSiderMoveData[0].set(0.0f,1.0f,0.5f);
		m_BGSiderMoveData[1].set(0.0f,1.0f,0.5f);
	}
	if(m_isGunColorBoom != 0)
	{
		if(m_colorBoomFrame.getIsEnd() != 0)
		{
			m_colorBoomFrame.reset();
			_XSound::GetInstance().play(m_soundEnergyFree);
		}
		m_colorBoomFrame.move(interval);
	}
	//下面是震动的计算
	UIShakeMove(interval);
	gunShakeMove(interval);

	gunFireGlare(interval);
	numberOfEnergyGlare(interval);	//能量值偏小时的闪烁
	energyAddMove(interval);	//能量值偏小时的闪烁
	numberChange(interval);	//用于显示数字滚动
	goldAddMove(interval);	//数字的移动
	goldFlyMove(interval);	//飞动金币的动作
	toolsMove(interval);	//道具效果
	waeponInfoMove(interval);	
	warnningMove(interval);	
	helpMove(interval);	
	processMove(interval);	
	ticketMove(interval);	
	configMove(interval);	
	instructMove(interval);
	smallStarMove(interval);
	faceMove(interval);
	userBuffMove(interval);
	toolsEffectMove(interval);

	goldBoxEffectmove(interval);

//	userHeadMove(interval);

	//下面检查道具是否出界
	if(m_toolsFlag == -1)
	{
		for(int i = m_gems->m_nowMaxtrixGemSum - 1;i >= 0;-- i)
		{
			if(m_gems->m_gem[i].m_isEnable != 0 && m_gems->m_gem[i].m_type >= GEM_TYPE_16 && m_gems->m_gem[i].m_type <= GEM_TYPE_27)
			{//道具的话出界就无效
				if(m_gems->m_gem[i].m_position .x < -128.0f) 
				{
					m_gems->m_gem[i].m_isEnable = 0;
					m_toolsFlag = 0;
				}
				break;	//目前程序逻辑保证只会产生一个道具，所以这里可以直接返回
			}
		}
	}
	//下面是彩票机状态更新
	m_releaseArmTicket = 0;
	static int upLotteryState = 1;
	//if(upLotteryState != 0)
	//{//设置有问题
	//	setTicketWrong();
	//}
	//if(upLotteryState == 0)
	//{
	//	setTicketWell();
	//}

	if(m_isCameraSupply == 0)
	{
		m_notSupplyAlpha += 0.002f * interval;
		if(m_notSupplyAlpha >= PI)
		{
			m_notSupplyAlpha = 0.0f;
		}
		m_cameraNotSupply.setAlpha(sin(m_notSupplyAlpha));
	}
}

void _XGun::gunFireGlare(int interval)
{
	if(m_gunFireAlphaStage == 1)
	{
		m_gunFireAlpha += 0.05f * interval;
		if(m_gunFireAlpha >= 2.2f)
		{
			m_gunFireAlphaStage = 2;
		}
		if(m_gunFireAlpha <= 1.0f) m_gunFireSprite_00.setAlpha(m_gunFireAlpha);
	}else
	if(m_gunFireAlphaStage == 2)
	{
		m_gunFireAlpha -= 0.05f * interval;
		if(m_gunFireAlpha <= 0.0f)
		{
			m_gunFireAlpha = 0.0f;
			m_gunFireAlphaStage = 0;
		}
		if(m_gunFireAlpha <= 1.0f) m_gunFireSprite_00.setAlpha(m_gunFireAlpha);
	}
}

void _XGun::energyAddMove(int interval)
{
	if(m_energyAddStage == 1)
	{//初始化
		m_energyAddAlpha = 1.0f;
		m_energyAddNum.setColor(-1.0f,-1.0f,-1.0f,1.0f);
		m_energyAddStage = 2;
	}else
	if(m_energyAddStage == 2)
	{//持续
		m_energyAddAlpha += interval;
		if(m_energyAddAlpha >= 100)
		{
			m_energyAddAlpha = 1.0f;
			m_energyAddStage = 3;
		}
	}else
	if(m_energyAddStage == 3)
	{//消失
		m_energyAddAlpha -= 0.001f * interval;
		if(m_energyAddAlpha <= 0)
		{
			m_energyAddAlpha = 0.0f;
			m_energyAddStage = 4;
		}
		m_energyAddNum.setColor(-1.0f,-1.0f,-1.0f,m_energyAddAlpha);
	}
}

void _XGun::energyAddDraw()
{
	if(m_energyAddStage != 0) m_energyAddNum.draw();
}

void _XGun::energyAdd(int temp,int mode)
{
	if(mode == 0)
	{
		m_energyArmSum += temp;
		if(m_energyArmSum < 0) m_energyArmSum = 0;
		char str[20];
		if(temp >= 0)
		{
			sprintf(str,"+%d",temp);
		}else
		{
			sprintf(str,"%d",temp);
		}
		m_energyAddNum.setNumber(str);
		m_energyAddStage = 1;
	}else
	{
		char str[20];
		sprintf(str,"-0");
		m_energyAddNum.setNumber(str);
		m_energyAddStage = 1;
	}
}


void _XGun::numberOfEnergyGlare(int interval)	//能量数字较小时闪烁的动作
{
	if(m_energySum >= ENERGY_WARNNING || m_energySum == 0)
	{
		if(m_isEnergyGlare != 0) 
		{
			m_energyNum.setColor(-1.0f,-1.0f,-1.0f,1.0f);
			m_isEnergyGlare = 0;
		}
	}else
	{
		if(m_isEnergyGlare == 0)
		{
			m_isEnergyGlare = 1;
			m_energyAlpha = 1.0f;
			m_energyAlphaStage = 0;
		}
	}
	if(m_isEnergyGlare != 0)
	{
		if(m_energyAlphaStage != 0)
		{
			m_energyAlpha -= 0.001f * interval;
			if(m_energyAlpha <= 0.2f)
			{
				m_energyAlpha = 0.2f;
				m_energyAlphaStage = 0;
			}
		}else
		{
			m_energyAlpha += 0.004f * interval;
			if(m_energyAlpha >= 1.0f)
			{
				m_energyAlpha = 1.0f;
				m_energyAlphaStage = 1;
			}
		}
		m_energyNum.setColor(-1.0f,-1.0f,-1.0f,m_energyAlpha);
		m_lowEnergy.setAlpha(0.5f + m_energyAlpha * 0.5f);
	}
}

float energyBarColor[6][4] = {
	{1.0f,0.0f,0.0f,1.0f},
	{0.0f,1.0f,0.0f,1.0f},
	{0.5f,0.5f,0.0f,1.0f},
	{1.0f,0.5f,0.0f,1.0f},
	{1.0f,1.0f,0.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f}};

void _XGun::numberChange(int interval)
{
	float tempSum;
	if(m_energyArmSum != m_energySum)
	{
		tempSum = m_energyArmSum - m_energySum;
		if(0.0075f * interval < 1.0f)
		{
			tempSum *= 0.0075f * interval;
		}
		if(tempSum < 1.0f && tempSum > -1.0f)
		{
			if(tempSum < 0) tempSum = -1.0f;
			else tempSum = 1.0f;
		//	m_energySum = m_energyArmSum; //换一种方式
		}
		m_energySum += tempSum;

		m_energyNum.setNumber(m_energySum);
		if(m_energySum <= 1000)
		{
			m_energyBarDown.setColor(0.0f,0.0f,0.0f,0.0f);
			m_energyBar.setColor(1.0f,0.0f,0.0f,1.0f);
		}else
		{
			int temp = m_energySum / 1000;
			if(temp > 5) temp = 5;
			m_energyBarDown.setColor(energyBarColor[temp - 1][0],energyBarColor[temp - 1][1],energyBarColor[temp - 1][2],energyBarColor[temp - 1][3]);
			m_energyBar.setColor(energyBarColor[temp][0],energyBarColor[temp][1],energyBarColor[temp][2],energyBarColor[temp][3]);
		}
		if(m_energySum / 1000 <= 5) m_energyBar.setClipRect(0.0f,0.0f,64.0f,140.0f * ((m_energySum - 1) % 1000) / 1000.0f);
	}
	if(m_goldArmSum != m_goldSum)
	{
		tempSum = m_goldArmSum - m_goldSum;
		if(0.0075f * interval < 1.0f)
		{
			tempSum *= 0.0075f * interval;
		}
		if(tempSum < 1.0f && tempSum > -1.0f)
		{
			if(tempSum < 0) tempSum = -1.0f;
			else tempSum = 1.0f;
		}
		m_goldSum += tempSum;

		m_goldNum.setNumber(m_goldSum);
	}
	if(m_scoresArmSum != m_scoresSum)
	{
		tempSum = m_scoresArmSum - m_scoresSum;
		if(0.0015f * interval < 1.0f)
		{
			tempSum *= 0.0015f * interval;
		}
		if(tempSum < 1.0f && tempSum > -1.0f)
		{
			if(tempSum < 0) tempSum = -1.0f;
			else tempSum = 1.0f;
		}
		m_scoresSum += tempSum;

		m_scoresNum.setNumber(m_scoresSum);
	}
	if(abs(m_levelFiniedArmSum - m_levelFiniedSum) > 0.0001f)
	{
		tempSum = m_levelFiniedArmSum - m_levelFiniedSum;
		if(tempSum < 0.0001f && tempSum > -0.0001f)
		{
			if(tempSum < 0) tempSum = -0.0001f;
			else tempSum = 0.0001f;
		}
		if(0.0015f * interval < 1.0f)
		{
			tempSum *= 0.0015f * interval;
		}
		m_levelFiniedSum += tempSum;

		//m_nowLevelProcess.setValue(m_levelFiniedSum * 100.0f);
		m_PrograssAlpha = m_levelFiniedSum;
	//	char tempStr[128];
	//	sprintf(tempStr,"%.1f %%",m_levelFiniedSum * 100.0f);
		m_nowLevelProcess.setClipRect(0,0,32,512.0f * m_levelFiniedSum);
	//	m_nowLevelProcessN.setNumber(tempStr);
	}
	ticketbarSumMove(interval);
}

void _XGun::goldAddMove(int interval)
{
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		if(m_goldAddFlag[i] == 1)
		{
			m_goldAddAlpha[i] = 0.0f;
			m_goldAddX[i] = 0.0f;
			m_goldAddNum[i].setColor(-1.0f,-1.0f,-1.0f,m_goldAddAlpha[i]);
			m_goldAddNum[i].setPosition(300 - m_goldAddX[i] + m_goldAddBasicY[i],m_goldAddBasicX[i]);
			m_goldAddFlag[i] = 2;
		}else
		if(m_goldAddFlag[i] == 2)
		{//淡入
			m_goldAddAlpha[i] += 0.02f * interval;
			if(m_goldAddAlpha[i] >= 1.0f)
			{
				m_goldAddAlpha[i] = 1.0f;
				m_goldAddFlag[i] = 3;
			}
			m_goldAddNum[i].setColor(-1.0f,-1.0f,-1.0f,m_goldAddAlpha[i]);
			m_goldAddX[i] += 0.2f * interval;
			m_goldAddNum[i].setPosition(280 - m_goldAddX[i] + m_goldAddBasicY[i],m_goldAddBasicX[i]);
		}else
		if(m_goldAddFlag[i] == 3)
		{//持续
			m_goldAddAlpha[i] += 0.0025f * interval;
			if(m_goldAddAlpha[i] >= 2.0f)
			{
				m_goldAddAlpha[i] = 1.0f;
				m_goldAddFlag[i] = 4;
			}
			m_goldAddX[i] += 0.2f * interval;
			m_goldAddNum[i].setPosition(280 - m_goldAddX[i] + m_goldAddBasicY[i],m_goldAddBasicX[i]);
		}else
		if(m_goldAddFlag[i] == 4)
		{//淡出
			m_goldAddAlpha[i] -= 0.005f * interval;
			if(m_goldAddAlpha[i] <= 0.0f)
			{
				m_goldAddAlpha[i] = 0.0f;
				m_goldAddFlag[i] = 0;
			}
			m_goldAddNum[i].setColor(-1.0f,-1.0f,-1.0f,m_goldAddAlpha[i]);
			m_goldAddX[i] += 0.1f * interval;
			m_goldAddNum[i].setPosition(280 - m_goldAddX[i] + m_goldAddBasicY[i],m_goldAddBasicX[i]);
		}
	}
}

void _XGun::goldFlyMove(int interval)
{
	for(int i = 0;i < MAX_BULLET_SUM * 10;++ i)
	{
		if(m_goldFlyFlag[i] == 1)
		{//初始化
			m_goldFlyAlpha[i] = 0.0f;
			m_goldFly[i].setColor(-1.0f,-1.0f,-1.0f,m_goldFlyAlpha[i]);
			m_goldFly[i].setPosition(m_goldFlyPosition[i]);
			m_goldFlyFlag[i] = 2;
		}else
		if(m_goldFlyFlag[i] == 2)
		{//淡入
			m_goldFlyAlpha[i] += 0.005f * interval;
			if(m_goldFlyAlpha[i] >= 1.0f)
			{
				m_goldFlyAlpha[i] = 1.0f;
				m_goldFlyFlag[i] = 3;
			}
			m_goldFly[i].setColor(-1.0f,-1.0f,-1.0f,m_goldFlyAlpha[i]);
			m_goldFly[i].move(interval);
		}else
		if(m_goldFlyFlag[i] == 3)
		{//持续
			m_goldFlyAlpha[i] += 0.0025f * interval;
			if(m_goldFlyAlpha[i] >= 1.75f + m_goldFlyParam[i])
			{
				m_goldFlyAlpha[i] = 0.0f;
				m_goldFlyFlag[i] = 4;
			}
			m_goldFly[i].move(interval);
		}else
		if(m_goldFlyFlag[i] == 4)
		{//飞行
			m_goldFlyAlpha[i] += 0.001f * interval;
			if(m_goldFlyAlpha[i] >= PI_HALF)
			{//飞行完成
				m_goldFlyAlpha[i] = PI_HALF;
				m_goldFlyFlag[i] = 0;
			}
			m_goldFly[i].setPosition(GOLD_FLY_ARM_X + (m_goldFlyPosition[i].x - GOLD_FLY_ARM_X) * cos(m_goldFlyAlpha[i]) + 300.0f * sin(m_goldFlyAlpha[i] * 2.0f),
				GOLD_FLY_ARM_Y + (m_goldFlyPosition[i].y - GOLD_FLY_ARM_Y) * cos(m_goldFlyAlpha[i]));
			m_goldFly[i].move(interval);
		}
	}
}

void _XGun::whirlGun(int direction)
{
	if(m_isInited == 0) return;
	if(m_isLightShoot != 0) return;	//激光炮发射过程中不能改变这个值
	if(direction > 0)
	{
		if(m_gunAngle < GUN_ANGLE_MAX) 
		{
			m_gunAngle += 0.5f;
			m_gunSprite.setAngle(m_gunAngle);
		}
	}else
	{
		if(m_gunAngle > GUN_ANGLE_MIN) 
		{
			m_gunAngle -= 0.5f;
			m_gunSprite.setAngle(m_gunAngle);
		}
	}
}

void _XGun::upgradeGun()
{
	if(m_isInited == 0) return;
	if(m_isLightShoot != 0) return;	//激光炮发射过程中不能改变这个值

	if(m_gunType <= GUN_TYPE_00 && (m_energyArmSum >= GUN_SHOOT_ENERGY_01 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))) 
	{
		m_gunType = GUN_TYPE_01;
		m_gunShootSpeed = GUN_SHOOT_SPEED_01;
	}else
	if(m_gunType <= GUN_TYPE_01 && (m_energyArmSum >= GUN_SHOOT_ENERGY_02 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))) 
	{
		m_gunType = GUN_TYPE_02;
		m_gunShootSpeed = GUN_SHOOT_SPEED_02;
	}else
	if(m_gunType <= GUN_TYPE_02 && (m_energyArmSum >= GUN_SHOOT_ENERGY_03 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))) 
	{
		m_gunType = GUN_TYPE_03;
		m_gunShootSpeed = GUN_SHOOT_SPEED_03;
	}else
	if(m_gunType <= GUN_TYPE_03 && (m_energyArmSum >= GUN_SHOOT_ENERGY_04 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))) 
	{
		m_gunType = GUN_TYPE_04;
		m_gunShootSpeed = GUN_SHOOT_SPEED_04;
	}else
	if(m_gunType <= GUN_TYPE_04 && (m_energyArmSum >= GUN_SHOOT_ENERGY_05 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))) 
	{
		m_gunType = GUN_TYPE_05;
		m_gunShootSpeed = GUN_SHOOT_SPEED_05;
	}else
	if(m_gunType <= GUN_TYPE_05) 
	{
		m_gunType = GUN_TYPE_00;
		m_gunShootSpeed = GUN_SHOOT_SPEED_00;
	}

	//玩家使用的枪会受到限制
	if(m_gunType > m_canUserGunType)
	{
		m_gunType = GUN_TYPE_00;
		m_gunShootSpeed = GUN_SHOOT_SPEED_00;
	}
	_XSound::GetInstance().play(m_soundChangeGun);
	checkGunType();
}

void _XGun::setGunType(_XGUN_TYPE type)
{
	if(m_isInited == 0) return;
	if(m_isLightShoot != 0) return;	//激光炮发射过程中不能改变这个值
	m_gunType = type;
	if(m_gunType == GUN_TYPE_00) 
	{
		m_gunShootSpeed = GUN_SHOOT_SPEED_00;
	}else
	if(m_gunType == GUN_TYPE_01) 
	{
		m_gunShootSpeed = GUN_SHOOT_SPEED_01;
	}else
	if(m_gunType == GUN_TYPE_02) 
	{
		m_gunShootSpeed = GUN_SHOOT_SPEED_02;
	}else
	if(m_gunType == GUN_TYPE_03) 
	{
		m_gunShootSpeed = GUN_SHOOT_SPEED_03;
	}else
	if(m_gunType == GUN_TYPE_04) 
	{
		m_gunShootSpeed = GUN_SHOOT_SPEED_04;
	}else
	if(m_gunType == GUN_TYPE_05) 
	{
		m_gunShootSpeed = GUN_SHOOT_SPEED_05;
	}
	//玩家使用的枪会受到限制
	if(m_gunType > m_canUserGunType)
	{
		m_gunType = GUN_TYPE_00;
		m_gunShootSpeed = GUN_SHOOT_SPEED_00;
	}
}

int _XGun::findAFreeBullet()
{
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		if(m_bullet[i].getIsEnd() != 0)
		{
			return i;
		}
	}
	return -1;
}

void _XGun::shootEnergyDecrease(int energy)
{
	if(m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0)
	{//免费射击道具的时限范围内
		energyAdd(0,1);
	}else
	{
		if(energy <= 1.0f)
		{
			energyAdd(- energy);
		}else
		{
			energyAdd(- energy * buffEnergeRate());
		}
		//m_energyArmSum -= energy;	//扣除子弹的能量
	}
}

void _XGun::gunFireSpriteSet(_XGUN_TYPE type)
{
	m_gunFireSprite_00.setPosition(100.0f * sin(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f,360.0f - 100.0f * cos(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f);
	m_gunFireSprite_00.setAngle(m_gunAngle + 90.0f);
	m_gunFireSprite_00.setAlpha(0.0f);

	m_gunFireAlphaStage = 1;	//开火
	m_gunFireAlpha = 0.0f;
	m_gunFireType = type;
}

void _XGun::gunShootBulletInit(int order)
{
	static int bulletShootPositon = 0;
	_XVector2 temp;
	if(m_isGunColorBoom != 0)
	{
		temp.set(135.0f * sin(m_gunAngle * ANGLE_TO_RADIAN) - 16.0f,360.0f - 135.0f * cos(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f);
		m_bullet[order].init(temp,_XVector2(0.8f * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),0.8f * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN)),
			GUN_SHOOT_ENERGY_X0,m_gunAngle,&m_bulletTex,BULLET_TYPE_X0);
	}else
	{
		if(m_gunType == GUN_TYPE_00)
		{
			bulletShootPositon ++;
			if(bulletShootPositon >= 4) bulletShootPositon = 0;

			if(bulletShootPositon == 0 || bulletShootPositon == 2) temp.set(135.0f * sin(m_gunAngle * ANGLE_TO_RADIAN) - 16.0f,360.0f - 135.0f * cos(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f); else
			if(bulletShootPositon == 1) temp.set(128.0f * sin((m_gunAngle - 5.0f) * ANGLE_TO_RADIAN) - 16.0f,360.0f - 128.0f * cos((m_gunAngle - 5.0f) * ANGLE_TO_RADIAN) - 32.0f); else
			if(bulletShootPositon == 3) temp.set(128.0f * sin((m_gunAngle + 5.0f) * ANGLE_TO_RADIAN) - 16.0f,360.0f - 128.0f * cos((m_gunAngle + 5.0f) * ANGLE_TO_RADIAN) - 32.0f);
			m_bullet[order].init(temp,_XVector2(0.8f * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),0.8f * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN)),
				GUN_SHOOT_ENERGY_00 * buffBulletPowerRate(),m_gunAngle,&m_bulletTex,BULLET_TYPE_00);
		}else
		if(m_gunType == GUN_TYPE_01)
		{
			temp.set(135.0f * sin(m_gunAngle * ANGLE_TO_RADIAN) - 16.0f,360.0f - 135.0f * cos(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f);
			m_bullet[order].init(temp,_XVector2(0.8f * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),0.8f * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN)),
				GUN_SHOOT_ENERGY_01 * buffBulletPowerRate(),m_gunAngle,&m_bulletTex,BULLET_TYPE_01);
		}else
		if(m_gunType == GUN_TYPE_02)
		{
			temp.set(135.0f * sin(m_gunAngle * ANGLE_TO_RADIAN) - 16.0f,360.0f - 135.0f * cos(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f);
			m_bullet[order].init(temp,_XVector2(0.8f * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),0.8f * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN)),
				GUN_SHOOT_ENERGY_X0,m_gunAngle,&m_bulletTex,BULLET_TYPE_02);
		}else
		if(m_gunType == GUN_TYPE_03)
		{
			temp.set(135.0f * sin(m_gunAngle * ANGLE_TO_RADIAN) - 16.0f,360.0f - 135.0f * cos(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f);
			m_bullet[order].init(temp,_XVector2(0.8f * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),0.8f * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN)),
				GUN_SHOOT_ENERGY_X0,m_gunAngle,&m_bulletTex,BULLET_TYPE_03);
		}else
		if(m_gunType == GUN_TYPE_04)
		{
			temp.set(135.0f * sin(m_gunAngle * ANGLE_TO_RADIAN) - 16.0f,360.0f - 135.0f * cos(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f);
			m_bullet[order].init(temp,_XVector2(0.8f * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),0.8f * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN)),
				GUN_SHOOT_ENERGY_04 * 0.75f * buffBulletPowerRate(),m_gunAngle,&m_bulletTex,BULLET_TYPE_04);
		}else
		if(m_gunType == GUN_TYPE_05)
		{
			temp.set(1124.0f * sin(m_gunAngle * ANGLE_TO_RADIAN) - 32.0f,360.0f - 1124.0f * cos(m_gunAngle * ANGLE_TO_RADIAN) - 1024.0f);
			m_bullet[order].init(temp,_XVector2(0.8f * sin((180.0f - m_gunAngle) * ANGLE_TO_RADIAN),0.8f * cos((180.0f - m_gunAngle) * ANGLE_TO_RADIAN)),
				GUN_SHOOT_ENERGY_05 * buffBulletPowerRate(),m_gunAngle,&m_bulletTex,BULLET_TYPE_05);
		}
	}
}

void _XGun::shoot()
{
	if(m_isInited == 0) return;
	if(m_gameStage != GAME_STAGE_GAMEPLAY) return;	//如果不是游戏阶段不能射击
	if(m_isLightShoot != 0) return;
	if(m_isGunColorBoom != 0)
	{//发射彩色子弹，使用最小的能量
		if(m_energyArmSum >= GUN_SHOOT_ENERGY_00 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))
		{//有能量才能发射
			int order = findAFreeBullet();
			if(order < 0) return;	//找不到有效地子弹，发射失败
			
			_XSound::GetInstance().play(m_soundGun_06);

			gunShootBulletInit(order);
			shootEnergyDecrease(GUN_SHOOT_ENERGY_00);
			gunFireSpriteSet(m_gunType);
			if(m_UIShakeStage == 0) m_UIShakeStage = 1;
			if(m_gunShakeStage == 0) m_gunShakeStage = 1;

			m_isGunColorBoom = 0;
		}
	}else
	if(m_gunType == GUN_TYPE_00)
	{
		if(m_energyArmSum >= GUN_SHOOT_ENERGY_00 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))
		{//有能量才能发射
			int order = findAFreeBullet();
			if(order < 0) return;	//找不到有效地子弹，发射失败
			
			_XSound::GetInstance().play(m_soundGun_00);

			gunShootBulletInit(order);
			shootEnergyDecrease(GUN_SHOOT_ENERGY_00);
			gunFireSpriteSet(m_gunType);
			if(m_UIShakeStage == 0) m_UIShakeStage = 1;
			if(m_gunShakeStage == 0) m_gunShakeStage = 1;
		}
	}else
	if(m_gunType == GUN_TYPE_01)
	{
		if(m_energyArmSum >= GUN_SHOOT_ENERGY_01 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))
		{//有能量才能发射
			int order = findAFreeBullet();
			if(order < 0) return;	//找不到有效地子弹，发射失败
			
			_XSound::GetInstance().play(m_soundGun_01);
			
			gunShootBulletInit(order);
			shootEnergyDecrease(GUN_SHOOT_ENERGY_01);
			gunFireSpriteSet(m_gunType);

			if(m_UIShakeStage == 0) m_UIShakeStage = 1;
			if(m_gunShakeStage == 0) m_gunShakeStage = 1;
			checkGunType();
		}
	}else
	if(m_gunType == GUN_TYPE_02)
	{
		if(m_energyArmSum >= GUN_SHOOT_ENERGY_02 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))
		{//有能量才能发射
			int order = findAFreeBullet();
			if(order < 0) return;	//找不到有效地子弹，发射失败
			
			_XSound::GetInstance().play(m_soundGun_02);
			
			gunShootBulletInit(order);
			shootEnergyDecrease(GUN_SHOOT_ENERGY_02);
			gunFireSpriteSet(m_gunType);

			if(m_UIShakeStage == 0) m_UIShakeStage = 1;
			if(m_gunShakeStage == 0) m_gunShakeStage = 1;
			checkGunType();
		}
	}else
	if(m_gunType == GUN_TYPE_03)
	{
		if(m_energyArmSum >= GUN_SHOOT_ENERGY_03 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))
		{//有能量才能发射
			int order = findAFreeBullet();
			if(order < 0) return;	//找不到有效地子弹，发射失败
			
			_XSound::GetInstance().play(m_soundGun_03);
			
			gunShootBulletInit(order);
			shootEnergyDecrease(GUN_SHOOT_ENERGY_03);
			gunFireSpriteSet(m_gunType);

			if(m_UIShakeStage == 0) m_UIShakeStage = 1;
			if(m_gunShakeStage == 0) m_gunShakeStage = 1;
			checkGunType();
		}
	}else
	if(m_gunType == GUN_TYPE_04)
	{
		if(m_energyArmSum >= GUN_SHOOT_ENERGY_04 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))
		{//有能量才能发射
			int order = findAFreeBullet();
			if(order < 0) return;	//找不到有效地子弹，发射失败
		
			_XSound::GetInstance().play(m_soundGun_04);
			
			gunShootBulletInit(order);
			shootEnergyDecrease(GUN_SHOOT_ENERGY_04);
			gunFireSpriteSet(m_gunType);

			if(m_UIShakeStage == 0) m_UIShakeStage = 1;
			if(m_gunShakeStage == 0) m_gunShakeStage = 1;
			checkGunType();
		}
	}else
	if(m_gunType == GUN_TYPE_05)
	{
		if(m_energyArmSum >= GUN_SHOOT_ENERGY_05 || (m_toolsType == TOOLS_TYPE_03 && m_toolsFlag > 0))
		{//有能量才能发射
			int order = findAFreeBullet();
			if(order < 0) return;	//找不到有效地子弹，发射失败
			
			m_isLightShoot = 1;
			_XSound::GetInstance().play(m_soundGun_05);
			
			gunShootBulletInit(order);
			shootEnergyDecrease(GUN_SHOOT_ENERGY_05);
			gunFireSpriteSet(m_gunType);

			if(m_UIShakeStage == 0) m_UIShakeStage = 1;
			if(m_gunShakeStage == 0) m_gunShakeStage = 1;
			checkGunType();
		}
	}
	setGameTimerReset(1);
}

int _XGun::gemBeCrashProc(_XGemMatrix *gems,int gemOrder,_XVector2 tempPoint,int energy)
{
	int result = 0;
	if(gems->m_gem[gemOrder].m_type >= GEM_TYPE_08)
	{
		if(gems->m_gem[gemOrder].m_energy <= energy)
		{
			gems->m_gem[gemOrder].m_energy = 0.0f;
			gems->m_gem[gemOrder].setEnd();
			addTicketInput(gems->m_gem[gemOrder].m_type,gems->m_gem[gemOrder].m_size);
			result += crashToolsProc(gems,gems->m_gem[gemOrder].m_type,tempPoint);
		}else
		{
			gems->m_gem[gemOrder].m_energy -= energy;
		}
	}else
	{
		if(gems->m_gem[gemOrder].m_type == GEM_TYPE_15)
		{//打不坏的道具
			gems->m_gem[gemOrder].m_isGlare = 100;
		}else
		{
			if(gems->m_gem[gemOrder].m_energy <= energy)	//这个是爆炸的能量 10
			{//炸完了
				gems->m_gem[gemOrder].m_energy = 0.0f;
				gems->m_gem[gemOrder].setEnd();
				addTicketInput(gems->m_gem[gemOrder].m_type,gems->m_gem[gemOrder].m_size);

				//int tempAddSum = gemBasicEnergy[gems->m_gem[gemOrder].m_type] * gems->m_gem[gemOrder].m_size * GOLD_RATE;
				if(gems->m_gem[gemOrder].m_type > GEM_TYPE_07) 
				{
					exit(0);
				}
				int tempAddSum = gemBasicEnergy[gems->m_gem[gemOrder].m_type] * gems->m_gem[gemOrder].m_size * gemE2GRate[gems->m_gem[gemOrder].m_type];
				m_goldArmSum += tempAddSum;
				m_scoresArmSum += tempAddSum;

				result += tempAddSum;
				//这里所搜飞行的金币
				findGoldFly(tempAddSum,gems->m_gem[gemOrder].m_position);
				canGetTools(gems,tempPoint);
			}else
			{//还有渣
				gems->m_gem[gemOrder].m_energy -= energy;
				gems->m_gem[gemOrder].m_isGlare = 100;
			}
		}
	}
	return result;
}

int _XGun::crashToolsProc(_XGemMatrix *gems,_XGEM_TYPE type,_XVector2 tempPoint1)//子弹的碰撞处理
{
	int result = 0;
	_XVector2 tempPoint2;	
	//下面是道具效果
	if(type == GEM_TYPE_08)
	{//炸弹的处理
		for(int k = 0;k < gems->m_nowMaxtrixGemSum;++ k)
		{
			if(gems->m_gem[k].canCrash() != 0)
			{//宝石有效
				tempPoint2.set(gems->m_gem[k].m_position.x + 64.0f,gems->m_gem[k].m_position.y + 64.0f);
				if(tempPoint2.getLength(tempPoint1) <= 150.0f)
				{//爆炸范围之内
					result += gemBeCrashProc(gems,k,tempPoint2,AREA_BOOM_ENERGY);
				}
			}
		}
		_XSound::GetInstance().play(m_soundBoom_01);
		m_fboShakeData.set(100.0f,0.0f,0.1f);
	}else
	if(type == GEM_TYPE_09)
	{//水平炸弹的处理
		for(int k = 0;k < gems->m_nowMaxtrixGemSum;++ k)
		{
			if(gems->m_gem[k].canCrash() != 0)
			{//宝石有效
				tempPoint2.set(gems->m_gem[k].m_position.x + 64.0f,gems->m_gem[k].m_position.y + 64.0f);
				if(tempPoint2.x <= tempPoint1.x + 16 && tempPoint2.x >= tempPoint1.x - 16
					&& tempPoint2.y <= tempPoint1.y + 256 && tempPoint2.y >= tempPoint1.y - 256)
				{
					result += gemBeCrashProc(gems,k,tempPoint2,LINE_BOOM_ENERGY);
				}
			}
		}
		_XSound::GetInstance().play(m_soundBoom_02);
	}else
	if(type == GEM_TYPE_10)
	{//垂直炸弹的处理
		for(int k = 0;k < gems->m_nowMaxtrixGemSum;++ k)
		{
			if(gems->m_gem[k].canCrash() != 0)
			{//宝石有效
				tempPoint2.set(gems->m_gem[k].m_position.x + 64.0f,gems->m_gem[k].m_position.y + 64.0f);
				if(tempPoint2.x <= tempPoint1.x + 256 && tempPoint2.x >= tempPoint1.x - 256
					&& tempPoint2.y <= tempPoint1.y + 16 && tempPoint2.y >= tempPoint1.y - 16)
				{
					result += gemBeCrashProc(gems,k,tempPoint2,LINE_BOOM_ENERGY);
				}
			}
		}
		_XSound::GetInstance().play(m_soundBoom_02);
	}else
	if(type == GEM_TYPE_11)
	{//彩色炸弹
	//	if(m_isLightShoot == 0)
	//	{//激光炮不会触发彩色蛋
			m_isGunColorBoom = 1;
			_XSound::GetInstance().play(m_soundColorBoom);
	//	}
	}else
	if(type == GEM_TYPE_14)
	{//增加一排下面的子弹
		int AddBulletSum = 0;
		_XVector2 temp;
		for(int k = 0;k < MAX_BULLET_SUM;++ k)
		{
			if(m_bullet[k].getIsEnd() != 0)
			{
				temp.set(-10,25 + 32 * AddBulletSum);
				m_bullet[k].init(temp,_XVector2(0.8f,0.0f),4,90,&m_bulletTex,BULLET_TYPE_01);

				AddBulletSum ++;
				if(AddBulletSum >= 20)break;
			}
		}
		_XSound::GetInstance().play(m_soundBoom_04);
	}else
	if(type == GEM_TYPE_21)
	{//扣除能量的道具
		//扣除100点能量
		if(m_energyArmSum >= TOOLS_DECREASE_ENERGY) energyAdd(-TOOLS_DECREASE_ENERGY);
		else energyAdd(-m_energyArmSum);
		m_toolsBoomFlag = 1;
		m_toolsBoom.reset();
		m_gunIsFlash = 1;
		m_gunFlashTime = 500;
		_XSound::GetInstance().play(m_soundBoom_01);
	}else
	if(type == GEM_TYPE_22)
	{//击中宝箱的道具
		int tempRand;
		if(m_configData.isTicketNeed != 0)
		{//如果出彩票
			tempRand = random(100);
			if(tempRand < 66 && tempRand >=33)
			{//出彩票
				tempRand = GOLD_BOX_TICKET_MIN + (GOLD_BOX_TICKET_MAX - GOLD_BOX_TICKET_MIN) * random(100) / 100.0f;
				addTicket(tempRand);
				findAGoldBoxEffect(1,tempPoint1,tempRand);
			}else
			if(tempRand < 33 && tempRand >= 0)
			{//奖励能量
				tempRand = GOLD_BOX_SCORES_MIN + random((GOLD_BOX_SCORES_MAX - GOLD_BOX_SCORES_MIN) * 0.01) * 100.0f;
				energyAdd(tempRand);
				findAGoldBoxEffect(0,tempPoint1,tempRand);
			}else
			{//什么也没有
			}
		}else
		{//如果不出彩票
			tempRand = random(100);
			if(tempRand < 55 && tempRand >= 0)
			{//奖励金币
				tempRand = GOLD_BOX_SCORES_MIN + random((GOLD_BOX_SCORES_MAX - GOLD_BOX_SCORES_MIN) * 0.01) * 100.0f;
				energyAdd(tempRand);
				findAGoldBoxEffect(0,tempPoint1,tempRand);
			}else
			{//什么也没有
			}
		}
		_XSound::GetInstance().play(m_soundBoom_01);
	}else
	if(type >= GEM_TYPE_16 && type <= GEM_TYPE_27)
	{
		toolsDoProc(tempPoint1,type);
		_XSound::GetInstance().play(m_soundSJTools);
	}
	return result;
}

int _XGun::colorBulletProc(_XGemMatrix *gems,_XGEM_TYPE type)
{
	if(type >= GEM_TYPE_08) return 0;
	int tempAddSum;
	int result = 0;
	for(int k = 0;k < gems->m_nowMaxtrixGemSum;++ k)
	{
		if(gems->m_gem[k].canCrash() != 0 && gems->m_gem[k].m_type == type)
		{
			if(gems->m_gem[k].m_type == GEM_TYPE_15)
			{//这种道具可以被彩色蛋打坏
				gems->m_gem[k].m_energy = 0.0f;
				gems->m_gem[k].setEnd();
				addTicketInput(gems->m_gem[k].m_type,gems->m_gem[k].m_size);
			//	gems->m_gem[k].m_isGlare = 100;
			}else
			{
				if(gems->m_gem[k].m_energy <= COLOR_BOOM_ENERGY * buffBulletPowerRate())	//这个是爆炸的能量 5
				{//炸完了
					gems->m_gem[k].m_energy = 0.0f;
					gems->m_gem[k].setEnd();
					addTicketInput(gems->m_gem[k].m_type,gems->m_gem[k].m_size);

					//tempAddSum = gemBasicEnergy[gems->m_gem[k].m_type] * gems->m_gem[k].m_size * GOLD_RATE;
					if(gems->m_gem[k].m_type > GEM_TYPE_07) 
					{
						exit(0);
					}
					tempAddSum = gemBasicEnergy[gems->m_gem[k].m_type] * gems->m_gem[k].m_size * gemE2GRate[gems->m_gem[k].m_type];
					m_goldArmSum += tempAddSum;
					m_scoresArmSum += tempAddSum;

					result += tempAddSum;
					//这里所搜飞行的金币
					findGoldFly(tempAddSum,gems->m_gem[k].m_position);
					canGetTools(gems,_XVector2(gems->m_gem[k].m_position.x + 64.0f,gems->m_gem[k].m_position.y + 64.0f));
				}else
				{//还有渣
					gems->m_gem[k].m_energy -= COLOR_BOOM_ENERGY * buffBulletPowerRate();
					gems->m_gem[k].m_isGlare = 100;
				}
			}
		}
	}
	return result;
}

int _XGun::bulletBoom(_XVector2 position,_XBULLET_TYPE type,_XGemMatrix *gems)
{//榴弹炸开
	if(type == BULLET_TYPE_02)
	{
		int order;
		int tempRand = random(100) / 100.0f * 0.08f;
		position.y -= 32.0f;
		for(int i = 0;i < 25;++ i)
		{
			order = findAFreeBullet();
			if(order < 0) return 0;
			m_bullet[order].init(position,_XVector2(0.8f * sin(PI * (0.08f * i + tempRand)),0.8f * cos(PI * (0.08f * i + tempRand))),
				GUN_SHOOT_ENERGY_00 * buffBulletPowerRate(),PI * (1.0f - (0.08f * i + tempRand)) * RADIAN_TO_ANGLE,&m_bulletTex,BULLET_TYPE_00);
		}
		return 0;
	}else
	if(type == BULLET_TYPE_03)
	{
		_XVector2 tempPoint2;
		int result = 0;
		for(int k = 0;k < gems->m_nowMaxtrixGemSum;++ k)
		{
			if(gems->m_gem[k].canCrash() != 0)
			{//宝石有效
				tempPoint2.set(gems->m_gem[k].m_position.x + 64.0f,gems->m_gem[k].m_position.y + 64.0f);
				if(tempPoint2.getLength(position) <= 100.0f)
				{//爆炸范围之内
					if(gems->m_gem[k].m_type <= GEM_TYPE_07)
					{
						result += gemBeCrashProc(gems,k,tempPoint2,AREA_BULLET_ENERGY * demageGunToGem[BULLET_TYPE_03][gems->m_gem[k].m_type] * buffBulletPowerRate());
					}else
					{
						result += gemBeCrashProc(gems,k,tempPoint2,AREA_BULLET_ENERGY * buffBulletPowerRate());
					}
				}
			}
		}
		return result;
	}
	return 0;
}

void _XGun::followBulletProc(_XGemMatrix *gems,int order)	//自动跟踪导弹的处理
{
	_XVector2 tempPoint;
	_XVector2 tempPoint1;
	tempPoint.set(m_bullet[order].m_position.x + 16.0f,m_bullet[order].m_position.y + 32.0f);
	if(m_bullet[order].m_armAngleFlag >= 0)
	{//有目标则检查目标是否有效
		if(gems->m_gem[m_bullet[order].m_armAngleFlag].canCrash() == 0)
		{//如果目标无效则取消目标
			m_bullet[order].m_armAngleFlag  = -1;
		}else
		{
			tempPoint1.set(gems->m_gem[m_bullet[order].m_armAngleFlag].m_position.x + 64.0f,
				gems->m_gem[m_bullet[order].m_armAngleFlag].m_position.y + 64.0f);
			if(tempPoint.getLength(tempPoint1) > 320.0)
			{//如果超出范围也无效
				m_bullet[order].m_armAngleFlag  = -1;
			}
		}
	}
	if(m_bullet[order].m_armAngleFlag == -1)
	{//下面开始寻找目标
		float tempMinLen = 320.0f;
		for(int j = 0;j < gems->m_nowMaxtrixGemSum;++ j)
		{
			if(gems->m_gem[j].canCrash() != 0 && (gems->m_gem[j].m_type != GEM_TYPE_15 && gems->m_gem[j].m_type != GEM_TYPE_21))	//自动跟踪的导弹不会打打不坏的目标,也不会追踪反面道具
			{//宝石有效
				tempPoint1.set(gems->m_gem[j].m_position.x + 64.0f,gems->m_gem[j].m_position.y + 64.0f);

				if(tempPoint.getLength(tempPoint1) < tempMinLen)
				{//启动跟踪过程
					m_bullet[order].m_armAngle = (90.0f + tempPoint.getAngle(tempPoint1) * RADIAN_TO_ANGLE) - m_bullet[order].m_angle;
					if(m_bullet[order].m_armAngle > 360.0f) m_bullet[order].m_armAngle -= 360.0f;
					if(m_bullet[order].m_armAngle > 180.0f) m_bullet[order].m_armAngle -= 360.0f;
					if(m_bullet[order].m_armAngle < -360.0f) m_bullet[order].m_armAngle += 360.0f;
					if(m_bullet[order].m_armAngle < -180.0f) m_bullet[order].m_armAngle += 360.0f;
					if(abs(m_bullet[order].m_armAngle) < 45.0f)
					{//需要一个临界角度
						tempMinLen = tempPoint.getLength(tempPoint1);
						m_bullet[order].m_armAngleFlag = j;
					}
				}
			}
		}
	}
	if(m_bullet[order].m_armAngleFlag >= 0)
	{//找到目标开始计算相关数据
		tempPoint1.set(gems->m_gem[m_bullet[order].m_armAngleFlag].m_position.x + 64.0f,
			gems->m_gem[m_bullet[order].m_armAngleFlag].m_position.y + 64.0f);

		m_bullet[order].m_armAngle = (90.0f + tempPoint.getAngle(tempPoint1) * RADIAN_TO_ANGLE) - m_bullet[order].m_angle;
		if(m_bullet[order].m_armAngle > 360.0f) m_bullet[order].m_armAngle -= 360.0f;
		if(m_bullet[order].m_armAngle > 180.0f) m_bullet[order].m_armAngle -= 360.0f;
		if(m_bullet[order].m_armAngle < -360.0f) m_bullet[order].m_armAngle += 360.0f;
		if(m_bullet[order].m_armAngle < -180.0f) m_bullet[order].m_armAngle += 360.0f;
	}
}

int _XGun::lightBulletProc(_XGemMatrix *gems,int order)
{
	int result = 0;
	_XVector2 tempPoint;
	_XVector2 tempPoint1;
	float tempAddSum;
	float tempLen;
	float tempTan = tan((m_bullet[order].m_angle + 90.0f) * ANGLE_TO_RADIAN); //tan(b)
	tempPoint.set(m_bullet[order].m_position.x + 32.0f,m_bullet[order].m_position.y + 1024.0f);
	for(int j = 0;j < gems->m_nowMaxtrixGemSum;++ j)
	{
		if(gems->m_gem[j].canCrash() != 0)
		{
			tempPoint1.set(gems->m_gem[j].m_position.x + 64.0f,gems->m_gem[j].m_position.y + 64.0f);
			//计算宝石到子弹线的位置
			tempLen = abs(tempTan * tempPoint1.x - tempPoint1.y + (tempPoint.y - tempTan * tempPoint.x)) /sqrt(tempTan * tempTan + 1.0f);
			if(tempLen <= gems->m_gem[j].m_size * 32.0f)
			{//被击中
				if(gems->m_gem[j].m_type >= GEM_TYPE_08)
				{//道具
					gems->m_gem[j].m_energy = 0.0f;
					gems->m_gem[j].setEnd();
					addTicketInput(gems->m_gem[j].m_type,gems->m_gem[j].m_size);
					result += crashToolsProc(gems,gems->m_gem[j].m_type,tempPoint1);
				}else
				{
					if(gems->m_gem[j].m_type == GEM_TYPE_15)
					{//打不坏的宝石
						gems->m_gem[j].m_energy = 0.0f;
						gems->m_gem[j].setEnd();
						addTicketInput(gems->m_gem[j].m_type,gems->m_gem[j].m_size);
						_XSound::GetInstance().play(m_soundGemBreak);
					}else
					{
						if(gems->m_gem[j].m_energy <= LINE_BULLET_DESTROY)
						{
							gems->m_gem[j].m_energy = 0.0f;
							gems->m_gem[j].setEnd();
							_XSound::GetInstance().play(m_soundGemBreak);
							addTicketInput(gems->m_gem[j].m_type,gems->m_gem[j].m_size);

							//tempAddSum = gemBasicEnergy[gems->m_gem[j].m_type] * gems->m_gem[j].m_size * GOLD_RATE;
							if(gems->m_gem[j].m_type > GEM_TYPE_07) 
							{
								exit(0);
							}
							tempAddSum = gemBasicEnergy[gems->m_gem[j].m_type] * gems->m_gem[j].m_size * gemE2GRate[gems->m_gem[j].m_type];
							m_goldArmSum += tempAddSum;
							m_scoresArmSum += tempAddSum;

							result += tempAddSum;
							//这里所搜飞行的金币
							findGoldFly(tempAddSum,gems->m_gem[j].m_position);
							canGetTools(gems,tempPoint1);
						}else
						{
							gems->m_gem[j].m_energy -= LINE_BULLET_DESTROY;
							gems->m_gem[j].m_isGlare = 100;
						}
					}
				}
			}
		}
	}
	return result; 
}

void _XGun::crashProc(_XGemMatrix *gems)//子弹的碰撞处理
{
	if(gems->m_matrixCanDraw == 0) return;
	if(m_isInited == 0) return;
	_XVector2 tempPoint;
	_XVector2 tempPoint1;
	int nowAddSum = 0;
	int tempAddSum;
	int hitToolsNeedHelp = 0;
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		if(m_bullet[i].getCanCrash() == 1)
		{//需要进行处理
			if(m_bullet[i].m_bulletType == BULLET_TYPE_04)
			{//启动跟踪过程
				followBulletProc(gems,i);
			}else
			if(m_bullet[i].m_bulletType == BULLET_TYPE_05)
			{
				if(m_bullet[i].m_bulletAlphaStage == 1)
				{
					m_bullet[i].m_bulletAlphaStage = 2;	//可以结束了
					nowAddSum += lightBulletProc(gems,i);
				}
				if(m_bullet[i].m_bulletAlphaStage == 3)
				{//标记完成
					m_isLightShoot = 0;
					m_bullet[i].m_bulletAlphaStage = 4;
					//根据能量修改炮的类型
					checkGunType();
				}
				break;
			}
			//计算子弹的位置
			tempPoint.set(m_bullet[i].m_position.x + 16.0f,m_bullet[i].m_position.y + 32.0f);
			for(int j = 0;j < gems->m_nowMaxtrixGemSum;++ j)
			{
				if(gems->m_gem[j].canCrash() != 0)
				{//宝石有效
					tempPoint1.set(gems->m_gem[j].m_position.x + 64.0f,gems->m_gem[j].m_position.y + 64.0f);
					if(tempPoint.getLength(tempPoint1) <= gems->m_gem[j].m_size * 16.0f)
					{//击中，下面计算能量损耗
						if(m_bullet[i].m_bulletType == BULLET_TYPE_02)
						{//如果是榴弹的话子弹会炸开
							nowAddSum += bulletBoom(tempPoint,BULLET_TYPE_02,gems);
							_XSound::GetInstance().play(m_soundBoom_03);
						}else
						if(m_bullet[i].m_bulletType == BULLET_TYPE_03)
						{//如果是高能榴弹炮的话，这里范围扫描,这样处理会有问题，需要先做销毁处理然后在做下面的处理，这样道具就被摧毁掉了，反射的道具无效了
							nowAddSum += bulletBoom(tempPoint,BULLET_TYPE_03,gems);
							m_bullet[i].setOver();
							_XSound::GetInstance().play(m_soundBoom_00);
							break;
						}
						if(gems->m_gem[j].m_type >= GEM_TYPE_08)
						{//道具
							if(m_bullet[i].m_energy >= gems->m_gem[j].m_energy)
							{
								hitToolsNeedHelp = 1;
								//下面是道具效果
								gems->m_gem[j].m_energy = 0.0f;
								gems->m_gem[j].setEnd();
								addTicketInput(gems->m_gem[j].m_type,gems->m_gem[j].m_size);
								nowAddSum += crashToolsProc(gems,gems->m_gem[j].m_type,tempPoint1);
								m_bullet[i].m_energy -= gems->m_gem[j].m_energy;
								if(gems->m_gem[j].m_type == GEM_TYPE_12)
								{//镜面反射炸弹,改变子弹的方向和能量,子弹变成加强型的子弹
									m_bullet[i].m_energy = ENERGY_ADD_TOOLS;
									m_bullet[i].m_angle = 90 - m_bullet[i].m_angle;
									m_bullet[i].m_bulletSprite_00.setAngle(m_bullet[i].m_angle);
									m_bullet[i].m_position.set(tempPoint1.x - 16,tempPoint1.y - 32);	//这个位置有问题
									m_bullet[i].m_speed.set(0.8f * sin((180.0f - m_bullet[i].m_angle) * ANGLE_TO_RADIAN),
										0.8f * cos((180.0f - m_bullet[i].m_angle) * ANGLE_TO_RADIAN));
									m_bullet[i].m_bulletType = BULLET_TYPE_X1;
									_XSound::GetInstance().play(m_soundFSTools);
								}else
								if(gems->m_gem[j].m_type == GEM_TYPE_13)
								{//镜面反射炸弹,改变子弹的方向和能量,子弹变成加强型的子弹
									m_bullet[i].m_energy = ENERGY_ADD_TOOLS;
									m_bullet[i].m_angle = 270 - m_bullet[i].m_angle;
									m_bullet[i].m_bulletSprite_00.setAngle(m_bullet[i].m_angle);
									m_bullet[i].m_position.set(tempPoint1.x - 16,tempPoint1.y - 32);
									m_bullet[i].m_speed.set(0.8f * sin((180.0f - m_bullet[i].m_angle) * ANGLE_TO_RADIAN),
										0.8f * cos((180.0f - m_bullet[i].m_angle) * ANGLE_TO_RADIAN));
									m_bullet[i].m_bulletType = BULLET_TYPE_X1;
									_XSound::GetInstance().play(m_soundFSTools);
								}
								if(m_bullet[i].m_energy < 1)
								{//子弹消失
									m_bullet[i].setOver();
									break;
								}
							}else
							{
								gems->m_gem[j].m_energy -= m_bullet[i].m_energy;
								m_bullet[i].setOver();
								_XSound::GetInstance().play(m_soundHitGem);
							}
						}else
						{//宝石
							if(gems->m_gem[j].m_type == GEM_TYPE_15)
							{//打不坏的道具，但是可以被彩色蛋打坏
								if(m_bullet[i].m_bulletType == BULLET_TYPE_X0)
								{//搜索所有的宝石，并处理
									hitToolsNeedHelp = 1;
									gems->m_gem[j].m_energy = 0.0f;
									gems->m_gem[j].setEnd();
									addTicketInput(gems->m_gem[j].m_type,gems->m_gem[j].m_size);
									nowAddSum += colorBulletProc(gems,gems->m_gem[j].m_type);
									m_bullet[i].setOver();
									_XSound::GetInstance().play(m_soundGemBreak);
									break;
								}else
								{
								//	gems->m_gem[j].m_isGlare = 100;
								//	m_bullet[i].setOver();
								//	_XSound::GetInstance().play(m_soundHitShield);
								//	break;
									if(m_bullet[i].m_energy >= gems->m_gem[j].m_energy)
									{
										m_bullet[i].m_energy -= gems->m_gem[j].m_energy;
										if(m_bullet[i].m_energy < 0.5f) m_bullet[i].setOver();

										gems->m_gem[j].m_energy = 0.0f;
										gems->m_gem[j].setEnd();
										addTicketInput(gems->m_gem[j].m_type,gems->m_gem[j].m_size);
										_XSound::GetInstance().play(m_soundGemBreak);
									}else
									{
										gems->m_gem[j].m_energy -= m_bullet[i].m_energy;
										m_bullet[i].m_energy = 0.0f;
										m_bullet[i].setOver();
										gems->m_gem[j].m_isGlare = 100;
										_XSound::GetInstance().play(m_soundHitShield);
									}
									break;
								}
							}else
							{//普通宝石
								float tempDamage = m_bullet[i].m_energy;
								if(m_bullet[i].m_bulletType <= BULLET_TYPE_05 && gems->m_gem[j].m_type <= GEM_TYPE_07)
								{
									tempDamage = m_bullet[i].m_energy * demageGunToGem[m_bullet[i].m_bulletType][gems->m_gem[j].m_type];
								}
								 
								if(tempDamage >= gems->m_gem[j].m_energy)
								{//宝石被摧毁
									if(m_bullet[i].m_bulletType <= BULLET_TYPE_05 && gems->m_gem[j].m_type <= GEM_TYPE_07)
									{
										if(demageGunToGem[m_bullet[i].m_bulletType][gems->m_gem[j].m_type] <= 0.0f)
										{
											exit(0);
										}
										m_bullet[i].m_energy -= gems->m_gem[j].m_energy / demageGunToGem[m_bullet[i].m_bulletType][gems->m_gem[j].m_type];
									}else
									{
										m_bullet[i].m_energy -= gems->m_gem[j].m_energy;
									}
									if(m_bullet[i].m_energy < 0.5f) m_bullet[i].setOver();
									_XSound::GetInstance().play(m_soundGemBreak);
									gems->m_gem[j].m_energy = 0.0f;
									gems->m_gem[j].setEnd();
									addTicketInput(gems->m_gem[j].m_type,gems->m_gem[j].m_size);

									//tempAddSum = gemBasicEnergy[gems->m_gem[j].m_type] * gems->m_gem[j].m_size * GOLD_RATE;
									if(gems->m_gem[j].m_type > GEM_TYPE_07) 
									{
										exit(0);
									}
									tempAddSum = gemBasicEnergy[gems->m_gem[j].m_type] * gems->m_gem[j].m_size * gemE2GRate[gems->m_gem[j].m_type];
									m_goldArmSum += tempAddSum;
									m_scoresArmSum += tempAddSum;

									nowAddSum += tempAddSum;
									//这里所搜飞行的金币
									findGoldFly(tempAddSum,gems->m_gem[j].m_position);
									canGetTools(gems,tempPoint1);
									if(m_bullet[i].m_bulletType == BULLET_TYPE_X0)
									{//搜索所有的宝石，并处理
										nowAddSum += colorBulletProc(gems,gems->m_gem[j].m_type);
										//彩色子弹不能穿透
										m_bullet[i].setOver();
									}
								}else
								{
									gems->m_gem[j].m_energy -= tempDamage;
									gems->m_gem[j].m_isGlare = 100;
									m_bullet[i].setOver();
									_XSound::GetInstance().play(m_soundHitGem);

									if(m_bullet[i].m_bulletType == BULLET_TYPE_X0)
									{//搜索所有的宝石，并处理
										nowAddSum += colorBulletProc(gems,gems->m_gem[j].m_type);
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	nowAddSum *= buffGoldRate();
	if(nowAddSum > 0)
	{
		for(int i = 0;i < MAX_BULLET_SUM;++ i)
		{
			if(m_goldAddFlag[i] == 0)
			{
				char data[20];
				if(m_userBuffFlag[0] != 0) sprintf(data,"+%d -2",(int)(nowAddSum / buffGoldRate()));
				else sprintf(data,"+%d",nowAddSum);
				m_goldAddNum[i].setNumber(data);
				m_goldAddFlag[i] = 1;
				m_goldAddBasicX[i] = 150;
				m_goldAddBasicY[i] = -10;
				break;
			}
		}
		//下面计算当前还剩下的宝石数量
		gems->getReleaseGemSum();
	//	m_levelFiniedArmSum = 1.0f - (float)((float)(gems->m_matrixReleaseGemSum) / (float)(gems->m_matrixAllGemSum));
		if((float)(gems->m_matrixAvailablyGemSum) == 0)
		{
			m_levelFiniedArmSum = 1.0f;
		}else
		{
			m_levelFiniedArmSum = 1.0f - (float)((float)(gems->m_matrixReleaseGemSum) / (float)(gems->m_matrixAvailablyGemSum));
		}
		if(m_levelFiniedArmSum >= 0.9f && m_gameCounter == 0)
		{//摧毁90%的宝石，及完成任务可以过关
			m_levelFlagStage = 1;	//设置过关条件跳动
			m_gameCounter = 1;
			m_gamePlayOverTimer = 5000;
		}
		//更新玩家的排名
		updateGameOrder();
		if(gems->upDateHelp() != 0)
		{
			if(gems->getHelpIsEnd() != 0)
			{
				setHelpEnd();
			}else
			{
				setHelpPosition(m_gems->m_gem[m_gems->m_helpData[m_gems->m_nowHelpPoint].gemOrder].m_position.x,
					m_gems->m_gem[m_gems->m_helpData[m_gems->m_nowHelpPoint].gemOrder].m_position.y);
			}
		}
	}else
	if(hitToolsNeedHelp == 1)
	{
		if(gems->upDateHelp() != 0)
		{
			if(gems->getHelpIsEnd() != 0)
			{
				setHelpEnd();
			}else
			{
				setHelpPosition(m_gems->m_gem[m_gems->m_helpData[m_gems->m_nowHelpPoint].gemOrder].m_position.x,
					m_gems->m_gem[m_gems->m_helpData[m_gems->m_nowHelpPoint].gemOrder].m_position.y);
			}
		}
	}
}

float goldFlyLevel[10][3] = {
	{ 10,1,0.25f},
	{ 15,2,0.25f},
	{ 20,3,0.25f},
	{ 30,1,0.5f},
	{ 35,2,0.5f},
	{ 40,3,0.5f},
	{ 50,1,0.75f},
	{ 55,2,0.75f},
	{ 60,3,0.75f},
	{ 70,1,1.0f}};

void _XGun::findGoldFly(int level,_XVector2 position)
{
	int needSum = 1;
	float size = 0;
	for(int i = 0;i < 10;++ i)
	{
		if(level <= goldFlyLevel[i][0] || i == 9)
		{
			needSum = goldFlyLevel[i][1];
			size = goldFlyLevel[i][2];
			break;
		}
	}
	int oldNeedSum = needSum;
	for(int i = 0;i < MAX_BULLET_SUM * 10;++ i)
	{
		if(m_goldFlyFlag[i] == 0)
		{
			m_goldFlyFlag[i] = 1;
			if(oldNeedSum == 1)
			{
				m_goldFlyPosition[i].set(position.x,position.y);
			}else
			if(oldNeedSum == 2)
			{
				m_goldFlyPosition[i].set(position.x + 128.0f * size * (needSum - 1),position.y);
			}else
			if(oldNeedSum <= 4)
			{
				m_goldFlyPosition[i].set(position.x + 128.0f * size * (needSum / 2.0f - 1.0f),position.y + 128.0f * size * ((needSum % 2) - 1.0f));
			}else
			if(oldNeedSum <= 9)
			{
				m_goldFlyPosition[i].set(position.x + 128.0f * size * (needSum / 3.0f - 1.0f),position.y + 128.0f * size * ((needSum % 3) - 1.0f));
			}
			m_goldFly[i].setSize(size,size);
			m_goldFlyParam[i] = random(100) / 200.0f;
			needSum --;
			if(needSum <= 0) break;
		}
	}
}

#define DATA_ID (1099)
_XGun::_XGun()
:m_isInited(0)
,m_gunAngle(90)
,m_gunType(GUN_TYPE_00)
,m_gunShootSpeed(GUN_SHOOT_SPEED_00)
,m_bullet(NULL)
,m_gunFireAlpha(0.0f)
,m_gunFireAlphaStage(0)
,m_creditSum(0)
,m_scoresSum(0)
,m_scoresArmSum(0)
,m_energySum(0)
,m_energyArmSum(0)
,m_energyAlphaStage(0)
,m_goldSum(0)
,m_goldArmSum(0)
,m_isGunColorBoom(0)
,m_gunIsFlash(0)
,m_ticketbarArmSum(0)
,m_ticketbarSum(0)
,m_toolsBoomFlag(0)
,m_instructionMode(0)
,m_instructionTimer(0)
//,m_backGroundColor(0)
//,m_backGroundTimer(0)
,m_backGroungPosition(0.0f)
,m_gameOrderCameraData(NULL)
,m_gameOrderUserHeadData(NULL)
,m_cameraTexData(NULL)
,m_takePhotoCanPressKey(0)
,m_takePhotoKeyFlag(2)
,m_takePhotoNeedShow(0)
,m_lineStartTime(0)
,m_lineStartX(0.0f)
{
	m_data00.init(DATA_ID,false,ANALYZING_IMMEDIATELY);
	m_data01.init(DATA_ID + 1,false,ANALYZING_IMMEDIATELY);
	m_data02.init(DATA_ID + 2,false,ANALYZING_IMMEDIATELY);
}

int _XBullet::init(_XVector2 position,_XVector2 speed,float energy,float angle,_XBulletTexture *tex,_XBULLET_TYPE type)
{
	if(tex == NULL) return 0;
	if(energy <= 0) return 0;
	m_speed = speed;
	m_nowSpeedData.set(0.25f,1.2f,0.1f,MOVE_DATA_MODE_COS_MULT);
	m_energy = energy;
	m_angle = angle;
	m_tex = tex;
	m_position = position;
	m_bulletType = type;

	if(m_bulletType == BULLET_TYPE_00 || m_bulletType == BULLET_TYPE_01 || m_bulletType == BULLET_TYPE_02 || 
		m_bulletType == BULLET_TYPE_03 || m_bulletType == BULLET_TYPE_04 ||
		m_bulletType == BULLET_TYPE_X0 || m_bulletType == BULLET_TYPE_X1)
	{
		m_bulletSprite_00.init(m_tex->m_bulletTex_00.texture.m_w,m_tex->m_bulletTex_00.texture.m_h);
		m_bulletSprite_00.setAngle(m_angle);
		m_bulletSprite_00.setPosition(m_position);
	}
	if(m_bulletType == BULLET_TYPE_05)
	{//激光炮
		m_bulletSprite_01.init(m_tex->m_bulletTex_05.texture.m_w,m_tex->m_bulletTex_05.texture.m_h);
		m_bulletSprite_01.setAngle(m_angle);
		m_bulletSprite_01.setPosition(m_position);
		m_bulletAlpha = 0.0f;
		m_bulletSprite_01.setAlpha(m_bulletAlpha);
		m_bulletAlphaStage = 0;
	}else
	{
		m_bulletSprite_00.setAlpha(1.0f);
	}
	if(m_bulletType == BULLET_TYPE_03)
	{
		m_bulletFrame.release();
		m_bulletFrame.setACopy(m_tex->m_bulletFrame);
		m_bulletFrame.reset();
		m_bulletFrame.setAngle(m_angle);
		m_bulletFrame.setPosition(m_position);
	}
	if(m_bulletType == BULLET_TYPE_00 || m_bulletType == BULLET_TYPE_01 || m_bulletType == BULLET_TYPE_04)
	{
		m_bulletFrame.release();
		m_bulletFrame.setACopy(m_tex->m_bulletBoom);
		m_bulletFrame.reset();
		m_bulletFrame.setAngle(m_angle);
		m_bulletFrame.setPosition(m_position);
	}
	m_armAngle = 0;
	m_armAngleFlag = -1;

	m_bulletStage = 1;
	return 1;
}

void _XBullet::draw()
{
	if(m_bulletStage == 0) return;
	if(m_bulletType == BULLET_TYPE_00)
	{
		//m_bulletSprite_00.draw(&(m_tex->m_bulletTex_00));
		if(m_bulletStage == 1)
		{
			m_bulletSprite_00.draw(&(m_tex->m_bulletTex_00));
		}else
		if(m_bulletStage == 3)
		{
			m_bulletFrame.draw();
		}
	}else
	if(m_bulletType == BULLET_TYPE_01)
	{
		//m_bulletSprite_00.draw(&(m_tex->m_bulletTex_01));
		if(m_bulletStage == 1)
		{
			m_bulletSprite_00.draw(&(m_tex->m_bulletTex_01));
		}else
		if(m_bulletStage == 3)
		{
			m_bulletFrame.draw();
		}
	}else
	if(m_bulletType == BULLET_TYPE_02)
	{
		m_bulletSprite_00.draw(&(m_tex->m_bulletTex_02));
	}else
	if(m_bulletType == BULLET_TYPE_03)
	{
		if(m_bulletStage == 1)
		{
			m_bulletSprite_00.draw(&(m_tex->m_bulletTex_03));
		}else
		if(m_bulletStage == 3)
		{
			m_bulletFrame.draw();
		}
	}else
	if(m_bulletType == BULLET_TYPE_04)
	{
		//m_bulletSprite_00.draw(&(m_tex->m_bulletTex_04));
		if(m_bulletStage == 1)
		{
			m_bulletSprite_00.draw(&(m_tex->m_bulletTex_04));
		}else
		if(m_bulletStage == 3)
		{
			m_bulletFrame.draw();
		}
	}else
	if(m_bulletType == BULLET_TYPE_05)
	{
		m_bulletSprite_01.draw(&(m_tex->m_bulletTex_05));
	}else
	if(m_bulletType == BULLET_TYPE_X0)
	{
		m_bulletSprite_00.draw(&(m_tex->m_bulletTex_X0));
	}else
	if(m_bulletType == BULLET_TYPE_X1)
	{
		m_bulletSprite_00.draw(&(m_tex->m_bulletTex_X1));
	}
}

int _XBullet::move(int interval)
{
	if(m_bulletStage == 0) return 0;
	int ret = 0;
	if(m_bulletType == BULLET_TYPE_05)
	{
		if(m_bulletStage == 1)
		{
			if(m_bulletAlphaStage == 0)
			{//出现
				m_bulletAlpha += 0.01f * interval;
				if(m_bulletAlpha >= 1.0f)
				{
					m_bulletAlpha = 1.0f;
					m_bulletAlphaStage = 1;
				}
				m_bulletSprite_01.setAlpha(m_bulletAlpha);
			}else
			if(m_bulletAlphaStage == 2)
			{//消失
				m_bulletAlpha -= 0.005f * interval;
				if(m_bulletAlpha <= 0.0f)
				{
					m_bulletAlpha = 0.0f;
					m_bulletAlphaStage = 3;
				}
				m_bulletSprite_01.setAlpha(m_bulletAlpha);
			}else
			if(m_bulletAlphaStage == 4)
			{//消失
				m_bulletStage = 0;
			}
		}
	}
	if(m_bulletType == BULLET_TYPE_00 || m_bulletType == BULLET_TYPE_01 || m_bulletType == BULLET_TYPE_02 || 
		m_bulletType == BULLET_TYPE_03 || m_bulletType == BULLET_TYPE_04 ||
		m_bulletType == BULLET_TYPE_X0 || m_bulletType == BULLET_TYPE_X1)
	{//机枪扫射子弹
		if(m_bulletStage == 1)
		{
			if(m_bulletType == BULLET_TYPE_04)
			{
				if(m_nowSpeedData.getIsEnd() == 0)
				{
					m_nowSpeedData.move(interval);
					m_position.x += m_speed.x * m_nowSpeedData.getNowData() * interval;
					m_position.y += m_speed.y * m_nowSpeedData.getNowData() * interval;
				}else
				{
					m_position.x += m_speed.x * interval;
					m_position.y += m_speed.y * interval;
				}
			}else
			{
				m_position.x += m_speed.x * interval;
				m_position.y += m_speed.y * interval;
			}
			m_bulletSprite_00.setPosition(m_position);

			//下面是导弹跟踪的计算
			if(m_bulletType == BULLET_TYPE_04)
			{
				if(m_armAngleFlag >= 0)
				{//已经启动跟踪
					if(m_armAngle != 0)
					{
						if(abs(m_armAngle) < 0.2f * interval)
						{
							m_angle += m_armAngle;
						}else
						{
							if(m_armAngle > 0) m_angle += 0.2f * interval;
							else m_angle -= 0.2f * interval; 
						}
						m_bulletSprite_00.setAngle(m_angle);
						m_speed.x = 0.8f * sin((180.0f - m_angle) * ANGLE_TO_RADIAN),
						m_speed.y = 0.8f * cos((180.0f - m_angle) * ANGLE_TO_RADIAN);
					}
				}
			}
			//死亡判断,下面需要增加边界反弹
			if(m_position.x >= 0 && m_position.x <= 1280 && m_bulletType != BULLET_TYPE_04)	//跟踪弹不能在墙面反弹
			{
				if(m_position.y <= 0.0f || m_position.y >= 720.0f - 64.0f)
				{//这里需要防止重复反弹
					if(m_angle > 360) m_angle -= 360;
					if(m_angle < 0) m_angle += 360;
					if(m_angle < 10 || m_angle > 350)
					{
						if(m_angle < 10) m_angle = 15.0f;	//防止重复反弹
						else  m_angle = 345.0f;	//防止重复反弹
						m_speed.x = 0.8f * sin((180.0f - m_angle) * ANGLE_TO_RADIAN),
						m_speed.y = 0.8f * cos((180.0f - m_angle) * ANGLE_TO_RADIAN);
					}
					m_speed.y = -m_speed.y;
					m_angle = 180 - m_angle;
					m_bulletSprite_00.setAngle(m_angle);
					if(m_position.y <= 0.0f) 
					{
						m_position.y = 1.0f;
						ret = 1;
					}
					if(m_position.y >= 720.0f - 64.0f) 
					{
						m_position.y = 720.0f - 64.0f - 1.0f;
						ret = 2;
					}
				}
			}
			if(m_energy <= 0 || m_position.x < - 128 || m_position.x > 1280 + 128 || m_position.y < -128 || m_position.y > 720 + 128)
			{//能量消耗完成或者是位置超出范围
				m_bulletStage = 0;
				m_bulletSprite_00.release();
			}
		}else
		if(m_bulletStage == 2)
		{
			if(m_bulletType == BULLET_TYPE_00 || m_bulletType == BULLET_TYPE_01 || m_bulletType == BULLET_TYPE_04)
			{
				m_bulletFrame.setPosition(m_position.x,m_position.y);
			}else
			if(m_bulletType == BULLET_TYPE_03)
			{
				m_bulletFrame.setPosition(m_position.x - 128.0f + 16.0f,m_position.y - 128.0f + 32.0f);
			}
			m_bulletStage = 3;
		}else
		if(m_bulletStage == 3)
		{
			m_bulletFrame.move(interval);
			if(m_bulletFrame.getIsEnd() == 1)
			{
				m_bulletStage = 0;
				m_bulletFrame.release();
			}
		}
	}
	return ret;
}

int _XBullet::getIsEnd()			//子弹是否已经无效
{
	if(m_bulletStage == 0) return 1;
	else return 0;
}

int _XBullet::getCanCrash()
{
	if(m_bulletStage == 1) return 1;
	else return 0;
}

_XBullet::_XBullet()
:m_bulletStage(0)
,m_bulletType(BULLET_TYPE_00)
{
}

int _XBulletTexture::init(_XResourcePosition resoursePosition)
{
	if(m_isInited == 1) return 1;
	if(m_bulletTex_00.load("ResourcePack/pic/gun/BB_000_01.png",resoursePosition) == 0) return 0;
	if(m_bulletTex_01.load("ResourcePack/pic/gun/BB_001_01.png",resoursePosition) == 0) return 0;
	if(m_bulletTex_02.load("ResourcePack/pic/gun/BB_002_01.png",resoursePosition) == 0) return 0;
	if(m_bulletTex_03.load("ResourcePack/pic/gun/BB_003_01.png",resoursePosition) == 0) return 0;
	if(m_bulletTex_04.load("ResourcePack/pic/gun/BB_004_01.png",resoursePosition) == 0) return 0;
	if(m_bulletTex_05.load("ResourcePack/pic/gun/BB_005_01.png",resoursePosition) == 0) return 0;
	if(m_bulletTex_X0.load("ResourcePack/pic/gun/BBC_000_01.png",resoursePosition) == 0) return 0;
	if(m_bulletTex_X1.load("ResourcePack/pic/gun/BBS_000_01.png",resoursePosition) == 0) return 0;

	if(m_bulletFrame.init("ResourcePack/pic/UI/Boom_04/Boom_000.png",resoursePosition) == 0) return 0;
	m_bulletFrame.setAttribute(_XVector2(0.0f,0.0f),0,1,0,0.05f,0);
	if(m_bulletBoom.init("ResourcePack/pic/UI/BulletBoom/BulletBoom_000.png",resoursePosition) == 0) return 0;
	m_bulletBoom.setAttribute(_XVector2(0.0f,0.0f),0,1,0,0.05f,0);
	
	m_isInited = 1;
	return 1;
}

_XBulletTexture::_XBulletTexture()
:m_isInited(0)
{
}

int _XGun::instructInit(_XResourcePosition resoursePosition)
{
	//if(m_instructionBG.init("normalResource/instruction.jpg",resoursePosition) == 0) return 0;		//说明界面的文字图
	if(m_instructionBG.init("ResourcePack/pic/instruction.png",resoursePosition) == 0) return 0;		//说明界面的文字图
	m_instructionBG.setPosition(0.0f,0.0f);
	m_instructionOBJ_00.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);		//说明界面的物件贴图
	m_instructionOBJ_00.setAngle(90);
	m_instructionOBJ_00.setPosition(1030.0f - 0.0f,80.0f);
	m_instructionOBJ_01.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_01.setAngle(90);
	m_instructionOBJ_01.setPosition(1030.0f - 110.0f,80.0f);
	m_instructionOBJ_02.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_02.setAngle(90);
	m_instructionOBJ_02.setPosition(1030.0f - 220.0f,80.0f);
	m_instructionOBJ_03.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_03.setAngle(90);
	m_instructionOBJ_03.setPosition(1030.0f - 330.0f,80.0f);
	m_instructionOBJ_04.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_04.setAngle(90);
	m_instructionOBJ_04.setPosition(1030.0f - 440.0f,80.0f);
	m_instructionOBJ_05.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_05.setAngle(90);
	m_instructionOBJ_05.setPosition(1030.0f - 550.0f,80.0f);
	m_instructionOBJ_06.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_06.setAngle(90);
	m_instructionOBJ_06.setPosition(1030.0f - 660.0f,80.0f);
	m_instructionOBJ_07.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_07.setAngle(90);
	m_instructionOBJ_07.setPosition(1030.0f - 770.0f,80.0f);
	m_instructionOBJ_13.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_13.setAngle(90);
	m_instructionOBJ_13.setPosition(1030.0f - 880.0f,80.0f);
	m_instructionOBJ_08.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_08.setAngle(90);
	m_instructionOBJ_08.setPosition(1030.0f - 0.0f,80.0f);
	m_instructionOBJ_09.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_09.setAngle(90);
	m_instructionOBJ_09.setPosition(1030.0f - 110.0f,80.0f);
	m_instructionOBJ_10.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);		//说明界面的物件贴图
	m_instructionOBJ_10.setAngle(90);
	m_instructionOBJ_10.setPosition(1030.0f - 220.0f,80.0f);
	m_instructionOBJ_11.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_11.setAngle(90);
	m_instructionOBJ_11.setPosition(1030.0f - 330.0f,80.0f);
	m_instructionOBJ_12.init(m_gems->m_gemTexTools[0].texture.m_w,m_gems->m_gemTexTools[0].texture.m_h);
	m_instructionOBJ_12.setAngle(90);
	m_instructionOBJ_12.setPosition(1030.0f - 440.0f,80.0f);
	if(m_instructionText_00.init("ResourcePack/pic/UI/instruction/instruction_00.png",resoursePosition) == 0) return 0;		//说明界面的文字图
	m_instructionText_00.setAngle(90);
	m_instructionText_00.setPosition(810.0f - 0.0f,405.0f);
	if(m_instructionText_01.init("ResourcePack/pic/UI/instruction/instruction_01.png",resoursePosition) == 0) return 0;
	m_instructionText_01.setAngle(90);
	m_instructionText_01.setPosition(810.0f - 110.0f,405.0f);
	if(m_instructionText_02.init("ResourcePack/pic/UI/instruction/instruction_02.png",resoursePosition) == 0) return 0;
	m_instructionText_02.setAngle(90);
	m_instructionText_02.setPosition(810.0f - 220.0f,405.0f);
	if(m_instructionText_03.init("ResourcePack/pic/UI/instruction/instruction_03.png",resoursePosition) == 0) return 0;
	m_instructionText_03.setAngle(90);
	m_instructionText_03.setPosition(810.0f - 330.0f,405.0f);
	if(m_instructionText_04.init("ResourcePack/pic/UI/instruction/instruction_04.png",resoursePosition) == 0) return 0;
	m_instructionText_04.setAngle(90);
	m_instructionText_04.setPosition(810.0f - 440.0f,405.0f);
	if(m_instructionText_05.init("ResourcePack/pic/UI/instruction/instruction_05.png",resoursePosition) == 0) return 0;
	m_instructionText_05.setAngle(90);
	m_instructionText_05.setPosition(810.0f - 550.0f,405.0f);
	if(m_instructionText_06.init("ResourcePack/pic/UI/instruction/instruction_06.png",resoursePosition) == 0) return 0;
	m_instructionText_06.setAngle(90);
	m_instructionText_06.setPosition(810.0f - 660.0f,405.0f);
	if(m_instructionText_07.init("ResourcePack/pic/UI/instruction/instruction_07.png",resoursePosition) == 0) return 0;
	m_instructionText_07.setAngle(90);
	m_instructionText_07.setPosition(810.0f - 770.0f,405.0f);
	if(m_instructionText_13.init("ResourcePack/pic/UI/instruction/instruction_13.png",resoursePosition) == 0) return 0;
	m_instructionText_13.setAngle(90);
	m_instructionText_13.setPosition(810.0f - 880.0f,405.0f);
	if(m_instructionText_08.init("ResourcePack/pic/UI/instruction/instruction_08.png",resoursePosition) == 0) return 0;
	m_instructionText_08.setAngle(90);
	m_instructionText_08.setPosition(810.0f - 0.0f,405.0f);
	if(m_instructionText_09.init("ResourcePack/pic/UI/instruction/instruction_09.png",resoursePosition) == 0) return 0;
	m_instructionText_09.setAngle(90);
	m_instructionText_09.setPosition(810.0f - 110.0f,405.0f);
	if(m_instructionText_10.init("ResourcePack/pic/UI/instruction/instruction_10.png",resoursePosition) == 0) return 0;
	m_instructionText_10.setAngle(90);
	m_instructionText_10.setPosition(810.0f - 220.0f,405.0f);
	if(m_instructionText_11.init("ResourcePack/pic/UI/instruction/instruction_11.png",resoursePosition) == 0) return 0;
	m_instructionText_11.setAngle(90);
	m_instructionText_11.setPosition(810.0f - 330.0f,405.0f);
	if(m_instructionText_12.init("ResourcePack/pic/UI/instruction/instruction_12.png",resoursePosition) == 0) return 0;
	m_instructionText_12.setAngle(90);
	m_instructionText_12.setPosition(810.0f - 440.0f,405.0f);
	return 1;
}

void _XGun::instructDraw()
{
	if(m_gameStage == GAME_STAGE_INSTRUCTION)
	{
		m_gameDemoBG.drawWithoutBlend();
		m_gameDemoText1.draw();
		m_gameDemoText2.draw();

		m_instructionBG.drawWithoutBlend();
		if(m_instructionMode == 0)
		{
			m_instructionOBJ_00.draw(&(m_gems->m_gemTexTools[10]));
			m_instructionOBJ_01.draw(&(m_gems->m_gemTexTools[0]));
			m_instructionOBJ_02.draw(&(m_gems->m_gemTexTools[4]));
			m_instructionOBJ_03.draw(&(m_gems->m_gemTexTools[1]));
			m_instructionOBJ_04.draw(&(m_gems->m_gemTexTools[12]));
			m_instructionOBJ_05.draw(&(m_gems->m_gemTexTools[3]));
			m_instructionOBJ_06.draw(&(m_gems->m_gemTexTools[11]));
			m_instructionOBJ_07.draw(&(m_gems->m_gemTexTools[2]));
			m_instructionOBJ_13.draw(&(m_gems->m_gemTexTools[13]));
			m_instructionText_00.draw();
			m_instructionText_01.draw();
			m_instructionText_02.draw();
			m_instructionText_03.draw();
			m_instructionText_04.draw();
			m_instructionText_05.draw();
			m_instructionText_06.draw();
			m_instructionText_07.draw();
			m_instructionText_13.draw();
		}else
		{
			m_instructionOBJ_08.draw(&(m_gems->m_gemTexTools[8]));
			m_instructionOBJ_09.draw(&(m_gems->m_gemTexTools[7]));
			m_instructionOBJ_10.draw(&(m_gems->m_gemTexTools[6]));
			m_instructionOBJ_11.draw(&(m_gems->m_gemTexTools[5]));
			m_instructionOBJ_12.draw(&(m_gems->m_gemTexTools[9]));
			m_instructionText_08.draw();
			m_instructionText_09.draw();
			m_instructionText_10.draw();
			m_instructionText_11.draw();
			m_instructionText_12.draw();
			userHeadDraw();
		}
		if(m_releaseTicket > 0) ticketDraw();
	}
}

void _XGun::instructMove(int interval)
{
	if(m_gameStage == GAME_STAGE_INSTRUCTION)
	{
		if(m_gameCounter == 0)
		{//初始化所有的变量
			m_instructionPosition = 0.0f;
			m_instructionBG.setPosition(1280.0f,0.0f);
			m_instructionOBJ_00.setPosition(1030.0f - 0.0f,720.0f + 80.0f);
			m_instructionOBJ_01.setPosition(1030.0f - 110.0f,720.0f + 80.0f);
			m_instructionOBJ_02.setPosition(1030.0f - 220.0f,720.0f + 80.0f);
			m_instructionOBJ_03.setPosition(1030.0f - 330.0f,720.0f + 80.0f);
			m_instructionOBJ_04.setPosition(1030.0f - 440.0f,720.0f + 80.0f);
			m_instructionOBJ_05.setPosition(1030.0f - 550.0f,720.0f + 80.0f);
			m_instructionOBJ_06.setPosition(1030.0f - 660.0f,720.0f + 80.0f);
			m_instructionOBJ_07.setPosition(1030.0f - 770.0f,720.0f + 80.0f);
			m_instructionOBJ_13.setPosition(1030.0f - 880.0f,720.0f + 80.0f);
			m_instructionOBJ_08.setPosition(1030.0f - 0.0f,720.0f + 80.0f);
			m_instructionOBJ_09.setPosition(1030.0f - 110.0f,720.0f + 80.0f);
			m_instructionOBJ_10.setPosition(1030.0f - 220.0f,720.0f + 80.0f);
			m_instructionOBJ_11.setPosition(1030.0f - 330.0f,720.0f + 80.0f);
			m_instructionOBJ_12.setPosition(1030.0f - 440.0f,720.0f + 80.0f);
			m_instructionText_00.setPosition(810.0f - 0.0f,720.0f + 405.0f);
			m_instructionText_01.setPosition(810.0f - 110.0f,720.0f + 405.0f);
			m_instructionText_02.setPosition(810.0f - 220.0f,720.0f + 405.0f);
			m_instructionText_03.setPosition(810.0f - 330.0f,720.0f + 405.0f);
			m_instructionText_04.setPosition(810.0f - 440.0f,720.0f + 405.0f);
			m_instructionText_05.setPosition(810.0f - 550.0f,720.0f + 405.0f);
			m_instructionText_06.setPosition(810.0f - 660.0f,720.0f + 405.0f);
			m_instructionText_07.setPosition(810.0f - 770.0f,720.0f + 405.0f);
			m_instructionText_13.setPosition(810.0f - 880.0f,720.0f + 405.0f);
			m_instructionText_08.setPosition(810.0f - 0.0f,720.0f + 405.0f);
			m_instructionText_09.setPosition(810.0f - 110.0f,720.0f + 405.0f);
			m_instructionText_10.setPosition(810.0f - 220.0f,720.0f + 405.0f);
			m_instructionText_11.setPosition(810.0f - 330.0f,720.0f + 405.0f);
			m_instructionText_12.setPosition(810.0f - 440.0f,720.0f + 405.0f);
			_XSound::GetInstance().play(m_soundScreenDown);
			m_gameCounter = 1;
			if(m_instructionMode != 0)
			{
				userHeadMove(interval,1280.0f);
			}
		}else
		if(m_gameCounter == 1)
		{//背景落下
			m_instructionPosition += 0.002f * interval;
			if(m_instructionPosition >= PI_HALF)
			{
				m_instructionTemp = m_instructionPosition;
				m_instructionPosition = PI_HALF;
				m_gameCounter = 2;
			}
			m_instructionBG.setPosition(1280.0f * cos(m_instructionPosition),0.0f);
			if(m_instructionMode != 0)
			{
				userHeadMove(interval,1280.0f * cos(m_instructionPosition));
			}
		}else
		if(m_gameCounter == 2)
		{//图案和文字出现
			if(m_instructionMode == 0)
			{
				m_instructionPosition += 0.004f * interval;
				if(m_instructionPosition >= PI_HALF * 10.0f)
				{
					m_instructionPosition = PI_HALF * 10.0f;
					m_gameCounter = 3;
				}
				if(m_instructionPosition >= PI_HALF && m_instructionPosition <= PI_HALF * 2.0)
				{
					m_instructionOBJ_00.setPosition(1030.0f - 0.0f,720.0f * cos(m_instructionPosition - PI_HALF) + 80.0f);
					m_instructionText_00.setPosition(810.0f - 0.0f,720.0f * cos(m_instructionPosition - PI_HALF) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 2.0f)
				{
					m_instructionOBJ_00.setPosition(1030.0f - 0.0f,80.0f);
					m_instructionText_00.setPosition(810.0f - 0.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 2.0f && m_instructionPosition <= PI_HALF * 3.0f)
				{
					m_instructionOBJ_01.setPosition(1030.0f - 110.0f,720.0f * cos(m_instructionPosition - PI_HALF * 2.0f) + 80.0f);
					m_instructionText_01.setPosition(810.0f - 110.0f,720.0f * cos(m_instructionPosition - PI_HALF * 2.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 3.0f)
				{
					m_instructionOBJ_01.setPosition(1030.0f - 110.0f,80.0f);
					m_instructionText_01.setPosition(810.0f - 110.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 3.0f && m_instructionPosition <= PI_HALF * 4.0f)
				{
					m_instructionOBJ_02.setPosition(1030.0f - 220.0f,720.0f * cos(m_instructionPosition - PI_HALF * 3.0f) + 80.0f);
					m_instructionText_02.setPosition(810.0f - 220.0f,720.0f * cos(m_instructionPosition - PI_HALF * 3.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 4.0f)
				{
					m_instructionOBJ_02.setPosition(1030.0f - 220.0f,80.0f);
					m_instructionText_02.setPosition(810.0f - 220.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 4.0f && m_instructionPosition <= PI_HALF * 5.0f)
				{
					m_instructionOBJ_03.setPosition(1030.0f - 330.0f,720.0f * cos(m_instructionPosition - PI_HALF * 4.0f) + 80.0f);
					m_instructionText_03.setPosition(810.0f - 330.0f,720.0f * cos(m_instructionPosition - PI_HALF * 4.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 5.0f)
				{
					m_instructionOBJ_03.setPosition(1030.0f - 330.0f,80.0f);
					m_instructionText_03.setPosition(810.0f - 330.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 5.0f && m_instructionPosition <= PI_HALF * 6.0f)
				{
					m_instructionOBJ_04.setPosition(1030.0f - 440.0f,720.0f * cos(m_instructionPosition - PI_HALF * 5.0f) + 80.0f);
					m_instructionText_04.setPosition(810.0f - 440.0f,720.0f * cos(m_instructionPosition - PI_HALF * 5.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 6.0f)
				{
					m_instructionOBJ_04.setPosition(1030.0f - 440.0f,80.0f);
					m_instructionText_04.setPosition(810.0f - 440.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 6.0f && m_instructionPosition <= PI_HALF * 7.0f)
				{
					m_instructionOBJ_05.setPosition(1030.0f - 550.0f,720.0f * cos(m_instructionPosition - PI_HALF * 6.0f) + 80.0f);
					m_instructionText_05.setPosition(810.0f - 550.0f,720.0f * cos(m_instructionPosition - PI_HALF * 6.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 7.0f)
				{
					m_instructionOBJ_05.setPosition(1030.0f - 550.0f,80.0f);
					m_instructionText_05.setPosition(810.0f - 550.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 7.0f && m_instructionPosition <= PI_HALF * 8.0f)
				{
					m_instructionOBJ_06.setPosition(1030.0f - 660.0f,720.0f * cos(m_instructionPosition - PI_HALF * 7.0f) + 80.0f);
					m_instructionText_06.setPosition(810.0f - 660.0f,720.0f * cos(m_instructionPosition - PI_HALF * 7.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 8.0f)
				{
					m_instructionOBJ_06.setPosition(1030.0f - 660.0f,80.0f);
					m_instructionText_06.setPosition(810.0f - 660.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 8.0f && m_instructionPosition <= PI_HALF * 9.0f)
				{
					m_instructionOBJ_07.setPosition(1030.0f - 770.0f,720.0f * cos(m_instructionPosition - PI_HALF * 8.0f) + 80.0f);
					m_instructionText_07.setPosition(810.0f - 770.0f,720.0f * cos(m_instructionPosition - PI_HALF * 8.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 9.0f)
				{
					m_instructionOBJ_07.setPosition(1030.0f - 770.0f,80.0f);
					m_instructionText_07.setPosition(810.0f - 770.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 9.0f && m_instructionPosition <= PI_HALF * 10.0f)
				{
					m_instructionOBJ_13.setPosition(1030.0f - 880.0f,720.0f * cos(m_instructionPosition - PI_HALF * 9.0f) + 80.0f);
					m_instructionText_13.setPosition(810.0f - 880.0f,720.0f * cos(m_instructionPosition - PI_HALF * 9.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 10.0f)
				{
					m_instructionOBJ_13.setPosition(1030.0f - 880.0f,80.0f);
					m_instructionText_13.setPosition(810.0f - 880.0f,405.0f);
				}
				for(int i = 0;i < 10;++ i)
				{
					if(m_instructionTemp <= PI_HALF * i && m_instructionPosition > PI_HALF * i)
					{
						m_instructionTemp = m_instructionPosition;
						_XSound::GetInstance().play(m_soundTextMove_01);
					}
				}
			}else
			{
				m_instructionPosition += 0.004f * interval;
				if(m_instructionPosition >= PI_HALF * 6.0f)
				{
					m_instructionPosition = PI_HALF * 6.0f;
					m_gameCounter = 3;
				}
				if(m_instructionPosition >= PI_HALF && m_instructionPosition <= PI_HALF * 2.0)
				{
					m_instructionOBJ_08.setPosition(1030.0f - 0.0f,720.0f * cos(m_instructionPosition - PI_HALF) + 80.0f);
					m_instructionText_08.setPosition(810.0f - 0.0f,720.0f * cos(m_instructionPosition - PI_HALF) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 2.0f)
				{
					m_instructionOBJ_08.setPosition(1030.0f - 0.0f,80.0f);
					m_instructionText_08.setPosition(810.0f - 0.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 2.0f && m_instructionPosition <= PI_HALF * 3.0f)
				{
					m_instructionOBJ_09.setPosition(1030.0f - 110.0f,720.0f * cos(m_instructionPosition - PI_HALF * 2.0f) + 80.0f);
					m_instructionText_09.setPosition(810.0f - 110.0f,720.0f * cos(m_instructionPosition - PI_HALF * 2.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 3.0f)
				{
					m_instructionOBJ_09.setPosition(1030.0f - 110.0f,80.0f);
					m_instructionText_09.setPosition(810.0f - 110.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 3.0f && m_instructionPosition <= PI_HALF * 4.0f)
				{
					m_instructionOBJ_10.setPosition(1030.0f - 220.0f,720.0f * cos(m_instructionPosition - PI_HALF * 3.0f) + 80.0f);
					m_instructionText_10.setPosition(810.0f - 220.0f,720.0f * cos(m_instructionPosition - PI_HALF * 3.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 4.0f)
				{
					m_instructionOBJ_10.setPosition(1030.0f - 220.0f,80.0f);
					m_instructionText_10.setPosition(810.0f - 220.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 4.0f && m_instructionPosition <= PI_HALF * 5.0f)
				{
					m_instructionOBJ_11.setPosition(1030.0f - 330.0f,720.0f * cos(m_instructionPosition - PI_HALF * 4.0f) + 80.0f);
					m_instructionText_11.setPosition(810.0f - 330.0f,720.0f * cos(m_instructionPosition - PI_HALF * 4.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 5.0f)
				{
					m_instructionOBJ_11.setPosition(1030.0f - 330.0f,80.0f);
					m_instructionText_11.setPosition(810.0f - 330.0f,405.0f);
				}

				if(m_instructionPosition >= PI_HALF * 5.0f && m_instructionPosition <= PI_HALF * 6.0f)
				{
					m_instructionOBJ_12.setPosition(1030.0f - 440.0f,720.0f * cos(m_instructionPosition - PI_HALF * 5.0f) + 80.0f);
					m_instructionText_12.setPosition(810.0f - 440.0f,720.0f * cos(m_instructionPosition - PI_HALF * 5.0f) + 405.0f);
				}else
				if(m_instructionPosition > PI_HALF * 6.0f)
				{
					m_instructionOBJ_12.setPosition(1030.0f - 440.0f,80.0f);
					m_instructionText_12.setPosition(810.0f - 440.0f,405.0f);
				}
				for(int i = 0;i < 6;++ i)
				{
					if(m_instructionTemp <= PI_HALF * i && m_instructionPosition > PI_HALF * i)
					{
						m_instructionTemp = m_instructionPosition;
						_XSound::GetInstance().play(m_soundTextMove_01);
					}
				}
				userHeadMove(interval,0.0f);
			}
		}else
		if(m_gameCounter == 3)
		{
			if(m_instructionMode == 0)
			{
				m_instructionOBJ_00.setPosition(1030.0f - 0.0f,80.0f);
				m_instructionOBJ_01.setPosition(1030.0f - 110.0f,80.0f);
				m_instructionOBJ_02.setPosition(1030.0f - 220.0f,80.0f);
				m_instructionOBJ_03.setPosition(1030.0f - 330.0f,80.0f);
				m_instructionOBJ_04.setPosition(1030.0f - 440.0f,80.0f);
				m_instructionOBJ_05.setPosition(1030.0f - 550.0f,80.0f);
				m_instructionOBJ_06.setPosition(1030.0f - 660.0f,80.0f);
				m_instructionOBJ_07.setPosition(1030.0f - 770.0f,80.0f);
				m_instructionOBJ_13.setPosition(1030.0f - 880.0f,80.0f);
				m_instructionText_00.setPosition(810.0f - 0.0f,405.0f);
				m_instructionText_01.setPosition(810.0f - 110.0f,405.0f);
				m_instructionText_02.setPosition(810.0f - 220.0f,405.0f);
				m_instructionText_03.setPosition(810.0f - 330.0f,405.0f);
				m_instructionText_04.setPosition(810.0f - 440.0f,405.0f);
				m_instructionText_05.setPosition(810.0f - 550.0f,405.0f);
				m_instructionText_06.setPosition(810.0f - 660.0f,405.0f);
				m_instructionText_07.setPosition(810.0f - 770.0f,405.0f);
				m_instructionText_13.setPosition(810.0f - 880.0f,405.0f);
			}else
			{
				m_instructionOBJ_08.setPosition(1030.0f - 0.0f,80.0f);
				m_instructionOBJ_09.setPosition(1030.0f - 110.0f,80.0f);
				m_instructionOBJ_10.setPosition(1030.0f - 220.0f,80.0f);
				m_instructionOBJ_11.setPosition(1030.0f - 330.0f,80.0f);
				m_instructionOBJ_12.setPosition(1030.0f - 440.0f,80.0f);
				m_instructionText_08.setPosition(810.0f - 0.0f,405.0f);
				m_instructionText_09.setPosition(810.0f - 110.0f,405.0f);
				m_instructionText_10.setPosition(810.0f - 220.0f,405.0f);
				m_instructionText_11.setPosition(810.0f - 330.0f,405.0f);
				m_instructionText_12.setPosition(810.0f - 440.0f,405.0f);
				userHeadMove(interval,0.0f);
			}
			m_gameCounter = 4;
		}
		if(m_gameCounter == 4)
		{//停顿一段时间
			m_instructionPosition += interval;
			if(m_instructionPosition >= 10000 || (m_creditSum >= m_configData.coinToCredit || m_energyArmSum > 0.0f))
			{
				_XSound::GetInstance().play(m_soundScreenUp);
				m_gameCounter = 5;
				m_instructionPosition = 0.0f;
			}
			if(m_instructionMode != 0)
			{
				userHeadMove(interval,0.0f);
			}
		}else
		if(m_gameCounter == 5)
		{//所有物件离场
			m_instructionPosition += 0.002f * interval;
			if(m_instructionPosition >= PI_HALF)
			{
				m_instructionPosition = PI_HALF;
				m_gameCounter = 6;
			}
			int temp = 1280.0f * sin(m_instructionPosition);
			m_instructionBG.setPosition(temp,0.0f);
			if(m_instructionMode == 0)
			{
				m_instructionOBJ_00.setPosition(1030.0f - 0.0f + temp,80.0f);
				m_instructionOBJ_01.setPosition(1030.0f - 110.0f + temp,80.0f);
				m_instructionOBJ_02.setPosition(1030.0f - 220.0f + temp,80.0f);
				m_instructionOBJ_03.setPosition(1030.0f - 330.0f + temp,80.0f);
				m_instructionOBJ_04.setPosition(1030.0f - 440.0f + temp,80.0f);
				m_instructionOBJ_05.setPosition(1030.0f - 550.0f + temp,80.0f);
				m_instructionOBJ_06.setPosition(1030.0f - 660.0f + temp,80.0f);
				m_instructionOBJ_07.setPosition(1030.0f - 770.0f + temp,80.0f);
				m_instructionOBJ_13.setPosition(1030.0f - 880.0f + temp,80.0f);
				m_instructionText_00.setPosition(810.0f - 0.0f + temp,405.0f);
				m_instructionText_01.setPosition(810.0f - 110.0f + temp,405.0f);
				m_instructionText_02.setPosition(810.0f - 220.0f + temp,405.0f);
				m_instructionText_03.setPosition(810.0f - 330.0f + temp,405.0f);
				m_instructionText_04.setPosition(810.0f - 440.0f + temp,405.0f);
				m_instructionText_05.setPosition(810.0f - 550.0f + temp,405.0f);
				m_instructionText_06.setPosition(810.0f - 660.0f + temp,405.0f);
				m_instructionText_07.setPosition(810.0f - 770.0f + temp,405.0f);
				m_instructionText_13.setPosition(810.0f - 880.0f + temp,405.0f);
			}else
			{
				m_instructionOBJ_08.setPosition(1030.0f - 0.0f + temp,80.0f);
				m_instructionOBJ_09.setPosition(1030.0f - 110.0f + temp,80.0f);
				m_instructionOBJ_10.setPosition(1030.0f - 220.0f + temp,80.0f);
				m_instructionOBJ_11.setPosition(1030.0f - 330.0f + temp,80.0f);
				m_instructionOBJ_12.setPosition(1030.0f - 440.0f + temp,80.0f);
				m_instructionText_08.setPosition(810.0f - 0.0f + temp,405.0f);
				m_instructionText_09.setPosition(810.0f - 110.0f + temp,405.0f);
				m_instructionText_10.setPosition(810.0f - 220.0f + temp,405.0f);
				m_instructionText_11.setPosition(810.0f - 330.0f + temp,405.0f);
				m_instructionText_12.setPosition(810.0f - 440.0f + temp,405.0f);
				userHeadMove(interval,1280.0f * sin(m_instructionPosition));
			}
		}else
		if(m_gameCounter == 6)
		{//离场完成，进入demo持续阶段
			m_gameStage = GAME_STAGE_GAMELOGO;
			m_gameCounter = 3;	//进入等待投币阶段
		}
	}
}

int _XGun::goldBoxEffectInit(_XResourcePosition resoursePosition)
{
	if(m_goldBoxTicketLogo.init("ResourcePack/pic/UI/TicketLogo.png",resoursePosition) == 0) return 0;
	if(m_goldBoxEnergyLogo.init("ResourcePack/pic/UI/EnergyLogo.png",resoursePosition) == 0) return 0;
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		m_goldBoxNum[i].setACopy(m_goldAddNum[0]);
		m_goldBoxNum[i].setAngle(90.0f);
		m_goldBoxNum[i].setNumber(0);
		m_goldBoxSprite[i].init(m_goldBoxTicketLogo.getTextureData()->texture.m_w,m_goldBoxTicketLogo.getTextureData()->texture.m_h,0);
		m_goldBoxSprite[i].setAngle(90.0f);
		m_goldBoxAlpha[i] = 0;
		m_goldBoxSize[i] = 0;
		m_goledBoxEffectFlag[i] = 0;
		m_goldBoxTex[i] = 0;
	}
	return 1;
}

void _XGun::findAGoldBoxEffect(int mode,_XVector2 position,int sum)
{
	if(mode != 0)
	{//彩票模式
		for(int i = 0;i < MAX_BULLET_SUM;++ i)
		{
			if(m_goledBoxEffectFlag[i] == 0)
			{
				m_goldBoxNum[i].setColor(1.0f,0.0f,1.0f,-1.0f);
				m_goldBoxNum[i].setPosition(position.x + 16.0f,position.y);
				char str[20];
				if(m_configData.ticketSum == 0) sum = sum * 0.25f;
				else if(m_configData.ticketSum == 1) sum = sum * 0.5f;
				else sum = sum * 1.0f;
				sprintf(str,"+%d",sum);
				m_goldBoxNum[i].setNumber(str);
		//		if(sum < 10) m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f + 44.0f); else
		//		if(sum < 100) m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f + 22.0f); else
		//		if(sum < 1000) m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f + 0.0f); else
		//		if(sum < 10000) m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f - 22.0f);
				m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f + 44.0f);
				m_goldBoxTex[i] = 0;
				m_goledBoxEffectFlag[i] = 1;
				break;
			}
		}
	}else
	{//能量模式
		for(int i = 0;i < MAX_BULLET_SUM;++ i)
		{
			if(m_goledBoxEffectFlag[i] == 0)
			{
				m_goldBoxNum[i].setColor(0.0f,1.0f,1.0f,-1.0f);
				m_goldBoxNum[i].setPosition(position.x + 16.0f,position.y);
				char str[20];
				sprintf(str,"+%d",sum);
				m_goldBoxNum[i].setNumber(str);
			//	if(sum < 10) m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f + 44.0f); else
			//	if(sum < 100) m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f + 22.0f); else
			//	if(sum < 1000) m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f + 0.0f); else
			//	if(sum < 10000) m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f - 22.0f);
				m_goldBoxSprite[i].setPosition(position.x - 64.0f,position.y - 128.0f + 44.0f);
				m_goldBoxTex[i] = 1;
				m_goledBoxEffectFlag[i] = 1;
				break;
			}
		}
	}
}

void _XGun::goldBoxEffectmove(int interval)
{
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		if(m_goledBoxEffectFlag[i] == 0) continue;
		if(m_goledBoxEffectFlag[i] == 1)
		{//初始化
			m_goldBoxAlpha[i] = 0.0f;
			m_goldBoxSize[i] = -PI_HALF;
			m_goldBoxNum[i].setSize(0.0f,0.0f);
			m_goldBoxNum[i].setColor(-1.0f,-1.0f,-1.0f,1.0f);
			m_goldBoxSprite[i].setSize(0.0f,0.0f);
			m_goldBoxSprite[i].setAlpha(1.0f);
			m_goledBoxEffectFlag[i] = 2;
		}else
		if(m_goledBoxEffectFlag[i] == 2)
		{//出现
			m_goldBoxSize[i] += 0.025f * interval;
			if(m_goldBoxSize[i] >= PI)
			{
				m_goldBoxSize[i] = PI;
				m_goledBoxEffectFlag[i] = 3;
				m_goldBoxNum[i].setSize(1.0f,1.0f);
				m_goldBoxSprite[i].setSize(1.0f,1.0f);
			}else
			{
				float temp = 1.0f + sin(m_goldBoxSize[i]);
				m_goldBoxNum[i].setSize(temp,temp);
				m_goldBoxSprite[i].setSize(temp,temp);
			}
		}else
		if(m_goledBoxEffectFlag[i] == 3)
		{//持续
			m_goldBoxSize[i] += interval;
			if(m_goldBoxSize[i] >= 3000)
			{
				m_goldBoxAlpha[i] = 1.0f;
				m_goldBoxSize[i] = 0.0f;
				m_goledBoxEffectFlag[i] = 4;
			}
		}else
		{//消失
			m_goldBoxAlpha[i] -= 0.05f * interval;
			if(m_goldBoxAlpha[i] <= 0.0f)
			{
				m_goldBoxAlpha[i] = 0.0f;
				m_goledBoxEffectFlag[i] = 0;
			}
			m_goldBoxNum[i].setColor(-1.0f,-1.0f,-1.0f,m_goldBoxAlpha[i]);
			m_goldBoxSprite[i].setAlpha(m_goldBoxAlpha[i]);
		}
	}
}

void _XGun::goldBoxEffectDraw()
{
	for(int i = 0;i < MAX_BULLET_SUM;++ i)
	{
		if(m_goledBoxEffectFlag[i] > 1)
		{
			if(m_goldBoxTex[i] == 0) m_goldBoxSprite[i].draw(m_goldBoxTicketLogo.getTextureData());
			else m_goldBoxSprite[i].draw(m_goldBoxEnergyLogo.getTextureData());
			m_goldBoxNum[i].draw();
		}
	}
}

void _XGun::updataHeadTex(int order)
{
	if(order < 0 || order >= MAX_GAME_ORDER_SUM) return;
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_cameraRecordTex[order]);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
	//	CAMERA_WIDTH, CAMERA_HIGHT, GL_RGB,//GL_LUMINANCE, 
	//	GL_UNSIGNED_BYTE, m_gameOrderCameraData + order * MAX_GAME_ORDER_CAMERA_DATA);
	//glDisable(GL_TEXTURE_2D);
	for(int i = 8;i < 248;++ i)
	{
		memcpy(m_cameraTexData + i * CAMERA_TEX_WIDTH * 3 + 96 * 3,m_gameOrderCameraData + order * MAX_GAME_ORDER_CAMERA_DATA + (i - 8) * CAMERA_WIDTH * 3,CAMERA_WIDTH * 3);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_cameraRecordTex[order]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
		CAMERA_TEX_WIDTH, CAMERA_TEX_HIGHT, GL_RGB,//GL_LUMINANCE, 
		GL_UNSIGNED_BYTE, m_cameraTexData);
	glDisable(GL_TEXTURE_2D);
}

void _XGun::updataUserHeadTex(int order)
{
	if(order < 0 || order >= USER_HEAD_SUM) return;
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_userHeadTex[order]);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
	//	CAMERA_WIDTH, CAMERA_HIGHT, GL_RGB,//GL_LUMINANCE, 
	//	GL_UNSIGNED_BYTE, m_gameOrderUserHeadData + order * MAX_GAME_ORDER_CAMERA_DATA);
	//glDisable(GL_TEXTURE_2D);
	for(int i = 8;i < 248;++ i)
	{
		memcpy(m_cameraTexData + i * CAMERA_TEX_WIDTH * 3 + 96 * 3,m_gameOrderUserHeadData + order * MAX_GAME_ORDER_CAMERA_DATA + (i - 8) * CAMERA_WIDTH * 3,CAMERA_WIDTH * 3);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_userHeadTex[order]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
		CAMERA_TEX_WIDTH, CAMERA_TEX_HIGHT, GL_RGB,//GL_LUMINANCE, 
		GL_UNSIGNED_BYTE, m_cameraTexData);
	glDisable(GL_TEXTURE_2D);;
}

void _XGun::userHeadMove(int delay,int h)
{
	if(m_isCameraSupply == 0) return;
	m_nowUserHeadData += 0.0004f * delay;
	if(m_nowUserHeadData >= 1.0f)
	{
		m_nowUserHeadData = 0.0f;
		m_nowUserHeadOrder ++;
		if(m_nowUserHeadOrder >= USER_HEAD_SUM) m_nowUserHeadOrder = 0;
		m_userheadName[0].setString(m_gameOrderUserHeadName[m_nowUserHeadOrder]);
		m_userheadName[1].setString(m_gameOrderUserHeadName[(m_nowUserHeadOrder + 1) % USER_HEAD_SUM]);
		m_userheadName[2].setString(m_gameOrderUserHeadName[(m_nowUserHeadOrder + 2) % USER_HEAD_SUM]);
		m_userheadName[3].setString(m_gameOrderUserHeadName[(m_nowUserHeadOrder + 3) % USER_HEAD_SUM]);
	}
	float positionX = - 20.0f - 140 * m_nowUserHeadData;
	float size = 0.5f - m_nowUserHeadData * 0.25f;
//	m_userHeadSprite[0].setPosition(100.0f,20.0f - 340.0f * m_nowUserHeadData);
	m_userHeadSprite[0].setPosition(100.0f + h,positionX);
	m_userHeadSprite[0].setSize(size,size);
	m_userHeadSprite[0].setAlpha(0.75f - m_nowUserHeadData * 0.25f);
	m_userheadName[0].setPosition(100.0f + 160.0f - 120.0f * size - 20.0f + h,positionX + 120.0f - 48.0f);
	m_userheadName[0].setColor(-1.0f,-1.0f,-1.0f,0.75f - m_nowUserHeadData * 0.25f);

	positionX = positionX + 160.0f + 160.0f * (0.5f - m_nowUserHeadData * 0.25f) + 20.0f - (160.0f - 160.0f * (1.0f - m_nowUserHeadData * 0.5f));
	size = 1.0f - m_nowUserHeadData * 0.5f;
//	m_userHeadSprite[1].setPosition(100.0f,360.0f - 340.0f * m_nowUserHeadData);
	m_userHeadSprite[1].setPosition(100.0f + h,positionX);
	m_userHeadSprite[1].setSize(size,size);
	m_userHeadSprite[1].setAlpha(1.0f - m_nowUserHeadData * 0.25f);
	m_userheadName[1].setPosition(100.0f + 160.0f - 120.0f * size - 20.0f + h,positionX + 120.0f - 48.0f);
	m_userheadName[1].setColor(-1.0f,-1.0f,-1.0f,1.0f - m_nowUserHeadData * 0.25f);

	positionX = positionX + 160.0f + 160.0f * (1.0f - m_nowUserHeadData * 0.5f) + 20.0f - (160.0f - 160.0f * (0.5f + m_nowUserHeadData * 0.5f));
	size = 0.5f + m_nowUserHeadData * 0.5f;
//	m_userHeadSprite[2].setPosition(100.0f,700.0f - 340.0f * m_nowUserHeadData);
	m_userHeadSprite[2].setPosition(100.0f + h,positionX);
	m_userHeadSprite[2].setSize(size,size);
	m_userHeadSprite[2].setAlpha(0.75f + m_nowUserHeadData * 0.25f);
	m_userheadName[2].setPosition(100.0f + 160.0f - 120.0f * size - 20.0f + h,positionX + 120.0f - 48.0f);
	m_userheadName[2].setColor(-1.0f,-1.0f,-1.0f,0.75f + m_nowUserHeadData * 0.25f);

	positionX = positionX + 160.0f + 160.0f * (0.5f + m_nowUserHeadData * 0.5f) + 20.0f - (160.0f - 160.0f * (0.25f + m_nowUserHeadData * 0.25f));
	size = 0.25f + m_nowUserHeadData * 0.25f;
//	m_userHeadSprite[3].setPosition(100.0f,1040.0f - 340.0f * m_nowUserHeadData);
	m_userHeadSprite[3].setPosition(100.0f + h,positionX);
	m_userHeadSprite[3].setSize(size,size);
	m_userHeadSprite[3].setAlpha(0.5f + m_nowUserHeadData * 0.25f);
	m_userheadName[3].setPosition(100.0f + 160.0f - 120.0f * size - 20.0f + h,positionX + 120.0f - 48.0f);
	m_userheadName[3].setColor(-1.0f,-1.0f,-1.0f,0.5f + m_nowUserHeadData * 0.25f);
}

void _XGun::userHeadDraw()
{
	if(m_isCameraSupply == 0) return;
	m_userHeadSprite[0].draw(&m_userHeadTex[m_nowUserHeadOrder]);
	m_userHeadSprite[1].draw(&m_userHeadTex[(m_nowUserHeadOrder + 1) % USER_HEAD_SUM]);
	m_userHeadSprite[2].draw(&m_userHeadTex[(m_nowUserHeadOrder + 2) % USER_HEAD_SUM]);
	m_userHeadSprite[3].draw(&m_userHeadTex[(m_nowUserHeadOrder + 3) % USER_HEAD_SUM]);
	m_userheadName[0].draw();
	m_userheadName[1].draw();
	m_userheadName[2].draw();
	m_userheadName[3].draw();
}