#ifndef _JIA_XGEM_
#define _JIA_XGEM_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:       2011.12.10
//--------------------------------

#include "XEffeEngine.h"
#define MAP_EDIT_MODE (0)		//是否开启地图编辑模式

#define MAX_GEM_ROW (20)		//宝石最多20列
#define MATRIX_DATA_TXT (0)		//是否使用文本的方式读取数据

#define MAX_LEVEL_SUM (1024)		//关卡的最大值

#define MAX_GEM_TYPE (37)

extern float demageGunToGem[6][8];	//子弹对宝石的破坏系数

extern float gemBasicEnergy[8];
extern float gemBasicTicket[MAX_GEM_TYPE - 1];
extern float gemE2GRate[8];
#define GOLD_BOX_TICKET_MIN (5)
#define GOLD_BOX_TICKET_MAX (100)
#define GOLD_BOX_SCORES_MIN (300)
#define GOLD_BOX_SCORES_MAX (1000)
#define SHIELD_ENERGY (250)	//盾牌的能量

enum _XGEM_TYPE
{
	GEM_TYPE_00,							
	GEM_TYPE_01,
	GEM_TYPE_02,
	GEM_TYPE_03,
	GEM_TYPE_04,
	GEM_TYPE_05,
	GEM_TYPE_06,
	GEM_TYPE_07,
	GEM_TYPE_15,	//永远打不坏的道具
	GEM_TYPE_08,	//爆炸的道具
	GEM_TYPE_09,	//水平直线攻击的道具
	GEM_TYPE_10,	//垂直直线攻击的道具
	GEM_TYPE_11,	//色彩攻击的道具
	GEM_TYPE_12,	//镜面反射攻击的道具1
	GEM_TYPE_13,	//镜面反射攻击的道具2
	GEM_TYPE_14,	//下面一排子弹的道具
	GEM_TYPE_21,	//扣除能量的道具
	GEM_TYPE_22,	//宝箱的道具

	GEM_TYPE_16,	//加能量的道具
	GEM_TYPE_17,	//加金币的道具
	GEM_TYPE_18,	//金币转化为能量的道具
	GEM_TYPE_19,	//10s免费射击的道具
	GEM_TYPE_20,	//加时间的道具

	//下面是状态道具
	GEM_TYPE_23,	//活得金币x2
	GEM_TYPE_24,	//时间变慢
	GEM_TYPE_25,	//威力加强
	GEM_TYPE_26,	//射速加快
	GEM_TYPE_27,	//消耗降低
};

enum
{
	GEM_STAGE_NORMAL,	//正常状态
	GEM_STAGE_DISPLAY,	//消失状态
	GEM_STAGE_COMEIN,	//进入状态
};

class _XGem
{
private:
public:
	_XGEM_TYPE m_type;
	char m_isEnable;
	float m_size;
	_XVector2 m_position;
	float m_energy;

	float m_lineAlpha;

	_XSprite m_gemSprite;
	_XTextureData *m_tex_00;
	_XTextureData *m_tex_01;
	_XTextureData *m_tex_02;
	_XTextureData *m_tex_03;

	_XFrameEx m_boomFrame;	//宝石被摧毁的效果
	_XFrameEx m_lightFrame;	//宝石被闪光的效果
	char m_showLight;
	_XFrameEx m_runFrame;	//宝石光圈动作

	int m_isGlare;
	int m_gemStage;	//阶段
	int m_gemCounter;
	float m_gemAlpha;
	//新增加的参数
	int m_moveMode;	//0不运动 1：水平运动 2：垂直运动
	float m_moveSpeed;	//运动参数
	float m_moveMin;	//运动的范围
	float m_moveMax;	
	char m_moveFlag;
public:
	void draw();
	void drawUp();
	void move(int interval);
	void release();
	void setEnd();	//进入消失阶段
	int canCrash();	//这个宝石是否能被打
};

#define GAME_DATA_FILE_NAME "gameData.txt"
#define GAME_DATA_MATRIX_FILE_NAME "gemMatrix_000.txt"

