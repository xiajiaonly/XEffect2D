#ifndef _JIA_XENGINELOGO_
#define _JIA_XENGINELOGO_
//这是一个显示引擎Logo的类
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XOSDefine.h"
#include "XSprite.h"
namespace XE{
class XEngineLogo
{
private:
	XResourcePosition m_resoursePosition;
	XBool m_isInited;
	XBool m_mode;	//0普通模式，1竖屏模式
	XVector2 m_position;
	XSprite m_back;	
	XSprite m_logoBack;
	XSprite m_logoLight;
	XSprite m_logoMiddle;
	XSprite m_logoTextX;
	XSprite m_logoTextE;
	XSprite m_logoTextEC;
	XSprite m_logoTextRE;
	//XSprite m_logoTextECopy[30];

	int m_logoStage;
	float m_xAlpha;
	float m_xSize;
	float m_dxLight;
	float m_eAlpha;
	
	char m_stageFlag[8];	//描述各个阶段标记的变量
	//下面这个是内部调用的方法
	void setPosition(const XVector2 &position);	//设置logo显示的位置

public:
	XBool init(const XVector2 &position,XResourcePosition resoursePosition,XBool mode = true);		//初始化
	void move(float timeDelay);			//显示logo的动画
	void reset();						//重置
	void draw();						//描绘logo
	XEngineLogo();						//构造函数
	~XEngineLogo();						//构造函数
	void setBackSize(const XVector2 &size);		//设置背景图的尺寸
	void setBackSize(float x,float y);		//设置背景图的尺寸

	//下面这两个方法尚未实现
	void setSize(const XVector2 &size);			//设置logo显示的尺寸
	XBool release();	//释放logo的资源
	XBool getIsEnd();
};

inline void XEngineLogo::setPosition(const XVector2 &position)	//设置logo显示的位置
{
	m_position = position;
}
inline XBool XEngineLogo::getIsEnd()
{
	if(!m_isInited) return XFalse;
	return m_logoStage == -2;
}
}
#endif