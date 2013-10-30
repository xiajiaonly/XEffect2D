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

class _XMultiListBasic:public _XControlBasic
{
private:
	char m_isInited;				//是否初始化

	const _XTextureData *m_mutiListNormal;	//多列列表的普通状态
	const _XTextureData *m_mutiListDisable;	//多列列表的无效状态
	const _XTextureData *m_mutiListSelect;	//多列列表中被选中行的背景
	const _XTextureData *m_mutiListMove;		//移动标题时的对齐标签
	const _XTextureData *m_mutiListTitle;		//多列列表的标题背景
	const _XTextureData *m_mutiListTitleEnd;	//多列列表的标题分隔符

	char m_needShowVSlider;			//是否需要显示垂直滑动条
	_XSlider m_verticalSlider;		//垂直滑动条
	char m_needShowHSlider;			//是否需要显示水平滑动条
	_XSlider m_horizontalSlider;	//水平滑动条
	
	int m_tableRowSum;				//表格中的列数(这里需要定义一个有效列数和最大列数的概念)
	_XMultiListOneRow *m_tableRow;	//列的链表
	int m_tableLineSum;				//表格中的行数(这里需要定义一个有效行数和最大行数的概念)
	_XMultiListOneBox *m_tableBox;	//表中元素的链表

	char m_haveSelect;				//是否有选择
	int m_selectLineOrder;			//选择的是哪一行
	_XRect m_selectRect;			//选择的行的套框
	bool *m_haveSelectFlag;			//用于标记所有的行列是否被选择

	int m_showStartLine;			//显示的是从第几行开始的
	float m_showStartRow;			//显示的头一列的像素宽度
	int m_canShowLineSum;			//控件中能显示的行数

	char m_needShowMove;			//是否需要显示移动对齐条

	_XSprite m_spriteBackGround;	//控件的大背景
	_XSprite m_spriteSelect;		//控件的选择(暂时只能单行选择)
	_XSprite m_spriteMove;			//鼠标移动的对齐标记

	friend void funMutiListBasicValueChangeV(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeH(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeMoveH(void *pClass,int objectID);
	friend void funMutiListBasicStateChange(void *pClass,int objectID);

	//下面是为了实现鼠标左键改变栏宽而定义的临时变量
	char m_mouseLeftButtonDown;		//鼠标左键是否按下
	int m_startX;					//鼠标开始拖动的起点
	int m_changeRowOrder;			//拖动列的编号
	_XMultiListOneRow * m_changeRow;	//拖动列的指针
//	int m_rowDx;					//拖动列的显示偏移位置

	//下面是为了实现鼠标拖动移动行而定义的变量
	char m_mouseMoveDown;	//鼠标是否有按下拖动
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
	int setRowWidth(int temp,int order);		//设置其中一行的宽度
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
	bool *m_check0State;		//是否可见的状态
	bool *m_check1State;		//是否可编辑的状态
public:
	bool getCheckState(int order,int lineOrder)
	{
		if(m_isInited == 0) return false;
	//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return false;
	//	if(order == 0) return m_check0[lineOrder].getState();
	//	else return m_check1[lineOrder].getState();
		if(lineOrder < 0 || lineOrder >= m_tableLineSum) return false;
		if(order == 0) return m_check0State[lineOrder];
		else return m_check1State[lineOrder];
	}
	void setCheckState(int order,int lineOrder,bool state)
	{
		if(m_isInited == 0) return;
	//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return;
	//	if(order == 0) m_check0[lineOrder].setState(state);
	//	else m_check1[lineOrder].setState(state);
		if(lineOrder < 0 || lineOrder >= m_tableLineSum) return;
		if(order == 0) m_check0State[lineOrder] = state;
		else m_check1State[lineOrder] = state;
		//下面更新列表框的选择状态的信息
		if(lineOrder >= m_showStartLine && lineOrder < min(m_showStartLine + m_canShowLineSum,m_tableLineSum))
		{
			if(m_check0State[lineOrder]) m_check0[lineOrder - m_showStartLine].setState(true);
			else m_check0[lineOrder - m_showStartLine].setState(false);
			if(m_check1State[lineOrder]) m_check1[lineOrder - m_showStartLine].setState(true);
			else m_check1[lineOrder - m_showStartLine].setState(false);
		}
	}

	int init(const _XVector2& position,		//空间所在的位置
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
protected:
	void draw();					//描绘函数
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);					//对于鼠标动作的响应函数
	int keyboardProc(int keyOrder,_XKeyState keyState){return 1;};	//do nothing
	int canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	int canLostFocus(float x,float y);
public:
	int exportData(const char *fileName = NULL);			//数据导出
	int importData(const char *fileName = NULL);			//数据导入
	int setRowSum(int rowSum);		//设置列数，超出部分填空白，或者删除	;需要重新设计，不能由于中途的错误，造成最终结果的逻辑错误
	int setLineSum(int lineSum);	//设置行数，超出的部分填空白，或者删除	;需要重新设计，不能由于中途的错误，造成最终结果的逻辑错误
	int deleteLine(int order);		//删除某一行
	int deleteRow(int order);		//删除某一列
	int insertALine(int order);		//在order处插入一行
	int insertARow(int order);		//在order处插入一列
	int moveDownLine(int order);	//将order行下移
	int moveRightRow(int order);	//将order列右移

	void setPosition(const _XVector2& position);		//设置位置的时候不能动态的改变列的宽度,否则将会出现bug
	void setPosition(float x,float y);
	_XVector2 getPosition() const {return m_position;}
	void setSize(const _XVector2& size);				//设置尺寸
	void setSize(float x,float y);			//设置尺寸
	_XVector2 getSize() const {return m_size;}
	void setTextColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_textColor = color;
		m_caption.setColor(m_textColor);
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//设置按钮的颜色
	_XFColor getColor() const {return m_color;}	//获取控件的颜色

	_XMultiListBasic();
	~_XMultiListBasic();
	void release();
private:
	void releaseTempMemory();	//释放不完整的内存空间
public:
	//下面定义的是内联函数
	int addALine();					//在末尾加入一行
	int addARow();					//在末尾加入一列
	int deleteSelectLine();			//删除选取的一行
	int moveUpLine(int order);		//将order行上移
	int moveLeftRow(int order);		//将order列左移
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	int isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XMultiListBasic(const _XMultiListBasic &temp);
	_XMultiListBasic& operator = (const _XMultiListBasic& temp);
};

inline int _XMultiListBasic::addALine()					//在末尾加入一行
{
	return setLineSum(m_tableLineSum + 1);
}

inline int _XMultiListBasic::addARow()					//在末尾加入一列
{
	return setRowSum(m_tableRowSum + 1);
}

inline int _XMultiListBasic::deleteSelectLine()			//删除选取的一行
{
	if(m_isInited == 0) return 0;
	if(m_haveSelect == 0) return 0;
	return deleteLine(m_selectLineOrder);
}

inline int _XMultiListBasic::moveUpLine(int order)		//将order行上移
{
	return moveDownLine(order - 1);
}

inline int _XMultiListBasic::moveLeftRow(int order)		//将order列左移
{
	return moveRightRow(order - 1);
}

inline void _XMultiListBasic::setSize(const _XVector2& size)	//设置尺寸
{
	setSize(size.x,size.y);
}

inline void _XMultiListBasic::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

#endif