#ifndef _JIA_XDIRECTORY_
#define _JIA_XDIRECTORY_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.13
//--------------------------------
//这是一个目前只支持win32的基于系统命令的目录管理的类，功能以及扩展性都有限
#include "windows.h"
#include <vector>
#include "XOSDefine.h"

class _XDir;
class _XFile
{
public:
	_XBool isEnable;		//是否有效
	char * allPath;		//全路径名
	char * filename;	//文件名
	_XBool isDirectory;	//是否是目录
	_XFile()
		:isEnable(XFalse)
		,directory(NULL)
		,allPath(NULL)
		,filename(NULL)
	{
	}
	_XDir * directory;	//如果是目录的话，这里是下一级目录的信息
	void release();
};
class _XDir
{
public:
	_XBool isEnable;				//目录是否有效
	char * directoryName;		//目录
	std::vector<_XFile *> files;		//目录下的文件(夹)
	_XDir()
		:isEnable(XFalse)
		,level(0)
		,directoryName(NULL)
	{
	}
	void release();
	int level;			//路径层级,是图化而定义的变量[0 - n]
};
class _XDirectory
{
private:
	_XBool m_isInited;		//是否初始化
public:
	_XDir m_nowDirectory;	//当前的目录信息
	_XBool init(const char *directoryName);	//使用绝对路径
	_XBool initEx(const char *directoryName);	//使用相对路径，相对于工程路径

	_XDirectory()
		:m_isInited(0)
	{
	}
	~_XDirectory()
	{
		release();
	}
	void release()
	{//释放所有的资源
		if(!m_isInited) return;
		m_nowDirectory.release();
		m_isInited = XFalse;
	}
};
extern void infoConvert(WIN32_FIND_DATA &fileInfo,_XFile * file);
extern _XBool enumerationFiles(const char *path,_XDir * dir);		//使用绝对路径,解析目录下的文件信息，包含子目录
extern _XBool enumerationFilesEx(const char *path,_XDir * dir);	//使用相对路径,解析目录下的文件信息，包含子目录

#pragma comment(lib, "Shlwapi.lib")
extern _XBool isExistFile(const char *path);		//判断绝对路径下面的文件或者文件夹是否存在
extern _XBool isExistFileEx(const char *path);	//判断相对路径下面的文件或者文件夹是否存在
extern _XBool isFolder(const char * path);		//判断目标是否为文件夹
extern _XBool isFolderEx(const char * path);		//判断目标是否为文件夹
extern _XBool deleteFile(const char *filename);	//删除指定文件
#endif