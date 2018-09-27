#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XStringFun.h"
#include "XCommonDefine.h"
#include "XNetWork\XWinSockCommon.h"

namespace XE{
const char *XStringConverter::ANSI2UTF8(const char*src)
{
	if (src == NULL) return NULL;
	int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	if (nRetLen == 0) return NULL;
	if (m_tmpArray.size() < nRetLen + 1) m_tmpArray.resize(nRetLen + 1);	//分配内存空间

	MultiByteToWideChar(CP_ACP, 0, src, -1, &(m_tmpArray[0]), nRetLen);
	nRetLen = WideCharToMultiByte(CP_UTF8, 0, &(m_tmpArray[0]), -1, NULL, 0, NULL, NULL);
	if (m_result.size() < nRetLen + 1) m_result.resize(nRetLen + 1);
	WideCharToMultiByte(CP_UTF8, 0, &(m_tmpArray[0]), -1, &m_result[0], nRetLen, NULL, NULL);

	return m_result.c_str();
}
namespace XString
{
	XBool binToHex(const unsigned char *in,int len,unsigned char *out)
	{//尚未经过测试
		if(in == NULL || out == NULL || len <= 0) return false;
		char *pTmp = (char *)out;
		for(int i = 0;i < len;++ i)
		{
			sprintf(pTmp + i * 2,"%02x",in[i]);
		}
		return true;
	}
	XBool getIsNumber(const char * p)
	{
		//方案1
		//000.000这种形式没有判断
		if(p == NULL) return XFalse;
		int index = 0;
		//第一个字符可以为'-' '+'
		//if((p[index] < '0' || p[index] > '9') && p[index] != '+' && p[index] != '-') return XFalse;
		if(p[index] == '+' || p[index] == '-') ++ index;
		int len = strlen(p);
		if(len <= index) return XFalse;
		XBool dotFlag = XFalse;
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
	XBool getIsUnsignedInt(const char * p)	//判断字符串是否为无符号整形数(尚未实现)
	{
		if(p == NULL) return XFalse;
		int len = strlen(p);
		for(int index = 0;index < len;++ index)
		{
			if(p[index] < '0' || p[index] > '9') return XFalse;
		}
		return XTrue;
	}
	XBool getIsInt(const char * p)			//判断字符串是否为整形数(尚未实现)
	{
		if(p == NULL) return XFalse;
		int len = strlen(p);
		int index = 0;
		//第一个字符可以为'-' '+'
		if(p[index] == '+' || p[index] == '-') ++ index;
		for(;index < len;++ index)
		{
			if(p[index] < '0' || p[index] > '9') return XFalse;
		}
		return XTrue;
	}
	XBool getIsValidIp(const char *ip)
	{
		//方案1：
		return inet_addr(ip) != INADDR_NONE;
		//方案3：
		//struct sockaddr_in sa;
		//int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
		//return result != 0;
		//方案2：
		//int p[4] ={0,0,0,0};
		//int len = strlen(ip);
		//int j = 0;
		//for(int i = 0; i < len;++ i)
		//{
		//	if(ip[i] == '.' || i == len -1)
		//	{/*分割符*/
		//		if(p[j] >= 0 && p[j] <= 255) ++ j;
		//		else return XFalse;
		//	}else
		//	{
		//		int d = ip[i] - '0';
		//		if(d > 9 || d < 0) return XFalse;
		//		else p[j] = p[j] * 10 + d;
		//	}
		//}
		//return true;
	}
	XBool getIsLocalhost(const char *p)
	{
		if(p == NULL) return XFalse;
		std::string tmp = p;
		for(unsigned int i = 0;i < tmp.size();++ i)
			if(tmp[i] >= 'A' && tmp[i] <= 'Z') tmp[i] -= 'A' - 'a';
		return tmp == "localhost";
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
		if(len <= 8)
		{//在unsigned int的范围内
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
		}else
		{//超出范围
			for(int i = 0;i < len;++ i)
			{
				if((p[i] >= '0' && p[i] <= '9') ||
					(p[i] >= 'A' && p[i] <= 'F') ||
					(p[i] >= 'a' && p[i] <= 'f'))
				{//合法的数据
					continue;
				}else
				if(p[i] == '\0') break; 
				else
				{
					return -1;
				}
			}
			return 1;
		}
	}
	long long getIsHexNumber64(const char * p)
	{
		if(p == NULL) return -1;
		int len = strlen(p);
		if(len <= 0) return -1;
		if(p[1] == 'x' || p[1] == 'X')
		{
			if(p[0] != '0') return -1;
			return getIsHexNumber64(p + 2);
		}
		if(len <= 16)
		{//在long long的范围内
			long long ret = 0;
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
		}else
		{//超出范围
			for(int i = 0;i < len;++ i)
			{
				if((p[i] >= '0' && p[i] <= '9') ||
					(p[i] >= 'A' && p[i] <= 'F') ||
					(p[i] >= 'a' && p[i] <= 'f'))
				{//合法的数据
					continue;
				}else
				if(p[i] == '\0') break; 
				else
				{
					return -1;
				}
			}
			return 1;
		}
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
	//下面的函数可以考虑使用memchr或strchr进行优化
	int getCharPosition(const char *p,char temp,int mode)
	{
		if(p == NULL) return -1;
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
	int getCharPositionEx(const char *p,char temp,int index,int mode)
	{
		if(p == NULL) return -1;
		if(index <= 0) return -1;
		int sum = 0;
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
				if(*p == temp)
				{
					++ sum;
					if(sum == index)
						return (p - p1);
				}
			}
		}else
		{//逆序查找
			for(int i = strlen(p) - 1;i >= 0;-- i)
			{
				if(i - 1 >= 0 && p[i - 1] < 0)
				{//汉字
					-- i;
					continue;
				}
				if(p[i] == temp)
				{
					++ sum;
					if(sum == index)
						return i;
				}
			}
		}
		return -1;
	}
	XBool getFirstWord(const char *p,int &s,int &e)
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
	XBool getFirtCanShowString(const char *p,int &s,int &e)
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
	std::string getCanShowString(const char *str,int canShowLen)
	{
		if(str == NULL || canShowLen <= 0) return XString::gNullStr;
		int len = strlen(str);
		if(len == 0) return XString::gNullStr;
		if(len <= canShowLen) return str;
		if(canShowLen == 1) return ".";
		if(canShowLen == 2) return "..";
		std::string ret = str;
		//下面需要整理字符串以便于输出
		ret.resize(canShowLen);
		if(isAtUnicodeEnd(str,canShowLen - 2))
		{
			ret.at(canShowLen - 2) = '.';
			ret.at(canShowLen - 1) = '.';
		}else
		{
			ret.at(canShowLen - 1) = '.';
		}
		return ret;
	}
	int getANSIStrPosLen(const char * str,int index)
	{
		if(str == NULL || index <= 0) return 0;
		int len = strlen(str);
		int ret = 0;
		int tmp = 0;
		for(int i = 0;i < len;++ i)
		{
			if(str[i] < 0)
			{
				ret += 2;
				++ i;
				++ tmp;
			}else
			{
				++ ret;
				++ tmp;
			}
			if(tmp == index) return ret;
		}
		return len;
	}
	int strToInt(const std::string& temp,int radix)
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
		if(src == NULL) return XString::gNullStr;
		int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		if(nRetLen == 0) return XString::gNullStr;
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
		if(unicode_buf == NULL) return XString::gNullStr;
		MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);
		nRetLen = WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
		std::string ret;
		ret.resize(nRetLen + 1);
		WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, &ret[0], nRetLen, NULL, NULL);
		XMem::XDELETE_ARRAY(unicode_buf);

