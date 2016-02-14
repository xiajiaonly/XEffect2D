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
	XTextureData m_texture;	//贴图数据
	XSprite m_sprite;	//用于显示
	int m_pixelSum;	//点的数量
public:
	XBool init(const char *filename);			//从文件中读取图片
	XBool init(int w,int h,XColorMode mode);	//建立空的图片
	void updateTxt(unsigned char *data)	{m_texture.updateTexture(data);}//更新贴图数据
	int getWidth() const {return (int)(m_texture.textureMove2.x - m_texture.textureMove.x);}		//获取图片的宽度
	int getHeight() const {return (int)(m_texture.textureMove2.y - m_texture.textureMove.y);}	//获取图片的高度
protected:
	void draw();
	void drawUp() {;}

	XBool mouseProc(float,float,XMouseState){return XFalse;}
	XBool keyboardProc(int,XKeyState){return XFalse;}
	void insertChar(const char *,int) {;}
	XBool canGetFocus(float x,float y);
	XBool canLostFocus(float,float){return XTrue;}
public:
	void bind(){m_texture.bind();}	//绑定贴图
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y);
	XImageCtrl()
		:m_isInited(XFalse)
	{
		m_ctrlType = CTRL_OBJ_IMAGE;
	}
	~XImageCtrl(){release();}
	void release();
	XBool isInRect(float x,float y) {return m_sprite.isInRect(x,y);}
	XVector2 getBox(int order) {return m_sprite.getBox(order);}
	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
};
#if WITH_INLINE_FILE
#include "XImageCtrl.inl"
#endif
}
#endif