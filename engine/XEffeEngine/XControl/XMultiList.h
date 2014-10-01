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
#include "XMouseRightButtonMenu.h"
//slider目录名为:/SliderH和/SliderV
//下面是多列列表控件的贴图
class _XMultiListTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *mutiListNormal;		//多列列表的普通状态
	_XTextureData *mutiListDisable;		//多列列表的无效状态
	_XTextureData *mutiListSelect;		//多列列表中被选中行的背景
	_XTextureData *mutiListMove;		//移动标题时的对齐标签
	_XTextureData *mutiListTitle;		//多列列表的标题背景
	_XTextureData *mutiListTitleEnd;	//多列列表的标题分隔符

	_XRect m_mouseRect;			//鼠标的响应范围

	_XMultiListTexture();
	~_XMultiListTexture(){release();}
	_XBool init(const char *normal,const char *disable,const char *select,const char *move,
		const char *title,const char *titleEnd,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

//表中的一份元素
struct _XMultiListOneBox
{
	_XBool isEnable;				//这个元素是否有效
	_XBool isShow;				//这个元素是否需要显示
	_XFontUnicode text;			//用于显示的字体

	_XVector2 order;				//这个元素所在的位置(也就是哪一列，哪一行)
	char *string;				//这个元素中的字符串
	int stringLength;			//字符串的长度
	_XMultiListOneBox *nextBox;	//下一个元素的指针
	_XMultiListOneBox()
		:string(NULL)
		,nextBox(NULL)
	{}
};

//表中的一列
struct _XMultiListOneRow
{
	_XBool isEnable;					//这一列是否有效
	char isShow;					//这一列素是否需要显示	0:不显示 1:完整显示 2:不显示分割符
	_XFontUnicode text;				//用于显示的字体

	int order;						//这一列的编号
	int stringShowWidth;			//这一列可以显示的字符的宽度
	_XVector2 position;				//这一列的相对控件左上角的像素位置
	_XVector2 pixSize;				//列中一个单位的像素尺寸,这个像素大小是没有经过缩放的
	int posX;
	int pixLen;						//列的宽度，经过裁剪的
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
	{}
};

#define MUTILIST_MIN_WIDTH (80)		//多列列表的最小宽度
#define MUTILIST_MAX_ROW_SUM (256)	//最多有多少列
#define MUTILIST_TITLE_EXPAND_LENGTH (100)	//最后一列用于扩展的宽度
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (32)		//默认的滑动条宽度
#endif	//DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_TITLE_HEIGHT
#define DEFAULT_TITLE_HEIGHT (32.0f)
#endif	//DEFAULT_TITLE_HEIGHT
#ifndef DEFAULT_END_WIDTH
#define DEFAULT_END_WIDTH (8.0f)
#endif	//DEFAULT_END_WIDTH

enum _XMultiListActionType
{//多行列表的动作分类
	MLTLST_ACTION_TYPE_IN,		//选项出现
	MLTLST_ACTION_TYPE_MOVE,	//选项移动
	MLTLST_ACTION_TYPE_DCLICK,	//双击
	MLTLST_ACTION_TYPE_OUT,		//取消选择
};
class _XMultiList:public _XControlBasic
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
	
	int m_tableRowSum;				//表格中的列数
	_XMultiListOneRow *m_tableRow;	//列的链表
	int m_tableLineSum;				//表格中的行数
	_XMultiListOneBox *m_tableBox;	//表中元素的链表

	_XBool m_haveSelect;				//是否有选择
	int m_selectLineOrder;			//选择的是哪一行
	int m_showStartLine;			//显示的是从第几行开始的
	int m_showStartRow;				//显示从第几列开始的
	int m_canShowLineSum;			//控件中能显示的行数

	_XBool m_needShowMove;			//是否需要显示移动对齐条

	_XSprite m_spriteBackGround;	//控件的大背景
	_XSprite m_spriteSelect;		//控件的选择(暂时只能单行选择)
	_XSprite m_spriteMove;			//鼠标移动的对齐标记

	friend void funMutiListValueChangeV(void *pClass,int objectID);
	friend void funMutiListValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiListValueChangeH(void *pClass,int objectID);
	friend void funMutiListValueChangeMoveH(void *pClass,int objectID);

	//下面是为了实现鼠标左键改变栏宽而定义的临时变量
	_XBool m_mouseLeftButtonDown;		//鼠标左键是否按下
	int m_startX;					//鼠标开始拖动的起点
	int m_changeRowOrder;			//拖动列的编号
	_XMultiListOneRow * m_changeRow;	//拖动列的指针
//	int m_rowDx;					//拖动列的显示偏移位置

	//下面是为了实现鼠标拖动移动行而定义的变量
	_XBool m_mouseMoveDown;	//鼠标是否有按下拖动
	int m_oldLineOrder;		//鼠标按下所在的行，用于计算需要如何移动

public:
	int getLineSum() const {return m_tableLineSum;}
	int getRowSum() const {return m_tableRowSum;}
	void setTitleStr(const char *str);	//每项使用;分开
	std::string getTitleStr();	//获取标题的整个字符串
	std::string getTitleStr(int order);	//获取指定标题的字符串
	void setTitleStr(const char *str,int order);		//设置某个标题的文字
	void setBoxStr(const char *str,int line,int row);	//设置某一个单元格的文字
	char *getBoxStr(int line,int row);		//获取某个单元格中的文字
	_XBool setRowWidth(int temp,int order);		//设置其中一行的宽度
	int getSelectIndex();
private:
	void (*m_funSelectFun)(void *,int ID);
	void (*m_funDClick)(void *,int ID);	//鼠标双击时间响应的函数
	void * m_pClass;
	_XBool m_withMouseDrag;	//是否支持鼠标拖动
public:
	void setSelectFun(void (* funSelectFun)(void *,int),void * pClass);
	void setDClickFun(void (* funDClick)(void *,int),void * pClass);
	void setWithMouseDrag(_XBool flag) {m_withMouseDrag = flag;}
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

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
public:
	_XBool init(const _XVector2& position,		//空间所在的位置
		const _XRect& Area,					//控件的实际显示区域
		const _XMultiListTexture &tex,		//控件的贴图
		const _XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum,				//控件中的行数
		//const _XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		const _XSlider &vSlider,	//垂直滑动条
		const _XSlider &hSlider);	//水平滑动条
	_XBool initEx(const _XVector2& position,		//对上面接口的简化
		const _XMultiListTexture &tex,		
		const _XFontUnicode &font,			
		float strSize,						
		int rowSum,					
		int lineSum,				
		//const _XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		const _XSlider &vSlider,	
		const _XSlider &hSlider);
	_XBool initPlus(const char * path,		//控件的贴图
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
	_XBool initWithoutTex(const _XRect& area,
		int rowSum,					//控件中的列数
		int lineSum)
	{
		return initWithoutTex(area,XEE::systemFont,1.0f,rowSum,lineSum);
	}
	_XBool initWithoutTex(const _XVector2& pixelSize,
		int rowSum,					//控件中的列数
		int lineSum)
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			XEE::systemFont,1.0f,rowSum,lineSum);
	}
protected:
	void draw();					//描绘函数
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y);
	void setLostFocus();
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

	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色

	_XMultiList();
	~_XMultiList(){release();}
	void release();
private:
	void releaseTempMemory();	//释放不完整的内存空间
public:
	//下面定义的是内联函数
	_XBool addALine();					//在末尾加入一行
	_XBool addARow();					//在末尾加入一列
	_XBool deleteSelectLine();			//删除选取的一行
	_XBool moveUpLine(int order);		//将order行上移
	_XBool moveLeftRow(int order);		//将order列左移
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//virtual void justForTest() {}
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XMultiList(const _XMultiList &temp);
	_XMultiList& operator = (const _XMultiList& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	_XMultiListActionType m_actionType;
	_XMoveData m_actionMD;
	int m_actionPosition;	//当前动作的位置
	void setAction(_XMultiListActionType type,int index);
	//---------------------------------------------------------
};
#include "XMultiList.inl"

#endif