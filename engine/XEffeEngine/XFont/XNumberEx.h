#ifndef _JIA_XNUMBEREX_
#define _JIA_XNUMBEREX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//这个类可以进行进一步的优化，做一个标记来标记序列中的字符是否存在
#include "XSprite.h"
namespace XE{
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
class XNumberTexture
{
private:
	XResourcePosition m_resoursePosition;	//资源位置 0:外部 1:内部
	XBool m_isInited;	//是否初始化
public:
	//int m_textureSum;		//贴图的数量
	XTextureData *m_texture00;
	XTextureData *m_texture01;
	XTextureData *m_texture02;
	XTextureData *m_texture03;
	XTextureData *m_texture04;
	XTextureData *m_texture05;
	XTextureData *m_texture06;
	XTextureData *m_texture07;
	XTextureData *m_texture08;
	XTextureData *m_texture09;
	XTextureData *m_texture0a;		//+
	XTextureData *m_texture0b;		//-
	XTextureData *m_texture0c;		//*
	XTextureData *m_texture0d;		///
	XTextureData *m_texture0e;		//%
	XTextureData *m_texture0f;		//.

public:
	XBool init(const char *fileName,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
	XNumberTexture();
};

class XNumberEx
{
private:
	const XNumberTexture *m_numberTexture;

	XSprite m_sprite;					//贴图

	float m_angleSin;	//角度的sin值
	float m_angleCos;
public:
	char *m_number;
	XVector2 m_position;	//字体的位置，这个是字体放置的位置
	XVector2 m_setPosition;	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	float m_angle;			//字体的角度
	float m_distance;		//文字之间的距离	
	XVector2 m_size;		//文字的尺寸（像素大小）
	XVector2 m_showSize;
	XVector2 m_rotateBasicPoint;	//字体旋转的中心点
	char m_blendTypeScr;
	char m_blendTypeDst;
private:
	XBool m_isInited;
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

	XBool init(const XNumberTexture *numberTexture,const XVector2 &size);		
	void draw();

	void setScale(const XVector2& scale);		//设置字体的显示大小
	void setScale(float x,float y);	//设置字体的显示大小
	void setAngle(float angle);	//设置字体显示的角度
	void setPosition(const XVector2& position);	//设置字体显示的位置
	void setPosition(float x,float y);	//设置字体显示的位置
	void setRotateBasePoint(float x,float y);

	XBool setNumber(int temp);								//设置整形数
	XBool setNumber(float temp,int decimalPartLength);		//设置带小数点的数
	XBool setNumber(const char *temp);								//设置一段合理的字符串
	XNumberEx& operator = (const XNumberEx& temp);
	XBool setACopy(const XNumberEx &temp);
	void setColor(float r,float g,float b,float a);	//设置颜色
	void setAlpha(float a);
	float getAlpha() const {return m_sprite.getAlpha();}
	XBool release();

	void setDistance(float distance){m_distance = distance;}
	float getDistance() const {return m_distance;}
	XNumberEx();
};
#if WITH_INLINE_FILE
#include "XNumberEx.inl"
#endif
}
#endif
