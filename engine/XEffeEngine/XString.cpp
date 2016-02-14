#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XString.h"
namespace XE{
//sunday
XBool XSunday::init(const char *patt)
{
	if(patt == NULL ||
		strlen(patt) <= 0) return XFalse;
	if(m_isInited)
	{//if is inited
		XMem::XDELETE_ARRAY(m_patt);
		//if(m_patt != NULL)
		//{
		//	delete [] m_patt;
		//	m_patt = NULL;
		//}
	}
	//init some value
	m_pattSize = strlen(patt);
	m_patt = XMem::createArrayMem<char>(m_pattSize + 1);
	if(m_patt == NULL) return XFalse;

	strcpy(m_patt,patt);
	int i;
	for(i = 0;i < m_maxCharsSum;++ i)
	{
		 *(m_shift + i) = m_pattSize + 1;	//init all value
	}
	for(i = 0;i < m_pattSize;++ i)
	{
		 *(m_shift + (unsigned char)(*(m_patt + i))) = m_pattSize - i; //init the character in the patt
	}
	m_isInited = XTrue;
	return XTrue;
}
XSunday::~XSunday()
{
	XMem::XDELETE_ARRAY(m_patt);
	XMem::XDELETE_ARRAY(m_shift);
}
XBool XSunday::search(const char *text,int &position)
{
	if(!m_isInited ||
		text == NULL ||
		position == NULL ||
		strlen(text) <= 0 ||
		m_pattSize > (int)(strlen(text))) return XFalse;

	int limit = strlen(text) - m_pattSize + 1;
	const char* match_text;
	int match_size;

	for(int i = 0;i < limit;i += m_shift[(unsigned char)(text[i + m_pattSize])])
	{
		if(text[i] == *m_patt)
		{//the first character is match
			match_text = text + i + 1;
			match_size = 1;
			while((*(match_text++)) == m_patt[match_size++])
			{//math the other character
				if(match_size == m_pattSize)
				{//All match
					position = i;
					return XTrue;
				}
			}
		}
	}
	position = 0;
	return XFalse;
}
namespace XString
{
	XBool sundaySearch(const char *text,const char *patt,int &position)
	{
		if(text == NULL ||
			patt == NULL ||
			strlen(patt) <= 0 ||
			strlen(text) <= 0 ||
			strlen(patt) > strlen(text)) return XFalse;
		int patt_size = strlen(patt);
		int shift[256];
		for(int i = 0;i < 256;++ i)
		{
		//	 *(shift + i) = patt_size + 1;	//init all value
			shift[i] = patt_size + 1;
		}
		for(int i = 0;i < patt_size;++ i)
		{
		//	 *(shift + (unsigned char)(*(patt + i))) = patt_size - i; //init the character in the patt
			shift[(unsigned char)patt[i]] = patt_size - i;
		}

		int limit = strlen(text) - patt_size + 1;
		const char* match_text;
		int match_size;

		for(int i = 0;i < limit;i += shift[(unsigned char)(text[i + patt_size])])
		{
			if(text[i] == *patt)
			{//the first character is match
				match_text = text + i + 1;
				match_size = 1;
				while((*(match_text++)) == patt[match_size++])
				{//math the other character
					if(match_size == patt_size)
					{//All match
						position = i;
						return XTrue;
					}
				}
			}
		}
		position = 0;
		return XFalse;
	}
	//kmp
	const int * kmpNext(const char *m) // count the longest prefex string ; 
	{ 
		//static vector<int> next(m.size()); 
		int len = strlen(m);
		int *next = XMem::createArrayMem<int>(len);
		next[0] = 0;	// the initialization of the next[0]; 

		int temp;	// the key iterator...... 

		for(int i = 1;i < len;++ i) 
		{ 
			temp = next[i - 1]; 

			while(m[i] != m[temp] && temp > 0) 
			{ 
				temp = next[temp - 1]; 
			} 

			if(m[i] == m[temp]) 
			{
				next[i] = temp + 1;
			}else 
			{
				next[i] = 0;
			}
		} 
		return next; 
	} 
	XBool kmpSearch(const char *text,const char *m,int &pos) 
	{ 
		if(m == NULL || strlen(m) <= 0 ||
			text == NULL || strlen(text) <= 0) return XFalse;
		int lenM = strlen(m);
		int lenText = strlen(text);
		const int * next = kmpNext(m); 

		int tp = 0; 
		int mp = 0; // text pointer and match string pointer; 
		pos = 0;

		for(tp = 0;tp < lenText;++ tp) 
		{ 
			while(text[tp] != m[mp] && mp)
			{
				mp = next[mp - 1]; 
			}

			if(text[tp] == m[mp]) ++ mp; 

			if(mp == lenM) 
			{ 
				pos = tp - mp + 1; 
				XMem::XDELETE_ARRAY(next);
				return XTrue; 
			} 
		} 

		XMem::XDELETE_ARRAY(next);
		return XFalse;
	}
}
}