#ifndef _JIA_XFONTBASIC_
#define _JIA_XFONTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.1
//--------------------------------
#include "XSprite.h"

//这个是字体的基础类，其他的字体类从这个类继承而来

enum _XFontAlignmentModeX
{
	FONT_ALIGNMENT_MODE_X_LEFT,	//左侧对齐
	FONT_ALIGNMENT_MODE_X_MIDDLE,	//居中对齐
	FONT_ALIGNMENT_MODE_X_RIGHT,	//右侧对齐
};
enum _XFontAlignmentModeY
{
	FONT_ALIGNMENT_MODE_Y_UP,		//上对齐
	FONT_ALIGNMENT_MODE_Y_MIDDLE,	//居中对齐
	FONT_ALIGNMENT_MODE_Y_DOWN,	//下对齐
};

#define STRING_MAX_SIZE (64)	//默认的最大字符串长度

class _XFontBasic:public _XObjectBasic
{
//private:
protected:
	_XBool m_isInited;		//是否已经初始化
	_XResourcePosition m_resoursePosition;	//资源位置 0:外部 1:内部
	_XSprite m_sprite;		//贴图
	char *m_string;			//需要显示的字符串
	int m_lineSum;			//显示字符串的行数
	int m_needShowTextSum;		//需要显示的字符的数量

	_XBool m_needUpdateData;	//是否需要更新内部数据

	int m_maxStringLen;		//支持的最大字符串长度

	_XFontAlignmentModeX m_alignmentModeX;	//字体的水平对齐方式
	_XFontAlignmentModeY m_alignmentModeY;	//字体的垂直对齐方式

	//为了提速这里定义相关FBO的东西
	_XBool m_withFBO;		//是否使用FBO
	_XBool m_needUpdateFBO;	//是否需要更新FBO
	_XFBO *m_pFbo;			//使用的FBO
	_XSprite m_fboSprite;	//贴图
public:
	using _XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a)
	{
		m_sprite.setColor(r,g,b,a);
		updateChildColor();
	}
	void setAlpha(float a) 
	{
		m_sprite.setAlpha(a);
		updateChildAlpha();
	}
	_XFColor getColor() const {return m_sprite.getColor();}
protected:
	_XBool m_isVisible;
public:
	void setVisible() 
	{
		m_isVisible = XTrue;
		updateChildVisible();
	}					//设置物件可见
	void disVisible() 
	{
		m_isVisible = XFalse;
		updateChildVisible();
	}					//设置物件不可见
	_XBool getVisible() const {return m_isVisible;}			//获取物件是否可见的状态 
protected:
	//+++++++++++++++++++++++++++
	//给字体增加裁减功能
	_XRect m_clipRect;	//字体裁减的矩型是相对于字体坐上角的一个相对矩形
	_XBool m_isCliped;
public:
	void setClipRect(const _XRect &temp){setClipRect(temp.left,temp.top,temp.right,temp.bottom);}
	void setClipRect(float left,float top,float right,float bottom)
	{
		m_clipRect.set(left,top,right,bottom);
		m_isCliped = XTrue;
		m_needUpdateData = XTrue;
	}
	void disClip()
	{
		if(m_isCliped) 
		{
			m_isCliped = XFalse;
			m_needUpdateData = XTrue;
		}
	}
	//---------------------------

	void setAlignmentModeX(_XFontAlignmentModeX mode)
	{
		if(mode == m_alignmentModeX) return;
		m_alignmentModeX = mode;
		m_needUpdateData = XTrue;
	}
	void setAlignmentModeY(_XFontAlignmentModeY mode)
	{
		if(mode == m_alignmentModeY) return;
		m_alignmentModeY = mode;
		m_needUpdateData = XTrue;
	}
protected:
	float m_angle;			//字体的角度
	float m_angleSin;		//字体角度的正弦	引入这两个变量是为了节省计算时间
	float m_angleCos;		//字体角度的余弦

	float m_distance;		//字符之间的距离	
	float m_distanceY;		//行之间的距离

	_XVector2 m_position;		//字体的位置
	_XVector2 m_setPosition;	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	_XVector2 m_rotateBasicPoint;	//字体旋转的中心点

	_XVector2 m_size;		//文字的尺寸（像素大小）
	_XVector2 m_showSize;	//显示的字体的缩放大小
public:

	void setDistance(float distance);			//设置字与字之间的距离
	void setDistance(float x,float y);			//设置字与字之间的距离
	void setAngle(float angle);	//设置字体显示的角度
	float getAngle() const {return m_angle;}
	virtual const char * getString() const {return m_string;}

	using _XObjectBasic::setSize;
	void setSize(float x,float y);			//设置字体的显示大小
	using _XObjectBasic::setPosition;
	void setPosition(float x,float y);			//设置字体显示的位置
	void setRotateBasePoint(float x,float y);

	_XVector2 getTextSize() const {return m_size;}
	_XVector2 getSize() const {return m_showSize;}
	_XVector2 getPosition() const {	return _XVector2(m_position.x,m_position.y);}
protected:
	//不裁剪的前提下的像素范围
	int m_maxPixelWidth;	//字体的最大像素宽度
	int m_maxPixelHeight;	//字体的最大像素高度

	virtual void updateData() = 0;	//这是一个纯虚函数，其实现参考其派生类
public:
	int getMaxPixelWidth()
	{
		if(m_needUpdateData)
		{
			updateData();
			m_needUpdateData = XFalse;
		}
		return m_maxPixelWidth;
	}
	int getMaxPixelHeight()
	{
		if(m_needUpdateData)
		{
			updateData();
			m_needUpdateData = XFalse;
		}
		return m_maxPixelHeight;
	}

