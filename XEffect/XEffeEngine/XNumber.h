#ifndef _JIA_XNUMBER_
#define _JIA_XNUMBER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XSprite.h"

#ifndef TEXT_MODE
#define TEXT_MODE ("0123456789+-*/%.")
#endif

#ifndef MAX_NUMBER_LENGTH
#define MAX_NUMBER_LENGTH (256)
#endif

//数字的对齐方式
enum _XNumberAlignmentMode
{
	_NUMBER_ALIGNMENT_MODE_LEFT,	//左侧对齐
	_NUMBER_ALIGNMENT_MODE_MIDDLE,	//居中对齐
	_NUMBER_ALIGNMENT_MODE_RIGHT,	//右侧对齐
};
//24568
//+1258
//-1258
//-1258%
//-1.25%
//+-%.0123456789
class _XNumber:public _XObjectBasic
{
private:
	int m_resoursePosition;	//资源位置 0:外部 1:内部

	_XSprite m_sprite;					//贴图
	int m_isInited;
	char m_isVisiable;

	float m_angle;			//字体的角度
	float m_angleSin;	//角度的sin值
	float m_angleCos;

	_XNumberAlignmentMode m_alignmentMode;
public:
	void setAlignmentMode(_XNumberAlignmentMode mode)
	{
		if(mode != m_alignmentMode)
		{
			m_alignmentMode = mode;
			m_needUpdateData = 1;
		}
	}
	void setVisiable() {m_isVisiable = 1;}					//设置物件可见
	void disVisiable() {m_isVisiable = 0;}						//设置物件不可见
	char getVisiable() const {return m_isVisiable;}					//获取物件是否可见的状态 
private:
	char *m_string;

	//不裁剪的前提下的像素范围
	int m_maxPixelWidth;	//字体的最大像素宽度
	int m_maxPixelHeight;	//字体的最大像素高度
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
private:
	char m_needUpdateData;	//是否需要更新内部数据
	void updateData();		//根据需要更新内部数据
	_XVector2 *m_textPosition;	//用于保存每个字符的位置
	_XRect *m_textRect;		//用于保存每个字符的剪切盒子
	int m_needShowTextSum;	//需要显示的字符的数量

	_XVector2 m_position;	//字体的位置，这个是字体放置的位置
	_XVector2 m_setPosition;	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	float m_alpha;			//透明通道
	float m_distance;		//文字之间的距离	
	_XVector2I m_size;		//文字的尺寸（像素大小）
	_XVector2I m_layout;		//字体图片的布局
	_XVector2 m_showSize;
	_XVector2 m_rotateBasicPoint;	//字体旋转的中心点
public:
	int init(const char *fileName,	//字体图片的名字
		_XVector2I size,			//字体的像素大小
		_XVector2I layout,		//字体图片的布局
		int resoursePosition = 0);		
	void draw();

	void setSize(const _XVector2& size);		//设置字体的显示大小
	void setSize(float x,float y);	//设置字体的显示大小
	_XVector2 getSize() const
	{
		return m_showSize;
	}
	void setAngle(float angle);	//设置字体显示的角度
	float getAngle() const
	{
		return m_angle;
	}
	void setPosition(const _XVector2& position);	//设置字体显示的位置
	void setPosition(float x,float y);	//设置字体显示的位置
	_XVector2 getPosition() const
	{
		return m_setPosition;
	}
	void setRotateBasePoint(float x,float y);

	int setNumber(int temp);								//设置整形数
	int setNumber(float temp,int decimalPartLength);		//设置带小数点的数
	int setNumber(const char *temp);		
	//设置一段合理的字符串
	_XNumber& operator = (const _XNumber& temp);
	int setACopy(const _XNumber &temp);

	void setColor(float r,float g,float b,float a);	//设置颜色
	void setColor(const _XFColor& color);
	void setAlpha(float a);
	float getAlpha() const
	{
		return m_alpha;
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
	//为了实现物件管理，下面增加几个接口
	int isInRect(float x,float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);	//获取四个顶点的坐标，目前先不考虑旋转和缩放

	_XNumber();
	~_XNumber();
};

#include "XNumber.inl"

#endif
