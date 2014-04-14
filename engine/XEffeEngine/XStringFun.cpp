//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XStringFun.h"
#include "XCommonDefine.h"
#include "XBasicFun.h"

_XBool getIsNumber(const char * p)
{//000.000这种形式没有判断
	if(p == NULL) return XFalse;
	int index = 0;
	//第一个字符可以为'-' '+'
	//if((p[index] < '0' || p[index] > '9') && p[index] != '+' && p[index] != '-') return XFalse;
	if(p[index] == '+' || p[index] == '-') ++ index;
	_XBool dotFlag = XFalse;
	int len = strlen(p);
	for(;index < len;++ index)
	{
		if(p[index] >= '0' && p[index] <= '9') continue;
		if(p[index] == '.' && !dotFlag)
		{
			dotFlag = XTrue;
			continue;
		}
		return XFalse;
	}
	//asdakldjlasdjlalk
	return XTrue;
}
int getCharSum(const char *p,char temp)
{
	if(p == NULL) return 0;
	int ret = 0;
	for(;*p != '\0';++ p)
	{
		if((* p) == temp) ++ ret;
	}
	//for(int i = 0;;++ i)
	//{
	//	if(p[i] == '\0') break;
	//	if(p[i] == temp) ++ ret;
	//}
	return ret;
}
int getCharPosition(const char *p,char temp,int mode)
{
	if(p == NULL) return 0;
	const char *p1 = p;
	if(mode == 0)
	{//正序查找
		for(;*p != '\0';++ p)
		{
			if(*p < 0)
			{//汉字
				++ p;
				continue;
			}
			if(*p == temp) return (p - p1);
		}
		//for(int i = 0;;++ i)
		//{
		//	if(p[i] < 0)
		//	{//汉字
		//		++ i;
		//		continue;
		//	}
		//	if(p[i] == '\0') return -1;	//结束
		//	if(p[i] == temp) return i;
		//}
	}else
	{//逆序查找
		for(int i = strlen(p) - 1;i >= 0;-- i)
		{
			if(i - 1 >= 0 && p[i - 1] < 0)
			{//汉字
				-- i;
				continue;
			}
			if(p[i] == temp) return i;
		}
	}
	return -1;
}
_XBool fileNameCompare(const char *p1,const char *p2)
{
	if(p1 == NULL || p2 == NULL) return XFalse; 
	int len1 = strlen(p1);
	int len2 = strlen(p2);
	if(len1 != len2) return XFalse;
	//int temp = 'A' - 'a';
	for(int i = 0;i < len1;++ i)
	{
		if(p1[i] == p2[i] && p2[i] < 0)
		{//汉字
			if(i + 1 < len1)
			{
				if(p1[i + 1] == p2[i + 1]) 
				{
					++ i;
					continue;
				}else
				{
					return XFalse;
				}
			}
		}
		if(p1[i] == p2[i]) continue;
		if(p1[i] >= 'a' && p1[i] <= 'z' && p1[i] - 'a' + 'A' == p2[i]) continue;else
		if(p1[i] >= 'A' && p1[i] <= 'Z' && p1[i] - 'A' + 'a' == p2[i]) continue;else
		if((p1[i] == '/' && p2[i] == '\\') || (p1[i] == '\\' && p2[i] == '/') ) continue;
		return XFalse;
	}
	return XTrue;
}
int getPathDeep(const char *p)
{
	if(p == NULL) return 0;
	int len = strlen(p);
	if(len >= MAX_FILE_NAME_LENGTH) return 0;
	int deep = 0;
	for(int i = 0;i < len;++ i)
	{
		if(p[i] < 0)
		{//汉字
			++ i;
			continue;
		}
		if(p[i] == '\\' || p[i] == '/') ++ deep;
	}
	return deep;
}
int getPathDeepByChar(const char *p)
{
	if(p == NULL) return 0;
	if(strlen(p) >= MAX_FILE_NAME_LENGTH) return 0;
	for(int i = strlen(p) - 1;i >= 0;-- i)
	{
		if(i - 1 >= 0 && p[i - 1] < 0)
		{//汉字
			-- i;
			continue;
		}
		if(p[i] == '\\' || p[i] == '/') return i;
	}
	return 0;
	//int len = strlen(p);
	//int deepByChar = 0;
	//for(int i = 0;i < len;++ i)
	//{
	//	if(p[i] == '\\' || p[i] == '/') 
	//	{
	//		deepByChar = i;
	//	}
	//}
	//return deepByChar;
}
_XBool getFirstWord(const char *p,int &s,int &e)
{
	if(p == NULL) return XFalse;
	s = -1;
	e = -1;
	int len = strlen(p);
	if(len <= 0) return XFalse;
	for(int i = 0;i < len;++ i)
	{
		if((p[i] <= 'Z' && p[i] >= 'A')
			|| (p[i] <= 'z' && p[i] >= 'a')
			|| p[i] == '_')
		{
			if(s == -1) s = i;
		}else
		{
			if(s != -1) 
			{
				e = i - 1;
				return XTrue;
			}
		}
	}
	if(s != -1) e = len - 1;
	return XTrue;
}
int strToInt(const std::string &temp,int radix)
{
	int ret = 0;
	if(radix == 16) sscanf(temp.c_str(),"%x",&ret);else
	if(radix == 10) return atoi(temp.c_str()); else
	if(radix == 2)
	{
		int size = temp.size();
		const char * tempStr = temp.c_str();
		for(int i = 0;i < size;++ i)
		{
			ret = ret << 1;
			if(tempStr[i] == '1') ret += 1;
		}
	}else
	if(radix == 8)
	{
		int size = temp.size();
		const char * tempStr = temp.c_str();
		for(int i = 0;i < size;++ i)
		{
			ret = ret << 3;
			ret += tempStr[i] - '0';
		}
	}

	return ret;
}
wchar_t oneCharToWchar(const char* src)
{
	if(src == NULL) return NULL;
	wchar_t ret;
	MultiByteToWideChar(CP_ACP, 0, src, 2,&ret, 1);
	return ret;
}
wchar_t* charToWchar(const char* src)
{
	if(src == NULL) return NULL;
	int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    wchar_t* unicode_buf = createArrayMem<wchar_t>(nRetLen + 1);
	if(unicode_buf == NULL) return NULL;
    MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);
	return unicode_buf;
}
char* localeToUTF8(const char* src)
{
	if(src == NULL) return NULL;
    // *function: MultiByteToWideChar (Maps a character string to a UTF-16 (wide character) string.)
    // - UINT CodePage (Code page to use in performing the conversion. )
    //                CP_ACP: The system default Windows ANSI code page.
    // - DWORD dwFlags (Flags indicating the conversion type)
    //                0:
    // - LPCSTR lpMultiByteStr (Pointer to the character string to convert.)
    //                src: the word that you want to conver
    // - int cbMultiByte (you want to process size of lpMultiByteStr)
    //                -1:  the function processes the entire input string, including the terminating null character. when the input string
    //                            not contains terminating null character, it will failure.
    // - LPWSTR lpWideCharStr (Pointer to a buffer that receives the converted string.)
    //                NULL: no receives WideChar.
    // - int cchWideChar (size of lpWideCharStr)
    //                0: set the paramter for the function returns the required buffer size.
    // * return value : because of cchWideChar is 0, so returns the required buffer size of lpWideCharStr
    int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    //  allocate space for unicode_buf
    wchar_t* unicode_buf = createArrayMem<wchar_t>(nRetLen + 1);
	if(unicode_buf == NULL) return NULL;
    // covert the src to utf-8, and store in unicode_buf
    MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);

    // *function: WideCharToMultiByte (Maps a UTF-16 (wide character) string to a new character string. )
    // - UINT CodePage (Code page to use in performing the conversion. )
    //                CP_UTF8: With this value set, lpDefaultChar and lpUsedDefaultChar must be set to NULL.
    // - DWORD dwFlags (Flags indicating the conversion type. )
    //                0 :
    // - LPCWSTR lpWideCharStr (Pointer to the Unicode string to convert.)
    //                unicode_buf : the word that you want to conver
    // - int cchWideChar (you want to process size of lpWideCharStr)
    //                -1: the function processes the entire input string, including the terminating null character. when the input string
    //                        not contains terminating null character, it will failure.
    // - LPSTR lpMultiByteStr (Pointer to a buffer that receives the converted string.)
    //                NULL : no receives MultiByteStr.
    // - int cbMultiByte (size of lpMultiByteStr)
    //                0: set the paramter for the function returns the required buffer size.
    // - LPCSTR lpDefaultChar (Pointer to the character to use if a character cannot be represented in the specified code page. )
    //                NULL : For the CP_UTF7 and CP_UTF8 settings for CodePage, this parameter must be set to NULL.
    // - LPBOOL lpUsedDefaultChar (Pointer to a flag that indicates if the function has used a default character in the conversion.)
    //                NULL : For the CP_UTF7 and CP_UTF8 settings for CodePage, this parameter must be set to NULL.
    nRetLen = WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
    //  allocate space for buf
    char * buf = createArrayMem<char>(nRetLen + 1);
	if(buf == NULL) return NULL;
    WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
    // release space of unicode_buf
	XDELETE_ARRAY(unicode_buf);

    return buf;
}
char* UTF8ToLocale(const char* src)
{
	if(src == NULL) return NULL;
    int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    wchar_t* unicode_buf = createArrayMem<wchar_t>(nRetLen + 1);
	if(unicode_buf == NULL) return NULL;
    MultiByteToWideChar(CP_UTF8, 0, src, -1, unicode_buf, nRetLen);
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
    char * buf = createArrayMem<char>(nRetLen + 1);
	if(buf == NULL) return NULL;
    WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
	XDELETE_ARRAY(unicode_buf);
    return buf;
}