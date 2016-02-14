#ifndef _JIA_XFILE_
#define _JIA_XFILE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//这里是文件相关操作的函数，用于对文件信息进行处理
#include "XStringFun.h"

#include "XEngineCommon.h"

#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")
namespace XE{
class XCColor;
namespace XFile
{
	//获取文件的大小
	inline int getFileLen(FILE * fp)
	{
		if (fp == NULL) return 0;
		int tempOffset = ftell(fp);	//记录当前的文件位置
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);	//获取文件的长度
		fseek(fp, tempOffset, SEEK_SET);	//恢复之前的文件位置
		return len;
	}
	extern char *readFileToStr(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//从filename的文件中读取shader代码	//资源位置，0为外部资源，默认为外部资源
	extern int writeStrToFile(const char *filename, const char *s);					//将shader代码s写入到filename的文件中
	extern bool getFileString(const char *filename,std::string &outStr);	//从指定的文件中读取数据
	//下面函数的例子：xiajia.txt,5 则输出 xiajia(5).txt
	extern std::string addIndexToFilename(const char *filename,int index);	//在文件名filename的后面添加index的编号，主要是用于防止文件名重复
	extern XBool fileNameCompare(const char *p1, const char *p2);	//比较两个路径或者文件名是否一致，不考虑大小写，
	extern int getPathDeep(const char *p);			//获取路径的深度，获取路径目录的层数 e.g.:"c:/x/y/xx.txt" return 3
	extern int getPathDeepByChar(const char *p);	//获取路径的深度，获取路径目录的字符串深度 e.g.:"c:/x/y/xx.txt" return 6
	//获取大当前工作路径
	inline std::string getWorkPath();
	//获取文件扩展名
	inline std::string getFileExtension(const char * filename);
	//设置当前工作路径
	inline XBool setWorkPath(const std::string &path);
	//获取当前exe文件的全路径文件名
	inline std::string getCurrentExeFileFullPath();
	//相对路径转换成绝对路径
	inline std::string getFullPath(const std::string &filename);
	//#include "Shlwapi.h"
	//./../../data/data.txt,depth为2，lastPos为8
	//depth：回退的路径层数，
	//lastPos：正式路径在字符串中的位置
	extern void calPathDepth(const char *p, int &depth, int &lastPos);	//这里假设路径是合法的
	//路径p回退backSum之后的路径backSum必须大于0
	extern std::string getBackPath(const char *p, int backSum);
	//设置exe目录作为启动目录
	inline void setExeAsWorkPath()
	{
		std::string tmp = getBackPath(XE::XFile::getCurrentExeFileFullPath().c_str(),0);
		tmp[tmp.size() - 1] = '\0';
		setWorkPath(tmp);
	}
	//判断路径是否为绝对路径
	inline XBool isAbsolutePath(const char * path);
	//根据路径和相对路径生成新的文件路径如：./data/1.txt和../x.txt合并之后为./x.txt
	inline std::string mergeFilename(const std::string &base, const std::string &offset);//这里假设base和offset都是合法的路径
	//判断路径中字符的数量
	extern int getCharsNumInPath(const char * path);
	extern bool isPathValid(const char * path);	//检查路径是否合法，尚未经过验证(目前存在问题：如./../data/a.dat)
	//弹出文件打开窗口并返回选择的文件的相对路径和绝对路径
	extern BOOL popFileOpenDlg(HWND hwnd, char* pstrFileName, char* pstrTitleName, char* title, bool isOpen, const std::string&defExt = "xml");
	//通过winAPI弹出文件打开窗口以便于人性化的选择文件名
	inline std::string getChooseFilename(char * defName, char * title, bool isOpen, const std::string&defExt = "xml");
	//通过winAPI弹出对话框选择文件夹
	extern std::string getChooseFolderName();
	//通过winAPI弹出窗口来选择需要的颜色
	extern XCColor getChooseColor(const XCColor &color);
	//通过winApi获取系统字体的路径
	inline std::string getWindowsSysFontPath();
	//从路径字符串中获取文件名
	inline std::string getFilenameFormPath(const char *path);
	//从文件全路径名中获取路径：如C:/nihao/x.dat返回C:/nihao，注意必须是绝对路径
	inline std::string getFilePath(const char *filename);
	//判断绝对路径下面的文件或者文件夹是否存在
	inline XBool isExistFile(const char *path)		
	{
		if(path == NULL) return XFalse;
	#ifdef UNICODE
		return PathFileExistsA(path);
	#else
		return PathFileExists(path);
	#endif
	}
	//判断相对路径下面的文件或者文件夹是否存在
	extern XBool isExistFileEx(const char *path);
	//判断目标是否为文件夹(绝对路径)
	inline XBool isFolder(const char * path)		
	{
		if(path == NULL) return XFalse;
	#ifdef UNICODE
		return (GetFileAttributesA(path) & FILE_ATTRIBUTE_DIRECTORY);
	#else
		return (GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY);
	#endif
	}
	//判断目标是否为文件夹(相对路径)
	extern XBool isFolderEx(const char * path);	
	//删除指定文件
	inline XBool deleteFile(const char *filename)
	{
		if(filename == NULL) return XFalse;
	#ifdef UNICODE
		return DeleteFileA(filename);
	#else
		return DeleteFile(filename);
	#endif
	}
#if WITH_INLINE_FILE
#include "XFile.inl"
#endif
}
}
#endif