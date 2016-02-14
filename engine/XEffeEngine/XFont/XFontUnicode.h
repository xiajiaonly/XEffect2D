#ifndef _JIA_XFONTUNICODE_
#define _JIA_XFONTUNICODE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XFontBasic.h"
//注意汉字需要排序，否则将不能正确显示。
namespace XE{
//建议将一些字体属性的内容放在字体文本文件当中
//注意页面值在代码中被限定最多不能超过100页
//这个字符的类使用一张完整的贴图
#define MAX_FONT_UNICODE_FILE_NAME_LENGTH (256)	//字符串图片的文件名的最大长度
#define UNICODE_FONT_PAGE_SUM (30)	//款字符串的页面数量(10)
#if WITH_FULL_ALL_CHINESE == 0
#define MAX_UNICODE_TEXT_SUM (8000)	//字体库中字体元素的数量上限(16384)
#endif
#if WITH_FULL_ALL_CHINESE == 1
#define MAX_UNICODE_TEXT_SUM (50000)	//字体库中字体元素的数量上限(16384)
#endif
#if WITH_FULL_ALL_CHINESE == 2
#define MAX_UNICODE_TEXT_SUM (256)	//字体库中字体元素的数量上限(16384)
#endif
#define UNICODE_BYTES_WIDTH (2)		//宽字节的字宽度为2

//给字体增加裁减功能
class XFontUnicode:public XFontBasic
{
private:
	XSCounter *m_cp;	//引用计数器

	int m_pageW;			//字体页面像素尺寸
	int m_pageH;
	char *m_fontPageText;	//字体库页面中字体的排布文本
	int m_libFontSum;		//字体库中实际的元素数量

	void updateData();		//根据需要更新内部数据
	XVector2 *m_textPosition;	//用于保存每个字符的位置
	XRect *m_textRect;		//用于保存每个字符的剪切盒子
	int *m_textPageOrder;	//显示的字符图片的序列编号

	XTextureData *m_texture;		//字体的贴图
	int m_pageSum;
	XVector2 m_layout;		//字体图片的布局

public:
	XBool init(const char *filenameIn,	//字体图片的文件名
		const XVector2 &size,			//字体的像素大小
		const XVector2 &layout,		//字体图片的布局
		int pageSum,			//字体库的页面数量
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,XBool withFBO = XFalse);	
	XBool initEx(const char *filenameIn,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,XBool withFBO = XFalse);	//简化版本，从资源中读取相关数据
	XBool initFromTTF(const char *filenameIn,int fontSize,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,bool withOutLine = false,XBool withFBO = XFalse);	//从TTF中初始化（尚未实现）

	int getStringLengthPix();	//获取字符串的像素宽度(为了向下兼容，任然保留这个接口，实际上已经有新的接口来替代)
private:
	bool loadFromFolder(const char *filename);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源
	bool loadInfoFromFolder(const char *filename,char *target);	//从文件夹中载入资源
	bool loadInfoFromPacker(const char *filename,char *target);	//从压缩包中载入资源
	bool loadInfoFromWeb(const char *filename,char *target);		//从网页中读取资源
	//char m_isAcopy;				//是否为一个备份
	int getTextIndex(const char p[UNICODE_BYTES_WIDTH]);	//找到指定的字符所在的位置
public:
	//建立temp的一个副本，自身没有资源，是用到的资源依赖于temp，temp的资源释放之后，就会出错
	XFontUnicode& operator = (const XFontUnicode& temp);
	XBool setACopy(const XFontUnicode & temp);
	XFontUnicode(const XFontUnicode & temp);	//拷贝构造函数

	void release(){}	//保持向下兼容，其实什么也不干
	void setRotateBasePoint(float x,float y);
	void draw();
	XFontUnicode();
	XFontUnicode(int maxStrLen);
	virtual ~XFontUnicode();
	//改变字符串的最大长度
	void setMaxStrLen(int maxStrLen);
};
#if WITH_INLINE_FILE
#include "XFontUnicode.inl"
#endif
}
#endif