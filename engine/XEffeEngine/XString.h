#ifndef _JIA_XSTRING_
#define _JIA_XSTRING_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
namespace XE{
//字符串匹配的类
class XSunday
{
private:
	static const int m_maxCharsSum = 256;
	XBool m_isInited;
	int *m_shift;
	int m_pattSize;
	char * m_patt;
public:
	XBool init(const char *patt);
	XBool search(const char *text, int &position);
	XSunday();
	~XSunday();
};
namespace XString
{
	//使用sunday算法在字符串中查找制定的字符串
	extern XBool sundaySearch(const char *text,const char *patt,int &position);
	//使用kmp算法在字符串中查找制定的字符串
	extern XBool kmpSearch(const char *text,const char *m,int &pos);
	static std::string gNullStr = "";
}
}
#endif