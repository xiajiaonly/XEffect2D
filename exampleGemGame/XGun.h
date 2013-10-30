#ifndef _JIA_XGUN_
#define _JIA_XGUN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:       2011.12.10
//--------------------------------

//点到直线的距离公式
//直线：a * x + b * y + c = 0;
//点：x0,y0
//距离公式：abs(a * x0 + b * y0 + c)/sqrt(a * a + b * b)
//y=kx-kx1+y1 -> kx - y - kx1 + y1 = 0; k = tan(B)
// tan(B) * x - y - tan(B) * x1 + y1 = 0;
//abs(tan(B) * x0 + (-1) * y0 + (y1 - tan(B) * x1)) /sqrt(tan(b) * tan(b) + 1)

#include "XEffeEngine.h"
#include "XGem.h"
#include "XCamera.h"

#define PC_FREE_VERSION (0) //给个人玩的免费版本

extern XMusicHandle BGMusicHandle[6];	//声音的句柄
extern _XMusic BGmusic;					//播放背景音乐的类

//#define GOLD_RATE (2)			//摧毁宝石之后宝石能量与获得金币的兑换关系
#define LINE_BOOM_ENERGY (8)	//线性道具
#define AREA_BOOM_ENERGY (10)	//范围爆炸道具的能量
#define AREA_BULLET_ENERGY (6)	//范围爆炸武器的能量
#define ENERGY_ADD_TOOLS (100)	//反射道具子弹的能量
#define COLOR_BOOM_ENERGY (8)	//彩色子弹的能量
#define TOOLS_DECREASE_ENERGY (200) //减少能量的道具减少的能量
#define TOOLS_ADD_ENERGY (400)	//增加能量的道具增加的能量
#define TOOLS_ADD_GOLD (800)	//增加金币的道具增加的金币
#define TOOLS_FREE_TIMER (10000)	//免费射击持续的时间
#define TOOLS_ADD_TIMER (15000) //增加时间的道具增加的时间
#define TOOLS_ENERGY_CHANGE (0.125f) //能量转换道具的转换比例
#define GUN_ANGLE_MAX (150.0f)
#define GUN_ANGLE_MIN (30.0f)

//子弹的连射速度
#define GUN_SHOOT_SPEED_00 (150)
#define GUN_SHOOT_SPEED_01 (240)
#define GUN_SHOOT_SPEED_02 (450)
#define GUN_SHOOT_SPEED_03 (450)
#define GUN_SHOOT_SPEED_04 (300)
#define GUN_SHOOT_SPEED_05 (700)
//子弹的能量
#define GUN_SHOOT_ENERGY_00 (1)
#define GUN_SHOOT_ENERGY_01 (4)
#define GUN_SHOOT_ENERGY_02 (20)
#define GUN_SHOOT_ENERGY_03 (30)
#define GUN_SHOOT_ENERGY_04 (8)
#define GUN_SHOOT_ENERGY_05 (200)	
#define GUN_SHOOT_ENERGY_X0 (1)	//彩蛋能量
#define MAX_BULLET_SUM (256)
#define LINE_BULLET_DESTROY (20)

#define LEVEL_OVER_ADD_SCORE (2000) //过关增加的分数

#define ENERGY_WARNNING (100)

#define SAVE_FILE_NAME ("databasic.dat")		//设置数据的读取和存放都在D盘
#define SAVE_FILE_NAME_HEAD ("dataHead.dat")	//设置数据的读取和存放都在D盘
#define SAVE_FILE_NAME_USER ("dataUser.dat")	//设置数据的读取和存放都在D盘

#define USER_HEAD_SUM (20)

#define USE_FBO (1)		//是否使用FBO来产生爆炸震动效果以及拖尾效果
#define USE_SHADER (1)	//是否使用shader来处理掉拖尾造成的残影

enum _XBULLET_TYPE
{
	BULLET_TYPE_00,		//机枪扫射子弹
	BULLET_TYPE_01,		//大口径火炮
	BULLET_TYPE_02,		//榴弹炮
	BULLET_TYPE_03,		//高能榴弹炮
	BULLET_TYPE_04,		//火箭炮
	BULLET_TYPE_05,		//激光炮
	BULLET_TYPE_06,		//x
	BULLET_TYPE_07,		//x
	BULLET_TYPE_08,		//x
	BULLET_TYPE_X0,		//色彩蛋
	BULLET_TYPE_X1,		//道具增强型子弹
};

//尺寸 32 x 64
class _XBulletTexture
{
private:
	char m_isInited;
public:
	_XTextureData m_bulletTex_00;
	_XTextureData m_bulletTex_01;
	_XTextureData m_bulletTex_02;
	_XTextureData m_bulletTex_03;
	_XTextureData m_bulletTex_04;
	_XTextureData m_bulletTex_05;
	_XTextureData m_bulletTex_X0;	//色彩蛋的子弹
	_XTextureData m_bulletTex_X1;	//道具增强型子弹
	_XFrameEx m_bulletFrame;	//子弹的动画
	_XFrameEx m_bulletBoom;	//子弹的动画
	int init(int resoursePosition = 0);
	_XBulletTexture();
};

#pragma pack(push)
#pragma pack(1)
struct _XConfigData
{
	char isTicketNeed;	//是否需要出彩票
	char ticketSum;		//少、一般、多
	char isAutoShoot;	//是否自动射击
	char coinToCredit;	//多少币玩一次
	int oneCreditEnergy;	//一个充能的量
	int volume;
	int allInCoin;	//总投币量
	int allOutTicket;	//总出彩票量
	char levelSum;	//可以玩的关卡数,6,7,8,9,不限制
};
#pragma pack(pop)

class _XBullet
{
private:
public:
	_XVector2 m_position;	//子弹所在的位置
	_XVector2 m_speed;		//子弹的速度
	_XMoveData m_nowSpeedData;	//用于跟踪弹的加速过程

	float m_energy;			//子弹的能量
	float m_angle;			//子弹的角度

	_XBULLET_TYPE m_bulletType;	//子弹的类型

	int m_bulletStage;		//子弹的阶段 0:静态终结阶段
	float m_bulletAlpha;	//子弹的alpha
	int m_bulletAlphaStage;	//子弹alpha的阶段

