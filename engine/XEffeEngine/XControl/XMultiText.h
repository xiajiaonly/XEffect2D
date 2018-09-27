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
#include <deque>
#include "XString.h"
#if WITH_LOG
#else
#include "XCritical.h"
#endif
namespace XE{
//slider目录名为:/SliderH和/SliderV
typedef XEditSkin XMultiTextSkin;

//单行的字符串
struct XMultiTextLineString
{
	XBool isEnable;				//这一行是否要显示，是否有效
	//std::string showString;		//这一行文字中需要显示的字符串
//	int showStart;				//这行字符串显示部分的头在当前行的偏移
//	int showLength;				//显示字符串的长度
	XFontUnicode curText;		//用于显示这一行字符串的字体
//	int lineHeadorder;			//这一行字符在全文中的头位置
//	int lineEndOrder;			//这一行字符在全文中的尾位置
	//int lineLength;				//这一行字符串的长度
	//char isEnterEnd;			//是否因为换行而换行显示1，或者是因为显示字符串的长度限制而换行0
	XSprite spriteSelect;		//字符串显示选择背景颜色的精灵
	XBool haveAllSelect;			//该行中是否存被整体选择

	int lineWidthPixel;			//行的像素宽度

	XMultiTextLineString()
	//	:showString("")
	{}
};

#define MAX_LINE_STRING_LENGTH (256)	//单行可以显示的字符串的最大长度
#define MAX_LINE_SUM (256)				//最多可以输入多少行字符串
//#define MAX_STRING_LENGTH (65536)		//允许输入的最大字符串的长度
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (MIN_FONT_CTRL_SIZE)		//默认的滑动条宽度
#endif //DEFAULT_SLIDER_WIDTH
enum XMultiTextActionType
{//多行列表的动作分类
	MLTTXT_ACTION_TYPE_IN,		//选项出现
	MLTTXT_ACTION_TYPE_MOVE,	//选项移动
	MLTTXT_ACTION_TYPE_DCLICK,	//双击
	MLTTXT_ACTION_TYPE_OUT,		//取消选择
};
class XMultiText:public XControlBasic
{
private:
	XBool m_isInited;				//是否初始化
	XBool m_haveSelect;				//是否在字符串中有选择

	XBool m_needShowVSlider;			//是否需要显示垂直滑动条
	XBool m_needShowHSlider;			//是否需要显示水平滑动条
	XSlider m_verticalSlider;		//垂直滑动条
	XSlider m_horizontalSlider;	//水平滑动条

	std::deque<std::string*> m_curStr;	//当前使用的字符串
	std::deque<std::string*> m_freeStr;	//当前尚未使用的字符串

	const XTextureData *m_multiEditNormal;	//输入框普通状态
	const XTextureData *m_multiEditDisable;	//输入框无效状态
	const XTextureData *m_multiEditSelect;	//输入框片选颜色
	const XTextureData *m_multiEditUpon;	//输入框片选颜色

	XSprite m_spriteBackGround;	//用于显示输入框的背景贴图
	//XVec2 m_textPosition;			//文字显示的位置，是相对于控件的位置来定的
	XVec2 m_textSize;				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	XFColor m_textColor;			//文字的颜色
	float m_textWidth;	//字体的宽度
	float m_curTextHeight;	//字体的高度

	int m_selectLineOrder;		//当前文档中被选择的是哪一行

	XMultiTextLineString *m_lineStr;
	int m_showStartLine;			//当前显示的第一行是总的第几行
	int m_canShowLineSum;		//当前可以显示多少行
//	int m_curShowLineWidth;		//当前一行可以显示的宽度
	int m_curMaxLineSum;		//当前字符串中实际的行数
//	int m_curMaxLineLength;		//当前字符串中实际的行宽
	void updateLineStr();		//更新当前输入的文本到每一行中

	//下面是为了实现像素移动而定义的新变量
	int m_curStartPixels;			//开始的像素位置
	int m_curMaxLineWidthPixels;	//当前最大的像素宽度
	int m_maxLineWidthPixels;		//总的最大像素宽度

//	void (*m_funInputChenge)(void *,int);		//输入内容发生改变的时候调用
//	void (*m_funInputOver)(void *,int);		//确认输入结束之后的时候调用
//	void *m_pClass;
	
	static void ctrlProc(void*,int,int);

