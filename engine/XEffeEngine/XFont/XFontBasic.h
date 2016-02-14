#ifndef _JIA_XFONTBASIC_
#define _JIA_XFONTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.1
//--------------------------------
#include "XSprite.h"
namespace XE{
class XFBO;
//这个是字体的基础类，其他的字体类从这个类继承而来
 enum XFontAlignmentModeX
{
	FONT_ALIGNMENT_MODE_X_LEFT,	//左侧对齐
	FONT_ALIGNMENT_MODE_X_MIDDLE,	//居中对齐
	FONT_ALIGNMENT_MODE_X_RIGHT,	//右侧对齐
};
enum XFontAlignmentModeY
{
	FONT_ALIGNMENT_MODE_Y_UP,		//上对齐
	FONT_ALIGNMENT_MODE_Y_MIDDLE,	//居中对齐
	FONT_ALIGNMENT_MODE_Y_DOWN,	//下对齐
};

class XFontBasic:public XObjectBasic
{
private:
	static const int m_stringMaxLength = 64;	//默认的最大字符串长度(这里需要优化掉)
protected:
	XBool m_isInited;		//是否已经初始化
	XResourcePosition m_resoursePosition;	//资源位置 0:外部 1:内部
	XSprite m_sprite;		//贴图
	char *m_string;			//需要显示的字符串
	int m_lineSum;			//显示字符串的行数
	int m_needShowTextSum;		//需要显示的字符的数量

	XBool m_needUpdateData;	//是否需要更新内部数据

	int m_maxStringLen;		//支持的最大字符串长度

	XFontAlignmentModeX m_alignmentModeX;	//字体的水平对齐方式
	XFontAlignmentModeY m_alignmentModeY;	//字体的垂直对齐方式

	//为了提速这里定义相关FBO的东西
	XBool m_withFBO;		//是否使用FBO
	XBool m_needUpdateFBO;	//是否需要更新FBO
	XFBO *m_pFbo;			//使用的FBO
	XSprite m_fboSprite;	//贴图
public:
	using XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	XFColor getColor() const {return m_sprite.getColor();}
protected:
	XBool m_isVisible;
public:
	void setVisible(); //设置物件可见
	void disVisible(); //设置物件不可见
	XBool getVisible() const {return m_isVisible;}			//获取物件是否可见的状态 
protected:
	//+++++++++++++++++++++++++++
	//给字体增加裁减功能
	XRect m_clipRect;	//字体裁减的矩型是相对于字体坐上角的一个相对矩形
	XBool m_isCliped;
public:
	void setClipRect(const XRect &temp){setClipRect(temp.left,temp.top,temp.right,temp.bottom);}
	void setClipRect(float left,float top,float right,float bottom);
	void disClip();
	//---------------------------

	void setAlignmentModeX(XFontAlignmentModeX mode);
	void setAlignmentModeY(XFontAlignmentModeY mode);
protected:
	float m_angle;			//字体的角度
	float m_angleSin;		//字体角度的正弦	引入这两个变量是为了节省计算时间
	float m_angleCos;		//字体角度的余弦

	float m_distance;		//字符之间的距离	
	float m_distanceY;		//行之间的距离

	XVector2 m_position;		//字体的位置
	XVector2 m_setPosition;	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	XVector2 m_rotateBasicPoint;	//字体旋转的中心点

	XVector2 m_size;		//文字的尺寸（像素大小）
	XVector2 m_scale;	//显示的字体的缩放大小
public:

	void setDistance(float distance);			//设置字与字之间的距离
	void setDistance(float x,float y);			//设置字与字之间的距离
	void setAngle(float angle);	//设置字体显示的角度
	float getAngle() const {return m_angle;}
	virtual const char * getString() const {return m_string;}

	using XObjectBasic::setScale;
	void setScale(float x,float y);			//设置字体的显示大小
	using XObjectBasic::setPosition;
	void setPosition(float x,float y);			//设置字体显示的位置
	void setRotateBasePoint(float x,float y);

	XVector2 getTextSize() const {return m_size;}
	XVector2 getScale() const {return m_scale;}
	XVector2 getPosition() const {	return XVector2(m_position.x,m_position.y);}
protected:
	//不裁剪的前提下的像素范围
	int m_maxPixelWidth;	//字体的最大像素宽度
	int m_maxPixelHeight;	//字体的最大像素高度

	virtual void updateData() = 0;	//这是一个纯虚函数，其实现参考其派生类
public:
	int getMaxPixelWidth();
	int getMaxPixelHeight();

public:
	//重载赋值操作符
	XFontBasic& operator = (const XFontBasic& temp);
	XBool setACopy(const XFontBasic &temp);
protected:
	XBool m_isPassword;
public:
	void setIsPassWord(XBool temp);
	XBool getIsPassword() const {return m_isPassword;}
protected:
	XFontBasic(int maxStrLen)
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
		,m_scale(1.0f,1.0f)
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
		m_string = XMem::createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
	}
	XFontBasic()
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
		,m_scale(1.0f,1.0f)
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
		m_maxStringLen = m_stringMaxLength;
		m_string = XMem::createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
	}
	//重载拷贝构造函数
	XFontBasic(const XFontBasic& temp);
public:
	XBool setString(const char *p);	//设置显示的字符串

	virtual ~XFontBasic();	//定义成虚函数是为了防止通过基类指针删除派生类对象时造成的内存泄露
	virtual void draw() = 0;
	virtual XBool initEx(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,XBool withFBO = XFalse) = 0;	
	XBool isInRect(float x,float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);	//获取四个顶点的坐标，目前先不考虑旋转和缩放
	virtual void setMaxStrLen(int maxStrLen) = 0;

	//virtual void justForTest() {;}
};
#if WITH_INLINE_FILE
#include "XFontBasic.inl"
#endif
}
#endif