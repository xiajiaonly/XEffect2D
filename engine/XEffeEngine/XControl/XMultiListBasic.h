#ifndef _JIA_XMUTILISTBASIC_
#define _JIA_XMUTILISTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.26
//--------------------------------

//这是一个基本的多列列表框，只实现了一些基本的功能

#include "XSlider.h"
#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"
#include "XMouseRightButtonMenu.h"
#include "XMultiList.h"
#include "XCheck.h"

#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (32)		//默认的滑动条宽度
#endif //DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_TITLE_HEIGHT
#define DEFAULT_TITLE_HEIGHT (32.0f)
#endif	//DEFAULT_TITLE_HEIGHT
#ifndef DEFAULT_END_WIDTH
#define DEFAULT_END_WIDTH (8.0f)
#endif	//DEFAULT_END_WIDTH
#ifndef DEFAULT_CHECK_SIZE
#define DEFAULT_CHECK_SIZE (32.0f)
#endif	//DEFAULT_CHECK_SIZE
class _XMultiListBasic:public _XControlBasic
{
private:
	_XBool m_isInited;				//是否初始化

	const _XTextureData *m_mutiListNormal;	//多列列表的普通状态
	const _XTextureData *m_mutiListDisable;	//多列列表的无效状态
	const _XTextureData *m_mutiListSelect;	//多列列表中被选中行的背景
	const _XTextureData *m_mutiListMove;		//移动标题时的对齐标签
	const _XTextureData *m_mutiListTitle;		//多列列表的标题背景
	const _XTextureData *m_mutiListTitleEnd;	//多列列表的标题分隔符

	_XBool m_needShowVSlider;			//是否需要显示垂直滑动条
	_XSlider m_verticalSlider;		//垂直滑动条
	_XBool m_needShowHSlider;			//是否需要显示水平滑动条
	_XSlider m_horizontalSlider;	//水平滑动条
	
	int m_tableRowSum;				//表格中的列数(这里需要定义一个有效列数和最大列数的概念)
	_XMultiListOneRow *m_tableRow;	//列的链表
	int m_tableLineSum;				//表格中的行数(这里需要定义一个有效行数和最大行数的概念)
	_XMultiListOneBox *m_tableBox;	//表中元素的链表

	_XBool m_haveSelect;				//是否有选择
	int m_selectLineOrder;			//选择的是哪一行
	_XRect m_selectRect;			//选择的行的套框
	_XBool *m_haveSelectFlag;			//用于标记所有的行列是否被选择
	_XKeyState m_stateOfShiftKey;			//shift按键的状态，用于片选

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
	_XBool getHaveSelectState(int i)
	{
		if(i < 0 || i >= m_tableLineSum) return XFalse;
		return m_haveSelectFlag[i];
	}
	int getTableLineSum()
	{
		return m_tableLineSum;
	}
private:
	//--------------------------------------------------------

	int m_showStartLine;			//显示的是从第几行开始的
	float m_showStartRow;			//显示的头一列的像素宽度
	int m_canShowLineSum;			//控件中能显示的行数

	_XBool m_needShowMove;			//是否需要显示移动对齐条

	_XSprite m_spriteBackGround;	//控件的大背景
	_XSprite m_spriteSelect;		//控件的选择(暂时只能单行选择)
	_XSprite m_spriteMove;			//鼠标移动的对齐标记

