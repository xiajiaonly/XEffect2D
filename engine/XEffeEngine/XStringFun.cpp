//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XStringFun.h"
#include "XCommonDefine.h"
#include "XBasicFun.h"

_XBool getIsNumber(const char * p)
{
	//方案1
	//000.000这种形式没有判断
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
	return XTrue;
	//方案2,功能强，速度慢
	//if(p == NULL) return XFalse;
	//char *ok = NULL;
	//strtod(p,&ok);
	//return !isspace(*p) && strlen(ok) == 0;
}
int getIsHexNumber(const char * p)
{
	if(p == NULL) return -1;
	int len = strlen(p);
	if(len <= 0) return -1;
	if(p[1] == 'x' || p[1] == 'X')
	{
		if(p[0] != '0') return -1;
		return getIsHexNumber(p + 2);
	}
	int ret = 0;
	for(int i = 0;i < len;++ i)
	{
		if((p[i] >= '0' && p[i] <= '9') ||
			(p[i] >= 'A' && p[i] <= 'F') ||
			(p[i] >= 'a' && p[i] <= 'f'))
		{//合法的数据
			if(p[i] >= '0' && p[i] <= '9') ret = (ret << 4) + p[i] - '0'; else
			if(p[i] >= 'A' && p[i] <= 'F') ret = (ret << 4) + p[i] - 'A' + 10; else
			if(p[i] >= 'a' && p[i] <= 'f') ret = (ret << 4) + p[i] - 'a' + 10; 
		}else
		if(p[i] == '\0') break; 
		else
		{
			return -1;
		}
	}
	return ret;
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
	return (s >= 0 && e >= 0 && e > s);
}
_XBool getFirtCanShowString(const char *p,int &s,int &e)
{
	if(p == NULL) return XFalse;
	s = -1;
	e = -1;
	int len = strlen(p);
	if(len <= 0) return XFalse;
	for(int i = 0;i < len;++ i)
	{
		if(p[i] >= 32 && p[i] <= 126)
		{//可显示的ASCII码
			if(s == -1) s = i;
		}else
		{
			if(p[i] < 0)
			{//中文
				if(s == -1) s = i;
				++ i;
			}else
			{//不能显示的字符
				if(s != -1)
				{//结束
					e = i - 1;
					return XTrue;
				}
			}
		}
	}
	if(s != -1) e = len - 1;
	return (s >= 0 && e >= 0 && e > s);
}
int strToInt(const std::string &temp,int radix)
{
	int ret = 0;
	if(radix == 16) if(sscanf(temp.c_str(),"%x",&ret) != 1) return  0;else
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
std::string ANSI2UTF8(const char * src)
{
	if(src == NULL) return "";
    int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	if(nRetLen == 0) return "";
    wchar_t* unicode_buf = createArrayMem<wchar_t>(nRetLen + 1);
	if(unicode_buf == NULL) return "";
    MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);
    nRetLen = WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
	std::string ret;
	ret.resize(nRetLen + 1);
    WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, &ret[0], nRetLen, NULL, NULL);
	XDELETE_ARRAY(unicode_buf);

    return ret;
}
char* UTF8ToANSI(const char* src)
{
	if(src == NULL) return NULL;
	//第一步UTF8转UTF16(windows下UTF16与wchar与unicode等价，但在别的系统下不等价)
    int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	if(nRetLen == 0) return NULL;
    wchar_t* unicode_buf = createArrayMem<wchar_t>(nRetLen + 1);
	if(unicode_buf == NULL) return NULL;
    MultiByteToWideChar(CP_UTF8, 0, src, -1, unicode_buf, nRetLen);
	//第二步UTF16转ANSI
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
    char * buf = createArrayMem<char>(nRetLen + 1);
	if(buf == NULL) 
	{
		XDELETE_ARRAY(unicode_buf);
		return NULL;
	}
    WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
	XDELETE_ARRAY(unicode_buf);
    return buf;
}
std::string UTF82ANSI(const char * src)
{
	if(src == NULL) return "";
    int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	if(nRetLen == 0) return "";
    wchar_t* unicode_buf = createArrayMem<wchar_t>(nRetLen + 1);
	if(unicode_buf == NULL) return NULL;
    MultiByteToWideChar(CP_UTF8, 0, src, -1, unicode_buf, nRetLen);
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, NULL, 0, NULL, NULL);

	std::string ret;
	ret.resize(nRetLen + 1);
    WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, &ret[0], nRetLen, NULL, NULL);
	XDELETE_ARRAY(unicode_buf);

    return ret;
}
//判断字符串是否为UTF8格式
bool isUTF8(const char * str,int length)  
{
	if(str == NULL) return false;
	DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节  
	UCHAR chr;  
	bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8  
	for(int i = 0;i < length;++ i)  
	{  
		chr = *(str+i);  
		if((chr&0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx  
			bAllAscii = false;  
		if(nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数  
		{  
			if(chr >= 0x80)  
			{  
				if(chr >= 0xFC && chr <= 0xFD)  
					nBytes = 6;  
				else if(chr >= 0xF8)  
					nBytes = 5;  
				else if(chr >= 0xF0)  
					nBytes = 4;  
				else if(chr >= 0xE0)  
					nBytes = 3;  
				else if(chr >= 0xC0)  
					nBytes = 2;  
				else  
					return false;  
				-- nBytes;  
			}  
		}else //多字节符的非首字节,应为 10xxxxxx  
		{  
			if((chr & 0xC0) != 0x80)  
				return false;  

			-- nBytes;  
		}  
	}  
	if(nBytes > 0) return false;//违返规则  
	if(bAllAscii) return false;//如果全部都是ASCII, 说明不是UTF-8  
	return true;  
}
_XBool isAtUnicodeEnd(const char *p,int pos)
{
	int len = strlen(p);
	if(p == NULL || pos < 0 || pos > len) return XFalse;
	if(pos == 0 || pos == len) return XTrue;
	int index = 0;
	while(index < pos)
	{
		if(p[index] < 0) index += 2;
		else ++ index;
	}
	return (index == pos);
}
std::string getTextFromClipboard()
{//从粘贴板中读取数据
	if(!IsClipboardFormatAvailable(CF_TEXT)) return ""; 
//	HWND h = FindWindow(NULL,"Control");
//	if(!OpenClipboard(h)) return ""; 
	if(!OpenClipboard(NULL)) return ""; 
	HGLOBAL hglb = GetClipboardData(CF_TEXT); 
    if(hglb == NULL) return "";
    LPVOID lptstr = GlobalLock(hglb);
	GlobalUnlock(hglb); 
	CloseClipboard();
	if(lptstr != NULL) return (char *)lptstr;
	else return "";
}
void setTextToClipboard(const std::string & str)
{//将数据写入到粘贴板
//	HWND h = FindWindow(NULL,"Control");
//	if(!OpenClipboard(h) || !EmptyClipboard()) return; 
	if(!OpenClipboard(NULL) || !EmptyClipboard()) return; 
	size_t size = sizeof(TCHAR)*(1 + str.length());
	HGLOBAL hResult = GlobalAlloc(GMEM_MOVEABLE,size); 
	if(hResult == NULL)
	{	//内存分配失败
		CloseClipboard();
		return;
	}
	LPSTR lpstrCopy = (LPSTR)GlobalLock(hResult);
	if(lpstrCopy == NULL)
	{
		GlobalFree(hResult);
		CloseClipboard();
		return;
	}
	memcpy(lpstrCopy,str.c_str(),size); 
//	lpstrCopy[size] = (TCHAR)0;   
	GlobalUnlock(hResult); 
#ifndef _UNICODE
	if(SetClipboardData(CF_TEXT,hResult) == NULL)
#else
	if(SetClipboardData(CF_UNICODETEXT,hResult) == NULL)
#endif
	{
		printf("Unable to set Clipboard data");
		GlobalFree(hResult); // Edit by Leo Davidson 16/Sep/2010: Free buffer if clipboard didn't take it.
	}

	CloseClipboard();
}
void calPathDepth(const char *p,int &depth,int &lastPos)
{//这里假设路径是合法的
	if(p == NULL) return;
	int len = strlen(p);
	int pointSum = 0;
	depth = 0;
	for(int i = 0;i < len;++ i)
	{
		if(p[i] == '.') 
		{
			pointSum ++;
			continue;
		}
		if(p[i] == '\\' || p[i] == '/')
		{
			if(pointSum == 2) ++ depth;
			pointSum = 0;
			continue;
		}
		lastPos = i;
		break;
	}
}
std::string getBackPath(const char *p,int backSum)
{
	if(p == NULL || backSum < 0) return p;
	if(backSum == 0)
	{//整理路径而已
		char buffer[MAX_PATH + 1] = "";
		strcpy(buffer,p);
		int len = strlen(buffer);
		for(int i = len - 1;i >= 0;-- i)
		{
			if(buffer[i] == '\\' || buffer[i] == '/')
			{
				buffer[i + 1] = '\0';
				return buffer;
			}
		}
		buffer[0] = '\0';
		return buffer;
	}
	int depth,lastPos;
	calPathDepth(p,depth,lastPos);
	int tmp = getPathDeep(p + lastPos);
	if(tmp == backSum)
	{//正好
		char buffer[MAX_PATH + 1] = "";
		strcpy(buffer,p);
		buffer[lastPos] = '\0';
		return buffer;
	}else
	if(tmp < backSum)
	{//还需要退
		char buffer[MAX_PATH + 1] = "";
		strcpy(buffer,p);
		int index = lastPos;
		for(int i = 0;i < backSum - tmp;++ i,index += 3)
		{
			buffer[index] = '.';
			buffer[index + 1] = '.';
			buffer[index + 2] = '/';
		}
		buffer[index] = '\0';
		return buffer;
	}else
	{//有路径
		char buffer[MAX_PATH + 1] = "";
		strcpy(buffer,p);
		int index = 0;
		int len = strlen(buffer);
		for(int i = lastPos + 1;i < len;++ i)
		{
			if(buffer[i] == '\\' || buffer[i] == '/')
			{
				index ++;
				if(index >= tmp - backSum)
				{
					buffer[i + 1] = '\0';
				}
			}
		}
		return buffer;
	}
}
int getCharsNumInPath(const char * path )
{//尚未经过验证
    int cnt = 0;
    int n = strlen( path );

    for(int i = 0; i < n;++ i)
    {
		if((path[i] < 0x80) || (path[i] > 0xbf))
			++ cnt;
    }

    return cnt;
}
bool isPathValid(const char * path)
{//检查路径是否合法，尚未经过验证(目前存在问题：如./../data/a.dat)
    if(path == NULL) return false;

    if(getCharsNumInPath(path) > MAX_PATH) return false;
    int n = strlen(path);

    for(int i = 0; i < n;++ i)
    {
		switch( path[i] )
		{
			// ? " / < > * |
			case '?':
			case '\"':
			//case '/':
			case '<':
			case '>':
			case '*':
			case '|':
				return false;
			// for example D:\folder\file.txt
			case ':':
				if( i != 1 )  return false;
				else break;
			case ' ':
			case '.':
				if(i + 1 == n || path[i+1] == '\\' || path[i+1] == '/') return false;
				else break;
			case '\\':
				if( i > 0 && (path[i - 1] == '\\' || path[i - 1] == '/')) return false;
				else break;
			case '/':
				if( i > 0 && (path[i - 1] == '\\' || path[i - 1] == '/')) return false;
				else break;
		}
    }
	return true;
}
BOOL popFileOpenDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitleName,PTSTR title,bool isOpen)
{//弹出文件打开窗口并返回选择的文件的相对路径和绝对路径
	OPENFILENAME ofn;
	char lpFileName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,lpFileName);	//设置当前路径为默认路径

	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.hInstance         = NULL ;
	ofn.lpstrFilter       = TEXT ("Text Files (*.XML)\0*.xml\0") ;
	ofn.lpstrCustomFilter = NULL ;
	ofn.nMaxCustFilter    = 0 ;
	ofn.nFilterIndex      = 0 ;
	ofn.nMaxFile          = MAX_PATH ;
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.lpstrInitialDir   = lpFileName ;
	ofn.lpstrTitle        = TEXT (title) ;
	ofn.nFileOffset       = 0 ;
	ofn.nFileExtension    = 0 ;
	ofn.lpstrDefExt       = TEXT ("xml") ;
	ofn.lCustData         = 0L ;
	ofn.lpfnHook          = NULL ;
	ofn.lpTemplateName    = NULL ;

	ofn.hwndOwner         = hwnd ;
	ofn.lpstrFile         = pstrFileName ;
	ofn.lpstrFileTitle    = pstrTitleName ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;
	if(isOpen) return GetOpenFileName(&ofn);
	else return GetSaveFileName(&ofn);
}
wchar_t* ANSIToWchar(const char* src)
{
	if(src == NULL) return NULL;
	int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    wchar_t* unicode_buf = createArrayMem<wchar_t>(nRetLen + 1);
	if(unicode_buf == NULL) return NULL;
    MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);
	return unicode_buf;
}
char* ANSIToUTF8(const char* src)
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
	if(nRetLen == 0) return NULL;
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
	if(buf == NULL) 
	{
		XDELETE_ARRAY(unicode_buf);
		return NULL;
	}
    WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
    // release space of unicode_buf
	XDELETE_ARRAY(unicode_buf);

    return buf;
}
char* WcharToANSI(const wchar_t* src)
{
	if(src == NULL) return NULL;
    int nRetLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
    char * buf = createArrayMem<char>(nRetLen + 1);
	if(buf == NULL) return NULL;
    WideCharToMultiByte(CP_ACP, 0, src, -1, buf, nRetLen, NULL, NULL);
    return buf;
}
int getStrLen(const char * p)	//获得字符串的表意字符长度
{
	int len = 0;
	int length = strlen(p);
	for(int i = 0;i < length;++ i)
	{
		if(p[i] < 0) 
		{
			++ len;
			++ i;
		}else
		{
			++ len;
		}
	}
	return len;
}