struct _XHelpData
{
	int gemOrder;	//提示宝石的编号
	int otherGem;	//相关宝石的编号，如果这个相关宝石消失，或者这个宝石消失，则提示向下走
};

struct _XMatrixData
{
#if MATRIX_DATA_TXT
	char *data;	//文本数据
	_XMatrixData()
	:data(NULL)
	{
	}
#else
	unsigned char *binData;	//二进制数据
	_XMatrixData()
	:binData(NULL)
	{
	}
#endif
};

class _XGemMatrix
{
private:
public:
	_XGem *m_gem;
	_XFrameEx m_boomFrame;		//宝石被摧毁的效果
	_XFrameEx m_boomFrame_00;	//炸弹道具爆炸的效果
	_XFrameEx m_boomFrame_01;	//线炸弹道具爆炸的效果
	_XFrameEx m_boomFrame_02;	//额外道具被打坏的效果
	_XFrameEx m_boomFrame_03;	//道具被打坏的效果
	_XFrameEx m_lightFrame[8];		//宝石被摧毁的效果
	_XFrameEx m_runFrame;		//宝石跑线的效果

	_XTextureData m_gemTex[9][4];
	_XTextureData m_gemTexTools[19];

	char m_isInited;	//是否初始化
	int m_matrixSum;	//关卡数量
	int m_nowMatrixOrder;	//当前关卡编号
	int m_nowMaxtrixGemSum;	//当前关卡的宝石数量
	char m_matrixCanDraw;	//矩阵是否可以描绘
	int m_matrixAllGemSum;	//关卡中设定的宝石数量	
	int m_matrixReleaseGemSum;	//当前剩下宝石的数量
	int m_matrixAvailablyGemSum;	//矩阵中有效地宝石数量

	int m_timerMode;	//时间模式
	int m_timerSum;		//时间限制的数值，单位为ms

	//下面是关于提示的相关信息
	int m_helpSum;
	int m_nowHelpPoint;	//当前提示的是哪一个？
	_XHelpData *m_helpData;
	int getHelpIsEnd()
	{
		if(m_nowHelpPoint >= m_helpSum) return 1;
		else return 0;
	}
	int upDateHelp()
	{
		if(m_nowHelpPoint >= m_helpSum || m_nowHelpPoint < 0) return 0;
		if(m_helpData[m_nowHelpPoint].gemOrder >= 0 && m_helpData[m_nowHelpPoint].gemOrder < m_nowMaxtrixGemSum
			&& m_gem[m_helpData[m_nowHelpPoint].gemOrder].canCrash() == 0)
		{
			m_nowHelpPoint ++;
			return 1;
		}else
		if(m_helpData[m_nowHelpPoint].otherGem >= 0 && m_helpData[m_nowHelpPoint].otherGem < m_nowMaxtrixGemSum
			&& m_gem[m_helpData[m_nowHelpPoint].otherGem].canCrash() == 0)
		{
			m_nowHelpPoint ++;
			return 1;
		}
		return 0;
	}

	void releaseMatrix();
	void setGemAttribute(_XGem *tempGem);
#if MAP_EDIT_MODE
	int m_isShowReseau;	//是否显示网格
	_XNumber m_gemNum;
#endif

	//下面引入随机关卡的概念
	_XMatrixData *m_matrixData;	//所有关卡的数据
	int m_nowMatrix[MAX_LEVEL_SUM];		//当前玩家选中的六关

public:
	void resetMatrix();	//每个新的玩家都会重新你选择随机的6关
	int init(int resoursePosition = 0);
	int initInsideData();
	int readMatrixFromTxt(int matrixOrder);
	int readMatrix(int matrixOrder);
	int writeMatrix(int matrixOrder);	
	int changeGem(int x,int y,int flag = 0);
	void clearMatrix();
	void draw();
	void drawUp();
	void move(int interval);
	void getReleaseGemSum();

	int matrixTXT2Data(int matricOrder);	//宝石阵列数据由文本向2进制数据转换

	//增加一个道具的函数
	int addToolsGem(_XVector2 position,_XGEM_TYPE type);

	_XGemMatrix();
};

#endif