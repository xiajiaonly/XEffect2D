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
//slider目录名为:/SliderH和/SliderV
typedef _XEditTexture _XMultiTextTexture;

//单行的字符串
struct _XMultiTextLineString
{
	_XBool isEnable;				//这一行是否要显示，是否有效
	char *showString;			//这一行文字中需要显示的字符串
//	int showStart;				//这行字符串显示部分的头在当前行的偏移
//	int showLength;				//显示字符串的长度
	_XFontUnicode nowText;		//用于显示这一行字符串的字体
	int lineHeadorder;			//这一行字符在全文中的头位置
	int lineEndOrder;			//这一行字符在全文中的尾位置
	//int lineLength;				//这一行字符串的长度
	//char isEnterEnd;			//是否因为换行而换行显示1，或者是因为显示字符串的长度限制而换行0
	_XSprite spriteSelect;		//字符串显示选择背景颜色的精灵
	_XBool haveAllSelect;			//该行中是否存被整体选择

	int lineWidthPixel;			//行的像素宽度

	_XMultiTextLineString()
		:showString(NULL)
	{}
};

#define MAX_LINE_STRING_LENGTH (256)	//单行可以显示的字符串的最大长度
#define MAX_LINE_SUM (256)				//最多可以输入多少行字符串
#define MAX_STRING_LENGTH (65536)		//允许输入的最大字符串的长度
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (32)		//默认的滑动条宽度
#endif //DEFAULT_SLIDER_WIDTH
enum _XMultiTextActionType
{//多行列表的动作分类
	MLTTXT_ACTION_TYPE_IN,		//选项出现
	MLTTXT_ACTION_TYPE_MOVE,	//选项移动
	MLTTXT_ACTION_TYPE_DCLICK,	//双击
	MLTTXT_ACTION_TYPE_OUT,		//取消选择
};
class _XMultiText:public _XControlBasic
{
private:
	_XBool m_isInited;				//是否初始化

	_XBool m_needShowVSlider;			//是否需要显示垂直滑动条
	_XSlider m_verticalSlider;		//垂直滑动条
	_XBool m_needShowHSlider;			//是否需要显示水平滑动条
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
	float m_nowTextHeight;	//字体的高度

	_XBool m_haveSelect;				//是否在字符串中有选择
	int m_selectLineOrder;		//当前文档中被选择的是哪一行

	_XMultiTextLineString *m_lineStr;
	int m_showStartLine;			//当前显示的第一行是总的第几行
	int m_canShowLineSum;		//当前可以显示多少行
//	int m_nowShowLineWidth;		//当前一行可以显示的宽度
	int m_nowMaxLineSum;		//当前字符串中实际的行数
//	int m_nowMaxLineLength;		//当前字符串中实际的行宽
	void updateLineStr();		//更新当前输入的文本到每一行中

	//下面是为了实现像素移动而定义的新变量
	int m_nowStartPixels;			//开始的像素位置
	int m_nowMaxLineWidthPixels;	//当前最大的像素宽度
	int m_maxLineWidthPixels;		//总的最大像素宽度

//	void (*m_funInputChenge)(void *,int);		//输入内容发生改变的时候调用
//	void (*m_funInputOver)(void *,int);		//确认输入结束之后的时候调用
//	void *m_pClass;

	char m_timer;	//这是插入符号闪烁时使用的时间标记

	friend void funMutiTextValueChangeV(void *pClass,int objectID);
	friend void funMutiTextValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiTextValueChangeH(void *pClass,int objectID);
	friend void funMutiTextValueChangeMoveH(void *pClass,int objectID);

	void setStartLine(int temp);	//设置窗口显示的头行的行号
	void checkStr();							//检查字符串的单行宽度是否大于限制,这个函数主要是用于将字符串按照默认的宽度裁制成需要的样式,以便于以后的字符串操作

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式

	void (*m_funSelect)(void *,int ID);	//发生点选时调用
	void *m_pClass;
public:
	_XBool init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,	//控件的范围
		const _XMultiTextTexture& tex,	//控件的贴图
		const char *str,const _XFontUnicode& font, float strSize,		//控件的字体及相关信息
		//const _XVector2& strPosition,
		//const _XMouseRightButtonMenu &mouseMenu,
		const _XSlider &vSlider,const _XSlider &hSlider);	//控件的其他附属依赖控件
	_XBool initEx(const _XVector2& position,	//对上面接口的简化
		const _XMultiTextTexture& tex,	
		const char *str,const _XFontUnicode& font, float strSize,
		const _XSlider &vSlider,const _XSlider &hSlider);
	_XBool initPlus(const char *path,
		const char *str,const _XFontUnicode& font, float strSize = 1.0f,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect& area,
		const char *str,const _XFontUnicode& font, float strSize = 1.0f);
	_XBool initWithoutTex(const _XRect& area,const char *str)
	{
		return initWithoutTex(area,str,XEE::systemFont,1.0f);
	}
	_XBool initWithoutTex(const _XVector2& pixelSize,const char *str)
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			str,XEE::systemFont,1.0f);
	}

	void setSelectCallBackFun(void (* funSelect)(void *,int),void *pClass);
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);					//返回是否出发按键动作
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y);
	void setLostFocus();
public:
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);	//改变空间的位置

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);		//改变控件的尺寸

	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色

	void setString(const char *str);			//改变显示的字符串
	const char *getString()const{return m_nowString;}
	void addString(const char *str);			//向字符串中添加字符串
	void addALine(const char *str);			//向字符串中添加字符串

	_XBool setACopy(const _XMultiText &temp);
	
	_XBool exportData(const char *fileName = NULL);		//数据导出
	_XBool importData(const char *fileName = NULL);		//数据导入
	_XBool deleteSelectLine();						//删除当前选择的一行
	_XBool moveUpSelectLine();						//将选择的一行上移
	_XBool moveUpLine(int order);					//将某一行上移
	_XBool moveDownSelectLine();					//将选择的一行下移
	_XBool moveDownLine(int order);				//将某一行下移
	_XBool insertALine(const char *str,int lineOrder);	//向文本的指定行插入一行

	_XMultiText();
	~_XMultiText(){release();}
	//下面是内联函数
	void enable();		//使其有效
	void disable();		//使其无效
	void release();
	int getSelectLine() const;			//返回当前鼠标选择的是哪一行，-1位没有选择
	int getLineSum() const;
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//virtual void justForTest() {}
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XMultiText(const _XMultiText &temp);
	_XMultiText& operator = (const _XMultiText& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	_XMultiTextActionType m_actionType;
	_XMoveData m_actionMD;
	int m_actionPosition;	//当前动作的位置
	void setAction(_XMultiTextActionType type,int index);
	//---------------------------------------------------------
};
#include "XMultiText.inl"

#endif