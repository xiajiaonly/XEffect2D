#ifndef _JIA_XMUTILISTBASIC_
#define _JIA_XMUTILISTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.26
//--------------------------------
//这是一个基本的多列列表框，只实现了一些基本的功能

//#include "XSlider.h"
//#include "XMouseRightButtonMenu.h"
#include "XMultiList.h"
#include "XCheck.h"
namespace XE{
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (MIN_FONT_CTRL_SIZE)		//默认的滑动条宽度
#endif //DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_TITLE_HEIGHT
#define DEFAULT_TITLE_HEIGHT (MIN_FONT_CTRL_SIZE)
#endif	//DEFAULT_TITLE_HEIGHT
#ifndef DEFAULT_END_WIDTH
#define DEFAULT_END_WIDTH (8.0f)
#endif	//DEFAULT_END_WIDTH
#ifndef DEFAULT_CHECK_SIZE
#define DEFAULT_CHECK_SIZE (MIN_FONT_CTRL_SIZE)
#endif	//DEFAULT_CHECK_SIZE
class XMultiListBasic:public XControlBasic
{
private:
	XBool m_isInited;				//是否初始化

	const XTextureData *m_mutiListNormal;	//多列列表的普通状态
	const XTextureData *m_mutiListDisable;	//多列列表的无效状态
	const XTextureData *m_mutiListSelect;	//多列列表中被选中行的背景
	const XTextureData *m_mutiListMove;		//移动标题时的对齐标签
	const XTextureData *m_mutiListTitle;		//多列列表的标题背景
	const XTextureData *m_mutiListTitleEnd;	//多列列表的标题分隔符

	XBool m_needShowVSlider;			//是否需要显示垂直滑动条
	XSlider m_verticalSlider;		//垂直滑动条
	XBool m_needShowHSlider;			//是否需要显示水平滑动条
	XSlider m_horizontalSlider;	//水平滑动条
	
	int m_tableRowSum;				//表格中的列数(这里需要定义一个有效列数和最大列数的概念)
	XMultiListOneRow *m_tableRow;	//列的链表
	int m_tableLineSum;				//表格中的行数(这里需要定义一个有效行数和最大行数的概念)
	XMultiListOneBox *m_tableBox;	//表中元素的链表

	XBool m_haveSelect;				//是否有选择
	int m_selectLineOrder;			//选择的是哪一行
	XRect m_selectRect;			//选择的行的套框
	XBool *m_haveSelectFlag;			//用于标记所有的行列是否被选择
	XKeyState m_stateOfShiftKey;			//shift按键的状态，用于片选

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
	XBool getHaveSelectState(int i);
	int getTableLineSum();
private:
	//--------------------------------------------------------
	int m_showStartLine;			//显示的是从第几行开始的
	float m_showStartRow;			//显示的头一列的像素宽度
	int m_canShowLineSum;			//控件中能显示的行数

	XBool m_needShowMove;			//是否需要显示移动对齐条

	XSprite m_spriteBackGround;	//控件的大背景
	XSprite m_spriteSelect;		//控件的选择(暂时只能单行选择)
	XSprite m_spriteMove;			//鼠标移动的对齐标记

	//static void funMutiListBasicStateChange(void *pClass,int objectID);
	static void ctrlProc(void *,int,int);

	//下面是为了实现鼠标左键改变栏宽而定义的临时变量
	XBool m_mouseLeftButtonDown;		//鼠标左键是否按下
	int m_startX;					//鼠标开始拖动的起点
	int m_changeRowOrder;			//拖动列的编号
	XMultiListOneRow * m_changeRow;	//拖动列的指针
//	int m_rowDx;					//拖动列的显示偏移位置

	//下面是为了实现鼠标拖动移动行而定义的变量
	XBool m_mouseMoveDown;	//鼠标是否有按下拖动
	int m_oldLineOrder;		//鼠标按下所在的行，用于计算需要如何移动
public:
	enum XMultiListBasicEvent
	{
		MLTLST_SELECT,
		MLTLSTB_CHK_STATE_CHANGE,
	};
//	void (*m_funCheckStateChange)(void *,int ID);	//状态选择发生变化的时候调用的函数
//	void (*m_funSelectChange)(void *,int ID);		//选择发生变化时调用
//	void *m_pClass;				//回调函数的参数

public:
	int getSelectIndex() const;	//返回选择操作的行号
//	void setCallbackFun(void (* funSelectChange)(void *,int),void (* funCheckStateChange)(void *,int),void *pClass);
	void clearAllSelect();	//清除所有的选择

