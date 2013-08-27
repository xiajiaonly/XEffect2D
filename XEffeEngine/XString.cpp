//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicFun.h"
#include "string.h"
#include "XString.h"

//sunday
int _XSunday::init(const char *patt)
{
	if(patt == NULL) return 0;
	if(strlen(patt) <= 0) return 0;
	if(m_isInited != 0)
	{//if is inited
		XDELETE_ARRAY(m_patt);
		//if(m_patt != NULL)
		//{
		//	delete [] m_patt;
		//	m_patt = NULL;
		//}
	}
	//init some value
	m_pattSize = strlen(patt);
	try
	{
		m_patt = new char[m_pattSize + 1];
		if(m_patt == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	strcpy(m_patt,patt);
	int i;
	for(i = 0;i < MAX_CHAR_SUM;++ i)
	{
		 *(m_shift + i) = m_pattSize + 1;	//init all value
	}
	for(i = 0;i < m_pattSize;++ i)
	{
		 *(m_shift + (unsigned char)(*(m_patt + i))) = m_pattSize - i; //init the character in the patt
	}
	m_isInited = 1;
	return 1;
}

_XSunday::~_XSunday()
{
	XDELETE_ARRAY(m_patt);
	XDELETE_ARRAY(m_shift);
}

int _XSunday::search(const char *text,int &position)
{
	if(m_isInited == 0) return 0;
	if(text == NULL) return 0;
	if(position == NULL) return 0;
	if(strlen(text) <= 0) return 0;
	if(m_pattSize > (int)(strlen(text))) return 0;

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
					return 1;
				}
			}
		}
	}
	position = 0;
	return 0;
}

bool sundaySearch(const char *text,const char *patt,int &position)
{
	if(text == NULL) return false;
	if(patt == NULL) return false;
	if(strlen(patt) <= 0) return false;
	if(strlen(text) <= 0) return false;
	if(strlen(patt) > strlen(text)) return false;
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
					return true;
				}
			}
		}
	}
	position = 0;
	return false;
}
//kmp
const int * kmpNext(const char *m) // count the longest prefex string ; 
{ 
	//static vector<int> next(m.size()); 
	int len = strlen(m);
	int *next = new int[len];
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


bool kmpSearch(const char *text,const char *m,int &pos) 
{ 
	if(m == NULL || strlen(m) <= 0) return false;
	if(text == NULL || strlen(text) <= 0) return false;
	int lenM = strlen(m);
	int lenText = strlen(text);
	const int * next = kmpNext(m); 

	int tp = 0; 
	int mp = 0; // text pointer and match string pointer; 
	pos = 0;

	for(tp = 0;tp < lenText;tp ++) 
	{ 
		while(text[tp] != m[mp] && mp)
		{
			mp = next[mp - 1]; 
		}

		if(text[tp] == m[mp]) mp ++; 

		if(mp == lenM) 
		{ 
			pos = tp - mp + 1; 
			XDELETE_ARRAY(next);
			return true; 
		} 
	} 

	XDELETE_ARRAY(next);
	return false;
}