	friend void funMutiListBasicValueChangeV(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeH(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeMoveH(void *pClass,int objectID);
	friend void funMutiListBasicStateChange(void *pClass,int objectID);

	//下面是为了实现鼠标左键改变栏宽而定义的临时变量
	_XBool m_mouseLeftButtonDown;		//鼠标左键是否按下
	int m_startX;					//鼠标开始拖动的起点
	int m_changeRowOrder;			//拖动列的编号
	_XMultiListOneRow * m_changeRow;	//拖动列的指针
//	int m_rowDx;					//拖动列的显示偏移位置

	//下面是为了实现鼠标拖动移动行而定义的变量
	_XBool m_mouseMoveDown;	//鼠标是否有按下拖动
	int m_oldLineOrder;		//鼠标按下所在的行，用于计算需要如何移动

	void (*m_funCheckStateChange)(void *,int ID);	//状态选择发生变化的时候调用的函数
	void (*m_funSelectChange)(void *,int ID);		//选择发生变化时调用
	void *m_pClass;				//回调函数的参数

public:
	int getSelectOrder() const	//返回选择操作的行号
	{
		return m_selectLineOrder;
	}
	void setCallbackFun(void (* funSelectChange)(void *,int),void (* funCheckStateChange)(void *,int),void *pClass);
	void clearAllSelect();	//清除所有的选择

	void setTileStr(const char *str,int order);		//设置某个标题的文字
	void setBoxStr(const char *str,int line,int row);	//设置某一个单元格的文字
	_XBool setRowWidth(int temp,int order);		//设置其中一行的宽度
private:
	void updateShowChange();					//根据列表框的变化信息更新列表框中的表格的显示情况
	int m_showPixWidth;							//显示区域的像素宽度
	int m_showPixHight;							//显示区域的像素高度
	void updateSliderState();					//根据表格的情况，跟新滑动条的状态
	void updateSelectLine();					//更新选择行的状态
	void initANewRowData(_XMultiListOneRow * upRow,int i);	//初始化一个新的标题
	void initANewBoxData(_XMultiListOneBox * nowBox,_XMultiListOneRow * nowRow,int i,int j);	//初始化一个列表元素的数据

	_XFontUnicode m_caption;
	_XVector2 m_fontSize;
	_XFColor m_textColor;			//文字的颜色
	float m_nowTextWidth;			//当前的字体宽度
	float m_nowTextHeight;			//当前的字体高度

	_XCheck *m_check0;		//是否可见的状态选择框的按钮
	_XCheck *m_check1;		//是否可编辑的状态选择框的按钮
	_XBool *m_check0State;		//是否可见的状态
	_XBool *m_check1State;		//是否可编辑的状态
public:
	_XBool getCheckState(int order,int lineOrder)
	{
		if(!m_isInited) return XFalse;
	//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return XFalse;
	//	if(order == 0) return m_check0[lineOrder].getState();
	//	else return m_check1[lineOrder].getState();
		if(lineOrder < 0 || lineOrder >= m_tableLineSum) return XFalse;
		if(order == 0) return m_check0State[lineOrder];
		else return m_check1State[lineOrder];
	}
	void setCheckState(int order,int lineOrder,_XBool state)
	{
		if(!m_isInited) return;
	//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return;
	//	if(order == 0) m_check0[lineOrder].setState(state);
	//	else m_check1[lineOrder].setState(state);
		if(lineOrder < 0 || lineOrder >= m_tableLineSum) return;
		if(order == 0) m_check0State[lineOrder] = state;
		else m_check1State[lineOrder] = state;
		//下面更新列表框的选择状态的信息
		if(lineOrder >= m_showStartLine && lineOrder < min(m_showStartLine + m_canShowLineSum,m_tableLineSum))
		{
			if(m_check0State[lineOrder]) m_check0[lineOrder - m_showStartLine].setState(XTrue);
			else m_check0[lineOrder - m_showStartLine].setState(XFalse);
			if(m_check1State[lineOrder]) m_check1[lineOrder - m_showStartLine].setState(XTrue);
			else m_check1[lineOrder - m_showStartLine].setState(XFalse);
		}
	}

	_XBool init(const _XVector2& position,		//空间所在的位置
		const _XRect& Area,					//控件的实际显示区域
		const _XMultiListTexture &tex,		//控件的贴图
		const _XCheckTexture &checktex0,		//单选框的贴图
		const _XCheckTexture &checktex1,		//单选框的贴图
		const _XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum,				//控件中的行数
		//const _XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		const _XSlider &vSlider,	//垂直滑动条
		const _XSlider &hSlider);	//水平滑动条
	_XBool initEx(const _XVector2& position,		//空间所在的位置
		const _XMultiListTexture &tex,		//控件的贴图
		const _XCheckTexture &checktex0,		//单选框的贴图
		const _XCheckTexture &checktex1,		//单选框的贴图
		const _XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum,				//控件中的行数
		//const _XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		const _XSlider &vSlider,	//垂直滑动条
		const _XSlider &hSlider);	//水平滑动条
	_XBool initPlus(const char *path,		//单选框的贴图
		const _XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum,				//控件中的行数
		//const _XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	_XBool initWithoutTex(const _XRect& area,
		const _XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum);
protected:
	void draw();					//描绘函数
	void drawUp(){};						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y) {return XTrue;}
public:
	_XBool exportData(const char *fileName = NULL);			//数据导出
	_XBool importData(const char *fileName = NULL);			//数据导入
	_XBool setRowSum(int rowSum);		//设置列数，超出部分填空白，或者删除	;需要重新设计，不能由于中途的错误，造成最终结果的逻辑错误
	_XBool setLineSum(int lineSum);	//设置行数，超出的部分填空白，或者删除	;需要重新设计，不能由于中途的错误，造成最终结果的逻辑错误
	_XBool deleteLine(int order);		//删除某一行
	_XBool deleteRow(int order);		//删除某一列
	_XBool insertALine(int order);		//在order处插入一行
	_XBool insertARow(int order);		//在order处插入一列
	_XBool moveDownLine(int order);	//将order行下移
	_XBool moveRightRow(int order);	//将order列右移

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);			//设置尺寸

