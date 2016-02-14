#ifndef _JIA_XRADIOS_
#define _JIA_XRADIOS_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//这是一个单选框的类，这个类和MFC是有区别的。
//1、单选框不能自动组队需要在初始化的时侯设定，一组单选框中的单选项数量
//2、单个的单选框实际上是就是单个的复选框，实际上是没有区别的

//控件的激活方式需要修改

//整体的移动和缩放，统一使用相对于组空间基本点的移动和缩放
#include "XCheck.h"
namespace XE{
typedef XCheckSkin XRadiosSkin;

class XRadios:public XControlBasic,public XBasicOprate
{
private:
	XBool m_isInited;	//空间时都已经初始化

	int m_radioSum;		//选项的数量
	int m_curChoose;	//当前所选的项的编号
	XCheck *m_radio;	//所有单选项的指针
	XVector2 *m_checkPosition;	//单选项的相对位置
	XVector2 m_distance;	//设置的每个单选项之间的距离
public:
	enum XRadiosEvent
	{
		RDS_STATE_CHANGE,
	};
private:
//	void (*m_funStateChange)(void *,int);			//控件状态改变时调用
//	void *m_pClass;				//回调函数的参数

	const XCheckSkin *m_texture;

	XFontUnicode m_caption;
	float m_captionSize;
	XFColor m_textColor;	//显示的字体的颜色
	XVector2 m_textPosition;

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式
public:
	XBool init(int radioSum,			//选项的数量
		const XVector2& distance,	//每个单选项之间的距离
		const XVector2& position,	//控件的位置
		const XRect &Area,			//选择图标的鼠标响应范围
		const XRadiosSkin* tex,const XFontUnicode &font,float captionSize,
		const XVector2& textPosition);		//单选框初始化
	XBool initEx(int radioSum,			//对上面接口的简化
		const XVector2& distance,	
		const XVector2& position,	
		const XRadiosSkin* tex,const XFontUnicode &font,float captionSize = 1.0);
	XBool initPlus(int radioSum,			//选项的数量
		const XVector2& distance,	//每个单选项之间的距离
		const char *path,const XFontUnicode &font,float captionSize = 1.0f,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initWithoutSkin(int radioSum,
		const XVector2& distance,
		const XRect &area,
		const XFontUnicode &font,float captionSize,
		const XVector2& textPosition);
	XBool initWithoutSkin(int radioSum,
		const XVector2& distance,
		const XRect &area,
		const XFontUnicode &font,
		const XVector2& textPosition)
	{
		return initWithoutSkin(radioSum,distance,area,font,1.0f,textPosition);
	}
	XBool initWithoutSkin(int radioSum,
		const XVector2& distance,
		const XRect &area,
		const XVector2& textPosition)
	{
		return initWithoutSkin(radioSum,distance,area,getDefaultFont(),1.0f,textPosition);
	}
	XBool initWithoutSkin(int radioSum,
		const XVector2& distance,
		const XVector2 &pixelSize,
		const XVector2& textPosition)
	{
		return initWithoutSkin(radioSum,distance,XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			getDefaultFont(),1.0f,textPosition);
	}
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);	//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//当处于激活状态的时候，可以通过上下左右4个按键改变选择的值
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(float,float){return XTrue;}
public:
	void disable();
	void enable();
	void setChoosed(int temp);									//设置当前选择的单选项
	void setRadioPosition(const XVector2& position,int order);			//设置单选项中某一项的位置(这里使用相对坐标)
	void setRadioPosition(float x,float y,int order);			//设置单选项中某一项的位置(这里使用相对坐标)
	
	void setDistance(const XVector2& distance);
	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(float x,float y);				//设置缩放比例

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);		//设置位置

	void setTextColor(const XFColor& color);//设置字体的颜色
	XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);//设置按钮的颜色
	void setAlpha(float a);//设置按钮的颜色

	XBool setACopy(const XRadios &temp);			//设置一个副本
	XBool setRadioSum(int radioSum);

	XRadios();
	~XRadios(){release();}
    //下面是内联函数
	void release();	//释放分配的资源
	//void setCallbackFun(void (* funStateChange)(void *,int),void *pClass = NULL);
	int getCurChoose() const;	//返回当前单选项中选择的项的编号
	void setRadioText(const char *temp,int order);	//设置单选项中某一项的文字
	void setRadioState(bool state,int order);	//设置单个选项是否可选
	void setRadiosText(const char * temp);			//设置多项的值，每项之间用';'隔开,如果总项数不匹配，则自动匹配
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//virtual void justForTest() {}
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XRadios(const XRadios &temp);
	XRadios& operator = (const XRadios& temp);
public:
	void setOprateState(void * data)
	{
		int index = *(int *)data;
		setChoosed(index);
	}
	void *getOprateState() const
	{
		int *data = XMem::createMem<int>();
		*data =  getCurChoose();
		return data;
	}
	void releaseOprateStateData(void *p)
	{
		int *data = (int*)p;
		XMem::XDELETE(data);
	}
	virtual bool isSameState(void * data)
	{
		if(data == NULL) return false;
		return(*(int*)data == getCurChoose());
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),XString::toString(getCurChoose()))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		int tmp;
		if(XXml::getXmlAsInt(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setChoosed(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XRadios.inl"
#endif
}
#endif