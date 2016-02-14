#ifndef _JIA_XCHECK_
#define _JIA_XCHECK_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//这是一个用于实现复选框的类
#include "XControlBasic.h"
#include "XResourceManager.h"
#include "../XMath/XMoveData.h"
#include "../XOprateDes.h"
#include "../XXml.h"
namespace XE{
class XCheckSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *checkChoosed;				//选择按钮选中的贴图
	XTextureData *checkDischoose;				//选择按钮未选中的贴图
	XTextureData *checkDisableChoosed;			//无效状态下选择按钮选中的贴图
	XTextureData *checkDisableDischoose;		//无效状态下选择按钮未选中的贴图

	XRect m_mouseRect;			//鼠标的响应范围
	XVector2 m_fontPosition;	//放置文字的位置

	XCheckSkin();
	~XCheckSkin(){release();}
	XBool init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initEx(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
private:
	bool loadFromFolder(const char *filename,XResourcePosition resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResourcePosition resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename,XResourcePosition resPos);		//从网页中读取资源
};
class XRadios;
class XMultiListBasic;
class XDirectoryList;
class XChart;
class XPropertyLine;
class XCheck:public XControlBasic,public XBasicOprate
{
	friend XRadios;
	friend XMultiListBasic;
	friend XDirectoryList;
	friend XChart;
	friend XPropertyLine;
private:
	//static const int m_checkLeftAddLength = 15;	//在字符后面加上这个像素长度作为响应范围，为了使控制更加合理
	XBool m_isInited;					//进度条是否被初始化
	XFontUnicode m_caption;			//进度条的标题

	const XTextureData *m_checkChoosed;			//选择按钮选中的贴图
	const XTextureData *m_checkDischoose;		//选择按钮未选中的贴图
	const XTextureData *m_checkDisableChoosed;	//无效状态下选择按钮选中的贴图
	const XTextureData *m_checkDisableDischoose;	//无效状态下选择按钮未选中的贴图

	XSprite m_sprite;			//用于显示贴图的精灵
	XVector2 m_textPosition;		//文字的相对位置
	XVector2 m_textSize;			//文字的尺寸
	XFColor m_textColor;		//文字的颜色

	XBool m_state;				//复选框的选择状态
	XRect m_mouseClickArea;	//字体的鼠标响应范围
	XBool m_withCaption;			//是否拥有文字
		//下面是关于关联变量的接口
	XBool *m_pVariable;	//关联的变量

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式
public:
	void setConnectVar(XBool * p) {m_pVariable = p;}	//关联变量
	void disConnectVar() {m_pVariable = NULL;}			//取消变量关联

public:
	XBool getWithCaption() const {return m_withCaption;}
	void setWithCaption(XBool withCaption) {m_withCaption = withCaption;}
	XBool init(const XVector2 & position,	//控件的位置
		const XRect &Area,					//控件的图片的响应范围
		const XCheckSkin &tex,			//控件的贴图
		const char *caption,const XFontUnicode &font,float captionSize,	//显示的字体的相关信息
		const XVector2 &textPosition);		//字体的位置
	XBool initEx(const XVector2 & position,	//对上面接口的简化
		const XCheckSkin &tex,			
		const char *caption,const XFontUnicode &font,float captionSize = 1.0f);
	XBool initPlus(const char * path,			//控件的贴图的文件夹路径
		const char *caption,const XFontUnicode &font,float captionSize = 1.0f,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initWithoutSkin(const char *caption,				//文字
		const XFontUnicode &font,float captionSize,	//字体以及尺寸
		const XRect& area,const XVector2 &captionPosition);	//空间的大小以及文字的相对位置
	XBool initWithoutSkin(const char *caption,			//这个是上一个接口的简化版本
		const XFontUnicode &font,const XRect& area);
	XBool initWithoutSkin(const char *caption,const XRect& area) {return initWithoutSkin(caption,getDefaultFont(),area);}
	XBool initWithoutSkin(const char *caption,const XVector2& pixelSize) 
	{
		return initWithoutSkin(caption,getDefaultFont(),XRect(0.0f,0.0f,pixelSize.x,pixelSize.y));
	}
	XBool initWithoutSkin(const char *caption) 
	{
		return initWithoutSkin(caption, getDefaultFont(), XRect(0.0f, 0.0f, MIN_FONT_CTRL_SIZE, MIN_FONT_CTRL_SIZE));
	}
protected:
	void draw();
	void drawUp();
	XBool mouseProc(float x,float y,XMouseState mouseState);	//对于鼠标动作的响应函数(返回动作是否造成改变)
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//对键盘动作的响应
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(float,float){return XTrue;}
public:
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);//设置控件的位置

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(float x,float y);

