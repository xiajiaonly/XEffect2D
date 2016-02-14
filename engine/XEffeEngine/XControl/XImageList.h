#ifndef _JIA_XIMAGELIST_
#define _JIA_XIMAGELIST_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.5
//--------------------------------
//这是一个图形陈列的控件
#include "XButtonEx.h"
#include "XSlider.h"
#include <deque>
namespace XE{
class XImageList:public XControlBasic
{
private:
	XBool m_isInited;			
public:
	enum XImageListEvent
	{
		IMGLST_INIT,
		IMGLST_RELEASE,
		IMGLST_SELECT_CHANGE,
	};
//	void (*m_funInit)(void *,int ID);
//	void (*m_funRelease)(void *,int ID);
//	void (*m_funSelectChange)(void *,int ID);
//	void *m_pClass;				//回调函数的参数
protected:
	void draw();	
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);				//对于鼠标动作的响应函数
	XBool keyboardProc(int,XKeyState){return XFalse;}	//返回是否触发按键动作
private:
	float m_buttonWidth;	//左右按钮的宽度
	XVector2 m_imageSize;	//列表中的图片宽度
	int m_showImageSum;
	int m_curSelectImageIndex;	//当前选择的图片编号
	int m_curShowImageIndex;	//当前显示的图片的起始编号

	XButtonEx m_leftBtn;	//左边的按钮
	XButtonEx m_rightBtn;	//右边的按钮
	XSlider m_imageSld;	//滑动条

	std::deque<XSprite *> m_imageList;	//图片的序列
	void updateState(bool flag = true);	//根据实际的数据更新界面数据

	//static void imageListCtrCB(void * pClass,int id);
	static void ctrlProc(void *,int,int);
public:
	XBool initWithoutSkin(float buttonWidth,	//左右按钮的宽度
		const XVector2 &imageSize,	//图片的尺寸
		int showImageSum);	//显示图片的数量
	XBool addImage(const char * filename);
	XBool addImage(const XTextureData &tex);	//尚未实现
	XBool addImage(const XSprite &sprite);	//尚未实现

	XImageList()
		:m_isInited(XFalse)
//		,m_funInit(NULL)
//		,m_funRelease(NULL)
//		,m_funSelectChange(NULL)
//		,m_pClass(NULL)
	{
		m_ctrlType = CTRL_OBJ_IMAGELIST;
	}
	~XImageList() {release();}
	void release();
	XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y);

	void setTextColor(const XFColor&) {;}	//设置字体的颜色
	XFColor getTextColor() const {return XFColor::white;}	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funSelectChange)(void *,int),
		void *pClass = NULL);
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(float,float){return XTrue;}
	//void setLostFocus();	//设置失去焦点
	void disable();
	void enable();
	void setVisible();
	void disVisible();
	XBool setACopy(const XImageList &)			//建立一个副本
	{//尚未实现
		return XFalse;
	}

	//virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XImageList(const XImageList &temp);
	XImageList& operator = (const XImageList& temp);
};
#if WITH_INLINE_FILE
#include "XImageList.inl"
#endif
}
#endif