#ifndef _JIA_XFONTBASIC_
#define _JIA_XFONTBASIC_

#include "XSprite.h"

//这个是字体的基础类，其他的字体类从这个类继承而来

enum _XFontAlignmentModeX
{
	_FONT_ALIGNMENT_MODE_X_LEFT,	//左侧对齐
	_FONT_ALIGNMENT_MODE_X_MIDDLE,	//居中对齐
	_FONT_ALIGNMENT_MODE_X_RIGHT,	//右侧对齐
};
enum _XFontAlignmentModeY
{
	_FONT_ALIGNMENT_MODE_Y_UP,		//上对齐
	_FONT_ALIGNMENT_MODE_Y_MIDDLE,	//居中对齐
	_FONT_ALIGNMENT_MODE_Y_DOWN,	//下对齐
};

#define STRING_MAX_SIZE 2048	//字符串的最大长度

class _XFontBasic:public _XObjectBasic
{
//private:
protected:
	char m_isInited;		//是否已经初始化
	int m_resoursePosition;	//资源位置 0:外部 1:内部
	_XSprite m_sprite;		//贴图
	char *m_string;			//需要显示的字符串
	int m_lineSum;			//显示字符串的行数
	int m_needShowTextSum;		//需要显示的字符的数量

	char m_needUpdateData;	//是否需要更新内部数据

	_XFontAlignmentModeX m_alignmentModeX;	//字体的水平对齐方式
	_XFontAlignmentModeY m_alignmentModeY;	//字体的垂直对齐方式
public:
	void setAlignmentModeX(_XFontAlignmentModeX mode)
	{
		if(mode != m_alignmentModeX)
		{
			m_alignmentModeX = mode;
			m_needUpdateData = 1;
		}
	}
	void setAlignmentModeY(_XFontAlignmentModeY mode)
	{
		if(mode != m_alignmentModeY)
		{
			m_alignmentModeY = mode;
			m_needUpdateData = 1;
		}
	}
protected:
	float m_angle;			//字体的角度
	float m_angleSin;		//字体角度的正弦	引入这两个变量是为了节省计算时间
	float m_angleCos;		//字体角度的余弦

	float m_distance;		//字符之间的距离	
	float m_distanceY;		//行之间的距离

	_XVector2I m_position;		//字体的位置
	_XVector2I m_setPosition;	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	_XVector2 m_rotateBasicPoint;	//字体旋转的中心点

	_XVector2I m_size;		//文字的尺寸（像素大小）
	_XVector2 m_showSize;	//显示的字体的缩放大小
public:

	void setDistance(float distance);			//设置字与字之间的距离
	void setDistance(float x,float y);			//设置字与字之间的距离
	void setAngle(float angle);	//设置字体显示的角度
	float getAngle() const
	{
		return m_angle;
	}

	void setSize(const _XVector2& size);	//设置字体的显示大小
	void setSize(float x,float y);	//设置字体的显示大小
	void setPosition(const _XVector2& position);	//设置字体显示的位置
	//void setPosition(int x,int y);			//设置字体显示的位置
	void setPosition(float x,float y);			//设置字体显示的位置
	void setRotateBasePoint(float x,float y);

	_XVector2I getTextSize() const
	{
		return m_size;
	}
//	_XVector2 getShowSize() const
//	{
//		return m_showSize;
//	}
	_XVector2 getSize() const
	{
		return m_showSize;
	}
//	_XVector2I getPosition() const
//	{
//		return m_position;
//	}
	_XVector2 getPosition() const
	{
		return _XVector2(m_position.x,m_position.y);
	}
protected:
	//不裁剪的前提下的像素范围
	int m_maxPixelWidth;	//字体的最大像素宽度
	int m_maxPixelHeight;	//字体的最大像素高度

	virtual void updateData() = 0;	//这是一个纯虚函数，其实现参考其派生类
public:
	int getMaxPixelWidth()
	{
		if(m_needUpdateData != 0)
		{
			updateData();
			m_needUpdateData = 0;
		}
		return m_maxPixelWidth;
	}
	int getMaxPixelHeight()
	{
		if(m_needUpdateData != 0)
		{
			updateData();
			m_needUpdateData = 0;
		}
		return m_maxPixelHeight;
	}

public:
	//重载赋值操作符
	_XFontBasic& operator = (const _XFontBasic& temp);
	int setACopy(const _XFontBasic &temp);

protected:
	_XFontBasic()
		:m_needUpdateData(0)
		,m_alignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT)
		,m_alignmentModeY(_FONT_ALIGNMENT_MODE_Y_UP)
		,m_distance(0.0f)
		,m_distanceY(0.0f)
		,m_angle(0.0f)
		,m_angleSin(0.0f)
		,m_angleCos(1.0f)
		,m_position(0.0f,0.0f)
		,m_setPosition(0.0f,0.0f)
		,m_rotateBasicPoint(0.0f,0.0f)
		,m_showSize(1.0f,1.0f)
		,m_isInited(0)
		,m_maxPixelWidth(0)
		,m_maxPixelHeight(0)
		,m_lineSum(0)
		,m_needShowTextSum(0)
	{
		m_string = new char[STRING_MAX_SIZE];
		m_string[0] = '\0';
	}
	//重载拷贝构造函数
	_XFontBasic(const _XFontBasic& temp);
public:
	int setString(const char *p);	//设置显示的字符串

	virtual ~_XFontBasic()	//定义成虚函数是为了防止通过基类指针删除派生类对象时造成的内存泄露
	{
		XDELETE_ARRAY(m_string);
	}
	
	int isInRect(float x,float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);	//获取四个顶点的坐标，目前先不考虑旋转和缩放
};

inline int _XFontBasic::setString(const char *p)	//设置显示的字符串
{
	if(p == NULL) return 0;
	if(strlen(p) >= STRING_MAX_SIZE) return 0;
	strcpy(m_string,p);
	m_needUpdateData = 1;
	return 1;
}

inline void _XFontBasic::setPosition(const _XVector2& position)	//设置字体显示的位置
{
	setPosition(position.x,position.y);
}

inline void _XFontBasic::setPosition(float x,float y)			//设置字体显示的位置
{
	m_setPosition.set(x,y);
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = 1;
}
inline void _XFontBasic::setDistance(float distance)			//设置字与字之间的距离
{
	m_distance = distance;
	m_needUpdateData = 1;
}

inline void _XFontBasic::setDistance(float x,float y)
{
	m_distance = x;
	m_distanceY = y;
	m_needUpdateData = 1;
}

inline void _XFontBasic::setAngle(float angle)	//设置字体显示的角度
{
	m_angle = angle;
	m_sprite.setAngle(angle);
	m_angleSin = sin(angle * ANGLE_TO_RADIAN);
	m_angleCos = cos(angle * ANGLE_TO_RADIAN);
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = 1;
}

inline void _XFontBasic::setSize(const _XVector2& size)	//设置字体的显示大小
{
	setSize(size.x,size.y);
}

inline void _XFontBasic::setSize(float x,float y)	//设置字体的显示大小
{
	m_showSize.set(x,y);
	m_sprite.setSize(x,y);
	m_needUpdateData = 1;
}

inline void _XFontBasic::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}


#endif