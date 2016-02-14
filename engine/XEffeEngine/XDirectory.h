#ifndef _JIA_XDIRECTORY_
#define _JIA_XDIRECTORY_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.13
//--------------------------------
//这是一个目前只支持win32的基于系统命令的目录管理的类，功能以及扩展性都有限
//优化：1、按照指定的顺序排序
//2、过滤指定的后缀名
//3、是否包含子文件夹有设置开关
//#include "windows.h"
#include <vector>
#include "XOSDefine.h"
namespace XE{
class XDir;
class XFileInfo
{
public:
	XBool isEnable;	//是否有效
//	char * allPath;		//全路径名
//	char * filename;	//文件名
	std::string allPath;
	std::string filename;
	XBool isDirectory;	//是否是目录
	XFileInfo()
		:isEnable(XFalse)
		,directory(NULL)
//		,allPath(NULL)
//		,filename(NULL)
	{}
	XDir * directory;	//如果是目录的话，这里是下一级目录的信息
	void release();
	//额外的属性
	long size;		//文件大小
	FILETIME time;	//文件修改的时间
};
class XDir
{
public:
	XBool isEnable;				//目录是否有效
//	char * directoryName;		//目录
	std::string directoryName;
	std::vector<XFileInfo *> files;		//目录下的文件(夹)
	XDir()
		:isEnable(XFalse)
		,level(0)
//		,directoryName(NULL)
	{}
	void release();
	int level;			//路径层级,视图化而定义的变量[0 - n]
};
class XDirectory
{
private:
	XBool m_isInited;		//是否初始化
	XBool m_withChildFolder;	//是否包含其子文件夹 
public:
	XDir m_curDirectory;	//当前的目录信息
	XBool init(const char *directoryName,bool withChildFolder = true);	//使用绝对路径，路径不要以‘/’或者是‘\’结束
	XBool initEx(const char *directoryName,bool withChildFolder = true);	//使用相对路径，相对于工程路径，路径不要以‘/’或者是‘\’结束

	XDirectory()
		:m_isInited(XFalse)
		,m_withChildFolder(XTrue)
	{}
	~XDirectory(){release();}
	void release()
	{//释放所有的资源
		if(!m_isInited) return;
		m_curDirectory.release();
		m_isInited = XFalse;
	}
	std::vector<XFileInfo *> m_sortResult;	//排序结果
	enum XSortMode
	{
		SORT_BY_SYSTEM,	//系统排序
		SORT_BY_SIZE,	//按大小排序
		SORT_BY_TIME,	//按时间排序
	};
	void sortFiles(XSortMode mode = SORT_BY_SYSTEM);	//请先过滤文件再进行排序(尚未经过测试)
	void filterFiles()	//过滤文件(尚未经过测试)
	{
		m_curSortMode = SORT_BY_SYSTEM;
		m_sortResult.clear();
		listFiles(m_curDirectory);
	}
	void setFilterName(const std::string &name);	//设置过滤的文件扩展名
	void clearFilter(){m_filterNames.clear();}	//清空过滤器
private:
	XSortMode m_curSortMode;
	std::vector<std::string> m_filterNames;	//文件过滤的类型
	void listFiles(const XDir&dir);	//列出路径下的所有文件，并通过过滤器进行过滤
};
namespace XFile
{
	extern XBool infoConvert(WIN32_FIND_DATA &fileInfo,XFileInfo &file);
	extern XBool enumerationFiles(const char *path,XDir &dir,bool witchCilder = true);		//使用绝对路径,解析目录下的文件信息，包含子目录
	inline XBool enumerationFiles(const std::string &path,XDir &dir,bool witchCilder = true){return enumerationFiles(path.c_str(),dir,witchCilder);}
	extern XBool enumerationFilesEx(const char *path,XDir &dir,bool witchCilder = true);	//使用相对路径,解析目录下的文件信息，包含子目录
	inline XBool enumerationFilesEx(const std::string &path,XDir &dir,bool witchCilder = true){return enumerationFilesEx(path.c_str(),dir,witchCilder);}
}
}
#endif