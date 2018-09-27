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

#include "XMouseRightButtonMenu.h"
#include "../XXml.h"
namespace XE{
class XEditSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *editNormal;			//输入框普通状态
	XTextureData *editDisable;			//输入框无效状态
	XTextureData *editSelect;			//输入框片选颜色
	XTextureData *editInsert;			//输入框插入标记
	XTextureData *editUpon;			//上层的颜色

	XRect m_mouseRect;			//鼠标的响应范围

	XEditSkin();
	~XEditSkin(){release();}
	XBool init(const char *normal,const char *disable,const char *select,const char *insert,
		const char *upon = NULL,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResPos resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename,XResPos resPos);		//从网页中读取资源
};

class XDirectoryList;
class XPropertyLine;
class XEdit:public XControlBasic
{
	friend XDirectoryList;
	friend XPropertyLine;
private:
	static const int m_maxInputStringLength = 256;	//可以最大输入字符串的长度
	static const int m_textEdgeWidth = 5;			//输入字体的边界宽度
	static char m_copyString[m_maxInputStringLength]; //这一段是在Edit内部公用的内存空间，主要用于类实体之间的复制粘贴时的内存调用问题
	XBool m_isInited;	//是否初始化
	XBool m_haveSelect;		//是否在字符串中有选择
	XBool m_selectMouseDown;	//鼠标是否按下准备片选
	bool m_isPassword;	//是否为密码输入
	XMouseRightButtonMenu *m_mouseRightButtonMenu;	//鼠标右键菜单

	XFontUnicode m_caption;			//显示当前输入的字符串
//	XFontUnicode m_selectText;			//显示被选中的字符串(目前这个没有被使用到，可以屏蔽)

	char *m_curString;		//当前输入的字符串
	char *m_tempCurString;	//这个字符串主要用于辅助使用
//	char *m_insertString;	//插入的字符串
//	char *m_selectString;	//选取的字符串
	int m_curInsertPoint;	//当前选择输入的位置,整个字符串的整体位置
	int m_curStringLength;	//当前输入字符串的长度
	int m_selectStart;		//选择的起始位置
	int m_selectEnd;		//选择的结束位置

	const XTextureData *m_editNormal;		//输入框普通状态
	const XTextureData *m_editDisable;		//输入框无效状态
	const XTextureData *m_editSelect;		//输入框片选颜色
	const XTextureData *m_editInsert;		//输入框插入标记
	const XTextureData *m_editUpon;

	XSprite m_spriteBackGround;	//用于显示输入框的背景贴图
	XSprite m_spriteSelect;		//用于显示选择内容的背景颜色
	XSprite m_spriteInsert;		//用于显示插入符号
	XVec2 m_textPosition;			//文字显示的位置，是相对于控件的位置来定的
	XVec2 m_textSize;				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	XFColor m_textColor;		//文字的颜色
	float m_curTextWidth;		//当前字体的宽度，这个数值在字体的尺寸改变时需要更新
	float m_curTextHeight;		//当前字体的高度
public:
	enum XEditEvent
	{
		EDT_INPUT_CHANGE,		//输入数据发生变化的时候调用
		EDT_INPUT_OVER,			//输入数据结束的时候调用
	};
	enum XEditType
	{
		TYPE_STRING,		//字符串
		TYPE_UNSIGNED_INT,	//无符号整形数
		TYPE_INT,			//整形数
		TYPE_HEX,			//16进制数
		TYPE_FLOAT,			//浮点数
	};
	void setEditType(XEditType type){m_editType = type;}
	XEditType getEditType(){return m_editType;}
private:
	XEditType m_editType;	//输入框的类型	(相关功能尚未实现，需要修改insertChar和keyProc函数，判断是否能进行相关操作)
	XBool isStringPassable()	//判断当前字符串是否符合要求
	{
		if(m_curString[0] == '\0') return XTrue;	//空字符串是允许的
		switch(m_editType)
		{
		case TYPE_STRING:return XTrue;		//字符串
		case TYPE_UNSIGNED_INT:return XString::getIsUnsignedInt(m_curString);	//无符号整形数
		case TYPE_INT:return XString::getIsInt(m_curString);			//整形数
		case TYPE_HEX:return XString::getIsHexNumber(m_curString) >= 0;			//16进制数
		case TYPE_FLOAT:return XString::getIsNumber(m_curString);			//浮点数
		}
		return XFalse;
	}
//	void (*m_funInputChenge)(void *,int);		//输入内容发生改变的时候调用
//	void (*m_funInputOver)(void *,int);		//确认输入结束之后的时候调用
//	void *m_pClass;

