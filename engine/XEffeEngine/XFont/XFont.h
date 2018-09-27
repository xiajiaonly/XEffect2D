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
namespace XE{
#ifndef TEXT_FONT_MODE
#define TEXT_FONT_MODE " !\"#$%&\'()*+,-./0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#endif

class XFont:public XFontBasic
{
private:
	void updateData();			//根据需要更新内部数据
	std::vector<XVec2> m_textPosition;	//用于保存每个字符的位置
	std::vector<XRect> m_textRect;			//用于保存每个字符的剪切盒子
	XVec2 m_layout;		//字体图片的布局

public:
	XBool init(const char *filename,	//字体图片的文件名
		const XVec2& size,		//字体的像素大小
		const XVec2& layout,		//字体图片的布局
		XResPos resPos = RES_SYS_DEF,XBool withFBO = XFalse);		
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF,XBool withFBO = XFalse);	//精简接口,从资源中读取数据
	XBool initFromTTF(const char * filename,int fontSize,XResPos resPos = RES_SYS_DEF,XBool withFBO = XFalse);	//从TTF文件中初始化(尚未实现)

	XFont& operator = (const XFont& temp);
	XBool setACopy(const XFont &temp);
	XFont(const XFont &temp);	//拷贝构造函数

	void draw();
	XFont();
	XFont(int maxStrLen);
	~XFont();
	void setMaxStrLen(int maxStrLen)
	{
		XMem::XDELETE_ARRAY(m_string);

		if(maxStrLen < 2) m_maxStringLen = 2;
		else m_maxStringLen = maxStrLen;
		m_string = XMem::createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
		m_textPosition.resize(m_maxStringLen);
		m_textRect.resize(m_maxStringLen);

		m_needUpdateData = XTrue;
	}
private:
	bool loadFromFolder(const char *filename);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源
};
#if WITH_INLINE_FILE
#include "XFont.inl"
#endif
}
#endif