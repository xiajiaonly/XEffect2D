#ifndef _JIA_XGLFONT_
#define _JIA_XGLFONT_
#include "XMath\XFColor.h"
#include "windows.h"
namespace XE {
class XGLFont
{
public:
	void drawStr(const XVec2& pos, const std::string& strText, const XFColor& c = XFColor::white);
	void drawStrOutLine(const XVec2& pos, const std::string& strText, float z = 0.2f, const XFColor& c = XFColor::white);
	void create(const char *typeface = "宋体", int Height = 20, int BOLD = FW_BOLD, int italic = 0);

	XGLFont()
		:m_isInited(false)
		, m_hFont(nullptr)
	{}
	virtual ~XGLFont() { release(); }
	void release();
protected:
	bool m_isInited;
	float m_height;
	HFONT m_hFont;		//字体句柄
	XBuffer m_buff;		//内存大小
};
}

#endif
