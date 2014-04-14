#ifndef _JIA_XSTRINGFUN_
#define _JIA_XSTRINGFUN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//这里定义都是与字符串操作相关方法
#include "XOSDefine.h"
#include <string>
//从字符串中找到最近的一个符合要求的字符的位置,注意如果这里输入的是一个没有结束符的字符串将会导致严重错误
//返回-1表示找不到这个字符
extern _XBool getIsNumber(const char * p);	//检查字符串是否为数值
extern int getCharSum(const char *p,char temp);	//统计字符串中特定字符的数量
extern int getCharPosition(const char *p,char temp,int mode = 0);	//mode 0:正序查找，1:倒叙查找
extern _XBool fileNameCompare(const char *p1,const char *p2);	//比较两个路径或者文件名是否一致，不考虑大小写
extern int getPathDeep(const char *p);			//获取路径的深度，获取路径目录的层数 e.g.:c:/x/y/xx.txt return 3
extern int getPathDeepByChar(const char *p);	//获取路径的深度，获取路径目录的字符串深度 e.g.:c:/x/y/xx.txt return 6
extern _XBool getFirstWord(const char *p,int &s,int &e);		//获取字符串中的第一个单词 e.g:"  xia jia ... " return "xia"
inline  _XBool isAtUnicodeEnd(const char *p,int pos)	//在字符串中当前的位置是否为字符的结束(XTrue)，还是在字符的中间(XFalse)
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
//将16进制的p转换成10进制
inline int hexCharToInt(char p)	//16进制字符转int的数值
{
	if(p >= '0' && p <= '9') return p - '0';
	if(p >= 'a' && p <= 'f') return p - 'a' + 10;
	if(p >= 'A' && p <= 'F') return p - 'A' + 10;
	return '0';
}
//将temp转换成radix进制字符串，可以支持2进制，8进制，10进制，16进制
inline std::string intToStr(int temp,int radix = 16)
{
	char str[128];
	itoa(temp,str,radix);
	return str;
}
#include "windows.h"
inline std::string getWorkPath()
{//获取大当前工作路径
	char path[512];
	GetCurrentDirectory(512,path);
	return path;
}
inline void setWorkPath(const std::string &path)
{//设置当前工作路径
	SetCurrentDirectory(path.c_str());
}
inline std::string getCurrentExeFileFullPath()
{
	char lpFileName[MAX_PATH];
	GetModuleFileName(NULL,lpFileName,MAX_PATH);
	return lpFileName;
}
inline std::string getFullPath(const std::string &filename)
{
	//注意这里没有分析filename的前缀，如./或者../会存在问题
	//return getWorkPath() + "/" + filename;
	char currentPath[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH,currentPath);	//记录当前的路径
	int len = getCharPosition(filename.c_str(),'\\',1);
	int len1 = getCharPosition(filename.c_str(),'/',1);
	if(len < 0 && len1 < 0) return getWorkPath() + "/" + filename;
	
	char path[MAX_PATH + 1];
	char path1[MAX_PATH + 1];
	strcpy(path,filename.c_str());
	if(len >= len1) len = len + 1;
	else len = len1 + 1;
	path[len] = '\0';
	SetCurrentDirectory(path);  //用得到的相对路径设置当前目录
	GetCurrentDirectory(MAX_PATH,path1);  //得到当前目录的绝对路径
	
	SetCurrentDirectory(currentPath);	//恢复当前的路径
	strcat(path1,"/");
	strcpy(path,filename.c_str());
	strcat(path1,path + len);
	return  path1;
}
inline std::string getTextFromClipboard()
{//从粘贴板中读取数据
	HWND h = FindWindow(NULL,"Control");
	if(!OpenClipboard(h)) return ""; 
	if(!IsClipboardFormatAvailable(CF_TEXT)) return ""; 
	HGLOBAL hglb = GetClipboardData(CF_TEXT); 
    if(hglb == NULL) return "";
    LPVOID lptstr = GlobalLock(hglb);
	GlobalUnlock(hglb); 
	CloseClipboard();
	if(lptstr != NULL) return (char *)lptstr;
	else return "";
}
inline void setTextToClipboard(const std::string & str)
{//将数据写入到粘贴板
	HWND h = FindWindow(NULL,"Control");
	if(!OpenClipboard(h)) return; 
	size_t size = sizeof(TCHAR)*(1 + str.size());
	HGLOBAL hResult = GlobalAlloc(GMEM_MOVEABLE,size); 
	if(hResult == NULL)
	{	//内存分配失败
		return;
		CloseClipboard();
	}
	LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hResult);
	if(lptstrCopy == NULL)
	{
		GlobalFree(hResult);
		CloseClipboard();
	}
	memcpy(lptstrCopy,str.c_str(), size); 
	GlobalUnlock(hResult); 
#ifndef _UNICODE
	if(SetClipboardData(CF_TEXT,hResult) == NULL)
#else
	if(SetClipboardData(CF_UNICODETEXT,hResult) == NULL)
#endif
	{
		printf("Unable to set Clipboard data");
		GlobalFree(hResult); // Edit by Leo Davidson 16/Sep/2010: Free buffer if clipboard didn't take it.
		CloseClipboard();
		return;
	}

	CloseClipboard();
}
//将字符串temp装换成对应的整数，可以支持2进制，8进制，10进制，16进制
//注意这个函数没有考虑数越界问题，当字符串表示的数据超过int所能表示的最大范围时，这个函数会出现错误
//如果传入的字符串与制定进制数据不符时，该函数没有进行检查，将会造成错误
extern int strToInt(const std::string &temp,int radix = 10);
//字符转换的方法
extern char* localeToUTF8(const char* src);	//字符转换
extern char* UTF8ToLocale(const char* src);	
extern wchar_t* charToWchar(const char* src);
extern wchar_t oneCharToWchar(const char* src);
#endif