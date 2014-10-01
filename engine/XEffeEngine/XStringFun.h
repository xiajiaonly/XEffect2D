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
extern _XBool getIsNumber(const char * p);	//检查字符串是否为十进制数值
extern int getIsHexNumber(const char * p);	//检查字符串是否为16进制数，如果是的话直接返回数值，注意取值范围问题，-1输入非法
extern int getCharSum(const char *p,char temp);	//统计字符串中特定字符的数量
extern int getCharPosition(const char *p,char temp,int mode = 0);	//在字符串p中查找字符temp的位置，-1为没有这个字符。mode 0:正序查找，1:倒叙查找
extern _XBool fileNameCompare(const char *p1,const char *p2);	//比较两个路径或者文件名是否一致，不考虑大小写，
extern int getPathDeep(const char *p);			//获取路径的深度，获取路径目录的层数 e.g.:"c:/x/y/xx.txt" return 3
extern int getPathDeepByChar(const char *p);	//获取路径的深度，获取路径目录的字符串深度 e.g.:"c:/x/y/xx.txt" return 6
extern _XBool getFirstWord(const char *p,int &s,int &e);		//获取字符串中的第一个单词 e.g:"  xia jia ... " return "xia"，目前不支持中文，中文没有单词的形式表现
extern _XBool getFirtCanShowString(const char *p,int &s,int &e);		//获取第一段可以显示的字符串,输入字符串必须要是ANSI格式
//在字符串中当前的位置是否为字符的结束(XTrue)，还是在字符的中间(XFalse)：中文占用两个字节，判断当前位置是否在两个字节的中间或者结束
extern _XBool isAtUnicodeEnd(const char *p,int pos);
//将16进制的p转换成10进制，非法数据返回'0'
inline int hexCharToInt(char p);	//16进制字符转int的数值
//16进制转换成2进制，如"FFEECC",转换成{0xff,0xee,0xcc}
//注意检查输入输出的正确性：1、输入必须为2的倍数，2、输入输出的容量需要一致
inline _XBool hexToBin(const char *in,unsigned char *out);
inline _XBool binToHex(const char *in,unsigned char *out);	//尚未实现
//将temp转换成radix进制字符串，可以支持2进制，8进制，10进制，16进制
inline std::string intToStr(int temp,int radix = 16);
//将浮点数转换成字符串，比toString方法效率高
inline std::string floatToStr(float temp,int len = 6);
#include "windows.h"
//获取大当前工作路径
inline std::string getWorkPath();
//获取文件扩展名
inline std::string getFileExtension(const char * filename);
//设置当前工作路径
inline void setWorkPath(const std::string &path);
//获取当前exe文件的全路径文件名
inline std::string getCurrentExeFileFullPath();
//相对路径转换成绝对路径
inline std::string getFullPath(const std::string &filename);
extern std::string getTextFromClipboard();		//从粘贴板中读取数据
extern void setTextToClipboard(const std::string & str);		//将数据写入到粘贴板
//将字符串temp装换成对应的整数，可以支持2进制，8进制，10进制，16进制
//注意这个函数没有考虑数越界问题，当字符串表示的数据超过int所能表示的最大范围时，这个函数会出现错误
//如果传入的字符串与制定进制数据不符时，该函数没有进行检查，将会造成错误
extern int strToInt(const std::string &temp,int radix = 10);
//字符转换的方法(windows下UTF16与wchar与unicode等价，但在别的系统下不等价需要酢情考虑)
extern char* ANSIToUTF8(const char* src);			//注意：需要手动释放返回的字符串资源
extern std::string ANSI2UTF8(const char * src);
extern char* UTF8ToANSI(const char* src);			//注意：需要手动释放返回的字符串资源
extern std::string UTF82ANSI(const char * src);
extern wchar_t* ANSIToWchar(const char* src);	//注意：需要手动释放返回的字符串资源
extern char* WcharToANSI(const wchar_t* src);			//注意：需要手动释放返回的字符串资源
inline std::string Wchar2ANSI(const wchar_t* src);
inline wchar_t oneCharToWchar(const char* src);
extern bool isUTF8(const char * str,int length);
inline bool isUTF8(const std::string &str){return isUTF8(str.c_str(),str.size());}
extern bool isUTF16(const char * str,int length);	//尚未实现
extern bool isANSI(const char * str,int length);	//尚未实现
enum _XStringCodeType
{
	STR_TYPE_ANSI,
	STR_TYPE_UTF8,
	STR_TYPE_UTF16,
	STR_TYPE_UNKNOWN,	//未知的
};
//获取字符串的编码格式
extern _XStringCodeType getStrCodeType(const char * str,int length);	//尚未实现
//将一般数据转换成字符串
template <typename T>
std::string toString(const T& value)
{
	std::ostringstream out;
//	static ostringstream out;
//	out.str("");
	out << value;
	return out.str();
}
//将一个字符串转换为数字
template <typename T>
const T toValue(const std::string& str)
{
	T val;
	std::stringstream ss;
	ss << str;
	ss >> val;
	if(ss.fail())
			throw runtime_error((std::string)typeid(T).name() + "type wanted:" + str);
	return val;
}
//#include "Shlwapi.h"
//./../../data/data.txt,depth为2，lastPos为8
//depth：回退的路径层数，
//lastPos：正式路径在字符串中的位置
extern void calPathDepth(const char *p,int &depth,int &lastPos);	//这里假设路径是合法的
//路径p回退backSum之后的路径backSum必须大于0
extern std::string getBackPath(const char *p,int backSum);
//判断路径是否为绝对路径
inline _XBool isAbsolutePath(const char * path);
//根据路径和相对路径生成新的文件路径如：./data/1.txt和../x.txt合并之后为./x.txt
inline std::string mergeFilename(const std::string &base,const std::string &offset);//这里假设base和offset都是合法的路径
//判断路径中字符的数量
extern int getCharsNumInPath(const char * path);
extern bool isPathValid(const char * path);	//检查路径是否合法，尚未经过验证(目前存在问题：如./../data/a.dat)
//获取按键的状态
inline _XBool getCapsLockState(){return (GetKeyState(VK_CAPITAL) == 1);}
inline _XBool getNumLockState(){return (GetKeyState(VK_NUMLOCK) == 1);}
//设置按键状态
inline void setCapsLockState(_XBool state);
inline void setNumLockState(_XBool state);
//弹出文件打开窗口并返回选择的文件的相对路径和绝对路径
extern BOOL popFileOpenDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitleName,PTSTR title,bool isOpen);
//通过winAPI弹出文件打开窗口以便于人性化的选择文件名
inline std::string getChooseFilename(char * defName,char * title,bool isOpen);
//通过winApi获取系统字体的路径
inline std::string getWindowsSysFontPath();
//从路径字符串中获取文件名
inline std::string getFilenameFormPath(const char *path);
////windows下从系统注册表中获取系统字体的映射关系
//#include <map>
//bool initWindows()
//{
//	//下面测试关于系统字体的相关方法
//	std::map<string, string> fontsMap;	//系统字体与路径的映射表
//	const wchar_t *Fonts = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";	//注册表中字体的键值
//	HKEY key_ft;
//	LONG l_ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, Fonts, 0, KEY_QUERY_VALUE, &key_ft);	//从注册表中获取字体的键值
//	if (l_ret != ERROR_SUCCESS)
//	{
//		LogStr("initWindows(): couldn't find fonts registery key");	
//		return false;	//从注册表中获取系统字体键值失败
//	}
//	
//	DWORD value_count;
//	DWORD max_data_len;
//	wchar_t value_name[2048];
//	BYTE *value_data;
//	// get font_file_name -> font_face mapping from the "Fonts" registry key
//	l_ret = RegQueryInfoKeyW(key_ft, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, &max_data_len, NULL, NULL);
//	if(l_ret != ERROR_SUCCESS)
//	{
//	    LogStr("initWindows(): couldn't query registery for fonts");
//        return false;
//	}
//	// no font installed
//	if(value_count == 0)
//	{
//	    LogStr("initWindows(): couldn't find any fonts in registery");
//        return false;
//	}
//	// max_data_len is in BYTE
//	value_data = static_cast<BYTE *>(HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, max_data_len));
//	if(value_data == NULL) return false;
//
//	char value_name_char[2048];
//	char value_data_char[2048];
//
//    string fontsDir = getenv ("windir");
//    fontsDir += "\\Fonts\\";
//	for (DWORD i = 0; i < value_count; ++i)
//	{
//		DWORD name_len = 2048;
//		DWORD data_len = max_data_len;
//
//		l_ret = RegEnumValueW(key_ft, i, value_name, &name_len, NULL, NULL, value_data, &data_len);
//		if(l_ret != ERROR_SUCCESS)
//		{
//			LogStr("initWindows(): couldn't read registry key for font type");
//			continue;
//		}
//
//		wcstombs(value_name_char,value_name,2048);
//		wcstombs(value_data_char,reinterpret_cast<wchar_t *>(value_data),2048);
//		string curr_face = value_name_char;
//		string font_file = value_data_char;
//		curr_face = curr_face.substr(0, curr_face.find('(') - 1);
//		fontsMap[curr_face] = fontsDir + font_file;
//	}
//	HeapFree(GetProcessHeap(), 0, value_data);
//	l_ret = RegCloseKey(key_ft);
//}
//获取字符串中表意字符的长度，如果getStrLen("你好") = 2,getStrLen("abc你好") = 5;
extern int getStrLen(const char * p);
#include "XStringFun.inl"

#endif