	int getLineSum() const {return m_tableLineSum;}
	int getRowSum() const {return m_tableRowSum;}
	void setTitleStr(const char *str,int order);		//设置某个标题的文字
	void setBoxStr(const char *str,int line,int row);	//设置某一个单元格的文字
	XBool setRowWidth(int temp,int order);		//设置其中一行的宽度
private:
	void updateShowChange();					//根据列表框的变化信息更新列表框中的表格的显示情况
	int m_showPixWidth;							//显示区域的像素宽度
	int m_showPixHight;							//显示区域的像素高度
	void updateSliderState();					//根据表格的情况，跟新滑动条的状态
	void updateSelectLine();					//更新选择行的状态
	void initANewRowData(XMultiListOneRow * upRow,int i);	//初始化一个新的标题
	void initANewBoxData(XMultiListOneBox * curBox,XMultiListOneRow * curRow,int i,int j);	//初始化一个列表元素的数据

	XFontUnicode m_caption;
	XVector2 m_fontSize;
	XFColor m_textColor;			//文字的颜色
	float m_curTextWidth;			//当前的字体宽度
	float m_curTextHeight;			//当前的字体高度

	XCheck *m_check0;		//是否可见的状态选择框的按钮
	XCheck *m_check1;		//是否可编辑的状态选择框的按钮
	XBool *m_check0State;		//是否可见的状态
	XBool *m_check1State;		//是否可编辑的状态
public:
	XBool getCheckState(int order,int lineOrder);
	void setCheckState(int order,int lineOrder,XBool state);

	XBool init(const XVector2& position,		//空间所在的位置
		const XRect& Area,					//控件的实际显示区域
		const XMultiListSkin &tex,		//控件的贴图
		const XCheckSkin &checktex0,		//单选框的贴图
		const XCheckSkin &checktex1,		//单选框的贴图
		const XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum,				//控件中的行数
		//const XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		const XSlider &vSlider,	//垂直滑动条
		const XSlider &hSlider);	//水平滑动条
	XBool initEx(const XVector2& position,		//上面接口的简化版本
		const XMultiListSkin &tex,		
		const XCheckSkin &checktex0,		
		const XCheckSkin &checktex1,		
		const XFontUnicode &font,			
		float strSize,						
		int rowSum,				
		int lineSum,			
		//const XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		const XSlider &vSlider,	
		const XSlider &hSlider);
	XBool initPlus(const char *path,		//单选框的贴图
		const XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum,				//控件中的行数
		//const XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	XBool initWithoutSkin(const XRect& area,
		const XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum);
	XBool initWithoutSkin(const XRect& area,
		int rowSum,					//控件中的列数
		int lineSum)
	{
		return initWithoutSkin(area,getDefaultFont(),1.0f,rowSum,lineSum);
	}
	XBool initWithoutSkin(const XVector2& pixelSize,
		int rowSum,					//控件中的列数
		int lineSum)
	{
		return initWithoutSkin(XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			getDefaultFont(),1.0f,rowSum,lineSum);
	}
protected:
	void draw();					//描绘函数
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);					//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(float x,float y);
	void setLostFocus();
public:
	XBool exportData(const char *fileName = NULL);			//数据导出
	XBool importData(const char *fileName = NULL);			//数据导入
	XBool setRowSum(int rowSum);		//设置列数，超出部分填空白，或者删除	;需要重新设计，不能由于中途的错误，造成最终结果的逻辑错误
	XBool setLineSum(int lineSum);	//设置行数，超出的部分填空白，或者删除	;需要重新设计，不能由于中途的错误，造成最终结果的逻辑错误
	XBool deleteLine(int order);		//删除某一行
	XBool deleteRow(int order);		//删除某一列
	XBool insertALine(int order);		//在order处插入一行
	XBool insertARow(int order);		//在order处插入一列
	XBool moveDownLine(int order);	//将order行下移
	XBool moveRightRow(int order);	//将order列右移

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(float x,float y);			//设置尺寸

	void setTextColor(const XFColor& color);	//设置字体的颜色
	XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色

	XMultiListBasic();
	~XMultiListBasic(){release();}
	void release();
private:
	void releaseTempMemory();	//释放不完整的内存空间
public:
	//下面定义的是内联函数
	XBool addALine();					//在末尾加入一行
	XBool addARow();					//在末尾加入一列
	XBool deleteCurSelectLine();		//删除选取的一行
	XBool deleteSelectLines();		//删除所有被选中的行数
	XBool moveUpLine(int order);		//将order行上移
	XBool moveLeftRow(int order);		//将order列左移
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	//virtual void justForTest() {;}
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XMultiListBasic(const XMultiListBasic &temp);
	XMultiListBasic& operator = (const XMultiListBasic& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XMultiListBasic.inl"
#endif
}
#endif