#ifndef _JIA_XNUMBEREX_
#define _JIA_XNUMBEREX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//这个类可以进行进一步的优化，做一个标记来标记序列中的字符是否存在
#include "XSprite.h"

#ifndef TEXT_MODE
#define TEXT_MODE "0123456789+-*/%."
#endif
#ifndef MAX_NUMBER_LENGTH
#define MAX_NUMBER_LENGTH 256
#endif
//24568
//+1258
//-1258
//-1258%
//-1.25%
//+-%.0123456789
class _XNumberTexture
{
private:
	int m_resoursePosition;	//资源位置 0:外部 1:内部
	char m_isInited;	//是否初始化
public:
	//int m_textureSum;		//贴图的数量
	_XTextureData *m_texture00;
	_XTextureData *m_texture01;
	_XTextureData *m_texture02;
	_XTextureData *m_texture03;
	_XTextureData *m_texture04;
	_XTextureData *m_texture05;
	_XTextureData *m_texture06;
	_XTextureData *m_texture07;
	_XTextureData *m_texture08;
	_XTextureData *m_texture09;
	_XTextureData *m_texture0a;		//+
	_XTextureData *m_texture0b;		//-
	_XTextureData *m_texture0c;		//*
	_XTextureData *m_texture0d;		///
	_XTextureData *m_texture0e;		//%
	_XTextureData *m_texture0f;		//.

public:
	int init(const char *fileName,int resoursePosition = 0);
	void release();
	_XNumberTexture();
};

class _XNumberEx
{
private:
	const _XNumberTexture *m_numberTexture;

	_XSprite m_sprite;					//贴图
	int m_isInited;

	float m_angleSin;	//角度的sin值
	float m_angleCos;
public:
	char *m_number;
	_XVector2 m_position;	//字体的位置，这个是字体放置的位置
	_XVector2 m_setPosition;	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	float m_angle;			//字体的角度
	float m_distance;		//文字之间的距离	
	_XVector2I m_size;		//文字的尺寸（像素大小）
	_XVector2 m_showSize;
	_XVector2 m_rotateBasicPoint;	//字体旋转的中心点
	char m_blendTypeScr;
	char m_blendTypeDst;
public:
	void setBlendType(int typeScr,int typeDst)
	{
		if(typeScr < 0) typeScr = 0;
		if(typeDst < 0) typeDst = 0;
		if(typeScr >= 9 ) typeScr = 8;
		if(typeDst >= 8 ) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
		m_sprite.setBlendType(m_blendTypeScr,m_blendTypeDst);
	}

	int init(const _XNumberTexture *numberTexture,_XVector2I size);		
	void draw();

	void setSize(const _XVector2& size);		//设置字体的显示大小
	void setSize(float x,float y);	//设置字体的显示大小
	void setAngle(float angle);	//设置字体显示的角度
	void setPosition(const _XVector2& position);	//设置字体显示的位置
	void setPosition(float x,float y);	//设置字体显示的位置
	void setRotateBasePoint(float x,float y);

	int setNumber(int temp);								//设置整形数
	int setNumber(float temp,int decimalPartLength);		//设置带小数点的数
	int setNumber(const char *temp);								//设置一段合理的字符串
	_XNumberEx& operator = (const _XNumberEx& temp);
	int setACopy(const _XNumberEx &temp);
	void setColor(float r,float g,float b,float a);	//设置颜色
	void setAlpha(float a);
	float getAlpha() const
	{
		return m_sprite.getAlpha();
	}
	int release();

	void setDistance(float distance)
	{
		m_distance = distance;
	}
	float getDistance() const
	{
		return m_distance;
	}
	_XNumberEx();
};

//下面是一个用于实现动态数字变化的类,就是看起来是一个数字滚动
class _XMoveNumber
{
private:
	int m_nowNumber;	//当前的数值
	float m_armNumber;	//目标需要达到的数值
public:
	void addArmNumber(float number);	//设置目标数值的增幅
	void setArmNumber(float number);	//设置目标数值
	int getNowNumber();				//返回当前的数值
	int getArmNumber();
	int move(int delay);			//返回数据是否发生变化0：没有变化，1：发生了变化
	void reset();
	_XMoveNumber()
	:m_nowNumber(0)
	,m_armNumber(0.0f)
	{
	}
};
//下面是一个数据动态变化的类，可以用于实现一些动态的曲线以及动态的效果
enum _XMoveDataMode
{
	MOVE_DATA_MODE_LINE,		//线性变化
	MOVE_DATA_MODE_SIN,			//sin曲线变化,	先快后慢		计算消耗:中
	MOVE_DATA_MODE_SIN_MULT,	//sin曲线变化,	先快后慢		计算消耗:小
	MOVE_DATA_MODE_SQRT2,		//4次开方,		先快后慢		计算消耗:大
	MOVE_DATA_MODE_SQRT2_MULT,	//使用乘方实现,	先快后慢		计算消耗:中
	MOVE_DATA_MODE_COS,			//cos曲线变化,	先慢后快		计算消耗:中
	MOVE_DATA_MODE_COS_MULT,	//cos曲线变化,	先慢后快		计算消耗:小
	MOVE_DATA_MODE_DSIN,		//sin曲线变化,	先快后慢再快	计算消耗:中	
	MOVE_DATA_MODE_DCOS,		//cos曲线变化,	先慢后快再慢	计算消耗:中
	MOVE_DATA_MODE_DSIN_MULT,	//sin曲线变化,	先快后慢再快	计算消耗:小	
	MOVE_DATA_MODE_DCOS_MULT,	//cos曲线变化,	先慢后快再慢	计算消耗:小
};

class _XMoveData
{
private:
	float m_startData;	//起始数值
	float m_endData;	//终止数值
	float m_nowData;	//当前的数值
	float m_speed;
	float m_timer;		//时间标尺
	int m_loopTimer;	//循环的次数
	
	char m_isEnd;	//运动是否结束1:结束 0:运动中
	char m_isLoop;	//是否循环,这里是镜像循环
	_XMoveDataMode m_mode;
public:
	//isLoop:0,不循环,>0循环的次数,<0无限循环
	void reset(float start,float end,float speed,_XMoveDataMode mode = MOVE_DATA_MODE_SIN_MULT,int isLoop = 0)
	{
		m_isEnd = 0;
		m_startData = start;
		m_endData = end;
		m_speed = speed;
		m_nowData = start;
		m_timer = 0.0f;
		m_mode = mode;
		if(isLoop == 0) m_isLoop = 0;
		else m_isLoop = 1;
		if(isLoop >= 1) m_loopTimer = isLoop;
		else m_loopTimer = -1;	//无限循环
	}
	void move(int delay);
	float getNowData() const
	{
		 return m_nowData;
	}
	float getNowTimer() const 
	{
		return m_timer;
	}
	int getIsEnd() const
	{
		return m_isEnd;
	}
	void setEnd()
	{
		m_isEnd = 1;
		m_nowData = m_endData;
	}
	_XMoveData()
	:m_startData(0.0f)
	,m_endData(0.0f)
	,m_nowData(0.0f)
	,m_isEnd(1)
	,m_mode(MOVE_DATA_MODE_SIN)
	,m_isLoop(0)
	{
	}
};

#include "XNumberEx.inl"

#endif
