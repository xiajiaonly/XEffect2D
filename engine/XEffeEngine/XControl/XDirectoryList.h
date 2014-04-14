#ifndef _JIA_XDIRECTORYLIST_
#define _JIA_XDIRECTORYLIST_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XSlider.h"
#include "XEdit.h"
#include "XCheck.h"
#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "XButton.h"
#include "XMedia/XDirectory.h"
//工作正在进行中，尚未经过测试
//这是一个现实目录信息的控件的类，这个类由一个输入框，一个按钮，2个拖动条和1个展示框组成
class _XDirListTexture
{
private:
	_XBool m_isInited;
public:
	_XTextureData *dirListNormal;			//普通状态
	_XTextureData *dirListDisable;			//无效状态

	_XRect m_mouseRect;			//鼠标的响应范围

	_XDirListTexture();
	~_XDirListTexture();
	_XBool init(const char *normal,const char *disable,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};
//目录列表框的一行信息
class _XDirListOneLine
{
public:
	_XFontUnicode m_font;
	char *m_string;		//字符内容
	_XCheck *m_check;
	_XBool m_needCheck;
	_XBool m_isEnable;		//是否有效

	//_XVector2 m_pos;		//当前行所在的位置
	_XFile *m_file;			//指向文件信息的指针
	_XDirListOneLine()
		:m_isEnable(XFalse)
		,m_needCheck(XFalse)
		,m_check(NULL)
		,m_string(NULL)
		,m_file(NULL)
	{
	}
	~_XDirListOneLine()
	{
		release();
	}
	void release()
	{
		if(!m_isEnable) return;
		XDELETE_ARRAY(m_string);
		XDELETE(m_check);
		m_isEnable = XFalse;
	}
};
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (32)		//默认的滑动条宽度
#endif	//DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_DIRLIST_BT_SIZE
#define DEFAULT_DIRLIST_BT_SIZE (40)
#endif	//DEFAULT_DIRLIST_BT_SIZE
#ifndef DEFAULT_DIRLIST_CK_SIZE
#define DEFAULT_DIRLIST_CK_SIZE (28)
#endif	//DEFAULT_DIRLIST_CK_SIZE
class _XDirectoryList:public _XControlBasic
{
private:
	_XBool m_isInited;	//是否初始化

	_XDirectory m_dir;	//用于解析路径

	_XSprite m_spriteBackGround;
	const _XTextureData *m_dirListNormal;
	const _XTextureData *m_dirListDisable;

	_XBool m_needShowVSlider;			//是否需要显示垂直滑动条
	_XSlider m_verticalSlider;		//垂直滑动条
	_XBool m_needShowHSlider;			//是否需要显示水平滑动条
	_XSlider m_horizontalSlider;	//水平滑动条
	_XButton m_button;	//按钮
	_XEdit m_edit;
	_XCheck m_check;

	_XBool m_haveSelect;				//是否有选择
	int m_selectLineOrder;			//选择的是哪一行
	int m_showStartLine;			//显示的是从第几行开始的
	int m_canShowLineSum;			//控件中能显示的行数
	
	int m_nowLineSum;				//当前一共拥有多少行
	int m_mouseTime;				//用于鼠标计时，判断是否双击

	int m_maxLineWidth;				//当前的最大行宽[像素]
	int m_nowLineLeft;				//当前的间距[像素]
	int m_canShowMaxLineWidth;		//可以显示的最大行宽[像素]

	std::vector<_XDirListOneLine *> m_lineData;	//每一行的内容
	int m_showPixWidth;				//显示区域的像素宽度
	int m_showPixHight;				//显示区域的像素高度

	_XFontUnicode m_caption;
	_XVector2 m_fontSize;
	_XFColor m_textColor;			//文字的颜色
	float m_nowTextWidth;			//当前的字体宽度
	float m_nowTextHeight;			//当前的字体高度
	void furlFolder(int index,_XBool flag = XTrue);		//收拢某个文件夹,在控件回调中flag = XTrue;
	void updateHSliderState();	//更新水平条的状态
	void updateShowPosition();	//更新显示的状态