	void setTextColor(const XFColor& color);	//设置字体的颜色
	XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a); 	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色

	void setCaptionText(const char *temp);			//设置复选框的文字
	const char *getCaptionString() const {return m_caption.getString();}
	XBool setACopy(const XCheck &temp);			//复制一个副本
	XCheck();
	~XCheck(){release();}
	void release();
	void disable();
	void enable();
	XBool getState() const;
	void setState(XBool temp);
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	//virtual void justForTest() {;}
private:	//这里重载复制构造函数和赋值操作符，防止意外调用的错误
	XCheck(const XCheck &temp);
	XCheck& operator = (const XCheck& temp);
public:
	void setOprateState(void * data)
	{
		XBool index = *(XBool *)data;
		setState(index);
	}
	void *getOprateState() const
	{
		XBool *data = XMem::createMem<XBool>();
		*data =  getState();
		return data;
	}
	void releaseOprateStateData(void *p)
	{
		XBool *data = (XBool*)p;
		XMem::XDELETE(data);
	}
	virtual bool isSameState(void * data)
	{
		if(data == NULL) return false;
		return(*(XBool*)data == getState());
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	//XMoveData m_actionMoveData;	//动态效果的变量
	XVector2 m_oldPos;				//动作播放时的位置
	XVector2 m_oldSize;			//动作播放时的大小
	XMoveData m_lightMD;
	XRect m_lightRect;
protected:
	void update(float stepTime);
	//---------------------------------------------------------
public:
	enum XCheckEvent
	{
		CHK_INIT,
		CHK_RELEASE,
		CHK_MOUSE_ON,
		CHK_MOUSE_DOWN,
		CHK_MOUSE_UP,
		CHK_STATE_CHANGE,
	};
	enum XCheckStyle
	{
		CHK_STYLE_NORMAL,	//打勾模式
		CHK_STYLE_BOX,		//填充模式
		CHK_STYLE_MINI,		//填充圆模式
	};
	XCheckStyle getStyle()const{return m_style;}
	void setStyle(XCheckStyle style){m_style = style;}
private:
	XCheckStyle m_style;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		if(getState())
			return XXml::addLeafNode(e,m_ctrlName.c_str(),true);
		else
			return XXml::addLeafNode(e,m_ctrlName.c_str(),false);
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		bool tmpB;
		if(XXml::getXmlAsBool(e,m_ctrlName.c_str(),tmpB) == NULL) return XFalse;
		setState(tmpB);
		return XTrue;
	}
	//---------------------------------------------------------
};
//namespace XXml
//{
//inline TiXmlNode * getXmlValue(TiXmlNode *node,const char * name,XCheck &chk)
//{//这样整理之后代码虽然变得简洁的，但是效率会受到拖累
//	bool tmp;
//	TiXmlNode *ret = getXmlAsBool(node,name,tmp);
//	if(ret == NULL) return NULL;
//	chk.setState(tmp);
//	return ret;
//}
//inline XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XCheck &chk)
//{//这样整理之后代码虽然变得简洁的，但是效率会受到拖累
//	return addLeafNode(pElmParent,pszNode,(bool)chk.getState());
//}
//};
#if WITH_INLINE_FILE
#include "XCheck.inl"
#endif
}
#endif