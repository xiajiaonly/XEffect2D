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

#ifndef TEXT_FONT_MODE
#define TEXT_FONT_MODE (" !\"#$%&\'()*+,-./0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~")
#endif

class _XFont:public _XFontBasic
{
private:
	void updateData();			//根据需要更新内部数据
	_XVector2 *m_textPosition;	//用于保存每个字符的位置
	_XRect *m_textRect;			//用于保存每个字符的剪切盒子
	_XVector2 m_layout;		//字体图片的布局

public:
	_XBool init(const char *filename,	//字体图片的文件名
		const _XVector2& size,		//字体的像素大小
		const _XVector2& layout,		//字体图片的布局
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse);		
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse);	//精简接口,从资源中读取数据
	_XBool initFromTTF(const char * filename,int fontSize,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse);	//从TTF文件中初始化(尚未实现)

	_XFont& operator = (const _XFont& temp);
	_XBool setACopy(const _XFont &temp);
	_XFont(const _XFont &temp);	//拷贝构造函数

	void draw();
	_XFont();
	_XFont(int maxStrLen);
	~_XFont();
	void setMaxStrLen(int maxStrLen)
	{
		XDELETE_ARRAY(m_string);
		XDELETE_ARRAY(m_textPosition);
		XDELETE_ARRAY(m_textRect);

		if(maxStrLen < 2) m_maxStringLen = 2;
		else m_maxStringLen = maxStrLen;
		m_string = createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
		m_textPosition = createArrayMem<_XVector2>(m_maxStringLen);
		m_textRect = createArrayMem<_XRect>(m_maxStringLen);

		m_needUpdateData = XTrue;
	}
};

#include "XFont.inl"

#endif