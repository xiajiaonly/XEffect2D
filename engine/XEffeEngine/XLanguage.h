#ifndef _JIA_XLANGUAGE_
#define _JIA_XLANGUAGE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
#include "XOSDefine.h"
#include <string>
namespace XE{
enum XLanguage
{
	XLGG_CN,	//中文
	XLGG_EN,	//英文
};
class XLanguageBasic
{
protected:
	XBool m_isInited;
	XLanguage m_currentLanguage;			//当前的语言
	virtual XBool loadFromFile(const std::string &filename) = 0;	//从文件中读取字符串
	virtual XBool saveToFile(const std::string &filename) = 0;	//从文件中读取字符串
public:
	virtual XBool init(XLanguage language,const std::string &filename)
	{
		if(!setCurrentLanguage(language,filename)) return XFalse;
		m_isInited = XTrue;
		return XTrue;
	}
	virtual XBool setCurrentLanguage(XLanguage language,const std::string &filename) = 0;		//设置当前的语言
	virtual XLanguage getCurrentLanguage() const {return m_currentLanguage;}

	XLanguageBasic()
		:m_isInited(XFalse)
		,m_currentLanguage(XLGG_CN)
	{}
	virtual ~XLanguageBasic(){}
};
}
#endif