	//下面是内部控件的回调函数
	friend void funDirListButton(void *pClass,int objectID);
	friend void funDirListValueChangeV(void *pClass,int objectID);
	friend void funDirListValueChangeH(void *pClass,int objectID);
	friend void funDirListCheck(void *pClass,int objectID);

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式

	_XBool m_canChangePath;
public:
	_XBool getCanChangePath() const {return m_canChangePath;}
	void setCanChangePath(_XBool flag)
	{
		if((flag && m_canChangePath) || (!flag && !m_canChangePath)) return;
		m_canChangePath = flag;
		if(flag) m_edit.enable();
		else m_edit.disable();
	}
public:
	_XBool init(const _XVector2& position,
		const _XRect& Area,	
		_XDirListTexture & tex,
		_XFontUnicode &font,
		float fontSize,
		const _XCheck &check,
		const _XButton &button,
		const _XEdit &edit,
		const _XSlider &vSlider,	//垂直滑动条
		const _XSlider &hSlider);
	_XBool initEx(const _XVector2& position,
		_XDirListTexture & tex,
		_XFontUnicode &font,
		float fontSize,
		const _XCheck &check,
		const _XButton &button,
		const _XEdit &edit,
		const _XSlider &vSlider,	//垂直滑动条
		const _XSlider &hSlider);
	_XBool initPlus(const char * path,
		_XFontUnicode &font,
		float fontSize,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect& area,	
		_XFontUnicode &font,
		float fontSize);	//尚未实现
	const char * getSelectFileName() const	//获取全路径
	{
		if(m_haveSelect) return m_lineData[m_selectLineOrder]->m_file->allPath;
		else return NULL;
	}
	int getSelectLineOrder() const
	{
		if(m_haveSelect) return m_selectLineOrder;
		else return -1;
	}
protected:
	void draw();
	void drawUp(){;}	//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char * ch,int len) {;}
	_XBool canGetFocus(float x,float y)//用于判断当前物件是否可以获得焦点
	{
		if(!m_isInited) return XFalse;	//如果没有初始化直接退出
		if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
		if(!m_isVisiable) return XFalse;	//如果不可见直接退出
		if(!m_isEnable) return XFalse;		//如果无效则直接退出
		return isInRect(x,y);
	}
	_XBool canLostFocus(float x,float y){return XTrue;}
public:
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		updateShowPosition();
		if(!m_withoutTex) m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()) * m_size);
		m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
		m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()) * m_size);
		m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()) * m_size);
		m_edit.setPosition(m_position);
		updateChildPos();
	}

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y)
	{//左上角对其
		m_size.set(x,y);
		updateShowPosition();
		if(!m_withoutTex) 
		{
			m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()) * m_size);
			m_spriteBackGround.setSize(m_size);
		}
		m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
		m_button.setSize(m_size);
		m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()) * m_size);
		m_verticalSlider.setSize(m_size);
		m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()) * m_size);
		m_horizontalSlider.setSize(m_size);
		m_edit.setPosition(m_position);
		m_edit.setSize(m_size);
		updateChildSize();
	}			//设置尺寸

	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);						//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);						//获取四个顶点的坐标，目前先不考虑旋转和缩放

	using _XObjectBasic::setColor;		//避免覆盖的问题
	virtual void setColor(float r,float g,float b,float a){updateChildColor();}

	virtual void setAlpha(float a){updateChildAlpha();}
	virtual void justForTest() {;}

	void release();
	_XDirectoryList()
		:m_isInited(0)
		,m_resInfo(NULL)
		,m_withoutTex(XFalse)
		,m_canChangePath(XTrue)
	{
	}
	~_XDirectoryList()
	{
		release();
	}
};

#endif