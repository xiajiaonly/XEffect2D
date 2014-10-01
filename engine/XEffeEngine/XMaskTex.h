#ifndef _JIA_XMASKTEX_
#define _JIA_XMASKTEX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.22
//--------------------------------
//这是一个做像素遮罩的类
#include "XBasicClass.h"

class _XMaskTex
{
private:
	_XBool m_isInited;
	_XVector2 m_sourcePixelSize;	//原始的像素大小
	_XVector2 m_nowPixelSize;		//现在的像素大小
	unsigned char * m_pixelData;		//像素数据，这个像素数据一旦生成就不能修改

	_XVector2 m_position;
	_XVector2 m_size;
	_XRect m_rect;	//矩形盒子，用于做初步判断
public:
	_XBool init(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);

	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		m_rect.set(m_position.x,m_position.y,
			m_position.x + m_sourcePixelSize.x * m_size.x,m_position.y + m_sourcePixelSize.y * m_size.y);
	}
	void setPosition(const _XVector2 &pos)
	{
		m_position = pos;
		m_rect.set(m_position.x,m_position.y
			,m_position.x + m_sourcePixelSize.x * m_size.x,m_position.y + m_sourcePixelSize.y * m_size.y);
	}
	void setSize(float x,float y)
	{
		m_size.set(x,y);
		m_rect.set(m_position.x,m_position.y,
			m_position.x + m_sourcePixelSize.x * m_size.x,m_position.y + m_sourcePixelSize.y * m_size.y);
	}
	void setSize(const _XVector2 &size)
	{
		m_size = size;
		m_rect.set(m_position.x,m_position.y,
			m_position.x + m_sourcePixelSize.x * m_size.x,m_position.y + m_sourcePixelSize.y * m_size.y);
	}
	_XBool getIsMask(float x,float y) const;
	_XBool getIsMask(const _XVector2 &pos) const
	{
		getIsMask(pos.x,pos.y);
	}
	//void draw();	//用于测试

	_XMaskTex()
		:m_isInited(XFalse)
		,m_pixelData(NULL)
		,m_position(0.0f,0.0f)
		,m_size(0.0f,0.0f)
	{}
	~_XMaskTex(){release();}
	void release()
	{
		if(!m_isInited) return;
		XDELETE_ARRAY(m_pixelData);

		m_isInited = XFalse;
	}
};

#endif	//_JIA_XMASKTEX_