	void setTextColor(const _XFColor& color) 
	{
		if(!m_isInited) return;
		m_textColor = color;
		m_caption.setColor(m_textColor);
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色
	void setColor(const _XFColor& color) 
	{
		if(!m_isInited) return;
		m_color = color;
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildColor();
	}	//设置按钮的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a) 
	{
		if(!m_isInited) return;
		m_color.setColor(r,g,b,a);
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildColor();
	}	//设置按钮的颜色
	void setAlpha(float a) 
	{
		if(!m_isInited) return;
		m_color.setA(a);
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildAlpha();
	}	//设置按钮的颜色

	_XMultiListBasic();
	~_XMultiListBasic();
	void release();
private:
	void releaseTempMemory();	//释放不完整的内存空间
public:
	//下面定义的是内联函数
	_XBool addALine();					//在末尾加入一行
	_XBool addARow();					//在末尾加入一列
	_XBool deleteNowSelectLine();		//删除选取的一行
	_XBool deleteSelectLines();		//删除所有被选中的行数
	_XBool moveUpLine(int order);		//将order行上移
	_XBool moveLeftRow(int order);		//将order列左移
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	virtual void justForTest() {;}
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XMultiListBasic(const _XMultiListBasic &temp);
	_XMultiListBasic& operator = (const _XMultiListBasic& temp);
};

inline _XBool _XMultiListBasic::addALine()					//在末尾加入一行
{
	return setLineSum(m_tableLineSum + 1);
}
inline _XBool _XMultiListBasic::addARow()					//在末尾加入一列
{
	return setRowSum(m_tableRowSum + 1);
}
inline _XBool _XMultiListBasic::deleteNowSelectLine()			//删除选取的一行
{
	if(!m_isInited) return XFalse;
	if(!m_haveSelect) return XFalse;
	return deleteLine(m_selectLineOrder);
}
inline _XBool _XMultiListBasic::deleteSelectLines()
{
	if(!m_isInited) return XFalse;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		if(m_haveSelectFlag[i])
		{
			deleteLine(i);
			-- i;
		}
	}
	m_haveSelect = XFalse;	//清除选择的标记
	m_selectLineOrder = 0;
	return XTrue;
}
inline _XBool _XMultiListBasic::moveUpLine(int order)		//将order行上移
{
	return moveDownLine(order - 1);
}
inline _XBool _XMultiListBasic::moveLeftRow(int order)		//将order列左移
{
	return moveRightRow(order - 1);
}

#endif