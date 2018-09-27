#ifndef _JIA_XIMAGECTRL_
#define _JIA_XIMAGECTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//这是一个简单的贴图的控件，用于显示一张图片
#include "XControlBasic.h"
namespace XE{
class XImageCtrl:public XControlBasic
{
private:
	XBool m_isInited;	//是否初始化
	bool m_onlyWithTexture;
	bool m_connectWithTex;	//是否与贴图链接，链接之后只显示指定的贴图
	XTextureData *m_pTexture;	//贴图数据
	XSprite m_sprite;	//用于显示
	int m_pixelSum;	//点的数量
	int m_w;
	int m_h;
	unsigned int m_texID;
	unsigned int m_connectTex;
public:
	void setTextureID(unsigned int id){m_texID = id;}
	void imageDraw(const XVec2&scale,const XVec2&pos)
	{
		if(!m_isInited) return;
		XVec2 tmpScale = m_sprite.getScale();
		XVec2 tmpPos = m_sprite.getPosition();
		m_sprite.setScale(scale);
		m_sprite.setPosition(pos);
		m_sprite.draw(m_pTexture->texture.m_texture);
		m_sprite.setScale(tmpScale);
		m_sprite.setPosition(tmpPos);
	}
	bool getIsConnected()const{return m_connectWithTex;}	//获取是否连接
	void connectTex(unsigned int tex)	//与指定贴图链接
	{
		m_connectWithTex = true;
		m_connectTex = tex;
	}
	void disConnected(){m_connectWithTex = false;}	//取消链接
	XBool init(const char *filename);			//从文件中读取图片
	XBool init(int w,int h,XColorMode mode);	//建立空的图片
	XBool init(int w,int h,unsigned int tex,bool needResize = false);	//只通过贴图编号建立
	void updateTxt(const void *data)	//更新贴图数据
	{
		if(!m_onlyWithTexture && m_pTexture != NULL)
			m_pTexture->updateTexture(data);
	}
	void updateTxtR2B(const void *data)	//更新贴图数据
	{
		if(!m_onlyWithTexture && m_pTexture != NULL)
			m_pTexture->updateTextureR2B(data);
	}
	int getWidth() const //获取图片的宽度
	{
		if(m_onlyWithTexture) return m_w;
		else return (int)(m_pTexture->textureMove2.x - m_pTexture->textureMove.x);
	}		
	int getHeight() const //获取图片的高度
	{
		if(m_onlyWithTexture) return m_h;
		else return (int)(m_pTexture->textureMove2.y - m_pTexture->textureMove.y);
	}	
	void disable() { m_isEnable = XFalse; }					//使无效
	void enable() { m_isEnable = XTrue; }					//使无效
protected:
	void draw();
	void drawUp() {;}

	XBool mouseProc(const XVec2&,XMouseState){return XFalse;}
	XBool keyboardProc(int,XKeyState){return XFalse;}
	void insertChar(const char *,int) {;}
	XBool canGetFocus(const XVec2& p);
	XBool canLostFocus(const XVec2&){return XTrue;}
public:
	void bind(){if(m_pTexture != NULL) m_pTexture->bind();}	//绑定贴图
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);
	void fitScale(float x,float y);	//自动适应指定的大小
	XImageCtrl()
		:m_isInited(XFalse)
		,m_connectWithTex(false)
		,m_pTexture(NULL)
	{
		m_ctrlType = CTRL_OBJ_IMAGE;
	}
	~XImageCtrl(){release();}
	void release();
	XBool isInRect(const XVec2& p) {return m_sprite.isInRect(p);}
	XVec2 getBox(int order) {return m_sprite.getBox(order);}
	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);
};
#if WITH_INLINE_FILE
#include "XImageCtrl.inl"
#endif
}
#endif