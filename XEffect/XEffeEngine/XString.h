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
	char m_isInited;
	int *m_shift;
	int m_pattSize;
	char * m_patt;
public:
	int init(const char *patt);
	int search(const char *text,int &position);
	_XSunday()
	:m_isInited(0)
	,m_patt(NULL)
	{
		m_shift = new int[MAX_CHAR_SUM];
	}
	~_XSunday();
};

extern bool sundaySearch(const char *text,const char *patt,int &position);
extern bool kmpSearch(const char *text,const char *m,int &pos);

#endif