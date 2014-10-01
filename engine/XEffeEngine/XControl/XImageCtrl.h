#ifndef _JIA_XIMAGECTRL_
#define _JIA_XIMAGECTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//这是一个简单的贴图的控件，用于显示一张图片
#include "XControlBasic.h"

class _XImageCtrl:public _XControlBasic
{
private:
	_XBool m_isInited;	//是否初始化
	_XTextureData m_texture;	//贴图数据
	_XSprite m_sprite;	//用于显示
	int m_pixelSum;	//点的数量
public:
	_XBool init(const char *filename);			//从文件中读取图片
	_XBool init(int w,int h,_XColorMode mode);	//建立空的图片
	void updateTxt(unsigned char *data)	//更新贴图数据
	{
		m_texture.updateTexture(data);
	}
	int getWidth() const {return m_texture.textureMove2.x - m_texture.textureMove.x;}		//获取图片的宽度
	int getHeight() const {return m_texture.textureMove2.y - m_texture.textureMove.y;}	//获取图片的高度
protected:
	void draw()
	{
		if(!m_isInited ||	//如果没有初始化直接退出
			!m_isVisible) return;	//如果不可见直接退出
		m_sprite.draw();
	}
	void drawUp() {;}

	_XBool mouseProc(float,float,_XMouseState){return XFalse;}
	_XBool keyboardProc(int,_XKeyState){return XFalse;}
	void insertChar(const char *,int) {;}
	_XBool canGetFocus(float x,float y)
	{
		if(!m_isInited ||	//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible ||	//如果不可见直接退出
			!m_isEnable) return XFalse;		//如果无效则直接退出
		return isInRect(x,y);
	}
	_XBool canLostFocus(float,float){return XTrue;}
public:
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		m_sprite.setPosition(m_position);
	}
	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y)
	{
		m_size.set(x,y);
		m_sprite.setSize(m_size);
	}
	_XImageCtrl()
		:m_isInited(XFalse)
	{
		m_ctrlType = CTRL_OBJ_XIMAGE;
	}
	~_XImageCtrl(){release();}
	void release();
	_XBool isInRect(float x,float y) {return m_sprite.isInRect(x,y);}
	_XVector2 getBox(int order) {return m_sprite.getBox(order);}
	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a) 
	{
		m_color.setColor(r,g,b,a);
		return m_sprite.setColor(r,g,b,a);
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
		return m_sprite.setAlpha(a);
	}
};
#include "XImageCtrl.inl"
#endif