public:
	//重载赋值操作符
	_XFontBasic& operator = (const _XFontBasic& temp);
	_XBool setACopy(const _XFontBasic &temp);
protected:
	_XBool m_isPassword;
public:
	void setIsPassWord(_XBool temp)
	{
		if((m_isPassword && !temp) || (!m_isPassword && temp))
		{
			m_isPassword = temp;
			m_needUpdateData = XTrue;
		}
	}
	_XBool getIsPassword() const {return m_isPassword;}
protected:
	_XFontBasic(int maxStrLen)
		:m_needUpdateData(XFalse)
		,m_alignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT)
		,m_alignmentModeY(FONT_ALIGNMENT_MODE_Y_UP)
		,m_distance(0.0f)
		,m_distanceY(0.0f)
		,m_angle(0.0f)
		,m_angleSin(0.0f)
		,m_angleCos(1.0f)
		,m_position(0.0f,0.0f)
		,m_setPosition(0.0f,0.0f)
		,m_rotateBasicPoint(0.0f,0.0f)
		,m_showSize(1.0f,1.0f)
		,m_isInited(XFalse)
		,m_maxPixelWidth(0)
		,m_maxPixelHeight(0)
		,m_lineSum(0)
		,m_needShowTextSum(0)
		,m_isVisible(XTrue)
		,m_isCliped(XFalse)
		,m_clipRect(0,0,0,0)
		,m_isPassword(XFalse)
		,m_withFBO(XFalse)
		//,m_withFBO(XTrue)
		,m_needUpdateFBO(XFalse)
		,m_pFbo(NULL)
	{
		if(maxStrLen < 2) m_maxStringLen = 2;
		else m_maxStringLen = maxStrLen;
		m_string = createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
	}
	_XFontBasic()
		:m_needUpdateData(XFalse)
		,m_alignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT)
		,m_alignmentModeY(FONT_ALIGNMENT_MODE_Y_UP)
		,m_distance(0.0f)
		,m_distanceY(0.0f)
		,m_angle(0.0f)
		,m_angleSin(0.0f)
		,m_angleCos(1.0f)
		,m_position(0.0f,0.0f)
		,m_setPosition(0.0f,0.0f)
		,m_rotateBasicPoint(0.0f,0.0f)
		,m_showSize(1.0f,1.0f)
		,m_isInited(XFalse)
		,m_maxPixelWidth(0)
		,m_maxPixelHeight(0)
		,m_lineSum(0)
		,m_needShowTextSum(0)
		,m_isVisible(XTrue)
		,m_isCliped(XFalse)
		,m_clipRect(0,0,0,0)
		,m_isPassword(XFalse)
		,m_withFBO(XFalse)
		//,m_withFBO(XTrue)
		,m_needUpdateFBO(XFalse)
		,m_pFbo(NULL)
	{
		m_maxStringLen = STRING_MAX_SIZE;
		m_string = createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
	}
	//重载拷贝构造函数
	_XFontBasic(const _XFontBasic& temp);
public:
	_XBool setString(const char *p);	//设置显示的字符串

	virtual ~_XFontBasic()	//定义成虚函数是为了防止通过基类指针删除派生类对象时造成的内存泄露
	{
		XDELETE_ARRAY(m_string);
		if(m_pFbo != NULL) XDELETE(m_pFbo);
	}
	virtual void draw() = 0;
	virtual _XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse) = 0;	
	_XBool isInRect(float x,float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);	//获取四个顶点的坐标，目前先不考虑旋转和缩放
	virtual void setMaxStrLen(int maxStrLen) = 0;

	//virtual void justForTest() {;}
};
inline _XBool _XFontBasic::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XBool _XFontBasic::setString(const char *p)	//设置显示的字符串
{
	if(p == NULL) return XFalse;
	if(strcmp(p,m_string) == 0) return XTrue;	//如果是相同的字符串则不设置
	if(strlen(p) < m_maxStringLen)
	{
		strcpy(m_string,p);
	}else
	{
		memcpy(m_string,p,m_maxStringLen);
		if(isAtUnicodeEnd(m_string,m_maxStringLen - 3))
		{
			m_string[m_maxStringLen - 3] = '.';
			m_string[m_maxStringLen - 2] = '.';
			m_string[m_maxStringLen - 1] = '\0';
		}else
		{
			m_string[m_maxStringLen - 4] = '.';
			m_string[m_maxStringLen - 3] = '.';
			m_string[m_maxStringLen - 2] = '\0';
		}
		//strlen(p) >= m_maxStringLen
	}
	m_needUpdateData = XTrue;
	return XTrue;
}
inline void _XFontBasic::setPosition(float x,float y)			//设置字体显示的位置
{
	m_setPosition.set(x,y);
	updateChildPos();
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setDistance(float distance)			//设置字与字之间的距离
{
	m_distance = distance;
	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setDistance(float x,float y)
{
	m_distance = x;
	m_distanceY = y;
	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setAngle(float angle)	//设置字体显示的角度
{
	m_angle = angle;
	updateChildAngle();
	m_sprite.setAngle(angle);
	m_angleSin = sin(angle * DEGREE2RADIAN);
	m_angleCos = cos(angle * DEGREE2RADIAN);
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setSize(float x,float y)	//设置字体的显示大小
{
	m_showSize.set(x,y);
	updateChildSize();
	m_sprite.setSize(x,y);
	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}
#endif