#ifndef _JIA_XMUTILIST_
#define _JIA_XMUTILIST_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//下面是一个多列列表控件的类
//实现滑动条			x
//实现鼠标改变栏宽		x
//实现行选				x
//实现数据导入导出		x
//实现加入行，加入列	x
//实现插入行，插入列	x
//实现删除行，删除列	x
//实现移动行，移动列	x
//实现水平滑动条的逐像素移动
//鼠标拖动行的移动		x

#include "XSlider.h"
#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"
#include "XMouseRightButtonMenu.h"

//下面是多列列表控件的贴图
class _XMultiListTexture
{
private:
	char m_isInited;
public:
	_XTextureData *mutiListNormal;		//多列列表的普通状态
	_XTextureData *mutiListDisable;		//多列列表的无效状态
	_XTextureData *mutiListSelect;		//多列列表中被选中行的背景
	_XTextureData *mutiListMove;		//移动标题时的对齐标签
	_XTextureData *mutiListTitle;		//多列列表的标题背景
	_XTextureData *mutiListTitleEnd;	//多列列表的标题分隔符

	_XMultiListTexture();
	~_XMultiListTexture();
	int init(const char *normal,const char *disable,const char *select,const char *move,
		const char *title,const char *titleEnd,int resoursePosition = 0);
	void release();
};

//表中的一份元素
struct _XMultiListOneBox
{
	char isEnable;				//这个元素是否有效
	char isShow;				//这个元素是否需要显示
	_XFontUnicode text;			//用于显示的字体

	_XVector2 order;				//这个元素所在的位置(也就是哪一列，哪一行)
	char *string;				//这个元素中的字符串
	int stringLength;			//字符串的长度
	_XMultiListOneBox *nextBox;	//下一个元素的指针
	_XMultiListOneBox()
		:string(NULL)
		,nextBox(NULL)
	{
	}
};

//表中的一列
struct _XMultiListOneRow
{
	char isEnable;					//这一列是否有效
	char isShow;					//这一列素是否需要显示	0:不显示 1:完整显示 2:不显示分割符
	_XFontUnicode text;				//用于显示的字体

	int order;						//这一列的编号
	int stringShowWidth;			//这一列可以显示的字符的宽度
	_XVector2 position;				//这一列的相对控件左上角的像素位置
	_XVector2 pixSize;				//列中一个单位的像素尺寸,这个像素大小是没有经过缩放的
	char *title;					//这一列的标题
	_XMultiListOneRow *nextRow;		//下一列的指针
	_XSprite m_spriteTitleEnd;		//每列结束的标记符
	_XSprite m_spriteTitle;			//每列的标题
	char needChip;					//是否需要切割，0，不需要切割，1，前面切割，2，后面切割
	float left;						//左边的切割位置
	float right;					//右边的切割位置
	_XMultiListOneRow()
		:title(NULL)
		,nextRow(NULL)
	{
	}
};

#define MUTILIST_MIN_WIDTH 80		//多列列表的最小宽度
#define MUTILIST_MAX_ROW_SUM 256	//最多有多少列
#define MUTILIST_TITLE_EXPAND_LENGTH 100	//最后一列用于扩展的宽度

class _XMultiList:public _XControlBasic
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
	
	int m_tableRowSum;				//表格中的列数
	_XMultiListOneRow *m_tableRow;	//列的链表
	int m_tableLineSum;				//表格中的行数
	_XMultiListOneBox *m_tableBox;	//表中元素的链表

	char m_haveSelect;				//是否有选择
	int m_selectLineOrder;			//选择的是哪一行
	int m_showStartLine;			//显示的是从第几行开始的
	int m_showStartRow;				//显示从第几列开始的
	int m_canShowLineSum;			//控件中能显示的行数

	char m_needShowMove;			//是否需要显示移动对齐条

	_XSprite m_spriteBackGround;	//控件的大背景
	_XSprite m_spriteSelect;		//控件的选择(暂时只能单行选择)
	_XSprite m_spriteMove;			//鼠标移动的对齐标记

	friend void funMutiListValueChangeV(void *pClass,int objectID);
	friend void funMutiListValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiListValueChangeH(void *pClass,int objectID);
	friend void funMutiListValueChangeMoveH(void *pClass,int objectID);

	//下面是为了实现鼠标左键改变栏宽而定义的临时变量
	char m_mouseLeftButtonDown;		//鼠标左键是否按下
	int m_startX;					//鼠标开始拖动的起点
	int m_changeRowOrder;			//拖动列的编号
	_XMultiListOneRow * m_changeRow;	//拖动列的指针
//	int m_rowDx;					//拖动列的显示偏移位置

	//下面是为了实现鼠标拖动移动行而定义的变量
	char m_mouseMoveDown;	//鼠标是否有按下拖动
	int m_oldLineOrder;		//鼠标按下所在的行，用于计算需要如何移动

public:
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

public:
	int init(const _XVector2& position,		//空间所在的位置
		const _XRect& Area,					//控件的实际显示区域
		const _XMultiListTexture &tex,		//控件的贴图
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

	_XMultiList();
	~_XMultiList();
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
	_XMultiList(const _XMultiList &temp);
	_XMultiList& operator = (const _XMultiList& temp);
};

inline int _XMultiList::addALine()					//在末尾加入一行
{
	return setLineSum(m_tableLineSum + 1);
}

inline int _XMultiList::addARow()					//在末尾加入一列
{
	return setRowSum(m_tableRowSum + 1);
}

inline int _XMultiList::deleteSelectLine()			//删除选取的一行
{
	if(m_isInited == 0) return 0;
	if(m_haveSelect == 0) return 0;
	return deleteLine(m_selectLineOrder);
}

inline int _XMultiList::moveUpLine(int order)		//将order行上移
{
	return moveDownLine(order - 1);
}

inline int _XMultiList::moveLeftRow(int order)		//将order列左移
{
	return moveRightRow(order - 1);
}

inline void _XMultiList::setSize(const _XVector2& size)	//设置尺寸
{
	setSize(size.x,size.y);
}

inline void _XMultiList::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

#endif