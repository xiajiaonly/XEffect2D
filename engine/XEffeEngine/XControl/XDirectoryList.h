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
#include "XButton.h"
#include "XDirectory.h"
namespace XE{
//工作正在进行中，尚未经过测试
//这是一个现实目录信息的控件的类，这个类由一个输入框，一个按钮，2个拖动条和1个展示框组成
class XDirListSkin
{
private:
	XBool m_isInited;

	bool loadFromFolder(const char *filename,XResPos resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResPos resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename,XResPos resPos);		//从网页中读取资源
public:
	XTextureData *dirListNormal;			//普通状态
	XTextureData *dirListDisable;			//无效状态

	XRect m_mouseRect;			//鼠标的响应范围

	XDirListSkin();
	~XDirListSkin(){release();}
	XBool init(const char *normal,const char *disable,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
};
//目录列表框的一行信息
class XDirListOneLine
{
public:
	XFontUnicode m_font;
	char *m_string;		//字符内容
	XCheck *m_check;
	XBool m_needCheck;
	XBool m_isEnable;		//是否有效

	//XVec2 m_pos;		//当前行所在的位置
	XFileInfo *m_file;			//指向文件信息的指针
	XDirListOneLine()
		:m_isEnable(XFalse)
		,m_needCheck(XFalse)
		,m_check(NULL)
		,m_string(NULL)
		,m_file(NULL)
	{}
	~XDirListOneLine(){release();}
	void release();
};
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (MIN_FONT_CTRL_SIZE)		//默认的滑动条宽度
#endif	//DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_DIRLIST_BT_SIZE
#define DEFAULT_DIRLIST_BT_SIZE (MIN_FONT_CTRL_SIZE + 8.0f)
#endif	//DEFAULT_DIRLIST_BT_SIZE
#ifndef DEFAULT_DIRLIST_CK_SIZE
#define DEFAULT_DIRLIST_CK_SIZE (MIN_FONT_CTRL_SIZE - 4.0f)
#endif	//DEFAULT_DIRLIST_CK_SIZE
class XDirectoryList:public XControlBasic
{
private:
	XBool m_isInited;	//是否初始化

	XDirectory m_dir;	//用于解析路径

	XSprite m_spriteBackGround;
	const XTextureData *m_dirListNormal;
	const XTextureData *m_dirListDisable;

	XBool m_needShowVSlider;			//是否需要显示垂直滑动条
	XSlider m_verticalSlider;		//垂直滑动条
	XBool m_needShowHSlider;			//是否需要显示水平滑动条
	XSlider m_horizontalSlider;	//水平滑动条
	XButton m_button;	//按钮
	XEdit m_edit;
	XCheck m_check;

	XBool m_haveSelect;				//是否有选择
	int m_selectLineOrder;			//选择的是哪一行
	int m_showStartLine;			//显示的是从第几行开始的
	int m_canShowLineSum;			//控件中能显示的行数
	
	int m_curLineSum;				//当前一共拥有多少行
	int m_mouseTime;				//用于鼠标计时，判断是否双击

	int m_maxLineWidth;				//当前的最大行宽[像素]
	int m_curLineLeft;				//当前的间距[像素]
	int m_canShowMaxLineWidth;		//可以显示的最大行宽[像素]

	std::vector<XDirListOneLine *> m_lineData;	//每一行的内容
	int m_showPixWidth;				//显示区域的像素宽度
	int m_showPixHight;				//显示区域的像素高度

	XFontUnicode m_caption;
	XVec2 m_fontSize;
	XFColor m_textColor;			//文字的颜色
	float m_curTextWidth;			//当前的字体宽度
	float m_curTextHeight;			//当前的字体高度
	void furlFolder(int index,XBool flag = XTrue);		//收拢某个文件夹,在控件回调中flag = XTrue;
	void updateHSliderState();	//更新水平条的状态
	void updateShowPosition();	//更新显示的状态

	//下面是内部控件的回调函数
	static void ctrlProc(void *,int,int);

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式

	XBool m_canChangePath;
public:
	XBool getCanChangePath() const {return m_canChangePath;}
	void setCanChangePath(XBool flag);
public:
	XBool init(const XVec2& position,
		const XRect& Area,	
		XDirListSkin & tex,
		const XFontUnicode& font,
		float fontSize,
		const XCheck &check,
		const XButton &button,
		const XEdit &edit,
		const XSlider &vSlider,	//垂直滑动条
		const XSlider &hSlider);
	XBool initEx(const XVec2& position,	//对上面接口的简化
		XDirListSkin & tex,
		const XFontUnicode& font,
		float fontSize,
		const XCheck &check,
		const XButton &button,
		const XEdit &edit,
		const XSlider &vSlider,
		const XSlider &hSlider);
	XBool initPlus(const char * path,
		const XFontUnicode& font,float fontSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,	
		const XFontUnicode& font,float fontSize = 1.0f);
	XBool initWithoutSkin(const XRect& area) {return initWithoutSkin(area,getDefaultFont(),1.0f);}
	XBool initWithoutSkin(const XVec2& pixelSize) 
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),getDefaultFont(),1.0f);
	}
	const char * getSelectFileName() const;	//获取全路径
	int getSelectLineOrder() const;
protected:
	//下面三个函数需要进行优化，也可以进行优化
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);					//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int) {;}
	XBool canGetFocus(const XVec2& p);//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2& ){return XTrue;}
public:
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);			//设置尺寸

	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);						//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);						//获取四个顶点的坐标，目前先不考虑旋转和缩放

	using XObjectBasic::setColor;		//避免覆盖的问题
	virtual void setColor(const XFColor& c);
	virtual void setAlpha(float a);
	//virtual void justForTest() {;}

	void release();
	XDirectoryList()
		:m_isInited(XFalse)
		,m_resInfo(NULL)
		,m_withoutTex(XFalse)
		,m_canChangePath(XTrue)
	{
		m_ctrlType = CTRL_OBJ_DIRECTORYLIST;
	}
	~XDirectoryList(){release();}
};
#if WITH_INLINE_FILE
#include "XDirectoryList.inl"
#endif
}
#endif