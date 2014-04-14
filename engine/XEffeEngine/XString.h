#ifndef _JIA_XSTRING_
#define _JIA_XSTRING_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

//×Ö·û´®Æ¥ÅäµÄÀà
#define MAX_CHAR_SUM (256)

class _XSunday
{
private:
	_XBool m_isInited;
	int *m_shift;
	int m_pattSize;
	char * m_patt;
public:
	_XBool init(const char *patt);
	_XBool search(const char *text,int &position);
	_XSunday()
	:m_isInited(XTrue)
	,m_patt(NULL)
	{
		m_shift = createArrayMem<int>(MAX_CHAR_SUM);
	}
	~_XSunday();
};

extern _XBool sundaySearch(const char *text,const char *patt,int &position);
extern _XBool kmpSearch(const char *text,const char *m,int &pos);

#endif