	_XBulletTexture *m_tex;

	_XSprite m_bulletSprite_00;
	_XSprite m_bulletSprite_01;
	_XFrameEx m_bulletFrame;

	float m_armAngle;		//目标角度
	int m_armAngleFlag;	//目标角度是否有效
public:
	int init(_XVector2 position,_XVector2 speed,float energy,float angle,_XBulletTexture *tex,_XBULLET_TYPE type);
	void draw();
	int move(int interval);	//返回是否发生边界碰撞，1上边界碰撞，2下边界碰撞
	int getIsEnd();			//子弹是否已经无效
	int getCanCrash();		//子弹是否具有伤害能力
	void setOver()
	{
		m_energy = 0.0f;
		if(m_bulletType == BULLET_TYPE_00 || m_bulletType == BULLET_TYPE_01 || m_bulletType == BULLET_TYPE_03 || m_bulletType == BULLET_TYPE_04)
		{
			m_bulletStage = 2;
		}else
		{
			m_bulletStage = 0;
		}
	}

	_XBullet();
};

enum _XGUN_TYPE
{
	GUN_TYPE_NULL,	
	GUN_TYPE_00,	//机关枪
	GUN_TYPE_01,	//大口径火炮
	GUN_TYPE_02,	//榴弹炮
	GUN_TYPE_03,	//高能榴弹炮
	GUN_TYPE_04,	//火箭炮
	GUN_TYPE_05,	//激光炮
};

enum TOOLS_TYPE
{
	TOOLS_TYPE_00,	//加能量的道具
	TOOLS_TYPE_01,	//加金币的道具
	TOOLS_TYPE_02,	//金币转能量的道具
	TOOLS_TYPE_03,	//10s无限射击的道具
	TOOLS_TYPE_04,	//加时间的道具

	TOOLS_TYPE_05,	//金币翻倍
	TOOLS_TYPE_06,	//时间减慢
	TOOLS_TYPE_07,	//威力加倍
	TOOLS_TYPE_08,	//射速加快
	TOOLS_TYPE_09,	//消耗减半
};

enum GAME_STAGE
{
	GAME_STAGE_GAMELOGO,	//游戏DEMO界面
	GAME_STAGE_GAMEHEAD,	//每关开始的记录
	GAME_STAGE_GAMEPLAY,	//游戏进行
	GAME_STAGE_GAMEWIN,		//游戏胜利完成
	GAME_STAGE_GAMELOST,	//游戏失败结束
	GAME_STAGE_GAMEINPUTNAME,	//玩家输入名字
	GAME_STAGE_GAMESHOWORDER,	//显示排行榜
	GAME_STAGE_GAMEALLOVER,	//游戏通关
	GAME_STAGE_INSTRUCTION,	//游戏介绍界面
};
#define SMALL_STAR_SUM (256)

#define CAMERA_WIDTH (320)	//摄像头的宽度
#define CAMERA_HIGHT (240)	//摄像头的高度
#define CAMERA_TEX_WIDTH (512)	//摄像头的宽度
#define CAMERA_TEX_HIGHT (256)	//摄像头的高度
#define MAX_GAME_ORDER_SUM (10)
#define MAX_GAME_ORDER_NAME_LENGTH (7)	//不要大于15
#define MAX_GAME_ORDER_CAMERA_DATA (CAMERA_WIDTH * CAMERA_HIGHT * 3)	//头像的数据大小(10个单位)
enum INPUT_NAME_KEY
{
	INPUT_KEY_LEFT,
	INPUT_KEY_RIGHT,
	INPUT_KEY_ENTER,
};

enum CONFIG_KEY
{
	CONFIG_KEY_LEFT,
	CONFIG_KEY_RIGHT,
	CONFIG_KEY_UP,
	CONFIG_KEY_DOWN,
	CONFIG_KEY_SHOOT,
};

class _XGun
{
private:
	_XSprite m_cameraLogo;	//摄像头的图标
	_XSprite m_cameraNotSupply;	//摄像头不支持的图标
	float m_notSupplyAlpha;

	_XCamera m_camera;
	_XSprite m_cameraBG;
	//_XSprite m_cameraHead[MAX_GAME_ORDER_SUM];	//10个头像
	_XSprite m_cameraHead;	//10个头像
	GLuint m_cameraRecordTex[MAX_GAME_ORDER_SUM];		//10个头像的贴图
	GLuint m_userHeadTex[USER_HEAD_SUM];		//10个头像的贴图
	_XSprite m_takePhotoAttention;				//拍照提示
	char m_takePhotoNeedShow;					//是否需要显示拍照提示
	char m_takePhotoCanPressKey;				//是否接受按键信息	0:不接受按键 1接受按键 2已经按键并继续接受按键
	char m_takePhotoKeyFlag;					//拍照提示时的按键控制
	char m_takePhotoPoint;						//当前拍照的插入点
	char m_isCameraSupply;						//是否有摄像头支持

	_XSprite m_userHeadSprite[4];				//动态显示玩家贴图的sprite
	_XFont  m_userheadName[4];
	char m_nowUserHeadOrder;					//当前显示的玩家的头像的编号
	float m_nowUserHeadData;					//当前显示的玩家头像的参数

	void userHeadMove(int delay,int h);
	void userHeadDraw();
public:
	void pressShoot()
	{
		if(m_isCameraSupply == 0) return;
		if(m_takePhotoCanPressKey != 0) 
		{
			m_takePhotoCanPressKey = 2;
			m_takePhotoKeyFlag = 1;
		}
	}
	void pressChangeGun()
	{
		if(m_isCameraSupply == 0) return;
		if(m_takePhotoCanPressKey != 0) 
		{
			m_takePhotoCanPressKey = 2;
			m_takePhotoKeyFlag = 2;
		}
	}
private:
	void updataHeadTex(int order);
	void updataUserHeadTex(int order);
	//下面试拍照时的动画效果
	_XSprite m_takePhotoTex00;
	_XSprite m_takePhotoTex01;
	_XSprite m_takePhotoTex02;
	_XSprite m_takePhotoTex03;
	float m_takePhotoChange;

	//彩票的进度条
	_XSprite m_ticketBar;
	_XSprite m_ticketBarDown;
	_XFrameEx m_toolsBoom;	//反道具爆炸效果的特效
	char m_toolsBoomFlag;

