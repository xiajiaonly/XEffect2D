#ifndef _JIA_XMUTITEXT_
#define _JIA_XMUTITEXT_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//这是一个用于显示多行文本的类，本来有计划计入编辑功能，不过太复杂就没有做这样的尝试了

#include "XEdit.h"
#include "XSlider.h"

typedef _XEditTexture _XMultiTextTexture;

//单行的字符串
struct _XMultiTextLineString
{
	char isEnable;				//这一行是否要显示，是否有效
	char *showString;			//这一行文字中需要显示的字符串
	int showStart;				//这行字符串显示部分的头在当前行的偏移
	int showLength;				//显示字符串的长度
	_XFontUnicode nowText;		//用于显示这一行字符串的字体
	int lineHeadorder;			//这一行字符在全文中的头位置
	int lineEndOrder;			//这一行字符在全文中的尾位置
	int lineLength;				//这一行字符串的长度
	char isEnterEnd;			//是否因为换行而换行显示1，或者是因为显示字符串的长度限制而换行0
	_XSprite spriteSelect;		//字符串显示选择背景颜色的精灵
	char haveAllSelect;			//该行中是否存被整体选择
	_XMultiTextLineString()
		:showString(NULL)
	{
	}
};

#define MAX_LINE_STRING_LENGTH 256	//单行可以显示的字符串的最大长度
#define MAX_LINE_SUM 256				//最多可以输入多少行字符串
#define MAX_STRING_LENGTH 65536		//允许输入的最大字符串的长度

class _XMultiText:public _XControlBasic
{
private:
	char m_isInited;				//是否初始化

	char m_needShowVSlider;			//是否需要显示垂直滑动条
	_XSlider m_verticalSlider;		//垂直滑动条
	char m_needShowHSlider;			//是否需要显示水平滑动条
	_XSlider m_horizontalSlider;	//水平滑动条

	char *m_nowString;				//当前输入的字符串，整个

	const _XTextureData *m_mutiEditNormal;	//输入框普通状态
	const _XTextureData *m_mutiEditDisable;	//输入框无效状态
	const _XTextureData *m_mutiEditSelect;	//输入框片选颜色
	const _XTextureData *m_mutiEditUpon;	//输入框片选颜色

	_XSprite m_spriteBackGround;	//用于显示输入框的背景贴图
	//_XVector2 m_textPosition;			//文字显示的位置，是相对于控件的位置来定的
	_XVector2 m_textSize;				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	_XFColor m_textColor;			//文字的颜色
	float m_textWidth;	//字体的宽度
	float m_textHeight;	//字体的高度

	char m_haveSelect;				//是否在字符串中有选择
	int m_nowSelectLineOrder;		//当前文档中被选择的是哪一行

	_XMultiTextLineString *m_lineStr;
	int m_nowStartLine;			//当前显示的第一行是总的第几行
	int m_nowShowLineSum;		//当前可以显示多少行
	int m_nowShowLineWidth;		//当前一行可以显示的宽度
	int m_nowMaxLineSum;		//当前字符串中实际的行数
	int m_nowMaxLineLength;		//当前字符串中实际的行宽
	void updateLineStr();		//更新当前输入的文本到每一行中

//	void (*m_funInputChenge)(void *,int);		//输入内容发生改变的时候调用
//	void (*m_funInputOver)(void *,int);		//确认输入结束之后的时候调用
//	void *m_pClass;

	char m_timer;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式

	friend void funMutiTextValueChangeV(void *pClass,int objectID);
	friend void funMutiTextValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiTextValueChangeH(void *pClass,int objectID);
	friend void funMutiTextValueChangeMoveH(void *pClass,int objectID);

	void setStartLine(int temp);	//设置窗口显示的头行的行号
	void checkStr();							//检查字符串的单行宽度是否大于限制,这个函数主要是用于将字符串按照默认的宽度裁制成需要的样式,以便于以后的字符串操作

public:
	int init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,	//控件的范围
		const _XMultiTextTexture& tex,	//控件的贴图
		const char *str,const _XFontUnicode& font, float strSize,		//控件的字体及相关信息
		//const _XVector2& strPosition,
		//const _XMouseRightButtonMenu &mouseMenu,
		const _XSlider &vSlider,const _XSlider &hSlider);	//控件的其他附属依赖控件
protected:
	void draw();
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);					//对于鼠标动作的响应函数
	int keyboardProc(int keyOrder,_XKeyState keyState);							//返回是否出发按键动作
	int canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	int canLostFocus(float x,float y);
public:
	void setPosition(const _XVector2& position);	//改变空间的位置
	void setPosition(float x,float y);	//改变空间的位置
	_XVector2 getPosition() const {return m_position;}
	void setSize(const _XVector2& size);			//改变控件的尺寸
	void setSize(float x,float y);		//改变控件的尺寸
	_XVector2 getSize() const {return m_size;}
	void setTextColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_textColor = color;
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			m_lineStr[i].nowText.setColor(m_textColor);
		}
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_spriteBackGround.setColor(m_color);
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			m_lineStr[i].nowText.setColor(m_textColor * m_color);
		}
		m_verticalSlider.setColor(m_color);
		m_horizontalSlider.setColor(m_color);
	}	//设置按钮的颜色
	_XFColor getColor() const {return m_color;}	//获取控件的颜色

	void setString(const char *str);			//改变显示的字符串
	void addString(const char *str);			//向字符串中添加字符串
	void addALine(const char *str);			//向字符串中添加字符串

	int setACopy(const _XMultiText &temp);
	
	int exportData(const char *fileName = NULL);		//数据导出
	int importData(const char *fileName = NULL);		//数据导入
	int deleteSelectLine();						//删除当前选择的一行
	int moveUpSelectLine();						//将选择的一行上移
	int moveUpLine(int order);					//将某一行上移
	int moveDownSelectLine();					//将选择的一行下移
	int moveDownLine(int order);				//将某一行下移
	int insertALine(const char *str,int lineOrder);	//向文本的指定行插入一行

	_XMultiText();
	~_XMultiText();
	//下面是内联函数
	void enable();		//使其有效
	void disable();		//使其无效
	void release();
	int getSelectLine() const;			//返回当前鼠标选择的是哪一行，-1位没有选择
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	int isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XMultiText(const _XMultiText &temp);
	_XMultiText& operator = (const _XMultiText& temp);
};

inline void _XMultiText::enable()		//使其有效
{
	m_isEnable = 1;
	m_verticalSlider.enable();
	m_horizontalSlider.enable();
}

inline void _XMultiText::disable()		//使其无效
{
	m_isEnable = 0;
	m_verticalSlider.disable();
	m_horizontalSlider.disable();
}

inline int _XMultiText::getSelectLine() const			//返回当前鼠标选择的是哪一行，-1位没有选择
{
	if(m_isInited == 0) return -1;
	if(m_haveSelect == 0) return -1;
	else return m_nowSelectLineOrder;
}

inline void _XMultiText::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

inline void _XMultiText::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}
#endif