	void setStartLine(int temp);	//设置窗口显示的头行的行号
	void checkStr();							//检查字符串的单行宽度是否大于限制,这个函数主要是用于将字符串按照默认的宽度裁制成需要的样式,以便于以后的字符串操作

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式
	char m_timer;	//这是插入符号闪烁时使用的时间标记
public:
	enum XMultiTextEvent
	{
		MLTTXT_SELECT,
	};
//	void (*m_funSelect)(void *,int ID);	//发生点选时调用
//	void *m_pClass;
public:
	XBool init(const XVec2& position,	//控件所在的位置
		const XRect& Area,	//控件的范围
		const XMultiTextSkin& tex,	//控件的贴图
		const char *str,const XFontUnicode& font, float strSize,		//控件的字体及相关信息
		//const XVec2& strPosition,
		//const XMouseRightButtonMenu &mouseMenu,
		const XSlider &vSlider,const XSlider &hSlider);	//控件的其他附属依赖控件
	XBool initEx(const XVec2& position,	//对上面接口的简化
		const XMultiTextSkin& tex,	
		const char *str,const XFontUnicode& font, float strSize,
		const XSlider &vSlider,const XSlider &hSlider);
	XBool initPlus(const char *path,
		const char *str,const XFontUnicode& font, float strSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,
		const char *str,const XFontUnicode& font, float strSize = 1.0f);
	XBool initWithoutSkin(const XRect& area,const char *str)
	{
		return initWithoutSkin(area,str,getDefaultFont(),1.0f);
	}
	XBool initWithoutSkin(const XVec2& pixelSize,const char *str)
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),
			str,getDefaultFont(),1.0f);
	}

//	void setSelectCallBackFun(void (* funSelect)(void *,int),void *pClass);
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);					//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);					//返回是否出发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2& p);
	void setLostFocus();
public:
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);	//改变空间的位置

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);		//改变控件的尺寸

	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色

	//下面几个方法需要保证字符串内容的合法性
private:
	void clearAllStrData();	//清空所有的字符串
	bool addAStrData(const char * str);	//添加一条数据,返回是否发生了翻滚
	XCritical m_mutex;	//这里是为了多线程安全而定义的锁
public:
	void setString(const char *str);	//改变显示的字符串
	const std::string getString()const	//这个函数会造成较大的性能消耗
	{
		std::string tmpStr = "";
		for(auto it = m_curStr.begin();it != m_curStr.end();++ it)
		{
			tmpStr += (*(*it)) + "\n";
		}
		return tmpStr;
	}
	//获取指定行的字符串
	const char *getALineString(int index)const
	{
		if(index < 0 || m_curStr.size() <= index) return XString::gNullStr.c_str();
		else return m_curStr[index]->c_str();
	}
	bool setALineString(const char *str,int index)
	{//这个方法尚未经过严格测试
		if(str == NULL) return false;
		if(index < 0 || m_curStr.size() <= index) return false;
		if(*m_curStr[index] == str) return true;	//相同的字符串不需要替换
		*m_curStr[index] = str;
		checkStr();
		updateLineStr();
		return true;
	}
	void addString(const char *str);			//向字符串中添加字符串
	void addALine(const char *str);			//向字符串中添加字符串

	XBool setACopy(const XMultiText &temp);
	
	XBool exportData(const char *fileName = NULL);		//数据导出
	XBool importData(const char *fileName = NULL);		//数据导入
	XBool deleteSelectLine();						//删除当前选择的一行
	XBool moveUpSelectLine();						//将选择的一行上移
	XBool moveUpLine(int order);					//将某一行上移
	XBool moveDownSelectLine();					//将选择的一行下移
	XBool moveDownLine(int order);				//将某一行下移
	XBool insertALine(const char *str,int lineOrder);	//向文本的指定行插入一行

	XMultiText();
	~XMultiText(){release();}
	//下面是内联函数
	void enable();		//使其有效
	void disable();		//使其无效
	void release();
	int getSelectLine() const;			//返回当前鼠标选择的是哪一行，-1位没有选择
	int getLineSum() const;
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//virtual void justForTest() {}
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XMultiText(const XMultiText &temp);
	XMultiText& operator = (const XMultiText& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	XMultiTextActionType m_actionType;
	XMoveData m_actionMD;
	int m_actionPosition;	//当前动作的位置
	void setAction(XMultiTextActionType type,int index);
	//---------------------------------------------------------
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XMultiText.inl"
#endif
}
#endif