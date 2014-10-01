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

class _XImageList:public _XControlBasic
{
private:
	_XBool m_isInited;			

	void (*m_funInit)(void *,int ID);
	void (*m_funRelease)(void *,int ID);
	void (*m_funSelectChange)(void *,int ID);
	void *m_pClass;				//回调函数的参数
protected:
	void draw();	
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);				//对于鼠标动作的响应函数
	_XBool keyboardProc(int,_XKeyState){return XFalse;}	//返回是否触发按键动作
private:
	float m_buttonWidth;	//左右按钮的宽度
	_XVector2 m_imageSize;	//列表中的图片宽度
	int m_showImageSum;
	int m_nowSelectImageIndex;	//当前选择的图片编号
	int m_nowShowImageIndex;	//当前显示的图片的起始编号

	_XButtonEx m_leftBtn;	//左边的按钮
	_XButtonEx m_rightBtn;	//右边的按钮
	_XSlider m_imageSld;	//滑动条

	std::deque<_XSprite *> m_imageList;	//图片的序列
	void updateState(bool flag = true);	//根据实际的数据更新界面数据

	friend void imageListCtrCB(void * pClass,int id);
public:
	_XBool initWithoutTex(float buttonWidth,	//左右按钮的宽度
		const _XVector2 &imageSize,	//图片的尺寸
		int showImageSum);	//显示图片的数量
	_XBool addImage(const char * filename);
	_XBool addImage(const _XTextureData &tex);	//尚未实现
	_XBool addImage(const _XSprite &sprite);	//尚未实现

	_XImageList()
		:m_isInited(XFalse)
		,m_funInit(NULL)
		,m_funRelease(NULL)
		,m_funSelectChange(NULL)
		,m_pClass(NULL)
	{
		m_ctrlType = CTRL_OBJ_IMAGELIST;
	}
	~_XImageList() {release();}
	void release();
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor&) {;}	//设置字体的颜色
	_XFColor getTextColor() const {return _XFColor::white;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funSelectChange)(void *,int),
		void *pClass = NULL);
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}
	//void setLostFocus();	//设置失去焦点
	void disable();
	void enable();
	void setVisible();
	void disVisible();
	_XBool setACopy(const _XImageList &)			//建立一个副本
	{//尚未实现
		return XFalse;
	}

	//virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XImageList(const _XImageList &temp);
	_XImageList& operator = (const _XImageList& temp);

};

#include "XImageList.inl"

#endif