#include "XStdHead.h"
#include "XGLFont.h"

namespace XE {
void XGLFont::create(const char *typeface, int Height, int BOLD, int italic)
{
	LOGFONT logfont; //逻辑字体
	// 设置字体特征
	m_height = Height;
	logfont.lfHeight = Height;		//字高
	logfont.lfWidth = 0;			//字宽
	logfont.lfEscapement = 0;		//字符串角度(1/10度)
	logfont.lfOrientation = 0;
	logfont.lfWeight = NULL;		//粗体
	logfont.lfItalic = italic;		//斜体
	logfont.lfUnderline = FALSE;	//下划线
	logfont.lfStrikeOut = FALSE;	//删除线
	logfont.lfCharSet = DEFAULT_CHARSET;			//OEM_CHARSET; //ANSI_CHARSET;
	logfont.lfOutPrecision = OUT_TT_PRECIS;	//OUT_DEFAULT_PRECIS;	//OUT_TT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = ANTIALIASED_QUALITY;
	logfont.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;
	//logfont.lfFaceName[0] = NULL;
	strcpy(logfont.lfFaceName, typeface); //字体名称
	// 创建字体
	if (m_isInited) release();
	m_hFont = CreateFontIndirect(&logfont);
	m_isInited = true;
}
void XGLFont::release()
{
	if (!m_isInited) return;
	if (m_hFont != nullptr)
	{
		DeleteObject(m_hFont);
		m_hFont = nullptr;
	}
	m_isInited = false;
}
void XGLFont::drawStr(const XVec2& pos, const std::string& strText, const XFColor& c)
{
	glColor4fv(c);
	glRasterPos2fv(pos);

	if (!m_hFont) m_hFont = (HFONT)::GetStockObject(SYSTEM_FONT);	//字体

	HDC hdc = wglGetCurrentDC();	//设备场景
	HFONT hOldFont = (HFONT)::SelectObject(hdc, m_hFont);	//将字体选入场景
//	int nSmooth = 1000, nOldSmooth;
//	SystemParametersInfo(SPI_GETFONTSMOOTHINGCONTRAST, 0, &nOldSmooth, FALSE);
//	SystemParametersInfo(SPI_SETFONTSMOOTHINGCONTRAST, 0, &nSmooth, FALSE);
//	SetBkMode(hdc, TRANSPARENT);

	const char* pStr = &strText[0];
	int len = 0;
	for (int i = 0; i < strText.size(); ++i)
	{
		if (IsDBCSLeadByte(pStr[i])) //是否双字节(汉字)
			++i;
		++len;
	}
	m_buff.fitSize((len + 1) * sizeof(wchar_t));
	wchar_t* wstring = (wchar_t*)m_buff.getBuffer();

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strText.c_str(), -1, wstring, len);
	wstring[len] = L'\0';

	unsigned int nListNum = glGenLists(1);
	glPushMatrix();
	for (int i = 0; i < len;++ i)
	{
		wglUseFontBitmapsW(hdc, wstring[i], 1, nListNum);
		glCallList(nListNum);
	}
	glPopMatrix();
	glDeleteLists(nListNum, 1);
//	SystemParametersInfo(SPI_SETFONTSMOOTHINGCONTRAST, 0, &nOldSmooth, FALSE);
	::SelectObject(hdc, hOldFont);
}
void XGLFont::drawStrOutLine(const XVec2& pos, const std::string& strText, float z, const XFColor& c)
{
	glColor4fv(c);

	if (!m_hFont) m_hFont = (HFONT)::GetStockObject(SYSTEM_FONT);	//字体

	HDC hdc = wglGetCurrentDC();	//设备场景
	HFONT hOldFont = (HFONT)::SelectObject(hdc, m_hFont);	//将字体选入场景
	const char* pStr = &strText[0];
	int len = 0;
	for (int i = 0; i < strText.size(); ++i)
	{
		if (IsDBCSLeadByte(pStr[i])) //是否双字节(汉字)
			++i;
		++len;
	}

	m_buff.fitSize((len + 1) * sizeof(wchar_t));
	wchar_t* wstring = (wchar_t*)m_buff.getBuffer();
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strText.c_str(), -1, wstring, len);
	wstring[len] = L'\0';

	GLYPHMETRICSFLOAT pgmf[1];
	unsigned int nListNum = glGenLists(1);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0.0f);
	glScalef(m_height, -m_height, 1.0f);
	for (int i = 0; i < len; ++i)
	{
		//wglUseFontOutlinesW(hdc, wstring[i], 1, nListNum, 0.0f, z, WGL_FONT_LINES, pgmf);
		wglUseFontOutlinesW(hdc, wstring[i], 1, nListNum, 0.0f, z, WGL_FONT_POLYGONS, pgmf);
		glCallList(nListNum);
	}
	glPopMatrix();
	glDeleteLists(nListNum, 1);
	::SelectObject(hdc, hOldFont);
}
}