		return ret;
	}
	char* UTF8ToANSI(const char* src)
	{
		if(src == NULL) return NULL;
		//第一步UTF8转UTF16(windows下UTF16与wchar与unicode等价，但在别的系统下不等价)
		int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
		if(nRetLen == 0) return NULL;
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
		if(unicode_buf == NULL) return NULL;
		MultiByteToWideChar(CP_UTF8, 0, src, -1, unicode_buf, nRetLen);
		//第二步UTF16转ANSI
		nRetLen = WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
		char * buf = XMem::createArrayMem<char>(nRetLen + 1);
		if(buf == NULL) 
		{
			XMem::XDELETE_ARRAY(unicode_buf);
			return NULL;
		}
		WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
		XMem::XDELETE_ARRAY(unicode_buf);
		return buf;
	}
	std::string UTF82ANSI(const char * src)
	{
		if(src == NULL) return XString::gNullStr;
		int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
		if(nRetLen == 0) return XString::gNullStr;
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
		if(unicode_buf == NULL) return NULL;
		if(MultiByteToWideChar(CP_UTF8, 0, src, -1, unicode_buf, nRetLen) == 0) return XString::gNullStr;
		nRetLen = WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, NULL, 0, NULL, NULL);

		std::string ret;
		ret.resize(nRetLen + 1);
		if(WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, &ret[0], nRetLen, NULL, NULL) == 0) return XString::gNullStr;
		XMem::XDELETE_ARRAY(unicode_buf);

		return ret;
	}
	//判断字符串是否为UTF8格式(这个函数有问题，在"停止"字符串的判断上出错)
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
		if(nBytes > 0 ||//违返规则  
			bAllAscii) return false;//如果全部都是ASCII, 说明不是UTF-8  
		return true;  
	}
	bool isANSI(const char * str,int length)
	{
		if(str == NULL || length <= 0) return false;	//非法的数据
		//unsigned char *p = (unsigned char *)str;
		for(int i = 0;i < length;)
		{
			if(str[i] < 0x80) ++i;
			else
			{
				if(length == i) return false;	//长度不对
				if(str[i] >= 129 && str[i] <= 254
					&& str[i + 1] >= 64 && str[i + 1]<=254)
					i += 2;
				else
					return false;
			}
		}
		return true;
	}
	XBool isAtUnicodeEnd(const char *p,int pos)
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
		if(!IsClipboardFormatAvailable(CF_TEXT)) return XString::gNullStr;
	//	HWND h = FindWindow(NULL,"Control");
	//	if(!OpenClipboard(h)) return XString::gNullStr;
		if(!OpenClipboard(NULL)) return XString::gNullStr;
		HGLOBAL hglb = GetClipboardData(CF_TEXT); 
		if(hglb == NULL) return XString::gNullStr;
		LPVOID lptstr = GlobalLock(hglb);
		GlobalUnlock(hglb); 
		CloseClipboard();
		if(lptstr != NULL) return (char *)lptstr;
		else return XString::gNullStr;
	}
	void setTextToClipboard(const std::string& str)
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
	wchar_t* ANSIToWchar(const char* src)
	{
		if(src == NULL) return NULL;
		int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
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
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
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
		char * buf = XMem::createArrayMem<char>(nRetLen + 1);
		if(buf == NULL) 
		{
			XMem::XDELETE_ARRAY(unicode_buf);
			return NULL;
		}
		WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
		// release space of unicode_buf
		XMem::XDELETE_ARRAY(unicode_buf);

		return buf;
	}
	char* WcharToANSI(const wchar_t* src)
	{
		if(src == NULL) return NULL;
		int nRetLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
		char * buf = XMem::createArrayMem<char>(nRetLen + 1);
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
				++ i;
			++ len;
		}
		return len;
	}
	int getStrLen(const char * p, int len)
	{
		int length = 0;
		for (int i = 0; i < len; ++i)
		{
			if(p[i] < 0) 
				++ i;
			++length;
		}
		return length;
	}

	std::string g_strDigit[] = {"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};
	std::string g_strDigitEx[] = {"〇", "一", "二", "三", "四", "五", "六", "七", "八", "九"};
	std::string g_strUnit[] = {"圆", "拾", "佰", "仟", "万", "拾", "佰", "仟", "亿","拾", "佰", "仟", "万", "拾", "佰"};
	std::string g_strUnitEx[] = {"元", "十", "百", "千", "万", "十", "百", "千", "亿","十", "百", "千", "万", "十", "百"};
	std::string g_strUnit2[] = {"角", "分"};
	int replaceSubStr(std::string &strOrig,std::string strSub,const std::string &strReplace)
	{
		int pos = (int)strOrig.find(strSub);
		int length = (int)strSub.length();
    
		if (pos >= 0)
		{
			strOrig.replace(pos, length, strReplace);
			return 0;
		}
    
		return -1;
	}
	void replaceSubStrAtHead(std::string &strOrig,std::string strSub,const std::string &strReplace)
	{
		int pos = (int)strOrig.find(strSub);
		int length = (int)strSub.length();
    
		if (pos == 0)
			strOrig.replace(pos, length, strReplace);
	}
	std::string floatToRMB(float sum)
	{
		#define MAX_NUMBER_LEN 100
		int i               = 0;
		int ret             = 0;
		int length          = 0;
		char *p             = NULL;
		char *pcDecimal     = NULL; //保存小数部分字符
		char czNumber[MAX_NUMBER_LEN]  = {0};  //保存完整数字部分字符
		std::string strResult;

		//std::cout << "======================================" << std::endl;
		//std::cout << sum << std::endl;

		//判断是否为小数
		if (sum < 0)
		{
			strResult = "不支持读负数";   
			return strResult;   
		}

		//将数字转为数字字符串，利用sprintf_s的正则转换
		sprintf_s(czNumber, MAX_NUMBER_LEN, "%.15lg", sum);
		//printf("[No.0]%s\n", czNumber); 

		//如果数字是太大或太小的数，因为已经转为科学计数，所以会含有e字符
		p = strchr(czNumber,'e');  
		if (NULL!=p) 
		{
			strResult = "不支持读太大或太小的数";
			return strResult;
		}
    
		p = strchr(czNumber, '.');  
		if (NULL != p) 
		{       
			p[0] = 0;    
			pcDecimal = p + 1;   
		}    
		length = (int)strlen(czNumber);  
    
		for (i = 0; i<length; i++) 
		{        
			if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
			{
				strResult += g_strDigit[czNumber[i] - '0'];
			}else 
			{
				strResult += g_strDigit[czNumber[i] - '0'] + g_strUnit[length-1-i];
			}   
		}
		//std::cout << "[No.1]把数字直接替换为汉字: \n" << strResult << std::endl;

		//把strResult中的所有"零零"子串替换为"零"
		while (1)
		{
			ret = replaceSubStr(strResult, "零零", "零");
			if (ret < 0)
			{
				break;
			}
		}
		//std::cout << "[No.2]替换所有零零为零: \n" << strResult << std::endl;

		replaceSubStr(strResult, "零亿", "亿");
		replaceSubStr(strResult, "零万", "万");
		if (strResult != "零圆")    //如果整数部分全为0，则不要去除元单位前面的零
		{
			replaceSubStr(strResult, "零圆", "圆");
		}
    
		//std::cout << "[No.3]去除零亿、零万、零圆前面的零: \n" << strResult << std::endl;

		//小数精确到两位数，即精确到单位分
		if (NULL != pcDecimal) 
		{
			//如果小数部分有数值而整数部分为0，则删除字符串中的零元
			if (strResult == "零圆")
			{
				strResult.clear();
			}
			i = 0;
			while (1) 
			{           
				if (0 == pcDecimal[i] || i >= 2) 
					break;   
				strResult += g_strDigit[pcDecimal[i] - '0'] + g_strUnit2[i];
				i++;      
			}   
		}
		//std::cout << "[No.4]小数精确到两位数，即精确到单位分: \n" << strResult << std::endl;
    
		return strResult;
	}
	std::string floatToChinese(float sum,bool smallStr)
	{
		#define MAX_NUMBER_LEN 100
		int length          = 0;
		char *p             = NULL;
		char *pcDecimal     = NULL; //保存小数部分字符
		char czNumber[MAX_NUMBER_LEN]  = {0};  //保存完整数字部分字符
		std::string strResult;

		//std::cout << "======================================" << std::endl;
		//std::cout << sum << std::endl;

		//判断是否为小数
		if (sum < 0)
		{
			strResult = "不支持读负数";   
			return strResult;   
		}

		//将数字转为数字字符串，利用sprintf_s的正则转换
		sprintf_s(czNumber, MAX_NUMBER_LEN, "%.6f", sum);
		//printf("[No.0]%s\n", czNumber); 

		//如果数字是太大或太小的数，因为已经转为科学计数，所以会含有e字符
		p = strchr(czNumber,'e');  
		if (NULL!=p) 
		{
			strResult = "不支持读太大或太小的数";
			return strResult;
		}
    
		p = strchr(czNumber, '.');  
		if (NULL != p) 
		{       
			p[0] = 0;    
			pcDecimal = p + 1;   
		}    
		length = (int)strlen(czNumber);  
    
		if(smallStr)
		{
			for (int i = 0; i<length;++ i) 
			{        
				if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
					strResult += g_strDigitEx[czNumber[i] - '0'];
				else 
					strResult += g_strDigitEx[czNumber[i] - '0'] + g_strUnitEx[length-1-i];
			}
		}else
		{
			for (int i = 0; i<length;++ i) 
			{        
				if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
					strResult += g_strDigit[czNumber[i] - '0'];
				else 
					strResult += g_strDigit[czNumber[i] - '0'] + g_strUnit[length-1-i];
			}
		}
		//std::cout << "[No.1]把数字直接替换为汉字: \n" << strResult << std::endl;

		//把strResult中的所有"零零"子串替换为"零"
		if(smallStr)
		{
			while(replaceSubStr(strResult, "〇〇", "〇") >= 0)
			{}
			//std::cout << "[No.2]替换所有零零为零: \n" << strResult << std::endl;

			replaceSubStr(strResult, "〇亿", "亿");
			replaceSubStr(strResult, "〇万", "万");
			if (strResult != "〇元")    //如果整数部分全为0，则不要去除元单位前面的零
				replaceSubStr(strResult, "〇元", "元");
			//如果最前面两个是"一十"，应该去掉"一"
			replaceSubStrAtHead(strResult,"一十","十");
		}else
		{
			while(replaceSubStr(strResult, "零零", "零") >= 0)
			{}
			//std::cout << "[No.2]替换所有零零为零: \n" << strResult << std::endl;

			replaceSubStr(strResult, "零亿", "亿");
			replaceSubStr(strResult, "零万", "万");
			if (strResult != "零圆")    //如果整数部分全为0，则不要去除元单位前面的零
				replaceSubStr(strResult, "零圆", "圆");
			//如果最前面两个是"一十"，应该去掉"一"
			replaceSubStrAtHead(strResult,"壹拾","拾");
		}
    
		//std::cout << "[No.3]去除零亿、零万、零圆前面的零: \n" << strResult << std::endl;

		//小数精确到两位数，即精确到单位分
		if (NULL != pcDecimal) 
		{
			int pcDecimalLen = 5;
			for(pcDecimalLen = 5;pcDecimalLen >= 0;-- pcDecimalLen)
			{
				if(pcDecimal[pcDecimalLen] != '0') break;
			}
			if(pcDecimalLen <= 0)
			{//如果小数部分全部为0则   
				if(smallStr)
					replaceSubStr(strResult, "元", "");
				else
					replaceSubStr(strResult, "圆", "");
			}else
			{
				if(smallStr)
				{
					replaceSubStr(strResult, "元", "点");
					//继续转换小数部分
					for(int i = 0;i <= pcDecimalLen;++ i)
					{
						strResult += g_strDigitEx[pcDecimal[i] - '0'];
					}
				}else
				{
					replaceSubStr(strResult, "圆", "点");
					//继续转换小数部分
					for(int i = 0;i <= pcDecimalLen;++ i)
					{
						strResult += g_strDigit[pcDecimal[i] - '0'];
					}
				}
			}
		}else
		{
			if(smallStr)
				replaceSubStr(strResult, "元", "");
			else
				replaceSubStr(strResult, "圆", "");
		}
		//std::cout << "[No.4]小数精确到两位数，即精确到单位分: \n" << strResult << std::endl;
    
		return strResult;
	}
	std::string intToChinese(int sum,bool smallStr)
	{
		#define MAX_NUMBER_LEN 100
		char czNumber[MAX_NUMBER_LEN]  = {0};  //保存完整数字部分字符
		std::string strResult;

		//std::cout << "======================================" << std::endl;
		//std::cout << sum << std::endl;

		//判断是否为小数
		if (sum < 0)
		{
			strResult = "不支持读负数";   
			return strResult;   
		}

		//将数字转为数字字符串，利用sprintf_s的正则转换
		sprintf_s(czNumber, MAX_NUMBER_LEN, "%d", sum);
		//printf("[No.0]%s\n", czNumber); 

		//如果数字是太大或太小的数，因为已经转为科学计数，所以会含有e字符
		char *p = strchr(czNumber,'e');  
		if (NULL != p) 
		{
			strResult = "不支持读太大的数";
			return strResult;
		}  
		int length = (int)strlen(czNumber);  
    
		if(smallStr)
		{
			for (int i = 0; i<length; i++) 
			{        
				if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
				{
					strResult += g_strDigitEx[czNumber[i] - '0'];
				}else 
				{
					strResult += g_strDigitEx[czNumber[i] - '0'] + g_strUnitEx[length-1-i];
				}   
			}
		}else
		{
			for (int i = 0; i<length; i++) 
			{        
				if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
				{
					strResult += g_strDigit[czNumber[i] - '0'];
				}else 
				{
					strResult += g_strDigit[czNumber[i] - '0'] + g_strUnit[length-1-i];
				}   
			}
		}
		//std::cout << "[No.1]把数字直接替换为汉字: \n" << strResult << std::endl;

		//把strResult中的所有"零零"子串替换为"零"
		if(smallStr)
		{
			while(replaceSubStr(strResult, "〇〇", "〇") >= 0)
			{}
			//std::cout << "[No.2]替换所有零零为零: \n" << strResult << std::endl;

			replaceSubStr(strResult, "〇亿", "亿");
			replaceSubStr(strResult, "〇万", "万");
			if (strResult != "〇元")    //如果整数部分全为0，则不要去除元单位前面的零
			{
				replaceSubStr(strResult, "〇元", "元");
			}
			//如果最前面两个是"一十"，应该去掉"一"
			replaceSubStrAtHead(strResult,"一十","十");
		}else
		{
			while(replaceSubStr(strResult, "零零", "零") >= 0)
			{}
			//std::cout << "[No.2]替换所有零零为零: \n" << strResult << std::endl;

			replaceSubStr(strResult, "零亿", "亿");
			replaceSubStr(strResult, "零万", "万");
			if (strResult != "零圆")    //如果整数部分全为0，则不要去除元单位前面的零
			{
				replaceSubStr(strResult, "零圆", "圆");
			}
			//如果最前面两个是"一十"，应该去掉"一"
			replaceSubStrAtHead(strResult,"壹拾","拾");
		}
    
		//std::cout << "[No.3]去除零亿、零万、零圆前面的零: \n" << strResult << std::endl;
		if(smallStr)
			replaceSubStr(strResult, "元", "");
		else
			replaceSubStr(strResult, "圆", "");
		//std::cout << "[No.4]小数精确到两位数，即精确到单位分: \n" << strResult << std::endl;
    
		return strResult;
	}
	wchar_t oneCharToWchar(const char* src)
	{
		if(src == NULL) return NULL;
		wchar_t ret = 0;
		MultiByteToWideChar(CP_ACP, 0, src, 2,&ret, 1);
		return ret;
	}
	std::string Wchar2ANSI(const wchar_t* src)
	{
		if(src == NULL) return NULL;
		int nRetLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
 		std::string ret;
		ret.resize(nRetLen + 1);
		WideCharToMultiByte(CP_ACP, 0, src, -1, &ret[0], nRetLen, NULL, NULL);
		return ret;
	}
	int removeAllSpace(char *str, int strlen)
	{//尚未实现
		return 0;
	}
#if !WITH_INLINE_FILE
#include "XStringFun.inl"
#endif
}
}