	int m_resoursePosition;
	int m_BGPicOrder;	//应该显示的背景编号
	_XSprite m_backGround_00;
	_XSprite m_backGround_01;
	_XSprite m_backGround_02;
	_XSprite m_backGround_03;
	_XSprite m_backGround_04;
//	char m_backGroundColor;
//	int m_backGroundTimer;
	float m_backGroungPosition;
	_XSprite m_star[13];
	char m_starFlag[13];
	_XVector2 m_starSpeed[13];
	_XVector2 m_starPosition[13];

	_XSprite m_BGSider[2];
	_XMoveData m_BGSiderMoveData[2];

//+++++++++++++++++++++++++++++++++++++++
//下面是关于小行星的一些定义
	_XSprite m_smallStar[6];
	//float m_smallStarM;	//小行星的密度，用于描述小行星的出现次数
	char m_smallStarType[SMALL_STAR_SUM];
	char m_smallSrarStage[SMALL_STAR_SUM];	//当前小行星的阶段
	_XVector2 m_smallStarPosition[SMALL_STAR_SUM];	//当前小行星的位置
	_XVector2 m_smallStarSize[SMALL_STAR_SUM];		//当前小行星的大小
	_XVector2 m_smallStarSpeed[SMALL_STAR_SUM];		//当前小行星的速度
	_XMoveData m_smallStarMoveData;

	int smallStarInit(int resoursePosition);
	void smallStarMove(int delay);
	void smallStarDraw();
//---------------------------------------
//+++++++++++++++++++++++++++++++++++++++
//下面是关于人物头像的定义
	_XSprite m_girlFace[6];		//人物头像
	_XSprite m_faceBG;			//人物说话的背景
	_XFontUnicode m_faceText;	//人物对话的文字
	int m_faceStage;			//显示人物对话的阶段性标志
	_XMoveData m_faceMoveData;
	int m_nowFaceOrder;
	int m_faceTextSum;
	int m_faceTextTimer;		//显示文字的计时器
	int m_faceGameTimer;		//游戏计时
	char m_faceTextRandom;

