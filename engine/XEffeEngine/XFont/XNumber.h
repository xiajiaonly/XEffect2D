#ifndef _JIA_XNUMBER_
#define _JIA_XNUMBER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XSprite.h"
namespace XE{
#ifndef TEXT_MODE
#define TEXT_MODE "0123456789+-*/%."
#endif

#ifndef MAX_NUMBER_LENGTH
#define MAX_NUMBER_LENGTH (256)
#endif

//数字的对齐方式
enum XNumberAlignmentMode
{
	NUMBER_ALIGNMENT_MODE_LEFT,		//左侧对齐
	NUMBER_ALIGNMENT_MODE_MIDDLE,	//居中对齐
	NUMBER_ALIGNMENT_MODE_RIGHT,	//右侧对齐
};
//24568
//+1258
//-1258
//-1258%
//-1.25%
//+-%.0123456789
class XNumber:public XObjectBasic
{
private:
	XResourcePosition m_resoursePosition;	//资源位置 0:外部 1:内部

	XSprite m_sprite;					//贴图
	XBool m_isInited;
	XBool m_isVisible;
	XBool m_needUpdateData;	//是否需要更新内部数据

	float m_angle;			//字体的角度
	float m_angleSin;	//角度的sin值
	float m_angleCos;

	XNumberAlignmentMode m_alignmentMode;
public:
	void setAlignmentMode(XNumberAlignmentMode mode);
	void setVisible();					//设置物件可见
	void disVisible();						//设置物件不可见
	XBool getVisible() const {return m_isVisible;}					//获取物件是否可见的状态 
private:
	char *m_string;

	//不裁剪的前提下的像素范围
	int m_maxPixelWidth;	//字体的最大像素宽度
	int m_maxPixelHeight;	//字体的最大像素高度
public:
	int getMaxPixelWidth();
	int getMaxPixelHeight();
private:
	void updateData();		//根据需要更新内部数据
	XVector2 *m_textPosition;	//用于保存每个字符的位置
	XRect *m_textRect;		//用于保存每个字符的剪切盒子
	int m_needShowTextSum;	//需要显示的字符的数量

	XVector2 m_position;	//字体的位置，这个是字体放置的位置
	XVector2 m_setPosition;	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	float m_alpha;			//透明通道
	float m_distance;		//文字之间的距离	
	XVector2 m_size;		//文字的尺寸（像素大小）
	XVector2 m_layout;		//字体图片的布局
	XVector2 m_scale;
	XVector2 m_rotateBasicPoint;	//字体旋转的中心点

	bool loadFromFolder(const char *filename);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源
public:
	XBool init(const char *fileName,	//字体图片的名字
		const XVector2 &size,			//字体的像素大小
		const XVector2 &layout,			//字体图片的布局
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		
	XBool initEx(const char *fileName,	//字体图片的名字
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	XBool initFromTTF(const char *filename,	//ttf字体库的名称
		int fontSize,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//从TTF文件中读取资源(尚未实现)

	void draw();

	using XObjectBasic::setScale;
	void setScale(float x,float y);	//设置字体的显示大小
	XVector2 getScale() const {return m_scale;}
	void setAngle(float angle);	//设置字体显示的角度
	float getAngle() const {return m_angle;}
	
	using XObjectBasic::setPosition;
	void setPosition(float x,float y);	//设置字体显示的位置
	void setPositionX(float x);
	void setPositionY(float y);
	XVector2 getPosition() const {return m_setPosition;}
	void setRotateBasePoint(float x,float y);

	XBool setNumber(int temp);								//设置整形数
	XBool setNumber(float temp,int decimalPartLength);		//设置带小数点的数
	XBool setNumber(const char *temp);		
	//设置一段合理的字符串
	XNumber& operator = (const XNumber& temp);
	XBool setACopy(const XNumber &temp);

	using XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);	//设置颜色
	XFColor getColor() const {return m_sprite.getColor();}
	void setAlpha(float a);
	float getAlpha() const {return m_alpha;}
	XBool release();

	void setDistance(float distance) {m_distance = distance;}
	float getDistance() const {return m_distance;}
	//为了实现物件管理，下面增加几个接口
	XBool isInRect(float x,float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);	//获取四个顶点的坐标，目前先不考虑旋转和缩放

	XNumber();
	~XNumber();

	//virtual void justForTest() {;}
};
#if WITH_INLINE_FILE
#include "XNumber.inl"
#endif
}
#endif
