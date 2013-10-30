#ifndef _JIA_XENGINELOGO_
#define _JIA_XENGINELOGO_
//这是一个显示引擎Logo的类
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XEffeEngine.h"

class _XEngineLogo
{
private:
	int m_resoursePosition;
	char m_isInited;
	char m_mode;	//0普通模式，1竖屏模式
	_XVector2 m_position;
	_XSprite m_back;	
	_XSprite m_logoBack;
	_XSprite m_logoLight;
	_XSprite m_logoMiddle;
	_XSprite m_logoTextX;
	_XSprite m_logoTextE;
	_XSprite m_logoTextEC;
	_XSprite m_logoTextRE;
	//_XSprite m_logoTextECopy[30];

	int m_logoStage;
	float m_xAlpha;
	float m_xSize;
	float m_dxLight;
	float m_eAlpha;
	
	char m_stageFlag[8];	//描述各个阶段标记的变量
	//下面这个是内部调用的方法
	void setPosition(_XVector2 position);	//设置logo显示的位置

public:
	int init(_XVector2 position,int resoursePosition,int mode = 0);		//初始化
	void move(int timeDelay);			//显示logo的动画
	void reset();						//重置
	void draw();						//描绘logo
	_XEngineLogo();						//构造函数
	~_XEngineLogo();						//构造函数
	void setBackSize(_XVector2 size);		//设置背景图的尺寸
	void setBackSize(float x,float y);		//设置背景图的尺寸

	//下面这两个方法尚未实现
	void setSize(_XVector2 size);			//设置logo显示的尺寸
	int release();	//释放logo的资源
	int getIsEnd();
};

inline void _XEngineLogo::setPosition(_XVector2 position)	//设置logo显示的位置
{
	m_position = position;
}

inline void _XEngineLogo::setBackSize(_XVector2 size)		//设置背景图的尺寸
{
	m_back.setSize(size);
}

inline void _XEngineLogo::setBackSize(float x,float y)		//设置背景图的尺寸
{
	m_back.setSize(x,y);
}

inline int _XEngineLogo::getIsEnd()
{
	if(m_isInited == 0) return 0;
	if(m_logoStage == -2) return 1;
	else return 0;
}

#endif