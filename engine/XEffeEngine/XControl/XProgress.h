#ifndef _JIA_XPROGRESS_
#define _JIA_XPROGRESS_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//这是一个用于进度条的类，进度条使用三层贴图的解构，就是背景层，移动层和遮罩或者光反射层，其中移动层是必须要有的。
#include "XControlBasic.h"
#include "XResourceManager.h"
#include "../XFont/XNumber.h"
#include "../XFont/XFontUnicode.h"

namespace XE{
class XProgressSkin
{
private:
	XBool m_isInited;
public:
	XTextureData *progressBackGround;			//进度条的背景贴图
	XTextureData *progressMove;				//进度条用移动变化的贴图
	XTextureData *progressUpon;				//进度条中上层的遮罩或者立体光效

	XRect m_mouseRect;			//鼠标的响应范围
	XVec2 m_fontPosition;	//放置文字的位置

	XProgressSkin();
	~XProgressSkin(){release();}
	XBool init(const char *backgound,const char *move,const char *upon,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResPos resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename,XResPos resPos);		//从网页中读取资源
};

//进度条的数值设定范围为0.0 至 100.0
class XProgress:public XControlBasic
{
private:
	float m_curValue;					//进度条的当前值
	bool m_withNumber;					//是显示数字还是文字
	XNumber m_captionN;					//进度条的标题
	XFontUnicode m_caption;
public:
	void setFont(const XFontUnicode& font);	//必须在初始化之后立即调用，而且必须在初始化时没有指定字体
	void setNumber(const XNumber &number);	//必须在初始化之后立即调用，而且必须在初始化时没有指定字体
private:

	const XTextureData *m_progressBackGround;	//进度条的背景贴图
	const XTextureData *m_progressMove;			//进度条用移动变化的贴图
	const XTextureData *m_progressUpon;			//进度条中上层的遮罩或者立体光效

	XSprite m_spriteBackground;	//用于显示背景的精灵
	XSprite m_spriteMove;			//用于显示贴图的精灵
//	XVec2 m_movePosition;			//动作条相对于背景的位置
	XSprite m_spriteUpon;			//用于显示上层遮罩的精灵
//	XVec2 m_uponPosition;			//遮罩相对于背景的位置
	XVec2 m_textPosition;			//文字显示的位置
	XVec2 m_textSize;				//文字的尺寸
	XFColor m_textColor;		//文字的颜色

	XBool m_isShowFont;				//是否显示数字文字
	XBool m_isACopy;			//这个实体是否是一个副本
	XBool m_isInited;					//进度条是否被初始化
	XBool m_withoutTex;	//没有贴图的形式
	char m_mode;	//0普通模式 1竖屏模式
	XResourceInfo *m_resInfo;

	bool m_withVirtual;			//是否使用虚拟的数据进行中间过渡
	XMoveData m_valueMD;
public:
	XBool init(const XVec2& position,//控件所在的位置
		const XRect& Area,	//进度条区域的范围
		const XProgressSkin &tex,	//控件的贴图
		const XFontUnicode* font,float captionSize,const XVec2& textPosition,	//控件的文字
		int mode = 0);
	XBool initEx(const XVec2& position,//对上面接口的简化
		const XProgressSkin &tex,	
		const XFontUnicode* font,float captionSize = 1.0f,
		int mode = 0);
	XBool initPlus(const char * path,
		const XFontUnicode* font,float captionSize = 1.0f,	//控件的文字
		const int mode = 0,XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,
		const XFontUnicode* font = NULL,float captionSize = 1.0f,	//控件的文字
		const XVec2& textPosition = XVec2::zero,int mode = 0);
	XBool initWithoutSkin(const XVec2& pixelSize,float captionSize = 1.0f,int mode = 0)
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),
			&getDefaultFont(),captionSize,pixelSize * 0.5f,mode);
	}

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色

protected:
	void update(float stepTime){m_valueMD.move(stepTime);}
	void draw();
	void drawUp(){};						//do nothing
	XBool mouseProc(const XVec2&,XMouseState){return XTrue;}	//do nothing
	XBool keyboardProc(int,XKeyState){return XTrue;}	//do nothing
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2&){return XFalse;}	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return XTrue;}
public:
	void setValue(float temp);
	float getValue() const;

	XBool setACopy(const XProgress &temp);		//建立一个副本
	XProgress();
	~XProgress(){release();}
	void release();
	//下面是内联函数
	void enable();
	void disable();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	bool getWithVirtual()const{return m_withVirtual;}
	void setWithVirtual(bool flag){m_withVirtual = flag;}
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XProgress(const XProgress &temp);
	XProgress& operator = (const XProgress& temp);

	//virtual void justForTest() {}
};
#if WITH_INLINE_FILE
#include "XProgress.inl"
#endif
}
#endif