	int m_timer;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	//下面是用于按键判断的中间变量
	char m_keyShiftState;		//shift按键的状态	0弹起，1按下
	char m_keyCapsLockState;	//CapsLock按键的状态	1有效，0无效
	char m_keyNumLockState;		//NumLock按键的状态	1有效，0无效
	char m_keyCtrlState;		//Ctrl按键的状态 0弹起，1按下
	
	int m_canShowLength;		//输入框中可以显示的字符串的长度
	int m_curShowStart;			//当前输入框中显示的字符串的起始位置
	char *m_curShowString;		//当前显示的字符串 
	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式
	XRect m_selectRect;	//选择的范围
	int keyJudgement(int keyOrder);	//这里是案件判断处理的函数，供内部调用
public:
	void upDataShowStr();	//根据显示长度和光标的位置跟新显示的内容
	void upDateInsertShowPosition();	//根据实际情况更新光标显示的位置
	void changeInsertPoint(int sum);	//移动光标
	void upDataSelectShow();			//更新片选的显示信息
	void mouseRightMenuProc();			//右键菜单的处理
	void deleteSelectStr();			//删除选中的字符串并移动光标

public:
	XBool init(const XVec2& position,		//控件的位置
		const XRect& Area,					//控件的鼠标响应区间
		const XEditSkin &tex,			//控件的贴图
		const char *str,					//控件的初始化字符串
		const XFontUnicode& font,			//控件的字体
		float strSize = 1.0f,	//控件的字体信息
		XMouseRightButtonMenu * mouseMenu = NULL);		//控件的右键菜单
	XBool initEx(const XVec2& position,	//上面接口的简化版本
		const XEditSkin &tex,			
		const char *str,					
		const XFontUnicode& font,			
		float strSize = 1.0f,	
		XMouseRightButtonMenu * mouseMenu = NULL);
	XBool initPlus(const char * path,			//控件的贴图
		const char *str,					//控件的初始化字符串
		const XFontUnicode& font,			//控件的字体
		float strSize = 1.0f,	//控件的字体信息
		XMouseRightButtonMenu * mouseMenu = NULL,//控件的右键菜单
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,
		const char *str,					//控件的初始化字符串
		const XFontUnicode& font,			//控件的字体
		float strSize = 1.0f,	//控件的字体信息
		XMouseRightButtonMenu * mouseMenu = NULL);
	XBool initWithoutSkin(const XRect& area,
		const char *str,					//控件的初始化字符串
		XMouseRightButtonMenu * mouseMenu = NULL)
	{
		return initWithoutSkin(area,str,getDefaultFont(),1.0f,mouseMenu);
	}
	XBool initWithoutSkin(const XVec2& pixelSize,
		const char *str,					//控件的初始化字符串
		XMouseRightButtonMenu * mouseMenu = NULL)
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),str,getDefaultFont(),1.0f,mouseMenu);
	}
	XBool initWithoutSkin(float width,
		const char *str,					//控件的初始化字符串
		XMouseRightButtonMenu * mouseMenu = NULL)
	{
		return initWithoutSkin(XRect(0.0f, 0.0f, width, MIN_FONT_CTRL_SIZE), str, getDefaultFont(), 1.0f, mouseMenu);
	}
protected:
	void update(float stepTime);
	void draw();					//绘图函数
	void drawUp();				//描绘小菜单
	XBool mouseProc(const XVec2& p,XMouseState mouseState);					//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);							//返回是否出发按键动作
	void insertChar(const char * ch,int len);
	XBool canGetFocus(const XVec2& p);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2& p);
	virtual void setLostFocus(); 
	void setFocus() 
	{//设置为焦点
		//printf("设置为焦点\n");
		m_isBeChoose = XTrue;
		setImmPos();
	}	
	void setImmPos();
