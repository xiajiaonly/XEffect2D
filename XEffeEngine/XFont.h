#ifndef _JIA_XFONT_
#define _JIA_XFONT_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//有话说明，由于每次字符串内容发生改变时都需要对字符串的内部数据进行更新，这样较慢，可以保存上一次的字符串内容，比较不同的部分才进行更新。
//也可以直接将字符串通过FBO生成为一张贴图，直接在通过精灵一次性显示就不需要每次都进行计算，
//不过存在两个问题，1、内存消耗较大，2、贴图的尺寸会由于文字的内容而发生变化
#include "XFontBasic.h"

class _XFont:public _XFontBasic
{
private:
	//+++++++++++++++++++++++++++
	//给字体增加裁减功能
	_XRect m_clipRect;	//字体裁减的矩型是相对于字体坐上角的一个相对矩形
	char m_isCliped;
public:
	void setClipRect(_XRect temp);
	void setClipRect(float left,float top,float right,float bottom);
	void disClip();
	//---------------------------
private:
	void updateData();			//根据需要更新内部数据
	_XVector2 *m_textPosition;	//用于保存每个字符的位置
	_XRect *m_textRect;		//用于保存每个字符的剪切盒子
private:
	_XVector2I m_layout;		//字体图片的布局
	char m_isPassword;
	char m_isVisiable;
public:
	void setVisiable() {m_isVisiable = 1;}					//设置物件可见
	void disVisiable() {m_isVisiable = 0;}						//设置物件不可见
	char getVisiable() const {return m_isVisiable;}					//获取物件是否可见的状态 
	void setIsPassWord(int temp)
	{
		if(temp != 0) temp = 1;
		if(m_isPassword != temp)
		{
			m_isPassword = temp;
			m_needUpdateData = 1;
		}
	}
	char getIsPassword() const
	{
		return m_isPassword;
	}
public:
	int init(const char *filename,	//字体图片的文件名
		const _XVector2I& size,		//字体的像素大小
		const _XVector2I& layout,		//字体图片的布局
		int resoursePosition = 0);		
	int init(const char *filename,int resoursePosition = 0);	//精简接口,从资源中读取数据

	_XFont& operator = (const _XFont& temp);
	int setACopy(const _XFont &temp);
	_XFont(const _XFont &temp);	//拷贝构造函数

	void draw();
	_XFont();
	~_XFont();

	void setColor(float r,float g,float b,float a);
};

#include "XFont.inl"

#endif