#ifndef _JIA_XEDIT_
#define _JIA_XEDIT_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//这是一个单行输入的类
//需要支持的功能点
//1、超长文本输入
//2、文本中间插入
//3、鼠标选择文本(正常操作鼠标选择状态会反色，如果不使用CPU图像处理，这里必须要使用两种颜色的字体来实现)
//4、文本替换、剪切、复制、删除
//5、键盘控制键入位置的改变
//6、密码格式的文本输入

//控件的状态
//1、无效状态
//2、正常显示状态
//3、插入状态
//4、鼠标选择状态
//5、鼠标右键的弹出菜单（菜单内容：剪切、复制、粘贴、撤销）需要实现一个右键菜单的类

//注意：现在只支持英文输入

//小键盘输入	x
//键盘插入		x
//鼠标插入		x
//鼠标片选		x
//右键菜单		x
//复制与粘贴	x
//超出背景显示范围的处理	x
//shift + 方向键进行片选	x
//键盘属性键弹出右键菜单	x
//ctrl+c与ctrl+v的操作		x
//超范围片选				x
//shift + home				x
//shift + end				x
//密码模式					x
//鼠标双击全选

#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"
#include "XMouseRightButtonMenu.h"

class _XEditTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *editNormal;			//输入框普通状态
	_XTextureData *editDisable;			//输入框无效状态
	_XTextureData *editSelect;			//输入框片选颜色
	_XTextureData *editInsert;			//输入框插入标记
	_XTextureData *editUpon;			//上层的颜色

	_XRect m_mouseRect;			//鼠标的响应范围

	_XEditTexture();
	~_XEditTexture();
	_XBool init(const char *normal,const char *disable,const char *select,const char *insert,
		const char *upon = NULL,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

#define MAX_INPUT_STRING_LENGTH 256	//可以最大输入字符串的长度
#define TEXT_EDGE_WIDTH (10)		//输入字体的边界宽度

class _XDirectoryList;
class _XEdit:public _XControlBasic
{
	friend _XDirectoryList;
private:
	_XBool m_isInited;	//是否初始化
	_XMouseRightButtonMenu *m_mouseRightButtonMenu;	//鼠标右键菜单

	_XFontUnicode m_caption;			//显示当前输入的字符串
//	_XFontUnicode m_selectText;			//显示被选中的字符串(目前这个没有被使用到，可以屏蔽)

	char *m_nowString;		//当前输入的字符串
	char *m_tempNowString;	//这个字符串主要用于辅助使用
//	char *m_insertString;	//插入的字符串
//	char *m_selectString;	//选取的字符串
	int m_nowInsertPoint;	//当前选择输入的位置,整个字符串的整体位置
	int m_nowStringLength;	//当前输入字符串的长度
	_XBool m_haveSelect;		//是否在字符串中有选择
	_XBool m_selectMouseDown;	//鼠标是否按下准备片选
	int m_selectStart;		//选择的起始位置
	int m_selectEnd;		//选择的结束位置

	const _XTextureData *m_editNormal;		//输入框普通状态
	const _XTextureData *m_editDisable;		//输入框无效状态
	const _XTextureData *m_editSelect;		//输入框片选颜色
	const _XTextureData *m_editInsert;		//输入框插入标记
	const _XTextureData *m_editUpon;

	_XSprite m_spriteBackGround;	//用于显示输入框的背景贴图
	_XSprite m_spriteSelect;		//用于显示选择内容的背景颜色
	_XSprite m_spriteInsert;		//用于显示插入符号
	_XVector2 m_textPosition;			//文字显示的位置，是相对于控件的位置来定的
	_XVector2 m_textSize;				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	_XFColor m_textColor;		//文字的颜色
	float m_nowTextWidth;		//当前字体的宽度，这个数值在字体的尺寸改变时需要更新
	float m_nowTextHeight;		//当前字体的高度

	void (*m_funInputChenge)(void *,int);		//输入内容发生改变的时候调用
	void (*m_funInputOver)(void *,int);		//确认输入结束之后的时候调用
	void *m_pClass;

	int m_timer;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	//下面是用于按键判断的中间变量
	char m_keyShiftState;		//shift按键的状态	0弹起，1按下
	char m_keyCapsLockState;	//CapsLock按键的状态	1有效，0无效
	char m_keyNumLockState;		//NumLock按键的状态	1有效，0无效
	char m_keyCtrlState;		//Ctrl按键的状态 0弹起，1按下
	
	int m_canShowLength;		//输入框中可以显示的字符串的长度
	int m_nowShowStart;			//当前输入框中显示的字符串的起始位置
	char *m_nowShowString;		//当前显示的字符串 
	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
	_XRect m_selectRect;	//选择的范围
	int keyJudgement(int keyOrder);	//这里是案件判断处理的函数，供内部调用
public:
	void upDataShowStr();	//根据显示长度和光标的位置跟新显示的内容
	void upDateInsertShowPosition();	//根据实际情况更新光标显示的位置
	void changeInsertPoint(int sum);	//移动光标
	void upDataSelectShow();			//更新片选的显示信息
	void mouseRightMenuProc();			//右键菜单的处理
	void deleteSelectStr();			//删除选中的字符串并移动光标

public:
	_XBool init(const _XVector2& position,		//控件的位置
		const _XRect& Area,					//控件的鼠标响应区间
		const _XEditTexture &tex,			//控件的贴图
		const char *str,					//控件的初始化字符串
		const _XFontUnicode &font,			//控件的字体
		float strSize,	//控件的字体信息
		_XMouseRightButtonMenu * mouseMenu = NULL);		//控件的右键菜单
	_XBool initEx(const _XVector2& position,		//控件的位置
		const _XEditTexture &tex,			//控件的贴图
		const char *str,					//控件的初始化字符串
		const _XFontUnicode &font,			//控件的字体
		float strSize,	//控件的字体信息
		_XMouseRightButtonMenu * mouseMenu = NULL);		//控件的右键菜单
	_XBool initPlus(const char * path,			//控件的贴图
		const char *str,					//控件的初始化字符串
		const _XFontUnicode &font,			//控件的字体
		float strSize,	//控件的字体信息
		_XMouseRightButtonMenu * mouseMenu = NULL,//控件的右键菜单
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	_XBool initWithoutTex(const _XRect& area,
		const char *str,					//控件的初始化字符串
		const _XFontUnicode &font,			//控件的字体
		float strSize,	//控件的字体信息
		_XMouseRightButtonMenu * mouseMenu = NULL);
protected:
	void update(int stepTime);
	void draw();					//绘图函数
	void drawUp();				//描绘小菜单
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);							//返回是否出发按键动作
	void insertChar(const char * ch,int len);
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y);
	virtual void setLostFocus() 
	{
		if(m_haveSelect) m_haveSelect = XFalse;
		m_isBeChoose = XFalse;
	}	
public:
	void release();					//资源释放函数
	void setString(const char *str);		//设置输入的字符串

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);		//设置控件的位置

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);				//设置控件的缩放比例

	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a) 
	{
		m_color.setColor(r,g,b,a);
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteInsert.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildColor();
	}	//设置按钮的颜色
	void setAlpha(float a) 
	{
		m_color.setA(a);
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteInsert.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildAlpha();
	}	//设置按钮的颜色
	
	_XBool setACopy(const _XEdit &temp);				//设置为一个拷贝公用部分资源		

	_XEdit();
	~_XEdit();
	//下面是内联函数
	void disable();					//使无效
	void enable();					//使能
	char *getString() const;				//返回输入的字符串
	int getSelectLength() const;				//或得选区的长度
	int getSelectHead() const;				//获得选取的头
	int getSelectEnd() const;				//获得选取的尾
	void setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass = NULL);		//设置回调函数
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	virtual void justForTest() {;}
private:	//为了防止意外调用造成的错误，下面重载赋值构造函数和赋值操作符
	_XEdit(const _XEdit &temp);
	_XEdit& operator = (const _XEdit& temp);
};
inline void _XEdit::disable()					//使无效
{
	m_isEnable = XFalse;
	if(m_mouseRightButtonMenu != NULL) m_mouseRightButtonMenu->disVisiable(); //取消右键菜单的显示
}
inline void _XEdit::enable()					//使能
{
	m_isEnable = XTrue;
}
inline  char *_XEdit::getString() const				//返回输入的字符串
{
	return m_nowString;
}
inline int _XEdit::getSelectLength() const				//或得选区的长度
{
	if(m_selectEnd < m_selectStart) return m_selectStart - m_selectEnd;
	else return m_selectEnd - m_selectStart;
}
inline int _XEdit::getSelectHead() const				//获得选取的头
{
	if(m_selectEnd < m_selectStart)return  m_selectEnd;
	else return m_selectStart;
}
inline int _XEdit::getSelectEnd() const				//获得选取的尾
{
	if(m_selectEnd < m_selectStart) return m_selectStart;
	else return m_selectEnd;
}
inline void _XEdit::setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass)		//设置回调函数
{
	if(funInputChenge != NULL) m_funInputChenge = funInputChenge;
	if(funInputOver != NULL) m_funInputOver = funInputOver;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
#endif