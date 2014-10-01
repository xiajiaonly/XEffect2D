#ifndef _JIA_XCHECK_
#define _JIA_XCHECK_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//这是一个用于实现复选框的类

#include "XControlBasic.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

#define XCHECK_LEFT_ADD_LENGTH 15	//在字符后面加上这个像素长度作为响应范围，为了使控制更加合理

class _XCheckTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *checkChoosed;				//选择按钮选中的贴图
	_XTextureData *checkDischoose;				//选择按钮未选中的贴图
	_XTextureData *checkDisableChoosed;			//无效状态下选择按钮选中的贴图
	_XTextureData *checkDisableDischoose;		//无效状态下选择按钮未选中的贴图

	_XRect m_mouseRect;			//鼠标的响应范围
	_XVector2 m_fontPosition;	//放置文字的位置

	_XCheckTexture();
	~_XCheckTexture(){release();}
	_XBool init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};
class _XRadios;
class _XMultiListBasic;
class _XDirectoryList;
class _XChart;
class _XCheck:public _XControlBasic,public _XBasicOprate
{
	friend _XRadios;
	friend _XMultiListBasic;
	friend _XDirectoryList;
	friend _XChart;
private:
	_XBool m_isInited;					//进度条是否被初始化
	_XFontUnicode m_caption;			//进度条的标题

	const _XTextureData *m_checkChoosed;			//选择按钮选中的贴图
	const _XTextureData *m_checkDischoose;		//选择按钮未选中的贴图
	const _XTextureData *m_checkDisableChoosed;	//无效状态下选择按钮选中的贴图
	const _XTextureData *m_checkDisableDischoose;	//无效状态下选择按钮未选中的贴图

	void (*m_funInit)(void *,int ID);				//控件初始化的时候调用，（这个目前没有实际生效）
	void (*m_funRelease)(void *,int ID);				//控件注销的时候调用，（这个目前没有实际生效）
	void (*m_funMouseOn)(void *,int ID);				//鼠标悬浮时调用，（这个目前没有实际生效）
	void (*m_funMouseDown)(void *,int ID);			//鼠标按下时调用，（这个目前没有实际生效）		
	void (*m_funMouseUp)(void *,int ID);				//鼠标弹起时调用
	void (*m_funStateChange)(void *,int ID);			//控件状态改变时调用
	void *m_pClass;				//回调函数的参数

	_XSprite m_sprite;			//用于显示贴图的精灵
	_XVector2 m_textPosition;		//文字的相对位置
	_XVector2 m_textSize;			//文字的尺寸
	_XFColor m_textColor;		//文字的颜色

	_XBool m_state;				//复选框的选择状态
	_XRect m_mouseClickArea;	//字体的鼠标响应范围
	_XBool m_withCaption;			//是否拥有文字
		//下面是关于关联变量的接口
	_XBool *m_pVariable;	//关联的变量

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
public:
	void setConnectVar(_XBool * p) {m_pVariable = p;}	//关联变量
	void disConnectVar() {m_pVariable = NULL;}			//取消变量关联

public:
	_XBool getWithCaption() const {return m_withCaption;}
	void setWithCaption(_XBool withCaption) {m_withCaption = withCaption;}
	_XBool init(const _XVector2 & position,	//控件的位置
		const _XRect &Area,					//控件的图片的响应范围
		const _XCheckTexture &tex,			//控件的贴图
		const char *caption,const _XFontUnicode &font,float captionSize,	//显示的字体的相关信息
		const _XVector2 &textPosition);		//字体的位置
	_XBool initEx(const _XVector2 & position,	//对上面接口的简化
		const _XCheckTexture &tex,			
		const char *caption,const _XFontUnicode &font,float captionSize = 1.0f);
	_XBool initPlus(const char * path,			//控件的贴图的文件夹路径
		const char *caption,const _XFontUnicode &font,float captionSize = 1.0f,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const char *caption,				//文字
		const _XFontUnicode &font,float captionSize,	//字体以及尺寸
		const _XRect& area,const _XVector2 &captionPosition);	//空间的大小以及文字的相对位置
	_XBool initWithoutTex(const char *caption,			//这个是上一个接口的简化版本
		const _XFontUnicode &font,const _XRect& area);
	_XBool initWithoutTex(const char *caption,const _XRect& area) {return initWithoutTex(caption,XEE::systemFont,area);}
	_XBool initWithoutTex(const char *caption,const _XVector2& pixelSize) 
	{
		return initWithoutTex(caption,XEE::systemFont,_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y));
	}
	_XBool initWithoutTex(const char *caption) 
	{
		return initWithoutTex(caption,XEE::systemFont,_XRect(0.0f,0.0f,32.0f,32.0f));
	}
protected:
	void draw();
	void drawUp();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数(返回动作是否造成改变)
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//对键盘动作的响应
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}
public:
	void setCallbackFun(void (* funInit)(void *,int),
						 void (* funRelease)(void *,int),
						 void (* funMouseOn)(void *,int),
						 void (* funMouseDown)(void *,int),
						 void (* funMouseUp)(void *,int),
						 void (*funStateChange)(void *,int),void *pClass = NULL);
	void setStateChangeCB(void (*funStateChange)(void *,int),void *pClass = NULL);

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);//设置控件的位置

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a); 	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色

	void setCaptionText(const char *temp);			//设置复选框的文字
	const char *getCaptionString() const {return m_caption.getString();}
	_XBool setACopy(const _XCheck &temp);			//复制一个副本
	_XCheck();
	~_XCheck(){release();}
	void release();
	void disable();
	void enable();
	_XBool getState() const;
	void setState(_XBool temp);
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	//virtual void justForTest() {;}
private:	//这里重载复制构造函数和赋值操作符，防止意外调用的错误
	_XCheck(const _XCheck &temp);
	_XCheck& operator = (const _XCheck& temp);
public:
	void setOprateState(void * data)
	{
		_XBool index = *(_XBool *)data;
		setState(index);
	}
	void *getOprateState() const
	{
		_XBool *data = createMem<_XBool>();
		*data =  getState();
		return data;
	}
	virtual bool isSameState(void * data)
	{
		if(data == NULL) return false;
		return(*(_XBool*)data == getState());
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	//_XMoveData m_actionMoveData;	//动态效果的变量
	_XVector2 m_oldPos;				//动作播放时的位置
	_XVector2 m_oldSize;			//动作播放时的大小
	_XMoveData m_lightMD;
	_XRect m_lightRect;
public:
protected:
	void update(int stepTime);
	//---------------------------------------------------------

};
#include "XCheck.inl"

#endif