private:
	//下面是为了实现按键按下之后连续动作而定义的变量
	XBool keyProc(int keyOrder);
	int m_curKey;
	int m_curKeyDownTimer;
	int m_curKeyRepTimer;
	XBool m_curKeyDown;
public:
	void release();					//资源释放函数
	void setString(const char *str);		//设置输入的字符串

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);		//设置控件的位置

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);				//设置控件的缩放比例
	XBool getEdtIsNumber();	//输入的数据是否为数值
	int getAsInt();		//优化建议：尝试剔除头和尾的空格或者非法字符，增加容错性
	long long getAsInt64();		//优化建议：尝试剔除头和尾的空格或者非法字符，增加容错性
	//XBool getAsBool();
	float getAsFloat();	//优化建议：尝试剔除头和尾的空格或者非法字符，增加容错性
	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色
	
	XBool setACopy(const XEdit &temp);				//设置为一个拷贝公用部分资源		

	XEdit();
	~XEdit(){release();}
	//下面是内联函数
	void disable();					//使无效
	void enable();					//使能
	char *getString() const;				//返回输入的字符串
	int getSelectLength() const;				//或得选区的长度
	int getSelectHead() const;				//获得选取的头
	int getSelectEnd() const;				//获得选取的尾
	//void setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass = NULL);		//设置回调函数
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	//virtual void justForTest() {;}

	void setInputLen(int len);	//设置输入框的长度
private:	//为了防止意外调用造成的错误，下面重载赋值构造函数和赋值操作符
	XEdit(const XEdit &temp);
	XEdit& operator = (const XEdit& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	//XMoveData m_actionMoveData;	//动态效果的变量
	XVec2 m_oldPos;				//动作播放时的位置
	XVec2 m_oldSize;			//动作播放时的大小
	XMoveData m_lightMD;
	XRect m_lightRect;
	//插入符的动画
	XMoveData m_insertActionMD;	//插入符的动画效果
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面为输入框提供输入提示功能
	std::string m_promptStr;	//提示文字
	bool m_withPromptStr;
public:
	void setPromptStr(const std::string& str)
	{
		m_withPromptStr = true;
		m_promptStr = str;
	}
	void disPromptStr(){m_withPromptStr = false;}
	const std::string& getPromptStr(){return m_promptStr;}
	bool withPromptStr(){return m_withPromptStr;}
	//---------------------------------------------------------
public:
	void setIsPassword(bool flag);
	bool getIsPassword() const{return m_isPassword;}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),getString())) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		std::string tmp;
		if(XXml::getXmlAsString(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setString(tmp.c_str());
		return XTrue;
	}
	//---------------------------------------------------------
private:
	bool getCopyStrHaveEnd()	//判断拷贝字符串是否有结束符
	{
		for(int i = 0;i < m_maxInputStringLength;++ i)
		{
			if(m_copyString[i] == '\0')
				return true;
		}
		return false;
	}
	bool stringPlaster();	//从剪切板中粘贴数据
	void stringCopy()		//拷贝字符串到剪切板
	{//密码模式时不允许复制
		if(!m_haveSelect || m_isPassword) return;//存在片选，没有片选则什么也不作
			//拷贝片选的内容
#ifdef XEE_OS_WINDOWS
		memcpy(m_copyString,m_curString + getSelectHead(),getSelectLength());
		m_copyString[getSelectLength()] = '\0';
		XString::setTextToClipboard(m_copyString);
#endif
#ifdef XEE_OS_LINUX
		memcpy(m_copyString,m_curString + getSelectHead(),getSelectLength());
		m_copyString[getSelectLength()] = '\0';
#endif
	}
	void chooseAllStr()	//全选
	{
		m_haveSelect = XTrue;
		m_selectEnd = m_curStringLength;
		m_selectStart = 0;
		m_selectMouseDown = XFalse;
		upDataSelectShow();
	}
};
#if WITH_INLINE_FILE
#include "XEdit.inl"
#endif
}
#endif