	int faceInit(int resoursePosition);
	void faceMove(int delay);
	void faceDraw();
//public:
	void setFaceStart()
	{
		if(m_faceStage == 0) m_faceStage = 1;
	}
//private:
//---------------------------------------
//+++++++++++++++++++++++++++++++++++++++
//下面是关于残影效果的一些定义
#if USE_FBO
	_XFBO m_fbo;
	_XSprite m_fboSprite;
	_XSprite m_fboShake;
	int m_fboOrder;
	_XMoveData m_fboShakeData;
#if USE_SHADER
	void initShaderFromText();	//从文件中初始化sheder的代码
#endif
public:
	void useFBO()
	{
		if(m_fboShakeData.getIsEnd() == 0)
		{
			m_fbo.useFBO(1280,720);
			m_fbo.attachTex(2);
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
	void removeFBO()
	{
		if(m_fboShakeData.getIsEnd() == 0)
		{
			m_fbo.removeFBO();
		}
	}
	void drawFBO()
	{
		if(m_fboShakeData.getIsEnd() == 0)
		{
			GLuint tempTex = m_fbo.getTexture(2);
			m_fboShake.draw(&tempTex);
		}
	}
	void moveFBO(int delay)
	{
		if(m_fboShakeData.getIsEnd() == 0)
		{
			m_fboShakeData.move(delay);
			float randomA = random(360) * ANGLE_TO_RADIAN;
			m_fboShake.setPosition(m_fboShakeData.getNowData() * sin(randomA),m_fboShakeData.getNowData() * cos(randomA));
		}
	}
private:
#endif
//---------------------------------------
//+++++++++++++++++++++++++++++++++++++++
//下面是关于玩家状态的变量
	_XSprite m_userBuffLogo[6];	//玩家状态的标志，金币增强，时间减慢，威力加强，射速加快，消耗减半，消耗取消
	char m_userBuffFlag[6];	//玩家状态的标志	//状态一次对应于新加入的5种状态道具	
	int m_userBuffReleaseTime[6];	//状态的剩余时间
	_XMoveData m_userBuffData[6];

	int m_userBuffTimer;	//威力加强时的特效计时

	int userBuffInit(int resoursePosition);
	void userBuffMove(int delay);
	void userBuffDraw();
	void clearAllBuff()
	{
		for(int i = 0;i < 6;++ i)
		{
			if(m_userBuffFlag[i] > 0 && m_userBuffFlag[i] < 6) m_userBuffFlag[i] = 6;
			else m_userBuffFlag[i] = 0;
			m_userBuffReleaseTime[i] = 0;
		}
	}
public:
	float buffTimerRate()
	{
		if(m_userBuffFlag[1] != 0) return 0.5f;
		else return 1.0f;
	}
	float buffShootTimer()
	{
		if(m_userBuffFlag[3] != 0) return 0.33f;
		else return 1.0f;
	}
	float buffEnergeRate()
	{
		if(m_userBuffFlag[4] != 0) return 0.5f;
		else return 1.0f;
	}
	float buffBulletPowerRate()
	{
		if(m_userBuffFlag[2] != 0) return 2.0f;
		else return 1.0f;
	}
	float buffGoldRate()
	{
		if(m_userBuffFlag[0] != 0) return 2.0f;
		else return 1.0f;
	}
private:
//---------------------------------------
//+++++++++++++++++++++++++++++++++++++++
//下面是关于道具效果的定义
	_XSprite m_toolsEffect[6];	//分数，金币，能量，时间，能量，能量
	int  m_toolsEffectStage[6];
	_XMoveData m_toolsEffectData[6];

	int toolsEffectInit(int resoursePosition);
	void toolsEffectMove(int delay);
	void toolsEffectDraw();
//---------------------------------------

	_XTextureData m_gunTex_00;	//普通火炮
	_XTextureData m_gunTex_01;	//大口径火炮
	_XTextureData m_gunTex_02;	//榴弹炮
	_XTextureData m_gunTex_03;	//高能榴弹炮
	_XTextureData m_gunTex_04;	//火箭炮
	_XTextureData m_gunTex_05;	//激光炮

	_XTextureData m_gunFireTex_00;
	_XTextureData m_gunFireTex_01;
	_XTextureData m_gunFireTex_02;
	_XTextureData m_gunFireTex_03;
	_XTextureData m_gunFireTex_04;
	_XTextureData m_gunFireTex_05;

	char m_gunIsFlash;
	int m_gunFlashTime;
	_XSprite m_gunSprite;
	_XSprite m_gunFireSprite_00;
	float m_gunAngle;
	_XGUN_TYPE m_canUserGunType;	//通过这个变量限制玩家使用的枪
	_XGUN_TYPE m_gunType;

	void checkGunType();	//检查玩家的枪是否有足够的能量支持

	char m_isInited;

	_XBullet *m_bullet;	//子弹的指针
	_XBulletTexture m_bulletTex;
	float m_gunFireAlpha;
	int m_gunFireAlphaStage;
	_XGUN_TYPE m_gunFireType;
	void gunFireGlare(int interval);	//开火时枪口闪烁的效果

	//下面是为了显示分数而定义的一些成员
	_XSprite m_creditText;
	//_XSprite m_creditSprite;
	_XSprite m_goldSprite;
	_XSprite m_energySprite;
	//_XSprite m_scoresSprite;
	_XSprite m_scoresText;

	int m_creditSum;	//不需要滚动效果

	int m_scoresSum;
	int m_scoresArmSum;
	int m_energySum;	//需要滚动效果
	int m_energyArmSum;	//用于实现滚动效果
	int m_goldSum;
	int m_goldArmSum;	//用于实现滚动效果
	_XNumber m_creditNum;
	_XNumber m_scoresNum;
	_XNumber m_energyNum;
	_XNumber m_goldNum;
	void numberChange(int interval);	//数字滚动的动作

	_XNumber m_energyAddNum;
	float m_energyAddAlpha;
	int m_energyAddStage;
	void energyAddDraw();
	void energyAddMove(int interval);
	void energyAdd(int temp,int mode = 0);	//改变能量的数值，会有显示
	_XSprite m_lowEnergy;
	_XSprite m_energyBar;
	_XSprite m_energyBarDown;
	char m_isEnergyGlare;
	float m_energyAlpha;
	char m_energyAlphaStage;
	void numberOfEnergyGlare(int interval);	//能量数字较小时闪烁的动作
	int UIBoardInit(int resoursePosition);

	//用于显示每次增加的分数的数值
	_XNumber m_goldAddNum[MAX_BULLET_SUM];
	float m_goldAddAlpha[MAX_BULLET_SUM];
	float m_goldAddX[MAX_BULLET_SUM];
	char m_goldAddFlag[MAX_BULLET_SUM];
	int m_goldAddBasicX[MAX_BULLET_SUM];
	int m_goldAddBasicY[MAX_BULLET_SUM];
	void goldAddMove(int interval);
	//下面是针对于宝箱而增加的资源
	_XNumber m_goldBoxNum[MAX_BULLET_SUM];
	_XSprite m_goldBoxEnergyLogo;
	_XSprite m_goldBoxTicketLogo;
	_XSprite m_goldBoxSprite[MAX_BULLET_SUM];
	float m_goldBoxAlpha[MAX_BULLET_SUM];
	float m_goldBoxSize[MAX_BULLET_SUM];
	char m_goledBoxEffectFlag[MAX_BULLET_SUM];
	char m_goldBoxTex[MAX_BULLET_SUM];
	int goldBoxEffectInit(int resoursePosition);
	void goldBoxEffectmove(int interval);
	void goldBoxEffectDraw();
	void findAGoldBoxEffect(int mode,_XVector2 position,int sum);
	
	//用于显示飞动的金币
	_XFrameEx m_goldFly[MAX_BULLET_SUM * 10];
	float m_goldFlyAlpha[MAX_BULLET_SUM * 10];
	float m_goldFlyParam[MAX_BULLET_SUM * 10];
	_XVector2 m_goldFlyPosition[MAX_BULLET_SUM * 10];	//开始的位置
	char m_goldFlyFlag[MAX_BULLET_SUM * 10];

	void findGoldFly(int level,_XVector2 position);
	void goldFlyMove(int interval);
	int goldAddAndFlyInit(int resoursePosition);

	//枪的状态
	char m_isGunColorBoom;	//枪是否拥有色彩炸弹的效果
	_XFrameEx m_colorBoomFrame;	//彩色炸弹时的枪的特效

	//游戏状态的一些变量
	//_XProgress m_nowLevelProcess;
	_XSprite m_nowLevelProcessB;
	_XSprite m_nowLevelProcess;
	_XSprite m_nowLevelProcessU;
	//_XNumber m_nowLevelProcessN;
	_XSprite m_nowLevelProcessLight;
	_XSprite m_nowLevelProcessLage;
	_XFrameEx m_levelProcessBoom;
	int m_levelFlagStage;
	float m_levelFlagSize;
	float m_PrograssAlpha;
	float m_PrograssAlphaTemp;
	float m_levelFiniedSum;
	float m_levelFiniedArmSum;
	int processInit(int resoursePosition);
	void processMove(int interval);

	//下面是关于随机道具的一些变量
	_XTextureData m_tools_00;	//随即道具的贴图
	_XTextureData m_tools_01;	//随即道具的贴图
	_XTextureData m_tools_02;	//随即道具的贴图
	_XTextureData m_tools_03;	//随即道具的贴图
	_XTextureData m_tools_04;	//随即道具的贴图
	_XTextureData m_tools_05;	//随即道具的贴图
	_XTextureData m_tools_06;	//随即道具的贴图
	_XTextureData m_tools_07;	//随即道具的贴图
	_XTextureData m_tools_08;	//随即道具的贴图
	_XTextureData m_tools_09;	//随即道具的贴图
	_XSprite m_toolsSprite;	//随机道具的精灵
	float m_toolsSize;		//随机道具的尺寸
	char m_toolsFlag;		//随机道具的标志
	float m_toolsAlpha;		//随机道具的透明度
	//int m_toolsTimer;		//计时道具的计时变量
	TOOLS_TYPE m_toolsType;	//随机道具的类型
//	_XNumber m_toolsTimerText;
//	_XSprite m_toolsTextSprite;	//随机道具的精灵
//	float m_toolsTextAlpha;

	int toolsInit(int resoursePosition);
	void toolsMove(int interval);
	int canGetTools(_XGemMatrix *gems,_XVector2 position);	//判断是否可以获得道具
	void toolsDoProc(_XVector2 position,_XGEM_TYPE type);	//道具产生效果的函数
	void toolsDraw();

	//下面是关于游戏流程的一些变量
	GAME_STAGE m_gameStage;	//游戏阶段的标志
	GAME_STAGE m_upGameStage;	//上一个界面的名称
	int m_gameCounter;		//游戏阶段的辅助性参数
	int m_nowLevel;			//当前是第几关 0开始
	float m_lineStartX;
	int m_lineStartTime;
	//游戏头阶段的一些内容
	_XNumber m_gameHeadNum;
	_XSprite m_gameHeadText;
	_XSprite m_gameHeadBG;
	_XSprite m_gameHeadBGLevelInfo;
	float m_gameHeadTextY;
	float m_gameHeadSize;
	float m_gameHeadAlpha;
	int gameHeadInit(int resoursePosition);
	void gameHeadDraw();
	void gameHeadMove(int interval);
	//下面是游戏过程中的文字提示
	_XSprite m_gamePlayText00;
	_XSprite m_gamePlayText01;
	_XSprite m_gamePlayText02;
	int m_gamePlayTicketSum;		//过关增加彩票的数值
	_XNumber m_gamePlayNum;			//数字
	_XSprite m_gamePlayLogo00;		//图标00
	_XSprite m_gamePlayLogo01;		//图标01

	float m_gamePlayAlpha;
	float m_gamePlaySize;
	float m_gamePlayTextY;
	int m_gamePlayOverTimer;
	int gamePlayInit(int resoursePosition);
	void gamePlayDraw();
	void gamePlayMove(int interval);
	//下面是关于游戏胜利的一些变量
	_XSprite m_gameWinBG;
	_XSprite m_gameWinBGUp;
	_XSprite m_gameWinText00;	//完成度
	_XSprite m_gameWinText01;	//评分
	_XSprite m_gameWinText02;	//获得金币
	_XSprite m_gameWinText03;	//奖励能量
	_XSprite m_gameWinText04;	//额外奖励
	_XSprite m_gameWinText05;	//大口径火炮
	_XSprite m_gameWinText06;	//榴弹炮
	_XSprite m_gameWinText07;	//高能榴弹炮
	_XSprite m_gameWinText08;	//火箭弹
	_XSprite m_gameWinText09;	//激光炮
	_XNumber m_gameWinNum;
	_XTextureData m_gameWinStar_00;	
	_XTextureData m_gameWinStar_01;	
	_XSprite m_gameWinStar;	
	float m_gameWinBGAlpha;
	float m_gameWinTextY;
	float m_gameWinNum_00_00;	//完成度的数字
	float m_gameWinNum_00_01;	//完成度的数字
	float m_gameWinNum_01_00;	//评分的动作
	float m_gameWinNum_01_01;	//评分的动作
	float m_gameWinNum_02_00;	//金币的分数
	float m_gameWinNum_02_01;	//金币的分数
	float m_gameWinNum_03_00;	//能量的分数
	int m_gameWinNum_03_01;	//能量的分数
	float m_gameWinNum_04_00;	//额外奖励的文字
	float m_gameWinNum_04_01;	//额外奖励的文字
	int gameWinInit(int resoursePosition);
	void gameWinDraw();
	void gameWinMove(int interval);
	//下面是玩家死亡的变量
	_XSprite m_gameLostBG;	
	_XSprite m_gameLostText;	
	_XNumber m_gameLostNum;
	float m_gameLostAlpha;
	float m_gameLostY;
	int m_gameLostTimer;
	int gameLostInit(int resoursePosition);
	void gameLostDraw();
	void gameLostMove(int interval);
	//下面是demo界面的变量
	_XSprite m_gameDemoBG;	
	_XSprite m_gameDemoText1;	
	_XSprite m_gameDemoText2;
	_XSprite m_gameDemoText3;
	float m_gameDemoText2Alpha;
	float m_gameDemoText1Size;
	float m_gameDemoBGX;
	int gameDemoInit(int resoursePosition);
	void gameDemoDraw();
	void gameDemoMove(int interval);
//	void gameStartGame();
	//下面定义一些关于排行榜的变量,排行榜只保留前20名的信息
	int readRecordData();
	int writeRecordData();
	int readHeadData();
	int writeHeadData();
	int readUserData();
	int writeUserData();
public:
	int initRecordData();
	int initHeadData(int flagWrite = 0);
	int initUserData();
private:
	_XDataBasic m_data00;	//用于数据保存
	_XDataBasic m_data01;	//用于数据保存
	_XDataBasic m_data02;	//用于数据保存
	int m_gameOrderScores[MAX_GAME_ORDER_SUM];
	char m_gameOrderName[MAX_GAME_ORDER_SUM][MAX_GAME_ORDER_NAME_LENGTH];	//每个人的名字只保留6个字节
	unsigned char *m_gameOrderCameraData;	//头像数据
	unsigned char *m_gameOrderUserHeadData;	
	char m_gameOrderUserHeadName[USER_HEAD_SUM][MAX_GAME_ORDER_NAME_LENGTH];
	unsigned char *m_cameraTexData;
////	_XFontUnicode m_gameOrderFont;	//显示排行榜的提示文字
//	_XNumber m_gameOrderNumber_00;
//	_XNumber m_gameOrderNumber_01;
//	_XSprite m_gameOrderText_00;	
//	_XSprite m_gameOrderText_01;
//	char m_showGameOrderFlag;
	_XSprite m_gameOrderProBG;
	_XSprite m_gameOrderProMD;
	_XSprite m_gameOrderProUP;
	_XNumber m_gameOrderProNumber;
	_XMoveNumber m_gameOrderProMoveNumber;

	_XFontUnicode m_gameOrderFontInput;	//显示排行榜的提示文字
	int m_gameNowOrder;				//玩家当前的排名
	int m_gameOrderNextNeedScores;	//到达下个名次需要的额分数

	_XSprite m_gameInputNameBG;	//输入名字的背景
	_XFont m_inputNameFont;
	float m_inputNameY;
	float m_inputAlpha;
	int m_inputNameTimer;
	int gameOrderInit(int resoursePosition);
	void gameOrderDraw();
	void gameOrderDrawUp();
	void gameOrderMove(int interval);
	int getNowOrder();
	void updateGameOrder();

	char m_tempName[MAX_GAME_ORDER_NAME_LENGTH];
	int m_inputNamePosition;
	char m_nowChoose;
	int m_inputGlare;	//输入栏的闪烁
	float m_inputCharMove;
	char m_inputCharFlag;
	void inputNameDraw();
	void inPutNameMove(int interval);
	void drawChar(float alpha);
	//下面是关于显示排行榜的一些变量
	_XSprite m_orderShowBG;	//大背景，小背景
	_XSprite m_orderShowBG1;
	_XSprite m_orderShowBG2;
	_XSprite m_orderShowText;
	_XNumber m_orderShowNum;
	char m_orderShowFlag;	//用于标记是否是从idle界面中进入的 1:是，0:否
	float m_orderShowBGY;
	int orderShowInit(int resoursePosition);
	void orderShowDraw();
	void orderShowMove(int interval);
	void showOrder(int x,int y,int order,int flag);
	int m_soundShowOrderTemp;
	//下面是是通关后的界面
	_XSprite m_gameAllOverText;
	float m_gameAllOverY;
	int m_gameAllOverTimer;
	int gameAllOverInit(int resoursePosition);
	void gameAllOverDraw();
	void gameAllOverMove(int interval);
	//下面是关于时间限制的变量
	_XSprite m_gameTimerBG;			//显示时间的背景
	_XSprite m_gameTimerAutoShoot;	//显示自动射击
	_XNumber m_gameTimerNum;		//显示时间的数字
	char m_isNeedTimer;				//是否需要时间限制
	int m_gamePlayTimer;			//游戏计时器	//单位为毫秒 //注意如果不显示时间则会显示自动射击
	int m_gameSetPlayerTimer;		//这一把设置的计时时间
	int m_gameTimerStage;			//时间的显示阶段
	float m_gameTiemrX;
	float m_gameTimerAlpha;
	int gameTimerInit(int resoursePosition);
	void gameTimerDraw();
	void gameTimerMove(int interval);
	void setGameTimerReset(int mode = 0);	//设置计时重新开始
	void setGameTimerStart();				//设置开始计时
	void setGameTimerEnd(int mode);			//设置结束计时 0:停止 1:退出

	//下面是武器提示的界面元素
	_XSprite m_weaponInfoBG_00;					//选中的背景
	_XSprite m_weaponInfoBG_10;					//选中的背景
	_XSprite m_weaponInfoBG_01;					//没有选中或者不能选择的背景
	_XSprite m_weaponInfoBG_02;					
	_XSprite m_weaponInfoBG_03;					
	_XSprite m_weaponInfoBG_04;					
	_XSprite m_weaponInfoBG_05;					
	_XSprite m_weaponInfoBG_06;					
	_XSprite m_weaponInfoLG_00;					
	_XSprite m_weaponInfoLG_01;					
	_XSprite m_weaponInfoLG_02;					
	_XSprite m_weaponInfoLG_03;					
	_XSprite m_weaponInfoLG_04;					
	_XSprite m_weaponInfoLG_05;					
	_XSprite m_weaponInfoLK_00;					
	_XSprite m_weaponInfoLK_01;					
	_XSprite m_weaponInfoLK_02;					
	_XSprite m_weaponInfoLK_03;					
	_XSprite m_weaponInfoLK_04;					
	_XSprite m_weaponInfoLK_05;	
	_XGUN_TYPE m_upChooseGunType;		//上一次选择的武器是什么
	_XGUN_TYPE m_nowChooseGunType;		//这次选择武器的编号
	int m_chooseStage;			//武器选择动作的阶段性标志
	float m_chooseSize;
	int m_upCanUseGunType;	//当前能选择的武器是第几个
	int m_nowCanUseGunType;	//当前能选择的武器是第几个
	int m_canUseGunChangeStage;
	float m_canUserGunY;
	float m_canUserGunAlpha;
	float m_canUserGunSize;
	int weaponInfoInit(int resoursePosition);	//界面初始化
	void weaponInfoDraw();
	void waeponInfoMove(int interval);
//+++++++++++++++++++++++++++++++++++++++++++
//下面这个是为了实现新的界面中的元素变动而定义的变量
	_XMoveNumber m_weaponInfoMoveData;
	float m_weaponInfoUpPos;
	float m_weaponInfoNowPos;
	int m_weaponInfoTimer;
	void updataWeaponInfo();
//-------------------------------------------
	//下面是能量过低的文字提示以及时间不多的文字提示
	_XSprite m_energyLowFace;
	_XSprite m_energyLowText;
	_XSprite m_timerLowFace;
	_XSprite m_timerLowText;

	int m_upEnergySum;	//上一次的能量值
	int m_energyLowStage;
	float m_energyLowY;
	int m_upTimerSum;	//上一次的时间
	int m_timerLowStage;
	float m_timerLowY;

	int warnningInit(int resoursePosition);	//界面初始化
	void warnningDraw();
	void warnningMove(int interval);
	//下面是关于发炮弹的震动效果
	float m_UIShakeData;	//震动的的参数
	float m_UIShakeTempData;	//为了优化sin运算而定义的临时变量
	float m_UIShakeTempData1;	//为了优化sin运算而定义的临时变量
	int m_UIShakeStage;		//震动的阶段
	void UIShakeMove(int interval);
	float m_gunShakeData;	//震动的的参数
	float m_gunShakeTempData;	//为了优化sin运算而定义的临时变量
	float m_gunShakeTempData1;	//为了优化sin运算而定义的临时变量
	int m_gunShakeStage;		//震动的阶段
	void gunShakeMove(int interval);
	//下面是关于帮助的一些变量
	_XSprite m_helpText;
	_XSprite m_helpPic;
	float m_helpAlpha;
	float m_helpAngle;
	float m_helpTextAlpha;
	int m_helpStage;
	_XVector2 m_helpPosition;

	_XVector2 m_helpArmPosition;	//帮助的目标位置
	_XVector2 m_helpOldPosition;	//帮助的原始位置
	float m_helpMoveX;				//移动的参数
	int m_helpMoveStage;			//移动的标志

	void setHelpPosition(float x,float y);
	int helpInit(int resoursePosition);	//界面初始化
	void helpDraw();
	void helpMove(int interval);
	void setHelpEnd()
	{
		if(m_helpStage > 0 && m_helpStage < 4) m_helpStage = 4;
	}
	//下面是关于配置界面的一些变量
public:
	_XConfigData m_configData;
private:
	void initConfigData()
	{
		m_configData.isTicketNeed = 1;
		m_configData.ticketSum = 1;
		m_configData.isAutoShoot = 1;
		m_configData.coinToCredit = 1;
		m_configData.oneCreditEnergy = 2; //400 800 1200 1600 2000
		m_configData.volume = 10;	//0,10,20,30,40,50,60,70,80,90,100
		m_configData.allInCoin = 0;
		m_configData.allOutTicket = 0;
		m_configData.levelSum = 4;
	}
//	int readConfigData();	//直接调用记录函数即可
//	void writeConfigData();
	_XSprite m_configBG;
	_XSprite m_configText_00;
	_XSprite m_configText_01;
	_XSprite m_configText_02;
	_XSprite m_configText_03;
	_XSprite m_configText_04;
	_XSprite m_configText_05;
	_XSprite m_configText_06;
	_XSprite m_configText_07;
	_XSprite m_configText_08;
	_XSprite m_configText_09;
	_XSprite m_configText_10;
	_XSprite m_configText_11;
	_XSprite m_configChoose;
	_XFontUnicode m_configFont;
	float m_configRealY;
	float m_configY;
	int m_configNowChoose;	//配置界面的选择值
	char m_isInConfig;	//是否在配置界面的状态下
	int m_configStage;	//配置界面的阶段
	int m_configChooseStage;
	float m_configChooseSize;
	_XConfigData m_configBackData;
public:
	void enterConfig()
	{
		if(m_isInConfig == 0 && m_gameStage == GAME_STAGE_GAMELOGO && m_gameCounter == 3)
		{
			m_isInConfig = 1;
			m_configStage = 1;
			m_configNowChoose = 0;
			m_configChooseStage = 0;
			m_configChoose.setSize(1.0f,1.0f);
			m_configBackData = m_configData;
		}
	}
	void configKeyProc(CONFIG_KEY key);
private:
	int configInit(int resoursePosition);
	void configDraw();
	void configMove(int interval);
private:
	//下面加入介绍界面的内容
	_XSprite m_instructionBG;			//说明界面的背景
	_XSprite m_instructionOBJ_00;		//说明界面的物件贴图
	_XSprite m_instructionOBJ_01;
	_XSprite m_instructionOBJ_02;
	_XSprite m_instructionOBJ_03;
	_XSprite m_instructionOBJ_04;
	_XSprite m_instructionOBJ_05;
	_XSprite m_instructionOBJ_06;
	_XSprite m_instructionOBJ_07;
	_XSprite m_instructionOBJ_08;
	_XSprite m_instructionOBJ_09;
	_XSprite m_instructionOBJ_10;
	_XSprite m_instructionOBJ_11;
	_XSprite m_instructionOBJ_12;
	_XSprite m_instructionOBJ_13;
	_XSprite m_instructionText_00;		//说明界面的文字图
	_XSprite m_instructionText_01;
	_XSprite m_instructionText_02;
	_XSprite m_instructionText_03;
	_XSprite m_instructionText_04;
	_XSprite m_instructionText_05;
	_XSprite m_instructionText_06;
	_XSprite m_instructionText_07;
	_XSprite m_instructionText_08;
	_XSprite m_instructionText_09;
	_XSprite m_instructionText_10;
	_XSprite m_instructionText_11;
	_XSprite m_instructionText_12;
	_XSprite m_instructionText_13;
	float m_instructionTemp;
	float m_instructionPosition;
	float m_instructionMode;
	int m_instructionTimer;
	int instructInit(int resoursePosition);
	void instructDraw();
	void instructMove(int interval);
	//下面是与彩票相关的数据
	_XSprite m_ticketLog;
	_XSprite m_ticketLog0;
	_XSprite m_ticketText00;
	_XSprite m_ticketText01;
	_XNumber m_ticketNumber;
	_XNumber m_ticketAddNumber;
	int m_numberStage;
	float m_numberAlpha;
//	char m_isTicketNeed;	//是否需要出彩票
	int m_allOutTicket;		//总的出彩票的数量
	int m_allArmOutTicket;
	int m_releaseTicket;	//剩余的彩票数量
	int m_releaseArmTicket;
	char m_isWrong;
	int m_ticketWrongStage;
	float m_ticketWrongY;
	int m_ticketJumpStage;	//彩票跳跃的动画阶段
	float m_ticketJumpSize;		
	void addTicketInput(_XGEM_TYPE type,float size)
	{
		if(type >= GEM_TYPE_16) return;
		int temp;
		if(type <= GEM_TYPE_15)
		{
			if(size <= 1.5f) temp = type * 3; else
			if(size <= 2.5f) temp = type * 3 + 1; else
			temp = type * 3 + 2;
			if(temp >= 0 && temp < MAX_GEM_TYPE - 1) addTicket(gemBasicTicket[temp]);
		}else
		{
			temp = 26 + type - GEM_TYPE_15;
			if(temp >= 0 && temp < MAX_GEM_TYPE - 1) addTicket(gemBasicTicket[temp]);
		}
	}
public:
	float m_ticketbarArmSum;
	float m_ticketbarSum;
	void addTicket(float temp)
	{
		if(m_configData.isTicketNeed == 0) return;
		if(temp > 0)
		{
			if(m_configData.ticketSum == 0) m_ticketbarArmSum += temp * 0.25f;
			else if(m_configData.ticketSum == 1) m_ticketbarArmSum += temp * 0.5f;
			else m_ticketbarArmSum += temp * 1.0f;
			if(m_ticketbarArmSum > 1)
			{
				temp = (int)m_ticketbarArmSum;
				m_ticketbarArmSum -= temp;
				m_ticketbarSum = 0.0f;

				m_allArmOutTicket += temp;
				m_releaseArmTicket += temp;
				m_configData.allOutTicket += temp;
				m_ticketJumpStage = 1;
				m_numberStage = 1;
				char ch[20];
				sprintf(ch,"+%d",temp);
				m_ticketAddNumber.setNumber(ch);
				writeRecordData();
			}
		}
	}
	void ticketbarSumMove(int interval)
	{
		if(m_configData.isTicketNeed == 0) return;
		float tempSum;
		if(abs(m_ticketbarArmSum - m_ticketbarSum) > 0.0001f)
		{
			tempSum = m_ticketbarArmSum - m_ticketbarSum;
			if(tempSum < 0.0001f && tempSum > -0.0001f)
			{
				if(tempSum < 0) tempSum = -0.0001f;
				else tempSum = 0.0001f;
			}
			if(0.0015f * interval < 1.0f)
			{
				tempSum *= 0.0015f * interval;
			}
			m_ticketbarSum += tempSum;
			m_ticketBar.setClipRect(0.0f,0.0f,64.0f,251.0f * m_ticketbarSum);
		}
	}
	void decreaseTicket(int temp)
	{
		if(m_configData.isTicketNeed == 0) return;
		if(m_releaseArmTicket >= temp) m_releaseArmTicket -= temp;
	}
	void setTicketWrong()
	{
		if(m_isWrong == 0)
		{
			m_isWrong = 1;
			m_ticketWrongStage = 1;
		}else
		{
			if(m_ticketWrongStage >= 4) m_ticketWrongStage = 1;
		}
	}
	void setTicketWell()
	{
		if(m_isWrong == 1)
		{
		//	m_isWrong = 0;
			m_ticketWrongStage = 4;
		}
	}
private:
	int ticketInit(int resoursePosition);
	void ticketDraw();
	void ticketMove(int interval);

	char m_isLightShoot;	//是否是激光炮发射中
public:
	void inputName(INPUT_NAME_KEY key);
private:
	_XGemMatrix *m_gems;
public:
	void addCredit()
	{
		m_creditSum ++;
		m_configData.allInCoin ++;
		char temp[32];
		sprintf(temp,"%3d+%d",m_creditSum,m_configData.coinToCredit);
		m_creditNum.setNumber(temp);
		m_sound.play(m_soundCoinIn);
		writeRecordData();
	}
	void addEnergy()
	{
		if(m_creditSum >= m_configData.coinToCredit)
		{
			m_creditSum -= m_configData.coinToCredit;
			char temp[32];
			sprintf(temp,"%3d+%d",m_creditSum,m_configData.coinToCredit);
			m_creditNum.setNumber(temp);
		//	m_energyArmSum += 1000;
			energyAdd((m_configData.oneCreditEnergy + 1) * 400);
			m_sound.play(m_soundAddEnergy);
		}
	}
private:
	int findAFreeBullet();	//寻找一个有效地子弹
	void shootEnergyDecrease(int energy);
	void gunFireSpriteSet(_XGUN_TYPE type);
	//void gunShootBulletInit(int order,_XGUN_TYPE type);
	void gunShootBulletInit(int order);
	int bulletBoom(_XVector2 position,_XBULLET_TYPE type,_XGemMatrix *gems);	//榴弹炸开的处理

	//下面是关于声音部分的定义
	_XSound m_sound;
	XSoundHandle m_soundGun_00;
	XSoundHandle m_soundGun_01;
	XSoundHandle m_soundGun_02;
	XSoundHandle m_soundGun_03;
	XSoundHandle m_soundGun_04;
	XSoundHandle m_soundGun_05;
	XSoundHandle m_soundGun_06;
	XSoundHandle m_soundBoom_00;
	XSoundHandle m_soundBoom_01;
	XSoundHandle m_soundBoom_02;
	XSoundHandle m_soundBoom_03;
	XSoundHandle m_soundBoom_04;
	XSoundHandle m_soundCoinIn;
	XSoundHandle m_soundSumAdd_00;
	int m_soundSumAddTimer;
	XSoundHandle m_soundTimerOver;
	int m_soundTimerOverOld;
	XSoundHandle m_soundStarAdd;
	int m_soundStarSumOld;
	XSoundHandle m_soundTextDown;
	XSoundHandle m_soundTimer_00;
	int m_soundTimerOld;
	//XSoundHandle m_soundTimer_01;
	//int m_soundTimerOld_01;
	XSoundHandle m_soundAddEnergy;
	XSoundHandle m_soundChangeGun;
	XSoundHandle m_soundTextMove_00;
	XSoundHandle m_soundTextMove_01;
	XSoundHandle m_soundTypeIn;
	XSoundHandle m_soundTypeMove;
	XSoundHandle m_soundHitGem;
	XSoundHandle m_soundHitShield;
	XSoundHandle m_soundGemBreak;
	XSoundHandle m_soundEnergyFree;
	XSoundHandle m_soundHitSide;
	XSoundHandle m_soundSJTools;
	XSoundHandle m_soundFSTools;
	XSoundHandle m_soundScreenDown;
	XSoundHandle m_soundScreenUp;
	XSoundHandle m_soundColorBoom;
	int soundInit(int resoursePosition);
public:
	int init(_XGemMatrix *gems,int resoursePosition = 0);
	void drawDown();
	void draw();
	void drawUp();
	void move(int interval);
	void whirlGun(int direction);
	void upgradeGun();
	void setGunType(_XGUN_TYPE type);

	void crashProc(_XGemMatrix *gems);//子弹的碰撞处理
	int crashToolsProc(_XGemMatrix *gems,_XGEM_TYPE type,_XVector2 tempPoint1);//子弹的碰撞处理
	int colorBulletProc(_XGemMatrix *gems,_XGEM_TYPE type);
	int gemBeCrashProc(_XGemMatrix *gems,int gemOrder,_XVector2 tempPoint,int energy);	//宝石被打到的处理过程
	int lightBulletProc(_XGemMatrix *gems,int order);	//激光子弹的处理过程
	void followBulletProc(_XGemMatrix *gems,int order);	//自动跟踪导弹的处理

	void shoot();	//射击
	int m_gunShootSpeed;

//	char m_shootMode;	//射击模式0:不能连续射击，1:可以连续射击

	_XGun();
};

#endif